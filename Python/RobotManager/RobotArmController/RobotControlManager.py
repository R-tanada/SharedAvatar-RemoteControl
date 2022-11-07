# -----------------------------------------------------------------------
# Author:   Takayoshi Hagiwara (KMD)
# Created:  2021/8/19
# Summary:  Robot arm motion control manager
# -----------------------------------------------------------------------

import sys
import json
from datetime import datetime
from enum import Flag
import time
import datetime
import threading
from matplotlib.pyplot import flag
import numpy as np
# from ctypes import windll
from CoreRobot.CoreRobotManager import CoreRobotManager

# ----- Custom class ----- #
from RobotArmController.xArmTransform import xArmTransform
from CyberneticAvatarMotion.CyberneticAvatarMotionBehaviour import CyberneticAvatarMotionBehaviour
from Recorder.DataRecordManager import DataRecordManager
from LoadCell.LoadCellManager import LoadCellManager
from RobotArmController.WeightSliderManager import WeightSliderManager
# from Graph.Graph_2D import Graph_2D

sys.path.append('../')
from sdk.xarm.wrapper import XArmAPI

# ---------- Settings: Shared method ---------- #
sharedMethod = 'integration'

# ---------- Settings: Direction of participants ---------- #
directionOfParticipants  = 'same'
oppositeParticipants    = ['participant1']
inversedAxes = ['y', 'z']

# ----- Safety settings. Unit: [mm] ----- #
movingDifferenceLimit = 1000

