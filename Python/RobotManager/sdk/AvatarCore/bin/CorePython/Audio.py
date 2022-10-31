# This Python file uses the following encoding: utf-8
from enum import Enum


class AudioFormat(Enum):
    """Enum values for AudioFormat
    """
    UNKNOWN = 0, 'Unkown'
    U8 = 1, 'U8'
    S8 = 2, 'S8'
    U16 = 3, 'U16'
    S16 = 4, 'S16'
    U24 = 5, 'U24'
    S24 = 6, 'S24'
    U32 = 7, 'U32'
    S32 = 8, 'S32'
    F32 = 9, 'F32'

    def __new__(cls, value, name):
        member = object.__new__(cls)
        member._value_ = value
        member.fullname = name
        return member

    def __int__(self):
        return self.value
