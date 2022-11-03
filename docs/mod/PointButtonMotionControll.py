import numpy as np
from logs import logger

from mod.RobotSetting import *
import mod.RobotConfig as RC


class PointButtonMotionControll:
    grip_state = 'open'
    def __init__(self, button, x, y):
        self.button = button
        self.x = x
        self.y = y
        self.creamPos_list = []
        self.z = 430
        self.z_grip = 285
        self.z_up = 430
        self.frequency = 100
        self.s_flag = 0
        # self.max_z = 500
        # self.min_z = 270
        self.max_z = RC.MAX_Z
        self.min_z = RC.MIN_Z
        self.move_count = 0
        self.cream_number = 4
        for i in range(self.cream_number):
            self.creamPos_list.append(self.x - (i * 60))

    def set_point(self, data):
        if not data == None:
            if data['Button'] == self.button:
                if self.s_flag == 0:
                    self.distance_x = self.x - RC.pos[0]
                    self.distance_y = self.y - RC.pos[1]
                    self.distance_z = self.z - RC.pos[2]

                    self.linspace = np.linspace(-1, 1, int(self.frequency*2))
                    self.interval = [i/2+np.sin(np.pi*i)/(2*np.pi) for i in self.linspace]
                    self.interval_list_x = self.distance_x*np.abs(np.diff(self.interval))
                    self.interval_list_y = self.distance_y*np.abs(np.diff(self.interval))
                    self.interval_list_z = self.distance_z*np.abs(np.diff(self.interval))

                    self.iter_x = iter(self.interval_list_x)
                    self.iter_y = iter(self.interval_list_y)
                    self.iter_z = iter(self.interval_list_z)
                    self.s_flag = 1

        if self.s_flag == 1:
            try:
                self.diff_z = next(self.iter_z)
                RC.pos[2] += self.diff_z

            except StopIteration:
                logger.debug('Stop iteration z')

            try:
                self.diff_x = next(self.iter_x)
                self.diff_y = next(self.iter_y)
                RC.pos[0] += self.diff_x
                RC.pos[1] += self.diff_y

            except StopIteration:
                self.s_flag = 0
                logger.debug('Stop iteration xy')

    def set_point_cream_grip(self, data):
        if not data == None:
            if data['Button'] == self.button:
                if self.s_flag == 0:
                    self.distance_x = self.x - RC.pos[0]
                    self.distance_y = self.y - RC.pos[1]
                    self.distance_z = self.z_up - RC.pos[2]
                    self.distance_z_grip = self.z_grip - self.z_up
                    self.distance_z_up = self.z_up - self.z_grip
                    self.interval_x = self.distance_x / (self.frequency * 2)
                    self.interval_y = self.distance_y / (self.frequency * 2)
                    self.interval_z = self.distance_z / (self.frequency * 2)
                    self.interval_z_grip = self.distance_z_grip / (self.frequency * 2)
                    self.interval_z_up = self.distance_z_up / (self.frequency * 2)
                    self.interval_x_list = [self.interval_x] * (self.frequency * 2)
                    self.interval_y_list = [self.interval_y] * (self.frequency * 2)
                    self.interval_z_list = [self.interval_z] * (self.frequency * 2)
                    self.interval_z_grip_list = [self.interval_z_grip] * (self.frequency * 2)
                    self.interval_z_up_list = [self.interval_z_up] * (self.frequency * 2)
                    self.iter_x = iter(self.interval_x_list)
                    self.iter_y = iter(self.interval_y_list)
                    self.iter_z = iter(self.interval_z_list)
                    self.iter_z_grip = iter(self.interval_z_grip_list)
                    self.iter_z_up = iter(self.interval_z_up_list)
                    if PointButtonMotionControll.grip_state == 'open':
                        RC.gripper_pos = 850
                    self.diff_grip = 3
                    self.cream_threshold = 0.01
                    self.s_flag = 1

        if self.s_flag == 1:
            try:
                self.diff_x = next(self.iter_x)
                self.diff_y = next(self.iter_y)
                self.diff_z = next(self.iter_z)
                RC.pos[0] += self.diff_x
                RC.pos[1] += self.diff_y
                RC.pos[2] += self.diff_z

            except StopIteration:
                self.s_flag = 2

        elif self.s_flag == 2:
            try:
                self.diff_z_grip = next(self.iter_z_grip)
                RC.pos[2] += self.diff_z_grip

            except StopIteration:
                self.s_flag = 3

        elif self.s_flag == 3:
            if PointButtonMotionControll.grip_state == 'open':
                RC.gripper_pos -= self.diff_grip
                if RobotSetting.loadcell_val > self.cream_threshold or RC.gripper_pos < 500:
                # if RC.gripper_pos < 500:
                    self.s_flag = 4
                    PointButtonMotionControll.grip_state = 'grip'
                    logger.debug('Stop gripper')

            elif PointButtonMotionControll.grip_state == 'grip':
                RC.gripper_pos += self.diff_grip
                if RC.gripper_pos >= 840:
                    self.s_flag = 4
                    PointButtonMotionControll.grip_state = 'open'

        if self.s_flag == 4:
            try:
                self.diff_z_up = next(self.iter_z_up)
                RC.pos[2] += self.diff_z_up

            except StopIteration:
                self.s_flag = 0

    def set_point_cream_grip_v2(self, data):
        if not data == None:
            if data['Button'] == self.button:
                if self.s_flag == 0:
                    grip_state = {'Button': data['Button'], 'state':PointButtonMotionControll.grip_state}
                    RC.core.core_write_data('feedback-to-user', 'pickup_cream', grip_state)
                    # self.x, self.y = self.creamPos_list[self.move_count]
                    self.x = self.creamPos_list[self.move_count]
                    self.distance_x = self.x - RC.pos[0]
                    self.distance_y = self.y - RC.pos[1]
                    self.distance_z = self.z_up - RC.pos[2]
                    self.distance_z_grip = self.z_grip - self.z_up
                    self.distance_z_up = self.z_up - self.z_grip
                    self.interval_x = self.distance_x / (self.frequency * 2)
                    self.interval_y = self.distance_y / (self.frequency * 2)
                    self.interval_z = self.distance_z / (self.frequency * 2)
                    self.interval_z_grip = self.distance_z_grip / (self.frequency * 2)
                    self.interval_z_up = self.distance_z_up / (self.frequency * 2)
                    self.interval_x_list = [self.interval_x] * (self.frequency * 2)
                    self.interval_y_list = [self.interval_y] * (self.frequency * 2)
                    self.interval_z_list = [self.interval_z] * (self.frequency * 2)
                    self.interval_z_grip_list = [self.interval_z_grip] * (self.frequency * 2)
                    self.interval_z_up_list = [self.interval_z_up] * (self.frequency * 2)
                    self.iter_x = iter(self.interval_x_list)
                    self.iter_y = iter(self.interval_y_list)
                    self.iter_z = iter(self.interval_z_list)
                    self.iter_z_grip = iter(self.interval_z_grip_list)
                    self.iter_z_up = iter(self.interval_z_up_list)
                    if PointButtonMotionControll.grip_state == 'open':
                        RC.gripper_pos = 850
                    self.diff_grip = 3
                    self.cream_threshold = 0.01
                    self.s_flag = 1

        if self.s_flag == 1:
            try:
                self.diff_x = next(self.iter_x)
                self.diff_y = next(self.iter_y)
                self.diff_z = next(self.iter_z)
                RC.pos[0] += self.diff_x
                RC.pos[1] += self.diff_y
                RC.pos[2] += self.diff_z

            except StopIteration:
                self.s_flag = 2

        elif self.s_flag == 2:
            try:
                self.diff_z_grip = next(self.iter_z_grip)
                RC.pos[2] += self.diff_z_grip

            except StopIteration:
                self.s_flag = 3

        elif self.s_flag == 3:
            if PointButtonMotionControll.grip_state == 'open':
                RC.gripper_pos -= self.diff_grip
                if RobotSetting.loadcell_val > self.cream_threshold or RC.gripper_pos < 300:
                # if RC.gripper_pos < 500:
                    self.s_flag = 4
                    PointButtonMotionControll.grip_state = 'grip'
                    logger.debug('Stop gripper')

            elif PointButtonMotionControll.grip_state == 'grip':
                RC.gripper_pos += self.diff_grip
                if RC.gripper_pos >= 840:
                    self.s_flag = 4
                    self.move_count += 1
                    if self.move_count == self.cream_number:
                        self.move_count = 0
                    PointButtonMotionControll.grip_state = 'open'

        if self.s_flag == 4:
            try:
                self.diff_z_up = next(self.iter_z_up)
                RC.pos[2] += self.diff_z_up

            except StopIteration:
                self.s_flag = 0
                if PointButtonMotionControll.grip_state == 'open':
                    grip_state = {'Button': self.button, 'state':None}
                    RC.core.core_write_data('feedback-to-user', 'pickup_cream', grip_state)

    def set_heightPos(self, data):
        if not data == None:
            if data['Button'] == self.button:
                if self.s_flag == 0:
                    height_data = data['bool']
                    target_z = height_data * (self.max_z - self.min_z) / 100 + self.min_z
                    self.distance_z = target_z - RC.pos[2]
                    self.interval_z = self.distance_z / (self.frequency * 2)
                    self.interval_z_list = [self.interval_z] * (self.frequency * 2)
                    self.iter_z = iter(self.interval_z_list)
                    self.s_flag = 1

        if self.s_flag == 1:
            try:
                self.diff_z = next(self.iter_z)
                RC.pos[2] += self.diff_z
            except StopIteration:
                self.s_flag = 0
