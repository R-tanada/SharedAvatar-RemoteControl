# This Python file uses the following encoding: utf-8
import AvatarPython

import logging


class CoreAudio:
    """Python API for AudioWrapper class
    """

    def __init__(self, resource, on_frame_arrived=None):
        """Initializes the CoreAudio object with given resource and callback

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            resource (str): Name of the resource object
            on_frame_arrived (object): Callback object to be triggered when
                                       new audio frame is arrived
                                       (default is None)
        """

        self.__audio_wrapper = None
        self.__logger = logging.getLogger('Python::CoreAudio')
        self.__resource = None
        self.__on_frame_arrived = None

        self.__logger.setLevel(logging.INFO)
        ch = logging.StreamHandler()
        ch.setLevel(logging.INFO)
        formatter = logging.Formatter('%(name)s -- %(asctime)s ' +
                                      '[%(levelname)s]\t- %(message)s ',
                                      datefmt='%I:%M:%S')
        ch.setFormatter(formatter)
        self.__logger.addHandler(ch)

        self.__resource = resource
        if on_frame_arrived is not None:
            self.__on_frame_arrived = on_frame_arrived
        self.__audio_wrapper = AvatarPython.AudioWrapper()

    def destroy(self):
        """Destroys the CoreAudio object
        """
        self.stop()
        if self.__audio_wrapper is not None:
            self.__audio_wrapper.destroy()
            self.__audio_wrapper = None

    def start(self):
        """Start the event looper for CoreAudio object, callback will be called
           after start is called
        """
        if self.__audio_wrapper is None:
            self.__audio_wrapper = AvatarPython.AudioWrapper()

        self.__audio_wrapper.start_streaming(self.__on_frame_arrived,
                                             self.__resource)

    def stop(self):
        """Stops the event looper for CoreAudio, Object won't receive any
           callback after stop is called
        """
        if self.__audio_wrapper is not None:
            self.__audio_wrapper.stop_streaming()

    def write_frame(self, channels, rate, length, format, audio_data):
        """Writes the audio_data into resource of the CoreAudio with given
           format

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            channels (int): Number of channels
            rate (int): Sampling rate value
            length (int): Lenght of the data
            format (AudioFormat): Format of the audio
            audio_data (object): List, array or numpy.ndarray of the data.
                                 If video_data is a numpy.ndarray it should be
                                 one dimmensional array
        """

        if self.__audio_wrapper is None:
            self.__logger.warning('write_frame::' +
                                  'CoreAudio is stopped,' +
                                  'call start before calling' +
                                  'write_frame')
            return
        self.__audio_wrapper.write_frame(self.__resource,
                                         channels,
                                         rate,
                                         length,
                                         int(format),
                                         audio_data)

    def write_frame_to_resource(self,
                                resource,
                                channels,
                                rate,
                                length,
                                format,
                                audio_data):
        """Writes the audio_data into given resource with given format

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            resource (str): Name of the resource
            channels (int): Number of channels
            rate (int): Sampling rate value
            length (int): Lenght of the data
            format (AudioFormat): Format of the audio
            audio_data (object): List, array or numpy.ndarray of the data.
                                 If video_data is a numpy.ndarray it should be
                                 one dimmensional array
        """
        if self.__audio_wrapper is None:
            self.__logger.warning('write_frame_to_resource::' +
                                  'CoreAudio is stopped,' +
                                  'call start before calling' +
                                  'write_frame_to_resource')
            return

        self.__audio_wrapper.write_frame(resource,
                                         channels,
                                         rate,
                                         length,
                                         int(format),
                                         audio_data)
