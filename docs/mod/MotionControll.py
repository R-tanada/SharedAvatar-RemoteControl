from logs import logger

import mod.RobotConfig as RC
from mod.ButtonMotionControll import *
from mod.PointButtonMotionControll import *
from mod.PositionMotionControll import *


class MotionControll:
    def __init__(self, mode:str):
        self.s_front = 'front'
        self.s_back = 'back'
        self.s_right = 'right'
        self.s_left = 'left'
        self.s_roll_plus = 'roll_plus'
        self.s_roll_minus = 'roll_minus'
        self.s_up = 'up'
        self.s_down = 'down'
        self.s_grip = 'grip'
        self.s_cream = 'squeeze'
        self.s_open = 'g_open'

        self.pancake_button_motion = PointButtonMotionControll('cake', 260, -200)
        self.fruit_button_motion = PointButtonMotionControll('fruit', 260, 100)
        self.cream_button_motion_1 = PointButtonMotionControll('cream_1', 410, 280)
        self.cream_button_motion_2 = PointButtonMotionControll('cream_2', 410, 380)
        self.set_height_motion = PointButtonMotionControll('height_slider',300,200)

        self.position_motion = PositionMotionControll(200, 250*0.55, 250*0.45)

        self.mode = mode
        self.base_length = 20
        self.base_frequency = 30
        self.mc_front = ButtonMotionControll(self.s_front,
                                             distance=self.base_length,
                                             frequency=self.base_frequency)
        self.mc_back = ButtonMotionControll(self.s_back,
                                             distance=self.base_length,
                                             frequency=self.base_frequency)
        self.mc_right = ButtonMotionControll(self.s_right,
                                             distance=self.base_length,
                                             frequency=self.base_frequency)
        self.mc_left = ButtonMotionControll(self.s_left,
                                             distance=self.base_length,
                                             frequency=self.base_frequency)
        self.mc_roll_plus = ButtonMotionControll(self.s_roll_plus,
                                             distance=15,
                                             frequency=self.base_frequency)
        self.mc_roll_minus = ButtonMotionControll(self.s_roll_minus,
                                             distance=15,
                                             frequency=self.base_frequency)
        self.mc_up = ButtonMotionControll(self.s_up,
                                          distance=self.base_length*0.5,
                                          frequency=self.base_frequency)
        self.mc_down = ButtonMotionControll(self.s_down,
                                          distance=self.base_length*0.5,
                                          frequency=self.base_frequency)
        self.grip = ButtonMotionControll(self.s_grip,
                                          distance=self.base_length*0.5,
                                          frequency=self.base_frequency)
        self.cream = ButtonMotionControll(self.s_cream,
                                          distance=self.base_length*0.5,
                                          frequency=self.base_frequency)
        self.open = ButtonMotionControll(self.s_open,
                                          distance=self.base_length*0.5,
                                          frequency=self.base_frequency)

    def create_motion(self, data):
        self.pancake_button_motion.set_point(data)
        self.fruit_button_motion.set_point(data)
        self.set_height_motion.set_heightPos(data)
        self.cream_button_motion_1.set_point_cream_grip_v2(data)
        self.cream_button_motion_2.set_point_cream_grip_v2(data)
        self.position_motion.calcurate_position(data)
        RC.table_serial.write_signal(data)

        if self.mode =='liner':
            self.mc_front.liner(data)
            self.mc_back.liner(data)
            self.mc_right.liner(data)
            self.mc_left.liner(data)
            self.mc_roll_plus.liner(data)
            self.mc_roll_minus.liner(data)
            self.mc_up.liner(data)
            self.mc_down.liner(data)
            self.grip.g_auto(data)
            self.cream.g_cream(data)
            self.open.g_open(data)

        RC.xarm.send_data_to_robot()
        logger.debug(RC.pos)
