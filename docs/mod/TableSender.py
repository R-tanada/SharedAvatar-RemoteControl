import serial
from logs import logger

import mod.RobotConfig as RC


class TableSender:
    def __init__(self, port):
        self.table_state = 0
        self.enable_table = False

        try:
            self.table_serial = serial.Serial(port, 9600)
            self.enable_table = True
            logger.info('Successfully connected to the table')


        except:
            logger.error('Failed to connect to the table')

    def write_signal(self, data):
        if self.enable_table:
            if not data == None:
                if data['Button'] == 'table':
                    if data['bool'] == True:
                        if self.table_state == 0:
                            message = {'Button':'table', 'state':1}

                            RC.core.core_write_data('feedback-to-user', 'table-state', message)

                            try:
                                to_table_message = 1
                                self.table_serial.write(to_table_message.to_bytes(1, 'big'))

                            except:
                                logger.error('Failed to send table data')

                            self.table_state = 1

                        elif self.table_state == 1:
                            message = {'Button':'table', 'state':0}

                            RC.core.core_write_data('feedback-to-user', 'table-state', message)

                            try:
                                to_table_message = 0
                                self.table_serial.write(to_table_message.to_bytes(1, 'big'))

                            except:
                                logger.error('Failed to send table data')

                            self.table_state = 0
