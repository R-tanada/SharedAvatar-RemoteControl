# This Python file uses the following encoding: utf-8
from CorePython.CoreAPI import CoreAPI
from CorePython.CoreData import CoreData

import time

core = CoreAPI()
data_wrapper = None


def on_data_frame_arrived(resource, channel, frame):
    print(resource + ':' + channel)
    print(frame)


def stop():
    global data_wrapper
    data_wrapper.destroy()
    data_wrapper = None
    global core
    core.destroy_api()
    core = None
    print('Core destroy')


if __name__ == "__main__":
    core.init_api(app_path='../',port=0)
    data_wrapper = CoreData('local-data',
                            'app',
                            on_data_frame_arrived)
    data_wrapper.start()

    try:
        while True:
            time.sleep(1)
            pass
    except KeyboardInterrupt:  # Captures Ctrl + C
        stop()
