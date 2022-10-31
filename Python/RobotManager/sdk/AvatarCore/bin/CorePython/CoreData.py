# This Python file uses the following encoding: utf-8
import AvatarPython

import logging


class CoreData:
    """Python API for DataWrapper
    """

    def __init__(self, resource, channel, on_data_arrived=None):
        """Initializes the CoreData object with given resource, channel
           and callback

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            resource (str): Name of the resource object
            channel (str): Name of the channel
            on_data_arrived (object): Callback object to be triggered when
                                      new data frame is arrived
                                      (default is None)
        """
        self.__data_wrapper = None
        self.__logger = logging.getLogger('Python::CoreData')
        self.__resource = None
        self.__channel = None
        self.__on_data_arrived = None

        self.__logger.setLevel(logging.INFO)
        ch = logging.StreamHandler()
        ch.setLevel(logging.INFO)
        formatter = logging.Formatter('%(name)s -- %(asctime)s ' +
                                      '[%(levelname)s]\t- %(message)s ',
                                      datefmt='%I:%M:%S')
        ch.setFormatter(formatter)
        self.__logger.addHandler(ch)

        self.__data_wrapper = AvatarPython.DataWrapper()

        self.__resource = resource
        self.__channel = channel
        if on_data_arrived is not None:
            self.__on_data_arrived = on_data_arrived

    def destroy(self):
        """Destroys the CoreData object
        """
        self.stop()
        if self.__data_wrapper is not None:
            self.__data_wrapper.destroy()
            self.__data_wrapper = None

    def start(self):
        """Start the event looper for CoreData object, callbacks will called
           after start is called
        """
        if self.__data_wrapper is None:
            self.__data_wrapper = AvatarPython.DataWrapper()

        self.__data_wrapper.start_streaming(self.__on_data_arrived,
                                            self.__resource,
                                            self.__channel)

    def stop(self):
        """Stops the event looper for CoreData. Object won't receive any
           any callback calls after stop is called.
        """
        if self.__data_wrapper is not None:
            self.__data_wrapper.stop_streaming()

    def send_data(self, data):
        """Sends the data to object's resource and channel

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            data (str): Data to be sent
        """
        if self.__data_wrapper is None:
            self.__logger.warning('send_data::CoreData is stopped,' +
                                  'call start before calling send_data')
            return
        self.__data_wrapper.send_data(self.__resource, self.__channel, data)

    def send_data_to_channel(self, channel, data):
        """Sends data to given channel to object's resource

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            channel (str): Name of the channel
            data (str): Data to be sent
        """
        if self.__data_wrapper is None:
            self.__logger.warning('send_data_to_channel::' +
                                  'CoreData is stopped,' +
                                  'call start before calling' +
                                  'send_data_to_channel')
            return
        self.__data_wrapper.send_data(self.__resource, channel, data)

    def send_data_to_resource_channel(self, resource, channel, data):
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
        if self.__data_wrapper is None:
            self.__logger.warning('send_data_to_resource_channel::' +
                                  'CoreData is stopped' +
                                  'call start before calling' +
                                  'send_data_to_resource_channel')
            return
        self.__data_wrapper.send_data(resource, channel, data)
