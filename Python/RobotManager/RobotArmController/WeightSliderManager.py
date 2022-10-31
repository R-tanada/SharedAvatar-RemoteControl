# -----------------------------------------------------------------------
# Author:   Takayoshi Hagiwara (KMD)
# Created:  2021/11/17
# Summary:  Weight data from slider
# -----------------------------------------------------------------------

from UDP.UDPManager import UDPManager
import threading
import serial

class WeightSliderManager:
    def __init__(self, WeightSlider_ConnectionMethod: str = 'wireless', ip: str = '', port: int = 8000) -> None:
        if WeightSlider_ConnectionMethod == 'wireless':
            self.weightSliderUDP = UDPManager(port=port, localAddr=ip)
            weightSliderThread = threading.Thread(target=self.weightSliderUDP.UpdateData)
            weightSliderThread.setDaemon(True)
            weightSliderThread.start()

        elif WeightSlider_ConnectionMethod == 'wired':
            self.ser = serial.Serial("COM3",9600)
            self.not_used = self.ser.readline()
    
    def GetSliderValue(self):
        """
        Returns the value received from the weight adjustment slider.
        If it is not connected, return [0.5, 0.5].
        """
        """
        wireless
        """
        val = self.weightSliderUDP.data

        """
        wired
        """
        # val_arduino = self.ser.readline()
        # val = val_arduino.strip().decode('utf-8').split(',')
        

        if val is None:
            return [1, 0]
        else:
            return [float(val[0]) / 4095, float(val[1]) / 4095]

        