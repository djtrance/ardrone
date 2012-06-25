"""autogenerated by genmsg_py from Navdata.msg. Do not edit."""
import roslib.message
import struct


class Navdata(roslib.message.Message):
  _md5sum = "5caf99a96dc76ca4a30c12a29967a314"
  _type = "navdata_publisher/Navdata"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """uint16 navboard_seq
float32[3] acceleration
float32[3] gyro
float32 accelTemperature
float32 gyroTemperature
float32 vrefEpson
float32 vrefIDG
float32 height_us
uint16 us_echo_start
uint16 us_echo_end
uint16 us_association_echo
uint16 us_distance_echo
uint16 us_courbe_temps
uint16 us_courbe_valeur
uint16 us_courbe_ref
float32[7] newFloat
uint16[7] newUint16



"""
  __slots__ = ['navboard_seq','acceleration','gyro','accelTemperature','gyroTemperature','vrefEpson','vrefIDG','height_us','us_echo_start','us_echo_end','us_association_echo','us_distance_echo','us_courbe_temps','us_courbe_valeur','us_courbe_ref','newFloat','newUint16']
  _slot_types = ['uint16','float32[3]','float32[3]','float32','float32','float32','float32','float32','uint16','uint16','uint16','uint16','uint16','uint16','uint16','float32[7]','uint16[7]']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.
    
    The available fields are:
       navboard_seq,acceleration,gyro,accelTemperature,gyroTemperature,vrefEpson,vrefIDG,height_us,us_echo_start,us_echo_end,us_association_echo,us_distance_echo,us_courbe_temps,us_courbe_valeur,us_courbe_ref,newFloat,newUint16
    
    @param args: complete set of field values, in .msg order
    @param kwds: use keyword arguments corresponding to message field names
    to set specific fields. 
    """
    if args or kwds:
      super(Navdata, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.navboard_seq is None:
        self.navboard_seq = 0
      if self.acceleration is None:
        self.acceleration = [0.,0.,0.]
      if self.gyro is None:
        self.gyro = [0.,0.,0.]
      if self.accelTemperature is None:
        self.accelTemperature = 0.
      if self.gyroTemperature is None:
        self.gyroTemperature = 0.
      if self.vrefEpson is None:
        self.vrefEpson = 0.
      if self.vrefIDG is None:
        self.vrefIDG = 0.
      if self.height_us is None:
        self.height_us = 0.
      if self.us_echo_start is None:
        self.us_echo_start = 0
      if self.us_echo_end is None:
        self.us_echo_end = 0
      if self.us_association_echo is None:
        self.us_association_echo = 0
      if self.us_distance_echo is None:
        self.us_distance_echo = 0
      if self.us_courbe_temps is None:
        self.us_courbe_temps = 0
      if self.us_courbe_valeur is None:
        self.us_courbe_valeur = 0
      if self.us_courbe_ref is None:
        self.us_courbe_ref = 0
      if self.newFloat is None:
        self.newFloat = [0.,0.,0.,0.,0.,0.,0.]
      if self.newUint16 is None:
        self.newUint16 = [0,0,0,0,0,0,0]
    else:
      self.navboard_seq = 0
      self.acceleration = [0.,0.,0.]
      self.gyro = [0.,0.,0.]
      self.accelTemperature = 0.
      self.gyroTemperature = 0.
      self.vrefEpson = 0.
      self.vrefIDG = 0.
      self.height_us = 0.
      self.us_echo_start = 0
      self.us_echo_end = 0
      self.us_association_echo = 0
      self.us_distance_echo = 0
      self.us_courbe_temps = 0
      self.us_courbe_valeur = 0
      self.us_courbe_ref = 0
      self.newFloat = [0.,0.,0.,0.,0.,0.,0.]
      self.newUint16 = [0,0,0,0,0,0,0]

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    @param buff: buffer
    @type  buff: StringIO
    """
    try:
      buff.write(_struct_H.pack(self.navboard_seq))
      buff.write(_struct_3f.pack(*self.acceleration))
      buff.write(_struct_3f.pack(*self.gyro))
      _x = self
      buff.write(_struct_5f7H.pack(_x.accelTemperature, _x.gyroTemperature, _x.vrefEpson, _x.vrefIDG, _x.height_us, _x.us_echo_start, _x.us_echo_end, _x.us_association_echo, _x.us_distance_echo, _x.us_courbe_temps, _x.us_courbe_valeur, _x.us_courbe_ref))
      buff.write(_struct_7f.pack(*self.newFloat))
      buff.write(_struct_7H.pack(*self.newUint16))
    except struct.error, se: self._check_types(se)
    except TypeError, te: self._check_types(te)

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    @param str: byte array of serialized message
    @type  str: str
    """
    try:
      end = 0
      start = end
      end += 2
      (self.navboard_seq,) = _struct_H.unpack(str[start:end])
      start = end
      end += 12
      self.acceleration = _struct_3f.unpack(str[start:end])
      start = end
      end += 12
      self.gyro = _struct_3f.unpack(str[start:end])
      _x = self
      start = end
      end += 34
      (_x.accelTemperature, _x.gyroTemperature, _x.vrefEpson, _x.vrefIDG, _x.height_us, _x.us_echo_start, _x.us_echo_end, _x.us_association_echo, _x.us_distance_echo, _x.us_courbe_temps, _x.us_courbe_valeur, _x.us_courbe_ref,) = _struct_5f7H.unpack(str[start:end])
      start = end
      end += 28
      self.newFloat = _struct_7f.unpack(str[start:end])
      start = end
      end += 14
      self.newUint16 = _struct_7H.unpack(str[start:end])
      return self
    except struct.error, e:
      raise roslib.message.DeserializationError(e) #most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    @param buff: buffer
    @type  buff: StringIO
    @param numpy: numpy python module
    @type  numpy module
    """
    try:
      buff.write(_struct_H.pack(self.navboard_seq))
      buff.write(self.acceleration.tostring())
      buff.write(self.gyro.tostring())
      _x = self
      buff.write(_struct_5f7H.pack(_x.accelTemperature, _x.gyroTemperature, _x.vrefEpson, _x.vrefIDG, _x.height_us, _x.us_echo_start, _x.us_echo_end, _x.us_association_echo, _x.us_distance_echo, _x.us_courbe_temps, _x.us_courbe_valeur, _x.us_courbe_ref))
      buff.write(self.newFloat.tostring())
      buff.write(self.newUint16.tostring())
    except struct.error, se: self._check_types(se)
    except TypeError, te: self._check_types(te)

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    @param str: byte array of serialized message
    @type  str: str
    @param numpy: numpy python module
    @type  numpy: module
    """
    try:
      end = 0
      start = end
      end += 2
      (self.navboard_seq,) = _struct_H.unpack(str[start:end])
      start = end
      end += 12
      self.acceleration = numpy.frombuffer(str[start:end], dtype=numpy.float32, count=3)
      start = end
      end += 12
      self.gyro = numpy.frombuffer(str[start:end], dtype=numpy.float32, count=3)
      _x = self
      start = end
      end += 34
      (_x.accelTemperature, _x.gyroTemperature, _x.vrefEpson, _x.vrefIDG, _x.height_us, _x.us_echo_start, _x.us_echo_end, _x.us_association_echo, _x.us_distance_echo, _x.us_courbe_temps, _x.us_courbe_valeur, _x.us_courbe_ref,) = _struct_5f7H.unpack(str[start:end])
      start = end
      end += 28
      self.newFloat = numpy.frombuffer(str[start:end], dtype=numpy.float32, count=7)
      start = end
      end += 14
      self.newUint16 = numpy.frombuffer(str[start:end], dtype=numpy.uint16, count=7)
      return self
    except struct.error, e:
      raise roslib.message.DeserializationError(e) #most likely buffer underfill

_struct_I = roslib.message.struct_I
_struct_5f7H = struct.Struct("<5f7H")
_struct_H = struct.Struct("<H")
_struct_7H = struct.Struct("<7H")
_struct_3f = struct.Struct("<3f")
_struct_7f = struct.Struct("<7f")