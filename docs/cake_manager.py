import threading
import time
from concurrent.futures import ThreadPoolExecutor

import logs
import mod.RobotConfig as RC
from mod.CoreManager import *
from logs import logger
from mod.RobotSetting import *
from mod.TableSender import *

if __name__ == "__main__":
    logs.set_file_handler(logger, '../manager.log', 'INFO')

    def __set_core_thr():
        RC.core = CoreManager()

    core_thr = threading.Thread(target=__set_core_thr, daemon=True)
    core_thr.start()

    RC.table_serial = TableSender(port=RC.TABLE_PORT)

    RC.ENABLE_XARM = True
    RC.xarm = RobotSetting(isEnableArm=RC.ENABLE_XARM)
    RC.pos = RC.xarm.init_pos
    RC.gripper_pos = RC.xarm.init_gripper_pos
    RC.loadcell_val = 0

    def update():
        for i in list(RC.core.joined_dict.keys()):
            if not RC.core.joined_dict[i][1] == None:
                RC.core.joined_dict[i][1].create_motion(None)

                if not RC.core.joined_dict[i][2] == None:
                    RC.core.core_mutex.acquire()
                    logger.critical(RC.core.joined_dict[i][2])

                    RC.core.joined_dict[i][1].create_motion(RC.core.joined_dict[i][2])
                    RC.core.joined_dict[i][2] = None

                    RC.core.core_mutex.release()

    while RC.core == None:
        pass

    if RC.core.core_start:
        with ThreadPoolExecutor() as pool:
            while True:
                try:
                    pool.submit(update)
                    time.sleep(0.00001)

                except KeyboardInterrupt:
                    RC.core.stop_core()
                    if RC.core.core_finished:
                        exit()

                except:
                    logger.exception('Error: ')
