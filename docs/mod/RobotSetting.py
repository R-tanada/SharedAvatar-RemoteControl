from re import L
import sys
import threading
import time

from logs import logger

import mod.RobotConfig as RC

sys.path.append('../')
from sdk.xarm import XArmAPI


class RobotSetting:
    loadcell_val = 0
    def __init__(self, isEnableArm=False):
        self.isEnableArm = isEnableArm
        self.xArmIP = RC.XARM_IP

        self.init_x, self.init_y, self.init_z = RC.INIT_X, RC.INIT_Y, RC.INIT_Z
        self.init_roll, self.init_pitch, self.init_yaw = RC.INIT_ROLL, RC.INIT_PITCH, RC.INIT_YAW
        self.max_x, self.max_y, self.max_z = RC.MAX_X, RC.MAX_Y, RC.MAX_Z
        self.min_x, self.min_y, self.min_z = RC.MIN_X, RC.MIN_Y, RC.MIN_Z
        self.init_gripper_pos = RC.INIT_GRIPPER_POS
        self.init_pos = [self.init_x, self.init_y, self.init_z, self.init_roll, self.init_pitch, self.init_yaw]

        if isEnableArm:
            self.arm = XArmAPI(self.xArmIP)

            self.__initialize_all()
            self.__loadcell_setup()

            logger.info('xArm setup is complete')

        else:
            logger.info('xArm is not used')
            pass

    def send_data_to_robot(self):
        checked_position = self.__limitation_of_range(RC.pos)
        message = {'per-z':100*(checked_position[2]-RC.MIN_Z)/(RC.MAX_Z-RC.MIN_Z)}
        RC.core.core_write_data('feedback-to-user', 'robot-height', message)

        if self.isEnableArm:
            self.arm.set_servo_cartesian(checked_position)
            self.arm.getset_tgpio_modbus_data(self.convert_to_modbus_data(RC.gripper_pos))

    def __initialize_all(self):
        self.arm.connect()
        if self.arm.warn_code != 0:
            self.arm.clean_warn()
        if self.arm.error_code != 0:
            self.arm.clean_error()
        self.arm.motion_enable(enable=True)
        self.arm.set_mode(0)
        self.arm.set_state(state=0)

        self.arm.set_position(x=self.init_x,
                              y=self.init_y,
                              z=self.init_z,
                              roll=self.init_roll,
                              pitch=self.init_pitch,
                              yaw=self.init_yaw)
        logger.info('Initialized xArm position')

        self.arm.set_tgpio_modbus_baudrate(2000000)
        self.arm.set_gripper_mode(0)
        self.arm.set_gripper_enable(enable=True)
        self.arm.set_gripper_position(self.init_gripper_pos, speed=5000)
        logger.info('Initialized xArm gripper')

        self.arm.motion_enable(enable=True)
        self.arm.set_mode(1)
        self.arm.set_state(state=0)

    def __limitation_of_range(self, pos):
        self.mvpose = pos

        if self.mvpose[0] > self.max_x:
            self.mvpose[0] = self.max_x
            logger.error('max_x')
        elif self.mvpose[0] < self.min_x:
            self.mvpose[0] = self.min_x
            logger.error('min_x')

        if self.mvpose[1] > self.max_y:
            self.mvpose[1] = self.max_y
            logger.error('max_y')
        elif self.mvpose[1] < self.min_y:
            self.mvpose[1] = self.min_y
            logger.error('min_y')

        if self.mvpose[2] > self.max_z:
            self.mvpose[2] = self.max_z
            logger.error('max_z')
        elif self.mvpose[2] < self.min_z:
            self.mvpose[2] = self.min_z
            logger.error('min_z')

        return self.mvpose

    def convert_to_modbus_data(self, value: int):
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

    def __loadcell_setup(self):
        self.init_loadcell_val = self.arm.get_tgpio_analog(1)[1]
        self.loadcell_thr = threading.Thread(target=self.__get_loadcell_val, daemon=True)
        self.loadcell_thr.start()

    def __get_loadcell_val(self):
        while True:
            try:
                RC.loadcell_val = self.arm.get_tgpio_analog(1)[1] - self.init_loadcell_val
                time.sleep(0.01)

            except:
                RC.loadcell_val = 0
