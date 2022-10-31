# This Python file uses the following encoding: utf-8
from CorePython.CoreAPI import CoreAPI
from CorePython.CoreData import CoreData
from CorePython.CoreResourceManager import CoreResourceManager

import time

core_api = CoreAPI()


# Callback function for Data resources
def on_data_arrived(resource, channel, frame):
    print('Received a data resource: ' + resource)
    if channel == 'events':
        print('\t--> Event: ' + frame)

    elif channel == 'commands':
        print('\t--> Command: ' + frame)


# Callback function for a resource manager, notice different header
def on_resource_arrived(type, resource, channel, frame):
    print("Resource Arrived of type: " + type + "  - name: " + resource + "/" +
          channel)
    if (type == "data"):
        print("\t--> Data Contents: " + frame)
    return


if __name__ == "__main__":
    core_api.init_api(app_path='../', master=True, port=9594)

    # Create Data resources
    events_channel = CoreData('local-data', 'events', None)
    commands_channel = CoreData('local-data', 'commands', None)

    cmd_receiver = CoreData('local-data', 'commands', on_data_arrived)
    events_receiver = CoreData('local-data', 'events', on_data_arrived)

    # Alternatively, you can use CoreResourceManager to perform wider range of resource access
    resource_manager = CoreResourceManager(on_resource_arrived)
    resource_manager.subscribe_data('local-data', 'events')
    resource_manager.subscribe_data('local-data', 'commands')

    # Start listening to the subscribed resource
    cmd_receiver.start()
    events_receiver.start()
    resource_manager.start(1)

    # Send different data on different channels
    events_channel.send_data("woke up")
    events_channel.send_data("system ready")

    commands_channel.send_data("play music")
    # emulate some logic is going on before going to next data chunk
    time.sleep(0.1)

    # you can stop listening on specific resources like this
    events_receiver.stop()

    # send more data
    events_channel.send_data("music finished")
    commands_channel.send_data("clean up")

    # sleep for 100ms before cleaning up
    time.sleep(0.1)

    # Clean up all resources
    print("Cleaning up resources")
    events_channel.destroy()
    commands_channel.destroy()
    cmd_receiver.destroy()
    events_receiver.destroy()
    resource_manager.destroy()

    print("Shutting down CoreSDK")
    core_api.destroy_api()
