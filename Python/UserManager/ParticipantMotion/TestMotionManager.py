import numpy as np
import threading
import time

class TestManager:
    def __init__(self) -> None:
        self.amp = 100
        self.frecency = 100
    
    def create_Sinwave(self, t):
        motion = self.amp * np.sin(2 * np.pi * self.frecency * t)
        pos = [motion] * 3
        rot = [0] * 3

        dictpos = {'participant1':pos}
        dictrot = {'participant1':rot}
        return dictpos, dictrot