class RobotControlManagerClass:
    def __init__(self) -> None:
        setting_file = open('/Users/yuzu/Documents/GitHub/ms-sharedavatar/Python/setting.json', mode = 'r')
        setValue = json.load(setting_file)
        self.xArmIpAddress           = setValue['xArmIP']
        self.localIpAddress          = setValue['localIP']
        self.motiveserverIpAddress   = setValue['mocapserverAddress']
        self.motivelocalIpAddress    = setValue['mocaplocalAddress']
        self.bendingSensorPorts      = setValue['bendingSensorSerialRate']
        self.bendingSensorComs       = setValue['bendingSensorSerialCOMs']
        self.bendinSensorNumber      = setValue['bendingsenosrNum'] 

    def SendDataToRobot(self, participantNum, executionTime: int = 9999, isFixedFrameRate: bool = False, frameRate: int = 90, isChangeOSTimer: bool = False, isExportData: bool = True, isEnablexArm: bool = True):
        """
        Send the position and rotation to the xArm

        Parameters
        ----------
        participantNum: int
            Number of participants
        executionTime: (Optional) int
            Unit: [s]
            Execution time
        isFixedFrameRate: (Optional) bool
            Use fixed frame rate.
            Default is depending on the PC specs.
        frameRate: (Optional) int
            Frame rate of the loop of this method
        isChangeOSTimer: (Optional, only for Windows) bool
            Change the Windows OS timer.
            ----- CAUTION -----
                Since this option changes the OS timer, it will affect the performance of other programs.
                Ref: https://python-ai-learn.com/2021/02/07/time/
                Ref: https://docs.microsoft.com/en-us/windows/win32/api/timeapi/nf-timeapi-timebeginperiod
        isExportData: (Optional) bool
            Export recorded data.
            Participants' motion data (Position: xyz, Quaternion: xyzw)
            Other rigid bodys' motion data (Position: xyz, Quaternion: xyzw)
            Gripper value.
        isEnablexArm: (Optional) bool
            For debug mode. If False, xArm will not be enabled.
        """

        # ----- Change OS timer ----- #
        if isFixedFrameRate and isChangeOSTimer:
            windll.winmm.timeBeginPeriod(1)

        # ----- Process info ----- #
        self.loopCount      = 0
        self.taskTime       = []
        self.errorCount     = 0
        taskStartTime       = 0

        # ----- Set loop time from frameRate ----- #
        loopTime        = 1 / frameRate
        loopStartTime   = 0
        processDuration = 0
        listFrameRate   = []
        if isFixedFrameRate:
            print('Use fixed frame rate > ' + str(frameRate) + '[fps]')


        # ----- Instantiating custom classes ----- #
        caBehaviour                         = CyberneticAvatarMotionBehaviour(defaultParticipantNum=participantNum)
        transform                           = xArmTransform()
        coreRobotManager                    = CoreRobotManager(Avatar_Peer_ID = 199580)
        # dataRecordManager                   = DataRecordManager(participantNum=participantNum, otherRigidBodyNum=otherRigidBodyCount)
        # weightSliderManager                 = WeightSliderManager(WeightSlider_ConnectionMethod='wireless',ip=self.wirelessIpAddress,port=self.weightSliderPort)
        # Graph2DManager                      = Graph_2D(n=2)

        # ----- Initialize robot arm ----- #
        if isEnablexArm:
            arm = XArmAPI(self.xArmIpAddress)
            self.InitializeAll(arm, transform)

        # bendingFeedback = LoadCellManager(arm)

        # ----- Control flags ----- #
        isMoving    = False

        # ----- Internal flags ----- #
        isPrintFrameRate    = False     # For debug
        isPrintData         = False     # For debug

        if coreRobotManager.core_start:
            try:
                while True:
                    # if time.perf_counter() - taskStartTime > executionTime:
                    #     # ----- Exit processing after task time elapses ----- #
                    #     isMoving    = False

                    #     self.taskTime.append(time.perf_counter() - taskStartTime)
                    #     self.PrintProcessInfo()

                    #     # ----- Export recorded data ----- #
                    #     if isExportData:
                    #         dataRecordManager.ExportSelf(dirPath='C:/Users/cmm13037/Documents/Nishimura',participant=self.participantname,conditions=self.condition)

                    #     # ----- Disconnect ----- #
                    #     if isEnablexArm:
                    #         arm.disconnect()

                    #     windll.winmm.timeEndPeriod(1)

                    #     print('----- Finish task -----')
                    #     break

                    if isMoving:
                        # ---------- Start control process timer ---------- #
                        loopStartTime = time.perf_counter()
                        # print(coreRobotManager.joined_dict['1'])
                        print('hello')

                        # ----- Get transform data----- #
                        # localPosition = coreRobotManager.data['position']
                        # localRotation = coreRobotManager.data['rotation']

                        # # ----- For opposite condition ----- #
                        # if directionOfParticipants == 'opposite':
                        #     for oppositeParticipant in oppositeParticipants:
                        #         # yz: Mirror mode, xz: Natural mode
                        #         localRotation[oppositeParticipant] = caBehaviour.InversedRotation(localRotation[oppositeParticipant], axes=inversedAxes)

                        # # ----- Calculate shared transform ----- #
                        # position,rotation = caBehaviour.GetSharedTransform(localPosition, localRotation)
                        # position = position * 1000

                        # # ----- Set xArm transform ----- #
                        # transform.x, transform.y, transform.z           = position[2], position[0], position[1]
                        # transform.roll, transform.pitch, transform.yaw  = rotation[2], rotation[0], rotation[1]


                        # # ----- Safety check (Position) ---- #
                        # diffX = transform.x - beforeX
                        # diffY = transform.y - beforeY
                        # diffZ = transform.z - beforeZ
                        # beforeX, beforeY, beforeZ = transform.x, transform.y, transform.z


                        # if diffX == 0 and  diffY == 0 and diffZ == 0 and  isFixedFrameRate:
                        #     print('[WARNING] >> Rigid body is not captured by the mocap camera.')
                        # elif abs(diffX) > movingDifferenceLimit or abs(diffY) > movingDifferenceLimit or abs(diffZ) > movingDifferenceLimit :
                        #     isMoving = False
                        #     print('[ERROR] >> A rapid movement has occurred. Please enter "r" to reset xArm, or "q" to quit')
                        # else:
                        #     if isEnablexArm:
                        #         # ----- Send to xArm ----- #
                        #         arm.set_servo_cartesian(transform.Transform(isLimit=False,isOnlyPosition=False))
                                # arm_2.set_servo_cartesian(transform_2.Transform(isLimit=False,isOnlyPosition=False))

                        # ----- Bending sensor ----- #
                        # dictBendingValue = coreRobotManager.data['grip_val']

                        # ----- Bending sensor for integration or one side ----- #
                        # if self.bendinSensorNumber == '1':
                        #     gripperValue_1 = dictBendingValue['gripperValue1']
                        # elif self.bendinSensorNumber == '2':
                        #     gripperValue_1 = (dictBendingValue['gripperValue1'] + dictBendingValue['gripperValue2'])/2

                        # ----- Gripper control ----- #
                        # if isEnablexArm:
                        #     code, ret = arm.getset_tgpio_modbus_data(self.ConvertToModbusData(gripperValue_1))

                        # ----- Data recording ----- #
                        # relativePosition = CyberneticAvatarMotionBehaviour.GetRelativePosition_r(position=localPosition)
                        # relativeRotation = CyberneticAvatarMotionBehaviour.GetRelativeRotation_r(rotation=localRotation)
                        # dataRecordManager.Record(relativePosition, relativeRotation, dictBendingValue, time.perf_counter()-taskStartTime)
                        # dataRecordManager.Record(localPosition, localRotation, dictBendingValue, time.perf_counter()-taskStartTime)

                        # ----- If xArm error has occured ----- #
                        if isEnablexArm and arm.has_err_warn:
                            isMoving    = False
                            self.errorCount += 1
                            self.taskTime.append(time.perf_counter() - taskStartTime)
                            print('[ERROR] >> xArm Error has occured. Please enter "r" to reset xArm, or "q" to quit')

                        # ---------- End control process timer ---------- #
                        processDuration = time.perf_counter() - loopStartTime  # For loop timer

                        # ----- Fixed frame rate ----- #
                        if isFixedFrameRate:
                            sleepTime = loopTime - processDuration
                            if sleepTime < 0:
                                pass
                            else:
                                time.sleep(sleepTime)

                        # ----- (Optional) Check frame rate ----- #
                        if self.loopCount % 20 == 0 and isPrintFrameRate:
                            if self.loopCount != 0:
                                listFrameRate.append(1 / (time.perf_counter() - loopStartTime))
                                print("Average FPS: ", sum(listFrameRate)/len(listFrameRate))

                        # ----- (Optional) Check data ----- #
                        # if isPrintData:
                        #     print('xArm transform > ' + str(np.round(transform.Transform(), 1)) + '   Bending sensor > ' + str(dictBendingValue))

                        self.loopCount += 1

                    else:
                        keycode = input('Input > "q": quit, "r": Clean error and init arm, "s": start control \n')
                        # ----- Quit program ----- #
                        if keycode == 'q':
                            if isEnablexArm:
                                arm.disconnect()
                            self.PrintProcessInfo()

                            windll.winmm.timeEndPeriod(1)
                            break

                        # ----- Reset xArm and gripper ----- #
                        elif keycode == 'r':
                            if isEnablexArm:
                                self.InitializeAll(arm, transform)
                                # self.InitRobotArm(arm, transform)
                                # self.InitGripper(arm)

                        # ----- Start streaming ----- #
                        elif keycode == 's':
                            originalPos = {'participant1':[300, 0, 230]}
                            # caBehaviour.SetOriginPosition(coreRobotManager.data['position'])
                            # caBehaviour.SetInversedMatrix(coreRobotManager.data['rotation'])

                            # # ----- weight slider list (For participantNum)  ----- #
                            # weightSliderList = []
                            # weightSliderList_0 = [1/participantNum for n in range(participantNum)]
                            # weightSliderList = [weightSliderList_0,weightSliderList_0]

                            # ----- weight slider list ----- #
                            # self.weightSliderListPos[0].remove('weightSliderListPos')
                            # self.weightSliderListRot[0].remove('weightSliderListRot')
                            # weightSliderListPosstr = self.weightSliderListPos[0]
                            # weightSliderListRotstr = self.weightSliderListRot[0]
                            # weightSliderListPosfloat = list(map(float,weightSliderListPosstr))
                            # weightSliderListRotfloat = list(map(float,weightSliderListRotstr))
                            # weightSliderList = [weightSliderListPosfloat, weightSliderListRotfloat]

                            # self.weightSliderList = [[0,1],[0,1]]
                            # self.weightSliderList = [[0.5,0.5],[0.5,0.5]]

                            # print(weightSliderList)

                            # position, rotation = caBehaviour.GetSharedTransform(coreRobotManager.data['position'], coreRobotManager.data['rotation'])
                            # beforeX, beforeY, beforeZ = position[2], position[0], position[1]

                            # participantMotionManager.SetInitialBendingValue()

                            isMoving    = True
                            taskStartTime = time.perf_counter()

            except KeyboardInterrupt:
                print('\nKeyboardInterrupt >> Stop: RobotControlManager.SendDataToRobot()')

                self.taskTime.append(time.perf_counter() - taskStartTime)
                self.PrintProcessInfo()

                # if isExportData:
                #     dataRecordManager.ExportSelf(dirPath='C:/Users/kimih/Documents/Nishimura',participant=self.participantname,conditions=self.condition,number=self.number)

                # ----- Disconnect ----- #
                if isEnablexArm:
                    arm.disconnect()

                windll.winmm.timeEndPeriod(1)

            except :
                print('----- Exception has occurred -----')
                windll.winmm.timeEndPeriod(1)
                import traceback
                traceback.print_exc()

    def InitRobotArm(self, robotArm, transform, isSetInitPosition = True):
        """
        Initialize the xArm

        Parameters
        ----------
        robotArm: XArmAPI
            XArmAPI object.
        transform: xArmTransform
            xArmTransform object.
        isSetInitPosition: (Optional) bool
            True -> Set to "INITIAL POSITION" of the xArm studio
            False -> Set to "ZERO POSITION" of the xArm studio
        """

        robotArm.connect()
        robotArm.motion_enable(enable=True)
        robotArm.set_mode(0)             # set mode: position control mode
        robotArm.set_state(state=0)      # set state: sport state

        if isSetInitPosition:
            robotArm.clean_error()
            robotArm.clean_warn()
            initX, initY, initZ, initRoll, initPitch, initYaw = transform.GetInitialTransform()
            robotArm.set_position(x=initX, y=initY, z=initZ, roll=initRoll, pitch=initPitch, yaw=initYaw, wait=True)
        else:
            robotArm.reset(wait=True)

        robotArm.motion_enable(enable=True)
        robotArm.set_mode(1)
        robotArm.set_state(state=0)

        time.sleep(0.5)
        print('Initialized > xArm')

    def InitGripper(self, robotArm):
        """
        Initialize the gripper

        Parameters
        ----------
        robotArm: XArmAPI
            XArmAPI object.
        """

        robotArm.set_tgpio_modbus_baudrate(2000000)
        robotArm.set_gripper_mode(0)
        robotArm.set_gripper_enable(True)
        robotArm.set_gripper_position(0, speed=5000)

        robotArm.getset_tgpio_modbus_data(self.ConvertToModbusData(425))

        time.sleep(0.5)
        print('Initialized > xArm gripper')

    def ConvertToModbusData(self, value: int):
        """
        Converts the data to modbus type.

        Parameters
        ----------
        value: int
            The data to be converted.
            Range: 0 ~ 800
        """

        if int(value) <= 255 and int(value) >= 0:
            dataHexThirdOrder = 0x00
            dataHexAdjustedValue = int(value)

        elif int(value) > 255 and int(value) <= 511:
            dataHexThirdOrder = 0x01
            dataHexAdjustedValue = int(value)-256

        elif int(value) > 511 and int(value) <= 767:
            dataHexThirdOrder = 0x02
            dataHexAdjustedValue = int(value)-512

        elif int(value) > 767 and int(value) <= 1123:
            dataHexThirdOrder = 0x03
            dataHexAdjustedValue = int(value)-768

        modbus_data = [0x08, 0x10, 0x07, 0x00, 0x00, 0x02, 0x04, 0x00, 0x00]
        modbus_data.append(dataHexThirdOrder)
        modbus_data.append(dataHexAdjustedValue)

        return modbus_data

    def PrintProcessInfo(self):
        """
        Print process information.
        """

        print('----- Process info -----')
        print('Total loop count > ', self.loopCount)
        for ttask in self.taskTime:
            print('Task time\t > ', ttask, '[s]')
        print('Error count\t > ', self.errorCount)
        print('------------------------')

    # ----- For debug ----- #
    def BendingSensorTest(self):
        """
        For testing.
        Only get the value of the bending sensor.
        """

        bendingSensorManagerMaster          = BendingSensorManager(ip=self.wirelessIpAddress, port=self.bendingSensorPorts[0])
        bendingSensorManagerBeginner        = BendingSensorManager(ip=self.wirelessIpAddress, port=self.bendingSensorPorts[1])

        # ----- Start receiving bending sensor value from UDP socket ----- #
        bendingSensorThreadMaster = threading.Thread(target=bendingSensorManagerMaster.StartReceiving)
        bendingSensorThreadMaster.setDaemon(True)
        bendingSensorThreadMaster.start()

        bendingSensorThreadBeginner = threading.Thread(target=bendingSensorManagerBeginner.StartReceiving)
        bendingSensorThreadBeginner.setDaemon(True)
        bendingSensorThreadBeginner.start()

        try:
            while True:
                bendingSensorValue1 = bendingSensorManagerMaster.bendingValue
                bendingSensorValue2 = bendingSensorManagerBeginner.bendingValue
                print('Sensor1 > ' + str(bendingSensorValue1) + '   Sensor2 > ' + str(bendingSensorValue2))

        except KeyboardInterrupt:
            print('KeyboardInterrupt >> Stop: RobotControlManager.BendingSensorTest()')
            bendingSensorManagerMaster.EndReceiving()
            bendingSensorManagerBeginner.EndReceiving()

    def LoadCellTest(self):
        """
        For testing.
        Only get the value of the load cell.
        """

        transform = xArmTransform()
        loadCellManager = LoadCellManager()

        arm = XArmAPI(self.xArmIpAddress)
        self.InitRobotArm(arm, transform)

        while True:
            val = loadCellManager.GetLoadCellAnalogValue(arm)
            print(val)

    def CheckGraph(self):
        # ----- Settings: Plot ----- #
        import matplotlib.pyplot as plt
        import math

        times = [0 for i in range(200)]
        loads = [0 for i in range(200)]
        grippers = [0 for i in range(200)]

        time = 0
        load = 0
        gripper = 0

        # initialize matplotlib
        plt.ion()
        plt.figure()
        li_load,    = plt.plot(times, loads, color='red', label='Load value')
        li_gripper, = plt.plot(times, grippers, color='blue', label='Gripper')


        plt.ylim(-0.1,900)
        plt.xlabel("time")
        plt.ylabel("diff load value")
        #plt.title("real time plot")
        # ----- End settings: Plot ----- #


        arm = XArmAPI(self.xArmIpAddress)
        transform = xArmTransform()
        self.InitRobotArm(arm, transform)
        self.InitGripper(arm)

        loadCellManager = LoadCellManager(arm)
        beforeLoadValue = loadCellManager.InitialLoadCellValue

        bendingSensorManager = BendingSensorManager(ip=self.wirelessIpAddress, port=self.bendingSensorPorts[0])

        # ----- Start receiving bending sensor value from UDP socket ----- #
        bendingSensorThread = threading.Thread(target=bendingSensorManager.StartReceiving)
        bendingSensorThread.setDaemon(True)
        bendingSensorThread.start()

        try:
            while True:
                #code, ret = arm.getset_tgpio_modbus_data(self.ConvertToModbusData(bendingSensorManager.bendingValue))

                val = loadCellManager.GetLoadCellAnalogValue(arm)
                loadVal = abs(val[1][1] - beforeLoadValue)
                beforeLoadValue = val[1][1]

                time += 0.1
                load = loadVal * 10000
                gripper = arm.get_gripper_position()[1]

                times.append(time)
                times.pop(0)
                loads.append(load)
                loads.pop(0)
                grippers.append(gripper)
                grippers.pop(0)

                li_load.set_xdata(times)
                li_load.set_ydata(loads)
                li_gripper.set_xdata(times)
                li_gripper.set_ydata(grippers)

                plt.xlim(min(times), max(times))
                plt.draw()
                plt.legend()

                plt.pause(0.01)

        except KeyboardInterrupt:
            print('END')

    def InitializeAll(self, robotArm, transform, isSetInitPosition=True):
        """
        Initialize the xArm

        Parameters
        ----------
        robotArm: XArmAPI
            XArmAPI object.
        transform: xArmTransform
            xArmTransform object.
        isSetInitPosition: (Optional) bool
            True -> Set to "INITIAL POSITION" of the xArm studio
            False -> Set to "ZERO POSITION" of the xArm studio
        """

        robotArm.connect()
        if robotArm.warn_code != 0:
            robotArm.clean_warn()
        if robotArm.error_code != 0:
            robotArm.clean_error()
        robotArm.motion_enable(enable=True)
        robotArm.set_mode(0)             # set mode: position control mode
        robotArm.set_state(state=0)      # set state: sport state
        if isSetInitPosition:
            initX, initY, initZ, initRoll, initPitch, initYaw = transform.GetInitialTransform()
            robotArm.set_position(x=initX, y=initY, z=initZ, roll=initRoll, pitch=initPitch, yaw=initYaw, wait=True)
        else:
            robotArm.reset(wait=True)
        print('Initialized > xArm')

        robotArm.set_tgpio_modbus_baudrate(2000000)
        robotArm.set_gripper_mode(0)
        robotArm.set_gripper_enable(True)
        robotArm.set_gripper_position(850, speed=5000)
        robotArm.getset_tgpio_modbus_data(self.ConvertToModbusData(425))
        print('Initialized > xArm gripper')

        robotArm.set_mode(1)
        robotArm.set_state(state=0)
