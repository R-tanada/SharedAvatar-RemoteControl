# This Python file uses the following encoding: utf-8
from CorePython.CoreAPI import CoreAPI
from CorePython.CoreVideo import CoreVideo
import threading
import random

import time

core = CoreAPI()
video = None
stopped = False
width = 640
height = 480


def generate_video_data():
    print("Video data")
    if stopped is True:
        return

    video_data = [0 for i in range(width * height * 3)]

    for i in range(0, width):
        for j in range(0, height):
            index = (j * width + i) * 3
            video_data[index + 0] = random.randint(0, 255)
            video_data[index + 1] = random.randint(0, 255)
            video_data[index + 2] = random.randint(0, 255)

    video.write_frame(width, height, video.VideoFormat.RGB, video_data)
    threading.Timer(5, generate_video_data).start()


def stop():
    global stopped
    stopped = True
    global video
    video.destroy()
    video = None
    global core
    core.destroy_api()
    core = None
    print('Core destroyed')


if __name__ == "__main__":

    core.init_api(app_path='./')
    video = CoreVideo('local-image')
    video.start()

    generate_video_data()

    try:
        while True:
            time.sleep(1)
            pass
    except KeyboardInterrupt:
        stop()
