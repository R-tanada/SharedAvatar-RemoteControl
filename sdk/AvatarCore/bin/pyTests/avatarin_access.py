# This Python file uses the following encoding: utf-8
from CorePython.CoreAPI import CoreAPI
from CorePython.CoreData import CoreData
from CorePython.CoreVideo import CoreVideo
import CorePython.Video

import time

# Create CoreAPI SDK Instance, should be only one
core_api = CoreAPI()

# initialize variables to be used
local_video = None
remote_video = None
remote_bottom_video = None
local_data = None
remote_data = None


def stop():
    # Destroy and clean up any loaded resources
    global local_video
    local_video.destroy()
    local_video = None

    global remote_video
    remote_video.destroy()
    remote_video = None

    global remote_bottom_video
    remote_bottom_video.destroy()
    remote_bottom_video = None

    global local_data
    local_data.destroy()
    local_data = None

    global remote_data
    remote_data.destroy()
    remote_data = None

    global core_api
    core_api.destroy_api()
    core_api = None


def on_video_frame_arrived(resource, frame):
    # This callback will be called when
    # new video frames arrives for the subscribed resource
    print('[{0}] New frame arrived: '.format(resource) +
          '{0} x {1} - '.format(frame.width, frame.height) +
          'Format: {0}'.format(CorePython.Video.VideoFormat(frame.format)))


def on_data_frame_arrived(resource, channel, frame):
    # This callback will be called when
    # new data frames arrives for the subscribed resource
    print('[{0} : {1}] Data Arrived: {2}'.format(resource, channel, frame))


if __name__ == "__main__":
    # Initialize the SDK. Set master to false
    # since there should be an app running in the background
    core_api.init_api(app_path='../', master=False)

    # Optional to load services from a json file
    # core_api.load_services('services/Simple-services.json')
    # core_api.start_default_services()

    # Create resources to subscribe into. Link the callback functions
    local_video = CoreVideo('local-camera', on_video_frame_arrived)
    remote_video = CoreVideo('remote-camera', on_video_frame_arrived)
    remote_bottom_video = CoreVideo('remote-bottom-camera',
                                    on_video_frame_arrived)

    local_data = CoreData('local-data', 'app', on_data_frame_arrived)
    remote_data = CoreData('remote-data',
                           'app',
                           on_data_frame_arrived)

    # Start listening to the subscribed resources
    local_video.start()
    remote_video.start()
    remote_bottom_video.start()

    local_data.start()
    remote_data.start()

    try:
        while True:
            time.sleep(1)
            pass
    except KeyboardInterrupt:
        stop()
