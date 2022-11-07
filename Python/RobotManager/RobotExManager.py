# -----------------------------------------------------------------------
# Author:   Takayoshi Hagiwara (KMD)
# Created:  2021/8/25
# Summary:  Experiment manager
# -----------------------------------------------------------------------
import logs
from logs import logger
import RobotArmController.RobotControlManager
# from ctypes import windll

if __name__ == '__main__':
    logs.set_file_handler(logger, '../manager.log', 'INFO')
    robotControlManager = RobotArmController.RobotControlManager.RobotControlManagerClass()
    robotControlManager.SendDataToRobot(participantNum=2, executionTime=999999, isFixedFrameRate=False, frameRate=200, isChangeOSTimer=True, isExportData=False, isEnablexArm=False)

    print('\n----- End program: ExManager.py -----')