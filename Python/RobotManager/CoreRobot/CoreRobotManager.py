import json
import sys
import threading

from logs import logger

sys.path.append('../')
from sdk.AvatarCore.bin.CorePython.CoreAPI import CoreAPI
from sdk.AvatarCore.bin.CorePython.CoreResourceManager import \
    CoreResourceManager
from sdk.AvatarCore.bin.CorePython.CoreSession import CoreSession


class CoreRobotManager:
    def __init__(self, Avatar_Peer_ID):
        self.avatarPeerID = Avatar_Peer_ID

        self.core_resources = None
        self.core_start = False
        self.core_finished = False
        self.new_data_frame = False
        self.core_mutex = threading.Lock()
        self.joined_dict = {}

        self.__init_core()

    def __init_core(self):
        self.core_api = CoreAPI()
        self.core_session = CoreSession(self.core_api)

        self.core_api.init_api(app_path='../',
                               master=True,
                               port=0,
                               app_name='RobotClient')
        logger.info('Start the core_api')

        self.core_api.load_services('core_services/core_robot_cloud_services.json')
        self.core_api.load_services('core_services/core_robot_services.json')
        self.core_api.start_default_services()
        logger.info('Start the default services')

        self.core_api.subscribe_to_message('avatarin', 'cloud/status', self.__cloud_status)
        self.core_api.subscribe_to_message('avatarin', 'cloud/peer/joined', self.__peer_joined)
        self.core_api.subscribe_to_message('avatarin', 'cloud/peer/disconnected', self.__peer_disconnected)

        self.core_session.init_session('Robot/cloud')
        self.core_session.set_peer_id(self.avatarPeerID)
        self.core_api.start_service('Robot/.*')
        logger.info('Start the services')

        self.__create_resource()

    def __cloud_status(self, name, path, data):
        logger.info('Cloud status: {0}'.format(data))

    def __peer_joined(self, name, path, data):
        logger.debug('id:%s'%data)
        if data not in self.joined_dict.values():
            ID = self.__id_mapping(data)
            self.core_write_data('feedback-to-user',
                                'id',
                                {'PeerID':data, 'UserID':ID})
            self.__subscribe_resource(ID)

        logger.info('Joined remote peerID: %s'%data)
        logger.info('Joined userID: %s'%ID)
        self.core_start = True

    def __id_mapping(self, data):
        dict_keys = list(self.joined_dict.keys())
        dict_keys.sort()
        dict_keys = [int(i) for i in dict_keys]

        if dict_keys:
            for i in range(len(dict_keys)):
                if dict_keys[0] == 1:
                    if len(dict_keys) >= 2:
                        try:
                            if dict_keys[i+1] - dict_keys[i] == 1:
                                ID = i+3
                                self.joined_dict[str(ID)] = [data, None, None]
                        except:
                            pass
                    else:
                        ID = 2
                        self.joined_dict[str(ID)] = [data, None, None]
                else:
                    ID = 1
                    self.joined_dict[str(ID)] = [data, None, None]
        else:
            ID = 1
            self.joined_dict[str(ID)] = [data, None, None]
        logger.debug('mapping:%s'%self.joined_dict)

        return ID

    def __peer_disconnected(self, name, path, data):
        data = json.loads(data)

        for key, value in self.joined_dict.items():
            if str(data['id']) in value:
                ID = key
                self.joined_dict.pop(key)
                break

        logger.info('Disconnected peerID: %s'%data)
        logger.info('Disconnected userID: %s'%ID)

    def __create_resource(self):
        self.core_resources = CoreResourceManager(self.__on_resource_frame_arrived)
        self.core_resources.start(1)

        logger.info('Start the core_resources')

    def __subscribe_resource(self, ID):
        self.core_resources.subscribe_data('user-%s-data'%ID,
                                           'motion-data')

    def __on_resource_frame_arrived(self, type, resource, channel, frame):
        if type == 'data':
            self.__on_data_frame_arrived(resource, channel, frame)

    def __on_data_frame_arrived(self, resource, channel, frame):
        data = json.loads(frame)

        for i in self.joined_dict.keys():
            if resource == 'user-%s-data'%i:
                if channel == 'motion-data':
                    self.core_mutex.acquire()

                    self.joined_dict[i] = data
                    logger.info('receive data : %s - %s - %s'%(resource, channel, frame))

                    self.core_mutex.release()

    def core_write_data(self, resource, channel, message):
        message = json.dumps(message)
        self.core_resources.write_data(resource, channel, message)
        logger.info('send data : %s - %s - %s'%(resource, channel, message))

    def stop_core(self):
        self.core_session.disconnect_from_peer(self.avatarPeerID)
        self.core_session.clear_session_callbacks()
        logger.info('Stop the core_session')

        self.core_resources.destroy()
        self.core_resources = None
        logger.info('Stop the core_resources')

        self.core_finished = self.core_api.destroy_api()
        logger.info('Stop the core_api')