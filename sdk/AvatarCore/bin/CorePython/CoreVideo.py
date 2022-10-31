# This Python file uses the following encoding: utf-8
import AvatarPython

import logging


class CoreVideo:
    """Python API for VideoWrapper class
    """

    def __init__(self, resource, on_frame_arrived=None):
        """Initializes the CoreVideo object with given resource and callback

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

        self.__video_wrapper = None
        self.__logger = logging.getLogger('Python::CoreVideo')
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
        self.__video_wrapper = AvatarPython.VideoWrapper()

    def destroy(self):
        """Destroys the CoreVideo object
        """
        self.stop()
        if self.__video_wrapper is not None:
            self.__video_wrapper.destroy()
            self.__video_wrapper = None

    def start(self):
        """Start the event looper for CoreVideo object, callback will be called
           after start is called
        """
        if self.__video_wrapper is None:
            self.__video_wrapper = AvatarPython.VideoWrapper()

        self.__video_wrapper.start_streaming(self.__on_frame_arrived,
                                             self.__resource)

    def stop(self):
        """Stops the event looper for CoreVideo. Object won't receive any
           callback calls after stop is called.
        """
        if self.__video_wrapper is not None:
            self.__video_wrapper.stop_streaming()

    def write_frame(self, width, height, format, video_data):
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
        if self.__video_wrapper is None:
            self.__logger.warning('write_frame::CoreVideo is stopped,' +
                                  'call start before calling write_frame')
            return
        self.__video_wrapper.write_video_frame(self.__resource, width, height,
                                               int(format), video_data)

    def write_frame_to_resource(self, resource, width, height, format,
                                video_data):
        """Writes the video_data into given resource with given format

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            resource (str): Name of the resource
            width (int): Width of the frame
            heght (int): Height of the frame
            format (VideoFormat): Format of the video
            video_data (object): List, array or numpy.ndarray of the data.
                                 If video_data is a numpy.ndarray it should be
                                 one dimmensional array
        """
        if self.__video_wrapper is None:
            self.__logger.warning('write_frame_to_resource::' +
                                  'CoreVideo is stopped,' +
                                  'call start before calling' +
                                  'write_frame_to_resource')
            return
        self.__video_wrapper.write_video_frame(resource, width, height,
                                               int(format), video_data)
