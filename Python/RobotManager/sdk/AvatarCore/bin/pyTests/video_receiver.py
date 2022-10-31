# This Python file uses the following encoding: utf-8
from CorePython.CoreAPI import CoreAPI
from CorePython.CoreVideo import CoreVideo
import CorePython.Video

import time

core = CoreAPI()


def camera_frame_callback(resource, frame):
    print(dir(frame))
    print("New frame arrived: " +
          "{0} x {1}".format(frame.width, frame.height) +
          " - Format: {0}".format(CorePython.Video.VideoFormat(frame.format)))


def stop():
    global video
    video.destroy()
    video = None
    global core
    core.destroy_api()
    core = None
    print("Core destroyed")


if __name__ == "__main__":
    core.init_api(app_path="./")

    # Subscribe to a video resource named "local-image"
    video = CoreVideo("local-image", camera_frame_callback)
    video.start()

    try:
        while True:
            time.sleep(1)
            pass
    except KeyboardInterrupt:
        stop()
