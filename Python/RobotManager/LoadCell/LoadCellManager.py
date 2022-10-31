# -----------------------------------------------------------------------
# Author:   Takayoshi Hagiwara (KMD)
# Created:  2021/10/21
# Summary:  Load cell manager
# -----------------------------------------------------------------------

import threading
import time
import csv
import serial

# ----- Custom class ----- #
# from Audio.AudioManager import AudioManager
from RobotArmController.xArmIO import xArmIO
from UDP.UDPManager import UDPManager


# path = "C:/Users/kimih/Desktop/tanada/robot_system_data/loadcell_data_3grip/"
# filename = 'sc'
# exportPath = path + filename + '.csv'
# header = ['raw_data, init_data, angle, filt_data']


class LoadCellManager:
    def __init__(self, arm) -> None:
        self.xArmIO = xArmIO()
        # self.audioManager = AudioManager()
        # self.udpManager = UDPManager(9006, '192.168.11.5')

        self.isGripping = False
        self.loadVal = 0.0
        self.init_loadVal = 0.0

        self.InitialLoadCellValue = self.xArmIO.GetxArmAnalogInput(arm)[1][1]

        self.ard1 = serial.Serial('COM6',9600)
        self.ard2 = serial.Serial('COM7',9600)

        feedbackThread = threading.Thread(target=self.GrippingLoadFeedbackWithSqueeze, args=(arm,))
        feedbackThread.setDaemon(True)
        feedbackThread.start()
    
    def GetLoadCellAnalogValue(self, arm):
        """
        Get load cell analog value
        Parameters
        ----------
        arm: XArmAPI
            XArmAPI object
        """
        
        return self.xArmIO.GetxArmAnalogInput(arm)
    
    # def GrippingLoadFeedbackWitheRubber(self, arm, threshold: float = 0.2, isConst: bool = False):
    #     """
    #     Get feedback of gripping load.

    #     Parameters
    #     ----------
    #     arm: XArmAPI
    #         XArmAPI object
    #     threshold: (Optional) float
    #         Threshold of load value
    #     isConst: (Optional) bool
    #         Appear constant vibration when detect gripping
    #     """

    #     beforeLoadValue = self.InitialLoadCellValue

    #     while True:
    #         val = self.GetLoadCellAnalogValue(arm)
    #         loadVal = abs(val[1][1] - beforeLoadValue)

    #         print(loadVal)

    #         if loadVal < 0:
    #             loadVal = 0

    #         self.audioManager.AddRawAnalogValue(loadVal)

    #         # beforeLoadValue = val[1][1]

    #         # ----- Detect gripping ----- #
    #         loadDiffFromInit = val[1][1] - self.InitialLoadCellValue
    #         if not self.isGripping and loadDiffFromInit > threshold:
    #             self.isGripping = True
    #         elif self.isGripping and loadDiffFromInit < threshold:
    #             self.isGripping = False

    #         if self.isGripping and isConst:
    #             self.audioManager.PlaySinWave()

    def GrippingLoadFeedbackWithSqueeze(self, arm, threshold: float = 0.2, isConst: bool = False):
        """
        Get feedback of gripping load.
        Parameters
        ----------
        arm: XArmAPI
            XArmAPI object
        threshold: (Optional) float
            Threshold of load value
        isConst: (Optional) bool
            Appear constant vibration when detect gripping
        """
        start_time = time.perf_counter()

        beforeLoadValue = self.InitialLoadCellValue

        # with open(exportPath, 'w', newline='') as f:
        #     writer = csv.writer(f)
        #     writer.writerow(header)

        while True:
            data = []

            val = self.GetLoadCellAnalogValue(arm)
            self.loadVal = abs(val[1][1] - beforeLoadValue)

            # print(self.loadVal)
            
            angle = self.loadVal*1000/0.65 + 1000
            if angle >= 2000:
                angle = 2000

            # print(angle)

            # self.udpManager.SendData(angle, '192.168.11.8', 7000)
            # self.udpManager.SendData(angle, '192.168.11.9', 7000)

            mes = (str(angle) + '\0').encode()
            self.ard1.write(mes)
            self.ard2.write(mes)

            data = [str(val[1][1]), str(self.loadVal), str(angle), str(time.perf_counter() - start_time)]
                

                # writer.writerow(data)
        
            # print(loadVal)

    def Gripperposition_feedback(self, position, init_pos):

        if position < init_pos:
            angle = ((init_pos - position)/220)*1000 + 1000
            # print(angle)
        else:
            angle = 1000

        self.udpManager.SendData(angle, '192.168.11.9', 7000)