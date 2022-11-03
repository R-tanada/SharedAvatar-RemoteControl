import numpy as np
from logs import logger

import mod.RobotConfig as RC
from mod.RobotSetting import *
from mod.SliderReceiver import *


class ButtonMotionControll:
    iter_list = []
    alternative_flag = True
    r_flag = 0

    def __init__(self, button:str, distance:int, frequency:int):
        self.button = button
        self.distance = distance
        self.frequency = frequency

        self.force_threshold = 0.007
        self.gripper_pos_Min = 130
        self.auto_gripper_pos = 0
        self.cnt_force = 0

        self.s_flag = 0

        if self.button == 'front':
            self.index = 0
            self.plus = True
        elif self.button == 'back':
            self.index = 0
            self.plus = False
        elif self.button == 'left':
            self.index = 1
            self.plus = True
        elif self.button == 'right':
            self.index = 1
            self.plus = False
        elif self.button == 'roll_plus':
            self.index = 5
            self.plus = False
        elif self.button == 'roll_minus':
            self.index = 5
            self.plus = True
        elif self.button == 'up':
            self.index = 2
            self.plus = True
        elif self.button == 'down':
            self.index = 2
            self.plus = False

    def liner(self, data):
        if not data == None:
            if data['Button'] == self.button:
                self.linspace = np.linspace(-1, 1, int(self.frequency*2))
                self.interval = [i/2+np.sin(np.pi*i)/(2*np.pi) for i in self.linspace]
                self.interval_list = 1.3*self.distance*(2*0.5+0.1)*np.abs(np.diff(self.interval))

                if self.s_flag == 0:
                    self.s_flag = 1
                    self.iter = iter(self.interval_list)

        if self.s_flag == 1:
            try:
                self.diff = next(self.iter)

                if self.plus:
                    RC.pos[self.index] += self.diff

                else:
                    RC.pos[self.index] -= self.diff

            except StopIteration:
                self.diff = 0
                self.s_flag = 0

    def radicon(self, data):
        if not data == None:
            if data['Button'] == self.button:
                if ButtonMotionControll.r_flag == 0:
                    self.radicon_count = 0
                    ButtonMotionControll.r_flag = 1

                elif ButtonMotionControll.r_flag == 1:
                    ButtonMotionControll.r_flag = 2

        if ButtonMotionControll.r_flag == 1:
            self.radicon_count += 0.005*2
            self.calc = 1/(1+10*np.exp(-0.5*self.radicon_count))

            if self.plus:
                RC.pos[self.index] += self.calc

            else:
                RC.pos[self.index] -= self.calc

        elif ButtonMotionControll.r_flag == 2:
            self.linspace = np.linspace(0, 10, self.frequency)
            self.interval = [(self.calc/(1+0.1*np.exp(1/self.calc*i))) for i in self.linspace]
            self.iter = iter(self.interval)
            ButtonMotionControll.r_flag = 3

        elif ButtonMotionControll.r_flag == 3:
            try:
                if self.plus:
                    RC.pos[self.index] += next(self.iter)

                else:
                    RC.pos[self.index] -= next(self.iter)

            except StopIteration:
                ButtonMotionControll.r_flag = 0

    def g_auto(self, data):
        if not data == None:
            if data['Button'] == self.button:
                if self.s_flag == 0:
                    self.s_flag = 1

                    if ButtonMotionControll.alternative_flag == True:
                        grip_state = {'Button':'grip', 'state':1}
                        RC.core.core_write_data('feedback-to-user', 'grip-state', grip_state)

                    elif ButtonMotionControll.alternative_flag == False:
                        grip_state = {'Button':'grip', 'state':0}
                        RC.core.core_write_data('feedback-to-user', 'grip-state', grip_state)

        if self.s_flag == 1:
            if ButtonMotionControll.alternative_flag == True:
                RC.gripper_pos -= 3
                if RC.loadcell_val > self.force_threshold:
                    self.cnt_force += 1

                if self.cnt_force == 10 or RC.gripper_pos <= self.gripper_pos_Min:
                    self.s_flag = 0
                    self.cnt_force = 0
                    ButtonMotionControll.alternative_flag = False

            elif ButtonMotionControll.alternative_flag == False:
                RC.gripper_pos += 3

                if RC.gripper_pos >= 840:
                    self.s_flag = 0
                    ButtonMotionControll.alternative_flag = True

    def g_cream(self, data):
        if not data == None:
            if data['Button'] == self.button:
                if self.s_flag == 0:
                    self.s_flag = 1
                    grip_state = {'Button':'cream', 'state':1}
                    RC.core.core_write_data('feedback-to-user', 'grip-state', grip_state)

                elif self.s_flag == 1:
                    self.s_flag = 2
                    grip_state = {'Button':'cream', 'state':0}
                    RC.core.core_write_data('feedback-to-user', 'grip-state', grip_state)

        if self.s_flag == 1:
            if RC.gripper_pos >= self.gripper_pos_Min:
                RC.gripper_pos -= 0.5
            else:
                pass

        elif self.s_flag == 2:
            RC.gripper_pos += 3
            if RC.loadcell_val <= self.force_threshold:
                self.s_flag = 0

    def g_open(self, data):
        if not data == None:
            if data['Button'] == self.button:
                if self.s_flag == 0:
                    self.s_flag = 1
                    grip_state = {'Button':'open', 'state':1}
                    RC.core.core_write_data('feedback-to-user', 'grip-state', grip_state)

                elif self.s_flag == 1:
                    self.s_flag = 0
                    grip_state = {'Button':'open', 'state':0}
                    RC.core.core_write_data('feedback-to-user', 'grip-state', grip_state)

        if self.s_flag == 1:
            RC.gripper_pos += 1
            if RC.gripper_pos >= 840:
                self.s_flag = 0
                ButtonMotionControll.alternative_flag = True
                grip_state = {'Button':'auto', 'state':0}
                RC.core.core_write_data('feedback-to-user', 'grip-state', grip_state)
