import numpy as np

import mod.RobotConfig as RC
from logs import logger


class PositionMotionControll:
    def __init__(self, s, p, q):
        self.s = s
        self.p = p
        self.q = q
        self.frequency = 30
        self.s_flag = 0

    def calcurate_position(self, data):
        if not data == None:
            if data['Button'] == 'micro':
                self.per_x = data['bool'][0]
                self.per_y = data['bool'][1]

                if self.s_flag == 0:
                    self.height_correction = 0.8*RC.pos[2]/RC.xarm.init_z

                    self._s = self.s*self.height_correction
                    self._p = self.p*self.height_correction
                    self._q = self.q*self.height_correction

                    self.a = self.s*self.per_x

                    if self.per_y >= 0:
                        self.b = self._p*self.per_y

                    else:
                        self.b = self._q*self.per_y

                    self.r = np.sqrt(self.a**2+self.b**2)
                    self.delta = np.arctan2(self.b, self.a)

                    self.A = self.r*np.cos(self.delta+np.radians(RC.pos[5]))
                    self.B = self.r*np.sin(self.delta+np.radians(RC.pos[5]))

                    self.linspace = np.linspace(-1, 1, int(self.frequency*3))
                    self.interval = [i/2+np.sin(np.pi*i)/(2*np.pi) for i in self.linspace]
                    self.interval_list_x = self.B*np.abs(np.diff(self.interval))
                    self.interval_list_y = self.A*np.abs(np.diff(self.interval))

                    self.interval_xy = list(zip(self.interval_list_x, self.interval_list_y))

                    self.iter_xy = iter(self.interval_xy)

                    self.s_flag = 1

        if self.s_flag == 1:
            try:
                self.diff = next(self.iter_xy)

                RC.pos[0] += self.diff[0]
                RC.pos[1] -= self.diff[1]

            except StopIteration:
                self.s_flag = 0
