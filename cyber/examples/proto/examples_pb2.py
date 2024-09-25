# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: cyber/examples/proto/examples.proto
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='cyber/examples/proto/examples.proto',
  package='apollo.cyber.examples.proto',
  syntax='proto2',
  serialized_options=None,
  create_key=_descriptor._internal_create_key,
  serialized_pb=b'\n#cyber/examples/proto/examples.proto\x12\x1b\x61pollo.cyber.examples.proto\"5\n\x0cSamplesTest1\x12\x12\n\nclass_name\x18\x01 \x01(\t\x12\x11\n\tcase_name\x18\x02 \x01(\t\"S\n\x07\x43hatter\x12\x11\n\ttimestamp\x18\x01 \x01(\x04\x12\x17\n\x0flidar_timestamp\x18\x02 \x01(\x04\x12\x0b\n\x03seq\x18\x03 \x01(\x04\x12\x0f\n\x07\x63ontent\x18\x04 \x01(\x0c\"<\n\x06\x44river\x12\x0f\n\x07\x63ontent\x18\x01 \x01(\t\x12\x0e\n\x06msg_id\x18\x02 \x01(\x04\x12\x11\n\ttimestamp\x18\x03 \x01(\x04\"C\n\x07Student\x12\x0c\n\x04name\x18\x01 \x01(\t\x12\x0b\n\x03\x61ge\x18\x02 \x01(\x04\x12\x0e\n\x06height\x18\x03 \x01(\x01\x12\r\n\x05\x62ooks\x18\x04 \x03(\t'
)




_SAMPLESTEST1 = _descriptor.Descriptor(
  name='SamplesTest1',
  full_name='apollo.cyber.examples.proto.SamplesTest1',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='class_name', full_name='apollo.cyber.examples.proto.SamplesTest1.class_name', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='case_name', full_name='apollo.cyber.examples.proto.SamplesTest1.case_name', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=68,
  serialized_end=121,
)


_CHATTER = _descriptor.Descriptor(
  name='Chatter',
  full_name='apollo.cyber.examples.proto.Chatter',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='timestamp', full_name='apollo.cyber.examples.proto.Chatter.timestamp', index=0,
      number=1, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='lidar_timestamp', full_name='apollo.cyber.examples.proto.Chatter.lidar_timestamp', index=1,
      number=2, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='seq', full_name='apollo.cyber.examples.proto.Chatter.seq', index=2,
      number=3, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='content', full_name='apollo.cyber.examples.proto.Chatter.content', index=3,
      number=4, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=b"",
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=123,
  serialized_end=206,
)


_DRIVER = _descriptor.Descriptor(
  name='Driver',
  full_name='apollo.cyber.examples.proto.Driver',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='content', full_name='apollo.cyber.examples.proto.Driver.content', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='msg_id', full_name='apollo.cyber.examples.proto.Driver.msg_id', index=1,
      number=2, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='timestamp', full_name='apollo.cyber.examples.proto.Driver.timestamp', index=2,
      number=3, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=208,
  serialized_end=268,
)


_STUDENT = _descriptor.Descriptor(
  name='Student',
  full_name='apollo.cyber.examples.proto.Student',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='name', full_name='apollo.cyber.examples.proto.Student.name', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='age', full_name='apollo.cyber.examples.proto.Student.age', index=1,
      number=2, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='height', full_name='apollo.cyber.examples.proto.Student.height', index=2,
      number=3, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='books', full_name='apollo.cyber.examples.proto.Student.books', index=3,
      number=4, type=9, cpp_type=9, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=270,
  serialized_end=337,
)

DESCRIPTOR.message_types_by_name['SamplesTest1'] = _SAMPLESTEST1
DESCRIPTOR.message_types_by_name['Chatter'] = _CHATTER
DESCRIPTOR.message_types_by_name['Driver'] = _DRIVER
DESCRIPTOR.message_types_by_name['Student'] = _STUDENT
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

SamplesTest1 = _reflection.GeneratedProtocolMessageType('SamplesTest1', (_message.Message,), {
  'DESCRIPTOR' : _SAMPLESTEST1,
  '__module__' : 'cyber.examples.proto.examples_pb2'
  # @@protoc_insertion_point(class_scope:apollo.cyber.examples.proto.SamplesTest1)
  })
_sym_db.RegisterMessage(SamplesTest1)

Chatter = _reflection.GeneratedProtocolMessageType('Chatter', (_message.Message,), {
  'DESCRIPTOR' : _CHATTER,
  '__module__' : 'cyber.examples.proto.examples_pb2'
  # @@protoc_insertion_point(class_scope:apollo.cyber.examples.proto.Chatter)
  })
_sym_db.RegisterMessage(Chatter)

Driver = _reflection.GeneratedProtocolMessageType('Driver', (_message.Message,), {
  'DESCRIPTOR' : _DRIVER,
  '__module__' : 'cyber.examples.proto.examples_pb2'
  # @@protoc_insertion_point(class_scope:apollo.cyber.examples.proto.Driver)
  })
_sym_db.RegisterMessage(Driver)

Student = _reflection.GeneratedProtocolMessageType('Student', (_message.Message,), {
  'DESCRIPTOR' : _STUDENT,
  '__module__' : 'cyber.examples.proto.examples_pb2'
  # @@protoc_insertion_point(class_scope:apollo.cyber.examples.proto.Student)
  })
_sym_db.RegisterMessage(Student)


# @@protoc_insertion_point(module_scope)
