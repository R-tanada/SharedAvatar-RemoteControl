# This Python file uses the following encoding: utf-8
from CorePython.CoreAPI import CoreAPI
from CorePython.CoreAudio import CoreAudio
import numpy
import threading
import random
import array

import time

core = CoreAPI()
audio = None
stopped = False
length = 1024
sampling_rate = 44100
channel = 2


def generate_audio_data():
    print("Audio data")
    if stopped is True:
        return

    global length
    # audio_data = numpy.array([0] * length, dtype=numpy.uint8)
    audio_data = [0 for i in range(length)]
    # audio_data = array.array('B', [0] * length)

    for i in range(0, length):
        audio_data[i] = random.randint(0, 255)

    global sampling_rate
    global channel
    global audio

    audio.write_frame(channel,
                      sampling_rate,
                      length,
                      CoreAudio.AudioFormat.U8,
                      audio_data)
    threading.Timer(5, generate_audio_data).start()


def stop():
    global stopped
    stopped = True
    global audio
    audio.destroy()
    audio = None
    global core
    core.destroy_api()
    core = None
    print('Core destroyed')


if __name__ == "__main__":

    core.init_api(app_path='./')
    audio = CoreAudio('local-audio')
    audio.start()

    generate_audio_data()

    try:
        while True:
            time.sleep(1)
            pass
    except KeyboardInterrupt:
        stop()
