# This Python file uses the following encoding: utf-8
import AvatarPython

import logging


class CoreResourceManager:
    """Python API for VideoWrapper class
    """

    def __init__(self, on_frame_arrived=None):
        """Initializes the CoreResourceManager object with given resource and
           callback

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            resource (str): Name of the resource object
            on_frame_arrived (object): Callback object to be triggered when
                                       new video frame is arrived
                                       (default is None)
        """

        self.__manager_wrapper = None
        self.__logger = logging.getLogger('Python::CoreResourceManager')
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

        if on_frame_arrived is not None:
            self.__on_frame_arrived = on_frame_arrived
        self.__manager_wrapper = AvatarPython.ResourceManagerWrapper()

    def destroy(self):
        """Destroys the CoreResourceManager object
        """
        self.stop()
        if self.__manager_wrapper is not None:
            self.__manager_wrapper.destroy()
            self.__manager_wrapper = None

    def subscribe_video(self, resource):
        """Subscribes to a video resource
        """
        if self.__manager_wrapper is None:
            self.__manager_wrapper = AvatarPython.VideoWrapper()

        return self.__manager_wrapper.subscribe_video(resource)

    def subscribe_audio(self, resource):
        """Subscribes to a audio resource
        """
        if self.__manager_wrapper is None:
            self.__manager_wrapper = AvatarPython.VideoWrapper()

        return self.__manager_wrapper.subscribe_audio(resource)

    def subscribe_data(self, resource, channel):
        """Subscribes to a data channel using its resource name and channel
           name
        """
        if self.__manager_wrapper is None:
            self.__manager_wrapper = AvatarPython.VideoWrapper()

        return self.__manager_wrapper.subscribe_data(resource, channel)

    def unsubscribe_video(self, resource):
        """Unubscribes from a video resource
        """
        if self.__manager_wrapper is None:
            self.__manager_wrapper = AvatarPython.VideoWrapper()

        return self.__manager_wrapper.unsubscribe_video(resource)

    def unsubscribe_audio(self, resource):
        """Unubscribes from a audio resource
        """
        if self.__manager_wrapper is None:
            self.__manager_wrapper = AvatarPython.VideoWrapper()

        return self.__manager_wrapper.unsubscribe_audio(resource)

    def unsubscribe_data(self, resource, channel):
        """Unubscribes from a data channel using its resource name and channel
           name
        """
        if self.__manager_wrapper is None:
            self.__manager_wrapper = AvatarPython.VideoWrapper()

        return self.__manager_wrapper.unsubscribe_data(resource, channel)

    def start(self, timeout):
        """Start the event looper for CoreResourceManager object,
           callback will be called after start is called
        """
        if self.__manager_wrapper is None:
            self.__manager_wrapper = AvatarPython.VideoWrapper()

        self.__manager_wrapper.start_streaming(self.__on_frame_arrived,
                                               timeout)

    def stop(self):
        """Stops the event looper for CoreResourceManager.
           Object won't receive any callback calls after stop is called.
        """
        if self.__manager_wrapper is not None:
            self.__manager_wrapper.stop_streaming()

    def write_video(self, resource, width, height, format, video_data):
        """Writes the video_data into object's resource with given format

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            width (int): Width of the frame
            heght (int): Height of the frame
            format (VideoFormat): Format of the video
            video_data (object): List, array or numpy.ndarray of the data.
                                 If video_data is a numpy.ndarray it should be
                                 one dimmensional array
        """
        if self.__manager_wrapper is None:
            self.__logger.warning('write_video::CoreResourceManager' +
                                  ' is stopped, call start before calling' +
                                  ' write_frame')
            return
        self.__manager_wrapper.send_video(resource,
                                          width,
                                          height,
                                          int(format),
                                          video_data)

    def write_data(self, resource, channel, data):
        """Sends the data to given resource and channel

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            resource (str): Name of the resource
            channel (str): Name of the channel
            data (str): Data to be sent
        """
        if self.__manager_wrapper is None:
            self.__logger.warning('write_data::' +
                                  'CoreResourceManager is stopped' +
                                  'call start before calling' +
                                  'send_data_to_resource_channel')
            return
        self.__manager_wrapper.send_data(resource, channel, data)

    def write_audio(self,
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
        if self.__manager_wrapper is None:
            self.__logger.warning('write_audio::' +
                                  'CoreResourceManager is stopped,' +
                                  'call start before calling' +
                                  'write_frame_to_resource')
            return

        self.__manager_wrapper.send_audio(resource,
                                          channels,
                                          rate,
                                          length,
                                          int(format),
                                          audio_data)
