/*
    video.c - video driver

    Copyright (C) 2011 Hugo Perquin - http://blog.perquin.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA 02110-1301 USA.
*/
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h> 
#include <linux/videodev2.h>
#include <pthread.h>
#include "../util/util.h"
#include "video.h"

#define CLEAR(x) memset (&(x), 0, sizeof (x))

static
int video_RealInit(struct vid_struct *vid)
{
    struct v4l2_capability cap;
    struct v4l2_format fmt;
    unsigned int i, ret;
    enum v4l2_buf_type type;

    vid->seq=0;
    vid->trigger=0;
    if(vid->n_buffers==0) vid->n_buffers=4;
    
    printf("opening device %s\n", vid->device);
    vid->fd = open(vid->device, O_RDWR, 0);
    
    if (vid->fd <= 0) {
	    perror("open() failed");
	    return -1; 
    }

    printf("ioctl VIDIOC_QUERYCAP\n");
    if (ioctl(vid->fd, VIDIOC_QUERYCAP, &cap) < 0) {
		perror("ioctl() VIDIOC_QUERYCAP failed.\n");
		return -1;    
    }

    printf("2 driver = %s, card = %s, version = %d, capabilities = 0x%x\n", cap.driver, cap.card, cap.version, cap.capabilities);

    CLEAR(fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = vid->w;	
	fmt.fmt.pix.height = vid->h;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;

    if (ioctl(vid->fd, VIDIOC_S_FMT, &fmt) < 0) {
		perror("ioctl() VIDIOC_S_FMT failed ");
		return -1;    
    }

    //image_size = fmt.fmt.pix.width * fmt.fmt.pix.height *3/2;

    struct v4l2_requestbuffers req;
    int page_size = getpagesize();

    CLEAR(req);
    req.count = vid->n_buffers;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    //req.memory = V4L2_MEMORY_MMAP;
    req.memory = V4L2_MEMORY_USERPTR;

    if (ioctl(vid->fd, VIDIOC_REQBUFS, &req) < 0) {
		perror("ioctl() VIDIOC_REQBUFS failed ");
		return -1;    
    }

    printf("Buffer count = %d\n", vid->n_buffers);

    vid->buffers = (struct buffer_struct*)calloc(vid->n_buffers, sizeof(struct buffer_struct));
    for (i = 0; i < vid->n_buffers; ++i) {
	struct v4l2_buffer buf;

	CLEAR(buf);
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	//buf.memory = V4L2_MEMORY_MMAP;
	buf.memory = V4L2_MEMORY_USERPTR;
	buf.index = i;

	if (ioctl(vid->fd, VIDIOC_QUERYBUF, &buf) < 0) {
		perror("ioctl() VIDIOC_QUERYBUF failed ");
		return -1;    
	}

	vid->buffers[i].length = buf.length;
	printf("buffer%d.length=%d\n",i,buf.length);
	//vid->buffers[i].buf = mmap(NULL, buf.length, PROT_READ|PROT_WRITE, MAP_SHARED, vid->fd, buf.m.offset);
	ret = posix_memalign(&vid->buffers[i].buf, page_size, buf.length);
	if (ret < 0) {
		printf("allocate buffer %u (%d)\n", i, ret);
		perror("Unable to allocate buffer");
		return -ENOMEM;
	}
	vid->buffers[i].length = buf.length;
	printf("Buffer %u allocated at address %p.\n", i, vid->buffers[i].buf);
    }

    for (i = 0; i < vid->n_buffers; ++i) {
	struct v4l2_buffer buf;

	CLEAR(buf);
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_USERPTR;
	buf.index = i;
	buf.length = vid->buffers[i].length;
	buf.m.userptr = (unsigned long)vid->buffers[i].buf;

	if (ioctl(vid->fd, VIDIOC_QBUF, &buf) < 0) {
		perror("ioctl() VIDIOC_QBUF failed");
		return -1;    
	}
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(vid->fd, VIDIOC_STREAMON, &type)< 0) {
		perror("ioctl() VIDIOC_STREAMON failed");
		return -1;    
    }
 
    return 0;
}

pthread_t video_thread;
void *video_thread_main(void* data)
{
	struct vid_struct* vid = (struct vid_struct*)data;
	printf("video_thread_main started\n");

	if (video_RealInit(vid)) {
		printf("video init failed");
		return NULL;
	}

    for (;;) {
		struct v4l2_buffer buf;

		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_USERPTR;
		if (ioctl(vid->fd, VIDIOC_DQBUF, &buf) < 0) {
			printf("ioctl() VIDIOC_DQBUF failed.\n");
			break;    
		}

		assert(buf.index < vid->n_buffers);

		vid->seq++;
		
		if(vid->trigger) {
			vid->img->timestamp = util_timestamp();
			vid->img->seq = vid->seq;
			memcpy(vid->img->buf, vid->buffers[buf.index].buf, vid->w*vid->h*2);
			vid->trigger=0;
		}
	
		buf.m.userptr = (unsigned long)vid->buffers[buf.index].buf;
		if (ioctl(vid->fd, VIDIOC_QBUF, &buf) < 0) {
			printf("ioctl() VIDIOC_QBUF failed.\n");
			break;    
		}
    }
    return 0;
}


int video_Init(struct vid_struct *vid)
{
        /** @todo launch dsp init script */

	//start video thread 
	int rc = pthread_create(&video_thread, NULL, video_thread_main, vid); 
	if(rc) {
		printf("ctl_Init: Return code from pthread_create(mot_thread) is %d\n", rc);
		return 202;
	}

	return 0;
}

void video_Close(struct vid_struct *vid)
{
    for (int i = 0; i < vid->n_buffers; ++i) {
    	if (-1 == munmap(vid->buffers[i].buf, vid->buffers[i].length)) printf("munmap() failed.\n");
    }
    close(vid->fd);
}

struct img_struct *video_CreateImage(struct vid_struct *vid)
{
	struct img_struct* img = (struct img_struct*)malloc(sizeof(struct img_struct));
	img->w=vid->w;
	img->h=vid->h;
	img->buf = (unsigned char*)malloc(vid->h*vid->w*2);
	return img;
}

pthread_mutex_t video_grab_mutex = PTHREAD_MUTEX_INITIALIZER; 

/** @todo this is not very performant, as we only get every second image at max */
void video_GrabImage(struct vid_struct *vid, struct img_struct *img) {
	pthread_mutex_lock(&video_grab_mutex);
	vid->img = img;
	vid->trigger=1;
	while(vid->trigger) pthread_yield();
	pthread_mutex_unlock(&video_grab_mutex);
}

void write_pgm(struct img_struct *img, char *fn)
{
  FILE *fp=fopen(fn,"w");
  if(fp==NULL) {
    perror("Open pbm for writing");
    return;
  }
  fprintf(fp,"P2\n");
  int pos=0;
  fprintf(fp,"%d %d\n",img->w, img->h);
  fprintf(fp,"255\n");
  for(int h=0; h<img->h; h++) {
    for(int w=0; w<img->w/2; w++) {
//      unsigned int u=img->buf[pos++];
      unsigned int y1=img->buf[pos++];
//      unsigned int v=img->buf[pos++];
      unsigned int y2=img->buf[pos++];
      
      fprintf(fp, "%d %d ",y1,y2);
    }
    fprintf(fp,"\n");
  }
  fclose(fp);

}
