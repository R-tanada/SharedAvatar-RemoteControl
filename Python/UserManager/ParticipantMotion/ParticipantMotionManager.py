# -----------------------------------------------------------------------
# Author:   Takayoshi Hagiwara (KMD)
# Created:  2021/10/6
# Summary:  操作者の動きマネージャー
# -----------------------------------------------------------------------

import threading
import time
import numpy as np
import csv

# ----- Custom class ----- #
from UDP.UDPManager import UDPManager
from OptiTrack.OptiTrackStreamingManager import OptiTrackStreamingManager
from BendingSensor.BendingSensorManager import BendingSensorManager

# ----- Numeric range remapping ----- #
# targetMin   = 200
targetMin   = 150
targetMax   = 850
originalMin = 0
originalMax = 1

# ----- Settings: Recorded motion data ----- #
recordedMotionPath              = 'RecordedMotion/'
recordedMotionFileName          = 'Transform_Participant_'
recordedGripperValueFileName    = 'GripperValue_'

class ParticipantMotionManager:
    def __init__(self, defaultParticipantNum: int, recordedParticipantNum: int = 0, motionInputSystem: str = 'optitrack', mocapServer: str = '', mocapLocal: str = '', gripperInputSystem: str = 'bendingsensor', bendingSensorNum: int = 1, BendingSensor_ConnectionMethod: str = 'wireless', recordedGripperValueNum: int = 0, bendingSensorUdpIpAddress: str = '192.168.80.142', bendingSensorUdpPort: list = [9000, 9001], bendingSensorSerialCOMs: list = []) -> None:
        """
        Initialize

        Parameters
        ----------
        defaultParticipantNum: int
            Number of participants
        recordedParticipantNum: (Optional) int
            Number of recorded motion of participants.
            If 0, only the movements of the actual participants are applied.
            If not 0, load the data from the "recordedMotionPath" + "recordedMotionFileName".
            Recorded motions will be counted after the number of real participants.
            e.g. defaultParticipantNum = 2 => recordedParticipant = ['participant3'], ['participant4']...
        motionInputSystem: (Optional) str
            Source of participant motion data
            Options: optitrack, unity
        gripperInputSystem: (Optional) str
            Source of data for gripper control
            Options: bendingsensor, unity, debug
        bendingSensorNum: (Optional) int
            Number of bendingsensors
        recordedGripperValueNum: (Optional): int
            Number of recorded gripper value.
            If 0, only the gripper value of the real time data are applied.
            If not 0, load the data from the "recordedMotionPath" + "recordedGripperValueFileName".
            Recorded gripper value will be counted after the number of real sensor.
            e.g. bendingSensorNum = 1 => recordedGripperValue = ['gripperValue2'], ['gripperValue3']...
        bendingSensorUdpIpAddress: (Optional) str
            IP address for bending sensor UDP streaming
        bendingSensorUdpPort: (Optional) list[int]
            Port number for bending sensor UDP streaming
        """

        self.defaultParticipantNum      = defaultParticipantNum
        self.recordedParticipantNum     = recordedParticipantNum
        self.motionInputSystem          = motionInputSystem
        self.gripperInputSystem         = gripperInputSystem
        self.bendingSensorNum           = bendingSensorNum
        self.recordedGripperValueNum    = recordedGripperValueNum
        self.udpManager                 = None
        self.recordedMotion             = {}
        self.recordedGripperValue       = {}
        self.recordedMotionLength       = []
        self.InitBendingSensorValues    = []

        # ----- Initialize participants' motion input system ----- #
        if motionInputSystem == 'optitrack':
            self.optiTrackStreamingManager = OptiTrackStreamingManager(defaultParticipantNum=defaultParticipantNum,mocapServer=mocapServer,mocapLocal=mocapLocal)

            # ----- Start streaming from OptiTrack ----- #
            streamingThread = threading.Thread(target=self.optiTrackStreamingManager.stream_run)
            streamingThread.setDaemon(True)
            streamingThread.start()

        elif motionInputSystem == 'unity':
            # The number of participants will be adjusted by Unity, so it is assumed that only one participant will be received here.
            self.defaultParticipantNum = 1
            self.udpManager = UDPManager(port=8000, localAddr='127.0.0.1')

            streamingThread = threading.Thread(target=self.udpManager.UpdateData)
            streamingThread.setDaemon(True)
            streamingThread.start()
        
            # Wait for UDP streaming
            time.sleep(0.1)
        
        # Recorded motions will be counted after the number of real participants.
        # e.g. defaultParticipantNum = 2 => recordedParticipant = ['participant3'], ['participant4']...
        for i in range(recordedParticipantNum):
            with open(recordedMotionPath + recordedMotionFileName + str(i+1) + '.csv') as f:
                reader = csv.reader(f)
                data = [row for row in reader][1:]                  # Remove header
                data = [[float(v) for v in row] for row in data]    # Convert to float
                self.recordedMotion['participant' + str(defaultParticipantNum + i+1)] = data
                self.recordedMotionLength.append(len(data))
        
        # ----- Initialize gripper control system ----- #
        if gripperInputSystem == 'bendingsensor':
            self.bendingSensors             = []

            if BendingSensor_ConnectionMethod == 'wireless':
                print('wireless')
                self.ip = [bendingSensorUdpIpAddress,bendingSensorUdpIpAddress]
                self.port = bendingSensorUdpPort
            elif BendingSensor_ConnectionMethod == 'wired':
                print('wired')
                self.ip = bendingSensorSerialCOMs
                self.port = [115200, 115200]

            for i in range(bendingSensorNum):
                bendingSensorManager = BendingSensorManager(BendingSensor_connectionmethod=BendingSensor_ConnectionMethod, ip=self.ip[i], port=self.port[i])
                self.bendingSensors.append(bendingSensorManager)

                # ----- Start receiving bending sensor value from UDP socket ----- #
                bendingSensorThread = threading.Thread(target=bendingSensorManager.StartReceiving)
                bendingSensorThread.setDaemon(True)
                bendingSensorThread.start()

            # ----- Set init value ----- #
            self.SetInitialBendingValue()       
        
        elif gripperInputSystem == 'unity':
            if not self.udpManager:
                self.defaultParticipantNum = 1
                self.udpManager = UDPManager(port=8000, localAddr='127.0.0.1')

                streamingThread = threading.Thread(target=self.udpManager.UpdateData)
                streamingThread.setDaemon(True)
                streamingThread.start()
            
                # Wait for UDP streaming
                time.sleep(0.1)
        # Recorded motions will be counted after the number of bending sensors.
        # e.g. bendingSensorNum = 1 => recordedGripperValue = ['gripperValue2'], ['gripperValue3']...
        for i in range(recordedGripperValueNum):
            with open(recordedMotionPath + recordedGripperValueFileName + str(i+1) + '.csv') as f:
                reader = csv.reader(f)
                data = [row for row in reader][1:]                  # Remove header
                data = [[float(v) for v in row] for row in data]    # Convert to float
                self.recordedGripperValue['gripperValue' + str(bendingSensorNum + i+1)] = data

    def SetInitialBendingValue(self):
        """
        Set init bending value
        """
        
        if self.gripperInputSystem == 'bendingsensor':
            self.InitBendingSensorValues    = []

            for i in range(self.bendingSensorNum):
                self.InitBendingSensorValues.append(self.bendingSensors[i].bendingValue)

    def LocalPosition(self, loopCount: int = 0):
        """
        Local position

        Parameters
        ----------
        loopCount: (Optional) int
            For recorded motion.
            Count of loop.

        Returns
        ----------
        participants' local position: dict
        {'participant1': [x, y, z]}
        unit: [m]
        """

        dictPos = {}
        if self.motionInputSystem == 'optitrack':
            dictPos = self.optiTrackStreamingManager.position
        elif self.motionInputSystem == 'unity': 
            data = self.udpManager.data
            position = np.array([float(data[data.index('pos')+1]), float(data[data.index('pos')+2]), float(data[data.index('pos')+3])])
            dictPos['participant1'] = position
        
        # If the data is ended, the last value is returned.
        for i in range(self.recordedParticipantNum):
            recordedParticipantNum = self.defaultParticipantNum + i+1
            if loopCount >= self.recordedMotionLength[i]:
                dictPos['participant'+str(recordedParticipantNum)] = np.array(self.recordedMotion['participant'+str(recordedParticipantNum)][-1][0:3])
                continue

            dictPos['participant'+str(recordedParticipantNum)] = np.array(self.recordedMotion['participant'+str(recordedParticipantNum)][loopCount][0:3])

        return dictPos

    def LocalRotation(self, loopCount: int = 0):
        """
        Local rotation

        Parameters
        ----------
        loopCount: (Optional) int
            For recorded motion.
            Count of loop.

        Returns
        ----------
        participants' local rotation: dict
        {'participant1': [x, y, z, w] or [x, y, z]}
        """

        dictRot = {}
        if self.motionInputSystem == 'optitrack':
            dictRot = self.optiTrackStreamingManager.rotation
        elif self.motionInputSystem == 'unity':
            data = self.udpManager.data
            # ----- Rotation (x, y, z, w) or (x, y, z) ----- #
            if 'rotEuler' in data:
                rotation = float(data[data.index('rotEuler')+1]), float(data[data.index('rotEuler')+2]), float(data[data.index('rotEuler')+3])
            elif 'rotQuaternion' in data:
                rotation = np.array([float(data[data.index('rotQuaternion')+1]), float(data[data.index('rotQuaternion')+2]), float(data[data.index('rotQuaternion')+3]), float(data[data.index('rotQuaternion')+4])])
            dictRot['participant1'] = rotation
        
        # If the data is ended, the last value is returned.
        for i in range(self.recordedParticipantNum):
            recordedParticipantNum = self.defaultParticipantNum + i+1
            if loopCount >= self.recordedMotionLength[i]:
                dictRot['participant'+str(recordedParticipantNum)] = np.array(self.recordedMotion['participant'+str(recordedParticipantNum)][-1][3:8])
                continue

            dictRot['participant'+str(recordedParticipantNum)] = np.array(self.recordedMotion['participant'+str(recordedParticipantNum)][loopCount][3:8])
        
        return dictRot
    
    def GripperControlValue(self, loopCount: int = 0):
        """
        Value for control of the xArm gripper

        Parameters
        ----------
        loopCount: (Optional) int
            For recorded motion.
            Count of loop.

        Returns
        ----------
        Value for control of the xArm gripper: dict
        {'gripperValue1': float value}
        """
        
        if self.gripperInputSystem == 'bendingsensor':
            dictGripperValue = {}
            for i in range(self.bendingSensorNum):
                bendingVal = self.bendingSensors[i].bendingValue
                bendingValueNorm = bendingVal* (targetMax - targetMin) + targetMin

                if bendingValueNorm > targetMax:
                    bendingValueNorm = targetMax
                elif bendingValueNorm < targetMin:
                    bendingValueNorm = targetMin
                dictGripperValue['gripperValue'+str(i+1)] = bendingValueNorm

        elif self.gripperInputSystem == 'unity':
            dictGripperValue = {}
            data = self.udpManager.data
            if 'trigger' in data:
                triggerValue = float(data[data.index('trigger')+1])
                triggerValueNorm = (triggerValue - originalMin) / (originalMax - originalMin) * (targetMax - targetMin) + targetMin
                dictGripperValue['gripperValue1'] = targetMax - triggerValueNorm
        
        elif self.gripperInputSystem == 'debug':
            dictGripperValue = {}
            dictGripperValue['gripperValue1'] = targetMax / 2
        
        # If the data is ended, the last value is returned.
        for i in range(self.recordedGripperValueNum):
            recordedGripperNum = self.bendingSensorNum + i+1
            if loopCount >= self.recordedMotionLength[i]:
                dictGripperValue['gripperValue'+str(recordedGripperNum)] = np.array(self.recordedGripperValue['gripperValue'+str(recordedGripperNum)][-1][0])
                continue

            dictGripperValue['gripperValue'+str(recordedGripperNum)] = np.array(self.recordedGripperValue['gripperValue'+str(recordedGripperNum)][loopCount][0])
            
        return dictGripperValue