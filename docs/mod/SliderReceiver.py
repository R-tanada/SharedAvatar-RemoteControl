import serial

from logs import logger


class SliderReceiver:
    def __init__(self, port):
        self.slider_data = 0.5
        try:
            self.slider_serial = serial.Serial(port, 115200, timeout=0.1)
            logger.info('Successfully connected to the slider')
        except:
            logger.error('Failed to connect to the slider')
            pass

        self.receive()

    def receive(self):
        while True:
            try:
                line = self.slider_serial.readline()
                data = line.decode('utf-8').rstrip('\n')
                data = int(data)
                self.slider_data = data/4095
                logger.debug('Slider data : %s'%self.slider_data)

            except:
                self.slider_data = 0.5
