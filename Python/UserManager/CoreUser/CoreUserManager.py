import json
import os
import sys
import threading
import numpy as np
# import logs
# from logs import logger

from Camera.VideoWindowManager import VideoWidgetManager

sys.path.append(os.pardir)
from sdk.AvatarCore.bin.CorePython.CoreAPI import CoreAPI
from sdk.AvatarCore.bin.CorePython.CoreResourceManager import \
    CoreResourceManager
from sdk.AvatarCore.bin.CorePython.CoreSession import CoreSession
from sdk.AvatarCore.bin.CorePython.Timeout import set_timeout

class CoreUserManager:
    def __init__(self, User_Peer_ID, Avatar_Peer_ID):
        self.user_ID = None
        self.avatarPeerID = Avatar_Peer_ID
        self.UserPeerID = User_Peer_ID

        self.new_frame = False
        self.frame_data = None
        self.resource_name = ''
        self.resource_mutex = threading.Lock()
        self.videoWindowManager = VideoWidgetManager()

        self.__init_core()

    def __init_core(self):
        self.core_api = CoreAPI()
        self.core_session = CoreSession(self.core_api)

        self.core_api.init_api(app_path='../',
                               master=True,
                               port=0,
                               app_name='UserClient')
        # logger.info('Start the core_api')
        print('Start the core_api')

        self.core_api.load_services('core_services/core_user_cloud_services.json')
        self.core_api.start_default_services()
        # logger.info('Start the default services')
        print('Start the default services')

        self.core_api.subscribe_to_message('avatarin', 'cloud/status', self.__cloud_status)
        self.core_api.subscribe_to_message('avatarin', 'cloud/peer/disconnected', self.__peer_disconnected)

        self.core_session.init_session('User/cloud')
        self.core_session.set_peer_id(self.UserPeerID)
        self.core_api.start_service('User/.*')
        # logger.info('Start the services')
        print('Start the services')

        self.__create_resource()
        set_timeout(self.__connect_to_session, 2000)

    def __cloud_status(self, name, path, data):
        print('Cloud status: {0}'.format(data))

    def __connect_to_session(self):
        self.core_session.connect_to_peer(self.avatarPeerID)
        # logger.info('Connect to session : %s'%self.avatarPeerID)
        print('Connect to session : %s'%self.avatarPeerID)

    def __peer_disconnected(self, name, path, data):
        # logger.info('Peer disconnected : %s'%data)
        print('Peer disconnected : %s'%data)
        sys.exit()

    def __create_resource(self):
        global core_resources

        core_resources = CoreResourceManager(self.__on_resource_frame_arrived)

        core_resources.subscribe_video('robot-camera')

        core_resources.subscribe_data('feedback-to-user', 'motion-data')

        core_resources.start(1)

    def __on_resource_frame_arrived(self, type, resource, channel, frame):
        if type == 'data':
            self.__on_data_frame_arrived(resource, channel, frame)

        elif type == 'video':
            self.__on_video_frame_arrived(resource, frame)

    def __on_data_frame_arrived(self, resource, channel, frame):
        # logger.debug('receive data : %s - %s - %s'%(resource, channel, frame))
        print('receive data : %s - %s - %s'%(resource, channel, frame))

        data = json.loads(frame)

        if channel == 'id':
            if self.user_ID is None:
                if self.UserPeerID == data['PeerID']:
                    self.user_ID = data['UserID']

        if 'feedback' in resource:
            if channel == 'robot-motion':
                if not data['user'] == str(self.user_ID):
                    self.robot_motion = [data['x'], data['y']]

    def __on_video_frame_arrived(self, resource, frame):
        try:
            self.resource_mutex.acquire()
            VideoWidgetManager.ChangeNewFrame(frame)
            self.resource_name = resource

            self.resource_mutex.release()
        except:
            # logger.error('Error in handle new frame!!!')
            print('Error in handle new frame!!!')

    def core_write_data(self, channel, message):
        global core_resources

        message = json.dumps(message)
        resource = 'user-%s-data'%self.user_ID
        core_resources.write_data(resource, channel, message)

        # logger.debug('send data : %s - %s - %s'%(resource, channel, message))
        print('send data : %s - %s - %s'%(resource, channel, message))

    def stop_core(self):
        global core_resources

        self.core_session.disconnect_from_peer(self.avatarPeerID)
        self.core_session.clear_session_callbacks()
        # logger.info('Stop the core_session')
        print('Stop the core_session')

        core_resources.destroy()
        core_resources = None
        # logger.info('Stop the core_resources')
        print('Stop the core_resources')

        self.core_api.destroy_api()
        self.core_finished = True
        # logger.info('Stop the core_api')
        print('Stop the core_api')

    def start_window(self):
        self.videoWindowManager.StartWindow()
