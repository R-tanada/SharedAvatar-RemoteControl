# -----------------------------------------------------------------------
# Author:   Takayoshi Hagiwara (KMD)
# Created:  2021/10/30
# Summary:  xArm I/O manager
# -----------------------------------------------------------------------

class xArmIO:
    def __init__(self) -> None:
        pass

    def GetxArmAnalogInput(self, arm):
        """
        Get the analog input from xArm.

        Parameters
        ----------
        arm: XArmAPI
            XArmAPI object.
        """
        
        analogValue = arm.get_tgpio_analog()
        return analogValue