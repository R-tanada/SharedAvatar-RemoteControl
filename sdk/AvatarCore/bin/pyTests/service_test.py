# This Python file uses the following encoding: utf-8
from CorePython.CoreAPI import CoreAPI

import time

core = CoreAPI()


def loadedCallback(service, path, data):
    print('Subscriber callback in Python--> {0} : {1}'.format(path, data))


def startedCallback(service, path, data):
    print('Subscriber callback in Python--> {0} : {1}'.format(path, data))


def test(category, msg, level, vl):
    print('Test: {0} : {1} : {2} : {3}'.format(category, msg, level, vl))


def stop():
    # Clean up the CoreSDK
    global core
    print('Unsubscribing callbacks')
    core.unsubscribe_to_message('application', 'service/loaded')
    core.unsubscribe_to_message('application', 'service/started')
    print('API is going to be destroyed')
    core.destroy_api()
    core = None
    print('Core destroyed')


if __name__ == "__main__":
    core.init_api(app_path='./')

    # Subscribe to messages that are issued from the core.
    # The following are two examples that gets triggered when
    # services are loaded, and started.
    core.subscribe_to_message('application',
                              'service/loaded',
                              loadedCallback)
    core.subscribe_to_message('application',
                              'service/started',
                              startedCallback)

    # Load services from a json file, these services can be considered
    # as a small building blocks that handles peripheral devices like cameras,
    # microphones, or speakers. Also other type of services handles
    # data manipulation and video formatting
    core.load_services('services/Simple-services.json')

    # Start the loaded services, only if they were flagged as
    # "DefaultRunning":true
    core.start_default_services()

    try:
        while True:
            time.sleep(1)
            pass
    except KeyboardInterrupt:
        stop()
