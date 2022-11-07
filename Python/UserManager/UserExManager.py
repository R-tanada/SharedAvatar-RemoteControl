from asyncio import Task
import json
# from mimetypes import init
import time
import threading

# from matplotlib import test

# from ParticipantMotion.ParticipantMotionManager import ParticipantMotionManager
from CoreUser.CoreUserManager import CoreUserManager
from ParticipantMotion.TestMotionManager import TestManager

# setting_file = open('../../setting.json', mode = 'r')
# setValue = json.load(setting_file)

coreManager = CoreUserManager(User_Peer_ID = '214387', Avatar_Peer_ID = '199580')

def send_MotionData():
    taskStartTime = time.perf_counter()
    loopTime = 0
    loopCount = 0
    targetCycleTime = 0.01

    try:
        while True:
            if coreManager.core_start:
                loopStartTime = time.perf_counter() - taskStartTime

                # ---------- transform ---------- #
                # position = participantMotionManager.LocalPosition(loopCount)
                # rotation = participantMotionManager.LocalRotation(loopCount)

                # ---------- test ---------- #
                position, rotation = testManager.create_Sinwave(loopTime)
                message = {'position':position['participant1'], 'rotation':rotation['participant1']}
                coreManager.core_write_data('motion-data', message)

                # print('Position, Rotaton => {}, {}'.format(position, rotation))

                loopCount += 1

                # ---------- fix framerate ---------- #
                loopTime = time.perf_counter() - taskStartTime
                CycleTime = loopTime - loopStartTime
                if CycleTime >= targetCycleTime:
                    pass
                else:
                    time.sleep(targetCycleTime - CycleTime)
            
            else:
                pass

    except KeyboardInterrupt:
        print('finished')

testManager = TestManager()
motion_thred = threading.Thread(target = send_MotionData, daemon = True)
motion_thred.start()

coreManager.start_window()


# participantMotionManager = ParticipantMotionManager(defaultParticipantNum = setValue['participantNum'],
#                                                     recordedParticipantNum = setValue['recordedParticipantMotionCount'],
#                                                     motionInputSystem = setValue['motionDataInputMode'],
#                                                     mocapServer = setValue['motiveserverIpAddress'],
#                                                     mocapLocal = setValue['motivelocalIpAddress'],
#                                                     gripperInputSystem = setValue['gripperDataInputMode'], 
#                                                     bendingSensorNum = setValue['bendingSensorCount'],
#                                                     recordedGripperValueNum = setValue['recordedGripperValueCount'],
#                                                     BendingSensor_ConnectionMethod = setValue['BendingSensor_ConnectionMethod'],
#                                                     bendingSensorUdpIpAddress = setValue['wirelessIpAddress'],
#                                                     bendingSensorUdpPort = setValue['bendingSensorPorts'],
#                                                     bendingSensorSerialCOMs = setValue['bendingSensorComs'])


