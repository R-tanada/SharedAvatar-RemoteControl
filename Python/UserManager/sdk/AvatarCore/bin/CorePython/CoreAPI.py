# This Python file uses the following encoding: utf-8
import os
import platform
import sys
import time

# Python 3.8 and above in windows doesn't register PATH variable,
# and uses its own runtime path management. In order for it to find gstreamer
# runtimes, the following hacky lines are added
if (platform.system() == 'Windows'
   and sys.version_info.major >= 3
   and sys.version_info.minor >= 8):

    print(sys.version)
    for p in os.environ['PATH'].split(';'):
        if os.path.isdir(p):
            os.add_dll_directory(p)

import AvatarPython
from CorePython.Interval import set_interval
from CorePython.Timeout import set_timeout


import logging
from enum import Enum, IntEnum
import datetime


class CoreAPI():
    """Python wrapper for CoreAPI
    """

    class DefaultCallbacks(Enum):
        """Enum values for DefaultCallbacks
        """
        InitCallbackType = 'Init'
        DestroyCallbackType = 'Destroy'
        PreLoadServices = 'PreLoadServices'
        OnLog = 'OnLog'

    class ServiceNames(Enum):
        """Enum values for ServiceNames
        """
        CloudProxy = 'Management/proxy'
        VideoCapture = 'Capture/video/front'
        AudioCapture = 'Capture/audio'
        MediaSender = 'RTP/sender/media'
        MediaReceiver = 'RTP/receiver/media'
        DataSender = 'RTP/sender/data'
        DataReceiver = 'RTP/receiver/data'
        AudioPlayback = 'Capture/remote/audio'

    class LogLevel(IntEnum):
        """Enum values for LogLevel
        """
        Emergency = 0
        Alert = 1
        Critical = 2
        Error = 3
        Warning = 4
        Notice = 5
        Info = 6
        Debug = 7

    def __init__(self):
        """Initializes the CoreAPI object and it's members
        """
        self.__core_initing = False
        self.__core_inited = False
        self.__logger = logging.getLogger('Python::CoreAPI')
        self.__api_wrapper = None
        self.__service_status = {}
        self.__update_loop_event = None
        self.__callbacks = {}
        self.__app_path = ""

        self.__logger.setLevel(logging.INFO)
        ch = logging.StreamHandler()
        ch.setLevel(logging.INFO)
        formatter = logging.Formatter('%(name)s -- %(asctime)s ' +
                                      '[%(levelname)s]\t- %(message)s ',
                                      datefmt='%I:%M:%S')
        ch.setFormatter(formatter)
        self.__logger.addHandler(ch)

    def __service_status_reply(self, service, path, data):
        result = data.split(':')
        self.__service_status[result[0]] = True if result[1] == "1" else False

    def add_callback(self, callback_type, callback):
        """Adds given callback with given callback type

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            callback_type (str): String representation of the callback type
            callback (object): Callable object for method or function
        """
        if callback_type not in self.__callbacks:
            self.__callbacks[callback_type] = []

        try:
            self.__callbacks[callback_type].index(callback)
        except ValueError:
            self.__callbacks[callback_type].append(callback)

    def remove_callback(self, callback_type, callback):
        """Removes given callback with given callback type

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            callback_type (str): String representation of the callback type
            callback (object): Callable object for method or function
        """
        if callback_type not in self.__callbacks:
            return

        try:
            self.__callbacks[callback_type].remove(callback)
        except ValueError:
            self.__logger.warning('remove_callback::Function not found')

    def trigger_callback(self, callback_type, data):
        """Triggers callbacks with given callback type

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            callback_type (str): String representation of the callback type
            data (object): Message object that will be passed to callbacks
        """
        if callback_type in self.__callbacks:
            for func in self.__callbacks[callback_type]:
                func(data)

    def check_service_status(self, service, callback, timeout=10):
        """Checks the status of the service with given timeout and calls the
        callback function

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            service (str): Name of the service to be checked
            callback (object): Callable object for method or function
            timout (int, optional): Timeout value for callback call
                                    (default is 10)
        """
        self.__service_status[service] = False
        self.__api_wrapper.publish_message("",
                                           "service/status/get",
                                           service)
        set_timeout(callback, timeout, self.__service_status[service])

    def log_callback(self, source, message, level, verbose):
        """Log callback

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            source (str): String representaion of the callback type
            message (str): Message that would be logged
            level (LogLevel): LogLevel object for logging
            verbose (???): ???
        """
        self.trigger_callback(self.DefaultCallbacks.OnLog,
                              {'time_stamp': datetime.datetime.now(),
                               'source': source,
                               'message': message,
                               'log_type': "CoreSDK",
                               'log_level': level})

    def init_api(self,
                 master=False,
                 app_path="",
                 port=0,
                 preload_services=[],
                 default_services=[],
                 log_path=".",
                 app_name=""):
        """Initialize the core api

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            master (bool): If it is True then core will be initialized as a
                           master, (default is false)
            app_path (str): Path for the application folder
                            (default is empty string)
            port (int): Port value of the application (default is 0)
            preload_services (list): List of preload services
                                     (default is empty list)
            default_services (list): List of default services
                                     (default is empty list)
            log_path (str): Path for storing log files (default is ".")

        Returns:
            bool: True if the core is initialize succesfully, False otherwise
        """

        if self.__core_initing or self.__core_inited:
            return True

        self.__logger.info('init_api::init_api')
        self.__app_path = app_path
        self.__api_wrapper = AvatarPython.CoreAPIWrapper()
        self.__core_initing = True

        if self.__api_wrapper.init_api(master,
                                       self.__app_path,
                                       port,
                                       log_path,
                                       app_name) is False:
            self.__logger.warning('init_api::' +
                                  'Failed to initialize avatar core!')
            self.destroy_api()
            self.__core_initing = False
            return False

        self.__api_wrapper.set_service_mode(True)
        self.__core_inited = True
        self.trigger_callback(self.DefaultCallbacks.PreLoadServices, {})
        for service in preload_services:
            self.__api_wrapper.ref_service(service)

        for service in default_services:
            self.__api_wrapper.load_services(service)

        self.__api_wrapper.start_default_services()
        self.__api_wrapper.subscribe_to_message('Application',
                                                'service/status/reply',
                                                self.__service_status_reply)
        self.__core_initing = False
        self.update_loop()
        self.trigger_callback(self.DefaultCallbacks.InitCallbackType, {})
        return True

    def destroy_api(self):
        """Destroys initialized core

        Example:
            Usage example

        Note:
            Specific notes

        Returns:
            bool: True if the core is destroyed succesfully, False otherwise
        """
        if self.__core_inited is False:
            return False

        self.trigger_callback(self.DefaultCallbacks.DestroyCallbackType, {})
        if self.__update_loop_event:
            self.__update_loop_event.set()
            self.__update_loop_event = None
            time.sleep(0.1)

        self.__api_wrapper.unsubscribe_to_message('Application',
                                                  'service/status/reply')
        # self.__logger.info('destroy_api::destroy_api')
        self.__api_wrapper.destroy_api()
        AvatarPython.kill_process()
        self.__core_inited = False
        # self.__logger.info('destroy_api::destroy_api - Done')
        return True

    def load_services(self, file):
        """Loads the services in given file

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            file (str): Service file path; file should be in JSON format

        Returns:
            bool: True if the services loaded successfuly, False otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('load_services::CoreAPI is not inited')
            return False
        return self.__api_wrapper.load_services(file)

    def ref_service(self, service):
        """Ref the given service ???

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            service (str): Name of the service

        Returns:
            bool: True if the ref is succesful, False otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('ref_service::CoreAPI is not inited')
            return False
        return self.__api_wrapper.ref_service(service)

    def unref_service(self, service):
        """Unrefs the service ???

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            service (str): Name of the service

        Returns:
            bool: True if the unref is succesful, False otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('unref_service::CoreAPI is not inited')
            return False
        return self.__api_wrapper.unref_service(service)

    def subscribe_to_message(self, name, path, callback):
        """Subscribes to a message. Callback will be triggered when event
           occurs.

           Example:
               Usage example

           Note:
               Specific notes

           Args:
               name (str): Name of the event
               path (str): ???
               callback (object): Method or function object that will be
                                  triggered

           Returns:
               bool: True if the subscription is successful, False otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('subscribe_to_message::CoreAPI is not inited')
            return False
        return self.__api_wrapper.subscribe_to_message(name, path, callback)

    def unsubscribe_to_message(self, name, path):
        """Unsubscribes to a message. Callback that was given in
           subscribe_to_message method will be dropped and won't be triggered

           Example:
               Usage example

           Note:
               Specific notes

           Args:
               name (str): Name of the event
               path (str): ???

           Returns:
               bool: True if the unsubscription is successful, False otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('unsubscribe_to_message::' +
                               'CoreAPI is not inited')
            return False
        return self.__api_wrapper.unsubscribe_to_message(name, path)

    def unsubscribe(self, name):
        """Unsubscribes a service. Callbacks that were given in
           subscribe_to_message method will be dropped and won't be triggered

           Example:
               Usage example

           Note:
               Specific notes

           Args:
               name (str): Name of the subscriber to remove

           Returns:
               bool: True if the unsubscription is successful, False otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('unsubscribe::' +
                               'CoreAPI is not inited')
            return False
        return self.__api_wrapper.unsubscribe(name)

    def start_service(self, service):
        """Starts the given service alias name.
        The service should be already loaded from a JSON file in the same
        process or on a different one.


        Example:
            core.start_service("Capture/front/camera")

        Note:
            None

        Args:
            service (str): Alias name of the service defined in the service
            JSON file

        Returns:
            bool: True, unless the service name was not provided
        """
        if self.__core_inited is False:
            self.__logger.warn('start_service::CoreAPI is not inited')
            return False
        return self.__api_wrapper.run_service(service)

    def stop_service(self, service):
        """Stops the given service

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            service (str): Name of the service

        Returns:
            bool: True if the service stop operation is successful,
                  False otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('stop_service::CoreAPI is not inited')
            return False
        return self.__api_wrapper.stop_service(service)

    def start_default_services(self):
        """Starts default services

        Example:
            Usage example

        Note:
            Specific notes

        Returns:
            bool: True if the service operation is successful,
                  False otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('start_default_services::' +
                               'CoreAPI is not inited')
            return False
        return self.__api_wrapper.start_default_services()

    def publish_message(self, target, path, data):
        """Publishes data to given target and path

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            target (str): Name of the target
            path (str): Path fo the target
            data (str): Data to be sent

        Returns:
            bool: True if the operation is successful, False otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('publish_message::CoreAPI is not inited')
            return False
        return self.__api_wrapper.publish_message(target, path, data)

    def set_app_configuration_file_path(self, path, is_relative):
        """Sets the application configuration file

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            path (str): Path ???
            is_relative (bool): Sets the configuration file path is relative to
                                the executable

        Returns:
            bool: True if the operation is successful, False otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('set_app_configuration_file_path::CoreAPI' +
                               ' is not inited')
            return False
        return self.__api_wrapper.set_app_configuration_file_path(path,
                                                                  is_relative)

    def set_app_configuration(self, name, value):
        """Sets the configuration with given name and value

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            name (str): Name of the configuration property
            value (str): Value of the property

        Returns:
            bool: True if the operation is successful, False otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('set_app_configuration::CoreAPI is not inited')
            return False
        return self.__api_wrapper.set_app_configuration(name, value)

    def get_app_configuration(self, name, default_value):
        """Returns the configuration value with given name. If property is not
           exists returns default_value

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            name (str): Name of the property
            default_value (str): Default value for the given property

        Returns:
            str: Return the value of the given property if not exist returns
                 given default_value
        """
        if self.__core_inited is False:
            self.__logger.warn('get_app_configuration::CoreAPI is not inited')
            return ''
        return self.__api_wrapper.get_app_configuration(name, default_value)

    def is_core_inited(self):
        """Returns the core is initated status

        Example:
            Usage example

        Note:
            Specific notes

        Returns:
            bool: True if the core is initated, False otherwise
        """
        return self.__core_inited

    def refresh_devices(self):
        """Refreshes the devices

        Example:
            Usage example

        Note:
            Specific notes

        Returns:
            bool: True if the operation is successful, False otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('refresh_devices::CoreAPI is not inited')
            return False
        return self.__api_wrapper.refresh_devices()

    def get_camera_list(self):
        """Returns the Devices object that contains camera objects

        Example:
            Usage example

        Note:
            Specific notes

        Returns:
            Devices: Devices object if the operation is successful,
                     None otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('get_camera_list::CoreAPI is not inited')
            return None
        return self.__api_wrapper.get_camera_list()

    def get_speaker_list(self):
        """Returns the Devices object that contains speaker objects

        Example:
            Usage example

        Note:
            Specific notes

        Returns:
            Devices: Devices object if the operation is successful,
                     None otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('get_speaker_list::CoreAPI is not inited')
            return None
        return self.__api_wrapper.get_speaker_list()

    def get_mic_list(self):
        """Returns the Devices object that contains microphone objects

        Example:
            Usage example

        Note:
            Specific notes

        Returns:
            Devices: Devices object if the operation is successful,
                     None otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('get_mic_list::CoreAPI is not inited')
            return None
        return self.__api_wrapper.get_mic_list()

    def update_callback(self,*args,**kwargs):
        self.__api_wrapper.update_loop()
    def update_loop(self):
        """Uptdates the loop

        Example:
            Usage example

        Note:
            Specific notes

        Returns:
            bool: True if the operation is successful, False otherwise
        """
        if self.__core_inited is False:
            return False

        if self.__update_loop_event:
            self.__update_loop_event.set()
            self.__update_loop_event = None

        self.__update_loop_event = set_interval(self.update_callback, 50,args=self)
        return True

    def get_device_id(self):
        """Returns the device id

        Example:
            Usage example

        Note:
            Specific notes

        Returns:
            str: Device id if the operation is successful, None otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('get_device_id::CoreAPI is not inited')
            return None
        return self.__api_wrapper.get_device_id()

    def get_ip_address(self, interface):
        """Returns the ip address of given interface

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            interface (str): interface name

        Returns:
            str: Ip address of the given interface, None otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('get_ip_address::CoreAPI is not inited')
            return None
        return self.__api_wrapper.get_ip_address(interface)

    def get_mac_address(self, interface):
        """Returns the mac address of given interface

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            interface (str): interface name

        Returns:
            str: mac address of the given interface, None otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('get_mac_address::CoreAPI is not inited')
            return None
        return self.__api_wrapper.get_mac_address(interface)

    def get_sdk_version(self, sdk):
        """Returns the sdk info of the application

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            sdk (str): sdk name

        Returns:
            str: SDK info of given application, None otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('get_sdk_version::CoreAPI is not inited')
            return None
        return self.__api_wrapper.get_sdk_version(sdk)

    def get_application_path(self):
        """Returns the application path

        Example:
            Usage example

        Note:
            Specific notes

        Returns:
            str: Application path
        """
        return self.__app_path

    def log_message(self, category, message, level, verbose):
        """Logs a message to given category, level and verbosity

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            category (str): Log category name
            message (str): Message to be logged
            level (LogLevel): Log level enum
            verbose (???): ???

        Returns:
            bool: True if the operation is successful, False otherwise
        """
        if self.__core_inited is False:
            self.__logger.warn('log_message::CoreAPI is not inited')
            return False
        return self.__api_wrapper.log_message(category,
                                              message,
                                              level,
                                              verbose)
