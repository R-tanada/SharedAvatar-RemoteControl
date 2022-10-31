# This Python file uses the following encoding: utf-8
from CorePython.CoreAPI import CoreAPI
from CorePython.CoreAudio import CoreAudio

import time

core = CoreAPI()


def audio_frame_callback(resource, frame):
    print(dir(frame))
    print('New frame arrived: ' +
          '{0} x {1} - '.format(frame.channel, frame.rate) +
          'Format: {0}'.format(CoreAudio.AudioFormat(frame.format)) +
          '{0}'.format(frame.length))


def stop():
    global audio
    audio.destroy()
    audio = None
    global core
    core.destroy_api()
    core = None
    print("Core destroyed")


if __name__ == "__main__":
    core.init_api(app_path="../")
    audio = CoreAudio("local-audio", audio_frame_callback)
    audio.start()

    try:
        while True:
            time.sleep(1)
            pass
    except KeyboardInterrupt:
        stop()
