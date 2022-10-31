# This Python file uses the following encoding: utf-8
from enum import Enum


class VideoFormat(Enum):
    """Enum values for VideoFormat
    """
    ALPHA = 0, 'ALPHA'
    I420 = 1, 'I420'
    YUY2 = 2, 'YUY2'
    RGB = 3, 'RGB'
    RGBA = 4, 'RGBA'
    BGR = 5, 'BGR'
    BGRA = 6, 'BGRA'
    xRGB = 7, 'xRGB'
    xBGR = 8, 'xBGR'
    NV12 = 9, 'NV12'
    UNKOWN = 10, 'UNKOWN'

    def __new__(cls, value, name):
        member = object.__new__(cls)
        member._value_ = value
        member.fullname = name
        return member

    def __int__(self):
        return self.value
