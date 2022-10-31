import json
from mimetypes import init
import time

from matplotlib import test

from UserManager.ParticipantMotion.ParticipantMotionManager import ParticipantMotionManager
from CoreUser.CoreUserManager import CoreUserManager
from ParticipantMotion.TestMotionManager import TestManager

loopCount = 0
setting_file = open('../setting.json', mode = 'r')
setValue = json.load(setting_file)

coreManager = CoreUserManager(Avatar_Peer_ID=11111111)
testManager = TestManager()
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

start_time = time.perf_counter()
loopTime = 0
target_framerate = 120

try:
    while True:

        
        # position = participantMotionManager.LocalPosition(loopCount)
        # rotation = participantMotionManager.LocalRotation(loopCount)
        position, rotation = testManager.create_Sinwave(loopTime)
        message = {'position':position['participant1'], 'rotation':rotation['participant1']}
        coreManager.core_write_data('motion-data', message)

        print('Position, Rotaton => {}, {}'.format(position, rotation))

        loopCount += 1

        # ---------- fix framerate ---------- #
        loopTime = time.perf_counter() - start_time
        if 1/loopTime >= target_framerate:
            pass
        else:
            time.sleep(1/target_framerate - loopTime)


except KeyboardInterrupt:
    print('finished')