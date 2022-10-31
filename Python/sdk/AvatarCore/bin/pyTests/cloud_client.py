# This Python file uses the following encoding: utf-8
from CorePython.CoreAPI import CoreAPI
from CorePython.CoreSession import CoreSession
from CorePython.Timeout import set_timeout

import time

core_api = CoreAPI()
core_session = CoreSession(core_api)

clientPeerID= 509091
avatarPeerID= 509090

def stop():
    global core_session
    # Tell the cloud service that we are disconnecting for the avatar
    core_session.disconnect_from_peer(avatarPeerID)

    # Clear session
    core_session.clear_session_callbacks()

    # Destroy CoreSDK
    global core_api
    core_api.destroy_api()
    print('Core Destroyed!')


def cloud_status(name, path, data):
    # This call back will print out the status of the cloud
    print('Cloud status: {0}'.format(data))


if __name__ == "__main__":
    # Initialize CoreSDK, use master as False if you will have a different master process for the same port 
    # (if 0, it will use a preset value inside the CoreSDK)
    core_api.init_api(app_path='../', master=True, port=0, app_name="CloudClient")
    # Load cloud client service file, it contains the "CloudProxy" service that is responsible to connect to the cloud, 
    # as well as several RTPSender and RTPReceiver services in charge to encode/decode streams of video/audio/data resources
    core_api.load_services('services/CloudClient-services.json')
    # Start up default services (those defined in JSON service file with "DefaultRunning": "true")
    core_api.start_default_services()
    # Subscribe a callback to the cloud proxy status topic ('cloud/status'), 
    # when cloud status is changed (Offline <--> Online), this callback will be triggered with the status value
    core_api.subscribe_to_message('avatarin', 'cloud/status', cloud_status)

    # Use CoreSession to initialize cloud proxy. Use the alias name of the CloudProxy service defined in the (CloudClient-services.json)
    core_session.init_session('Client/cloud')

    # Setup peerID (please use the one issued by avatarin)
    core_session.set_peer_id(clientPeerID)

    # startup RTP services (use regular expression on the service alias name defined in the CloudClient-services.json services file)
    core_api.start_service('Client/.*')

    def __connect_to_session():
        # Connect to the avatar
        core_session.connect_to_peer(avatarPeerID)

    # Don't immediately connect to the peer as the cloud is still initializing, 
    # either use the timeout, or use the status callback to connect to the avatar
    set_timeout(__connect_to_session, 2000)

    try:
        while True:
            time.sleep(1)
            pass
    except KeyboardInterrupt:
        stop()
