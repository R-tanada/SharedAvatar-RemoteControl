# This Python file uses the following encoding: utf-8
import sys
from PySide6.QtCore import Qt
from PySide6.QtWidgets import QMainWindow, QApplication, QHBoxLayout, QLabel, \
    QFrame, QWidget, QGroupBox, QVBoxLayout, QComboBox

from PySide6.QtGui import QPixmap, QImage

from CorePython.CoreAPI import CoreAPI
from CorePython.CoreVideo import CoreVideo
import CorePython.Video
import cv2
import numpy


class MainWindow(QMainWindow):
    def __init__(self):
        QMainWindow.__init__(self)
        self.setWindowTitle("Video Writer")
        self.origLabel = None
        self.modifiedLabel = None
        self.combobox = None
        self.video = None
        self.video_2 = None
        self.core = CoreAPI()
        self.__init_core()
        self.__init_UI()

    def __init_UI(self):
        self.resize(1024, 800)

        originalBox = QGroupBox()
        originalBox.setTitle(self.tr("Original Stream"))
        origLayout = QVBoxLayout()
        self.combobox = QComboBox()
        self.combobox.addItems(['Sepia', 'Invert', 'Focus Blur', 'Portrait'])
        origLayout.addWidget(self.combobox)
        self.origLabel = QLabel()
        self.origLabel.setFrameShape(QFrame.Panel)
        self.origLabel.setFrameShadow(QFrame.Sunken)
        self.origLabel.setScaledContents(True)
        origLayout.addWidget(self.origLabel)
        originalBox.setLayout(origLayout)

        modifiedBox = QGroupBox()
        modifiedBox.setTitle(self.tr("Modified Stream"))
        modifiedLayout = QHBoxLayout()
        self.modifiedLabel = QLabel()
        self.modifiedLabel.setFrameShape(QFrame.Panel)
        self.modifiedLabel.setFrameShadow(QFrame.Sunken)
        self.modifiedLabel.setScaledContents(True)
        modifiedLayout.addWidget(self.modifiedLabel)
        modifiedBox.setLayout(modifiedLayout)

        centralWidget = QWidget()
        hLayout = QHBoxLayout(centralWidget)
        hLayout.addWidget(originalBox)
        hLayout.addWidget(modifiedBox)
        self.setCentralWidget(centralWidget)

    def __init_core(self):
        self.core.init_api(app_path='./')
        self.core.load_services('services/Simple-services.json')
        self.core.start_default_services()
        self.video = CoreVideo('local-image-RGB',
                               self.__on_frame_arrived)
        self.video_2 = CoreVideo('local-image-RGB-2')
        self.video_2.start()
        self.video.start()

    def __on_frame_arrived(self, resource, frame):
        print("New frame arrived: " +
              "{0} x {1} - ".format(frame.width, frame.height) +
              "Format: {0}".format(CorePython.Video.VideoFormat(frame.format)))

        image = QImage(frame.pixelData.tobytes(),
                       frame.width,
                       frame.height,
                       QImage.Format_RGB888)
        pixmap = QPixmap.fromImage(image)
        self.origLabel.setPixmap(pixmap.scaled(frame.width,
                                               frame.height,
                                               Qt.KeepAspectRatio,
                                               Qt.SmoothTransformation))

        data = numpy.frombuffer(frame.pixelData.tobytes(), dtype=numpy.uint8)
        # Use HxW representation for OpenCV to correctly handle the frame
        data = numpy.reshape(data, (frame.height, frame.width, 3))
        data = self.__select_filter(data)

        # Map the data to an image frame to be written into the video resource
        image = QImage(data,
                       frame.width,
                       frame.height,
                       QImage.Format_RGB888)
        pixmap = QPixmap.fromImage(image)
        self.modifiedLabel.setPixmap(pixmap.scaled(frame.width,
                                                   frame.height,
                                                   Qt.KeepAspectRatio,
                                                   Qt.SmoothTransformation))

        data = numpy.reshape(data, (frame.width * frame.height * 3))
        # Write it to video resource
        self.video_2.write_frame(frame.width,
                                 frame.height,
                                 CorePython.Video.VideoFormat.RGB,
                                 data)

    def __verify_alpha_channel(self, frame):
        # Convert this frame to a four channel frame (to include alpha channel)
        if len(frame.shape) != 4:
            frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGRA)
        return frame

    def __apply_color_overlay(self, frame, blue, green, red, intensity):
        frame = self.__verify_alpha_channel(frame)
        h, w, c = frame.shape
        color_bgra = (blue, green, red, 1)
        overlay = numpy.full((h, w, 4), color_bgra, dtype='uint8')
        cv2.addWeighted(overlay, intensity, frame, 1.0, 0, frame)
        return cv2.cvtColor(frame, cv2.COLOR_BGRA2RGB)

    def __apply_sepia(self, frame, intensity=0.5):
        return self.__apply_color_overlay(frame, 20, 66, 112, intensity)

    def __apply_invert(self, frame):
        return cv2.bitwise_not(frame)

    def __alpha_blend(self, foreground, background, mask):
        alpha = mask / 255.0
        return cv2.convertScaleAbs(foreground * alpha + background *
                                   (1 - alpha))

    def __apply_circle_focus_blur(self, frame, intensity=0.2):
        frame = self.__verify_alpha_channel(frame)
        h, w, c = frame.shape
        y = int(h/2)
        x = int(w/2)
        radius = int(x/2)
        center = (x, y)
        mask = numpy.zeros((h, w, 4), dtype='uint8')
        cv2.circle(mask, center, radius, (255, 255, 255), -1, cv2.LINE_AA)
        mask = cv2.GaussianBlur(mask, (21, 21), 0)
        blured = cv2.GaussianBlur(frame, (21, 21), 0)
        blended = self.__alpha_blend(blured, frame, 255 - mask)
        return cv2.cvtColor(blended, cv2.COLOR_BGRA2RGB)

    def __apply_portrait_mode(self, frame):
        frame = self.__verify_alpha_channel(frame)
        gray = cv2.cvtColor(frame, cv2.COLOR_BGRA2GRAY)
        _, mask = cv2.threshold(gray, 120, 255, cv2.THRESH_BINARY)
        mask = cv2.cvtColor(mask, cv2.COLOR_GRAY2BGRA)
        blured = cv2.GaussianBlur(frame, (21, 21), 0)
        blended = self.__alpha_blend(blured, frame, mask)
        return cv2.cvtColor(blended, cv2.COLOR_BGRA2RGB)

    def __select_filter(self, frame):
        selected = self.combobox.currentText()
        if selected == 'Sepia':
            return self.__apply_sepia(frame)
        elif selected == 'Invert':
            return self.__apply_invert(frame)
        elif selected == 'Focus Blur':
            return self.__apply_circle_focus_blur(frame)
        elif selected == 'Portrait':
            return self.__apply_portrait_mode(frame)
        return frame


def main():
    app = QApplication(sys.argv)

    global window
    window = MainWindow()
    window.show()
    app.exec()


if __name__ == '__main__':
    sys.exit(main())
