# This Python file uses the following encoding: utf-8
from CorePython.CoreAPI import CoreAPI

import logging
import json


class CoreSession:
    """Managers sessions for Cloud SDK
    """

    def __init__(self, core):
        """Initializes CoreSession object with given core

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            core (CoreAPI): core object reference
        """
        self.__internal_data = {'cloud_alias': 'Cloud/proxy'}
        self.__core_api = None
        self.__logger = logging.getLogger('Python::CoreSession')

        self.__logger.setLevel(logging.INFO)
        ch = logging.StreamHandler()
        ch.setLevel(logging.INFO)
        formatter = logging.Formatter('%(name)s -- %(asctime)s ' +
                                      '[%(levelname)s]\t- %(message)s ',
                                      datefmt='%I:%M:%S')
        ch.setFormatter(formatter)
        self.__logger.addHandler(ch)
        self.__core_api = core

    def init_session(self, cloud_alias):
        """Initializes session with given cloud_alias

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            cloud_alias (str): Name of the cloud alias
        """
        self.__internal_data['cloud_alias'] = cloud_alias

    def __status_callback(self, props, name, path, data):
        self.__logger.info('Cloud status is: ' + data)
        if props.CloudStatus is not None:
            props.CloudStatus(data)

    def __joining_callback(self, props, name, path, data):
        if props.PeerJoining is not None:
            props.PeerJoining(data)

    def __create_callback(self, props, name, path, data):
        if props.SessionCreated is not None:
            props.SessionCreated(data)
        self.__logger.info('Session was created')

    def __close_callback(self, props, name, path, data):
        if props.SessionClosed is not None:
            props.SessionClosed(data)
        self.__logger.info('Session was closed')

    def __receive_callback(self, props, name, path, data):
        if props.SessionReceived is not None:
            props.SessionReceived(data)
        self.__logger.info('Session was received')

    def __joined_callback(self, props, name, path, data):
        if props.PeerJoined is not None:
            props.PeerJoined(data)
        self.__logger.info('Remote peer successfuly joined our session')

    def __disconnect_callback(self, props, name, path, data):
        if props.PeerDisconnected is not None:
            props.PeerDisconnected(data)
        self.__logger.info('Remote peer disconnected')

    def setup_session_callbacks(self, props):
        """Setups the session callbacks with given props

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            props (???): ???
        """
        def __status(name, path, data):
            self.__status_callback(props,
                                   name,
                                   path,
                                   data)

        self.__core_api.subscribe_to_message('application',
                                             'cloud/status',
                                             __status)

        def __joining(name, path, data):
            self.__joining_callback(props,
                                    name,
                                    path,
                                    data)

        self.__core_api.subscribe_to_message('application',
                                             'cloud/peer/joining',
                                             __joining)

        def __create(name, path, data):
            self.__create_callback(props,
                                   name,
                                   path,
                                   data)

        self.__core_api.subscribe_to_message('application',
                                             'cloud/session/created',
                                             __create)

        def __close(name, path, data):
            self.__close_callback(props,
                                  name,
                                  path,
                                  data)

        self.__core_api.subscribe_to_message('application',
                                             'cloud/session/closed',
                                             __close)

        def __receive(name, path, data):
            self.__receive_callback(props,
                                    name,
                                    path,
                                    data)

        self.__core_api.subscribe_to_message('application',
                                             'cloud/session/received',
                                             __receive)

        def __joined(name, path, data):
            self.__joined_callback(props,
                                   name,
                                   path,
                                   data)

        self.__core_api.subscribe_to_message('application',
                                             'cloud/peer/joined',
                                             __joined)

        def __disconnect(name, path, data):
            self.__disconnect_callback(props,
                                       name,
                                       path,
                                       data)

        self.__core_api.subscribe_to_message('application',
                                             'cloud/peer/disconnected',
                                             __disconnect)

    def clear_session_callbacks(self):
        """Clears the session callbacks
        """
        self.__core_api.unsubscribe_to_message('application',
                                               'cloud/status')
        self.__core_api.unsubscribe_to_message('application',
                                               'cloud/peer/joining')
        self.__core_api.unsubscribe_to_message('application',
                                               'cloud/session/created')
        self.__core_api.unsubscribe_to_message('application',
                                               'cloud/session/closed')
        self.__core_api.unsubscribe_to_message('application',
                                               'cloud/peer/joined')
        self.__core_api.unsubscribe_to_message('application',
                                               'cloud/peer/disconnected')
        self.__core_api.unsubscribe_to_message('application',
                                               'cloud/session/received')

    def set_peer_id(self, peer_id):
        """Sets the peer id

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            peer_id (int): Peer id value
        """
        self.__logger.info('Setting up peer_id: {0}'.format(peer_id))
        peer_id_obj = {'peerID': str(peer_id)}
        self.__internal_data['local_peer_id'] = peer_id
        self.__core_api.publish_message(self.__internal_data['cloud_alias'],
                                        'cloud/peer/set',
                                        json.dumps(peer_id_obj))
        self.__core_api.publish_message(self.__internal_data['cloud_alias'],
                                        'cloud/status/get',
                                        '')

    def connect_to_peer(self, remote_peer_id):
        """Connects to the remove with given peer id

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            remote_peer_id (int): Remote peer id that wants to be connected
        """
        self.__logger.info('Connect to peer: ' + str(remote_peer_id))
        this_peer_id = self.__internal_data['local_peer_id']
        self.__internal_data['remote_peer_id'] = remote_peer_id
        if remote_peer_id is not None and this_peer_id is not remote_peer_id:
            self.__core_api.publish_message('',
                                            'cloud/joinpeer',
                                            str(remote_peer_id))

    def disconnect_from_peer(self, remote_peer_id):
        """Disconnects from given peer id

        Example:
            Usage example

        Note:
            Specific notes

        Args:
            remote_peer_id (int): Remote peer id
        """
        self.__core_api.unsubscribe_to_message('application',
                                               'cloud/peer/disconnected')
        this_peer_id = self.__internal_data['local_peer_id']
        if remote_peer_id == 0:
            remote_peer_id = self.__internal_data['remote_peer_id']

        if remote_peer_id is not None and this_peer_id is not remote_peer_id:
            self.__core_api.publish_message('',
                                            'cloud/disconnectpeer',
                                            str(remote_peer_id))
