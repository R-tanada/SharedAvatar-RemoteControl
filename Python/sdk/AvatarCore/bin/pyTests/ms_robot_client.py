# This Python file uses the following encoding: utf-8
from CorePython.CoreAPI import CoreAPI
from CorePython.CoreSession import CoreSession
from CorePython.CoreData import CoreData
from CorePython.Timeout import set_timeout

import time
import json

core_api = CoreAPI()
core_session = CoreSession(core_api)
user_data = None
robot_data = None


avatarPeerID= 199580

def stop():
    global core_session
    # Tell the cloud service that we are disconnecting for the avatar
    core_session.disconnect_from_peer(avatarPeerID)

    # Clear session
    core_session.clear_session_callbacks()


    global user_data
    global robot_data

    user_data.destroy()
    user_data=None
    robot_data.destroy()
    robot_data=None

    # Destroy CoreSDK
    global core_api
    core_api.destroy_api()
    print('Core Destroyed!')


def cloud_status(name, path, data):
    # This call back will print out the status of the cloud
    print('Cloud status: {0}'.format(data))


def on_data_frame_arrived(resource, channel, frame):
    print("Data received from user side: \n\t"+ frame)
    # process data frame

    data=json.loads(frame)
    if(data==None):
        return
    if(data["Type"]=="control"):
        # parse control data frame
        print("Recived control data frame")
    elif(data["Type"]=="sensor"):
        # parse sensor data frame
        print("Recived sensor data frame")
    #.... define your own custom messages


def create_resources():
    global user_data
    global robot_data

    # create a data resource to be used for writing data and to be sent to the user
    robot_data=CoreData("robot-data","app",None)

    # create a listener for user data
    user_data=CoreData("user-data","app",on_data_frame_arrived)
    user_data.start()


if __name__ == "__main__":
    # Initialize CoreSDK, use master as False if you will have a different master process for the same port 
    # (if 0, it will use a preset value inside the CoreSDK)
    core_api.init_api(app_path='./', master=True, port=0, app_name="RobotClient")
    # Load cloud client service file, it contains the "CloudProxy" service that is responsible to connect to the cloud, 
    # as well as several RTPSender and RTPReceiver services in charge to encode/decode streams of video/audio/data resources
    core_api.load_services('services/Robot-services.json')
    core_api.load_services('services/RobotCloud-services.json')
    # Start up default services (those defined in JSON service file with "DefaultRunning": "true")
    core_api.start_default_services()
    # Subscribe a callback to the cloud proxy status topic ('cloud/status'), 
    # when cloud status is changed (Offline <--> Online), this callback will be triggered with the status value
    core_api.subscribe_to_message('avatarin', 'cloud/status', cloud_status)

    # Use CoreSession to initialize cloud proxy. Use the alias name of the CloudProxy service defined in the cloud service file
    core_session.init_session('Robot/cloud')

    # Setup peerID (please use the one issued by avatarin)
    core_session.set_peer_id(avatarPeerID)

    # startup RTP services (use regular expression on the service alias name defined in the CloudClient-services.json services file)
    core_api.start_service('Robot/.*')

    create_resources()


    try:
        while True:
            time.sleep(1)
            pass
    except KeyboardInterrupt:
        stop()
