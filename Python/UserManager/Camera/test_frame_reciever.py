# This Python file uses the following encoding: utf-8
import sys
from PySide6.QtCore import Qt
from PySide6.QtWidgets import QMainWindow, QApplication, QHBoxLayout, QLabel, \
    QFrame, QWidget, QGroupBox

from PySide6.QtGui import QPixmap, QImage

sys.path.append('../')
from sdk.AvatarCore.bin.CorePython.CoreAPI import CoreAPI
from sdk.AvatarCore.bin.CorePython.CoreVideo import CoreVideo
import sdk.AvatarCore.bin.CorePython.Video


class MainWindow(QMainWindow):

    def __init__(self):
        QMainWindow.__init__(self)
        self.setWindowTitle("Video Receiver")
        self.origLabel = None
        self.modifiedLabel = None
        self.video = None
        self.core = CoreAPI()
        self.__init_core()
        self.__init_UI()

    def __init_UI(self):
        self.resize(1024, 800)

        originalBox = QGroupBox()
        originalBox.setTitle(self.tr("Incoming Stream"))
        origLayout = QHBoxLayout()
        self.origLabel = QLabel()
        self.origLabel.setFrameShape(QFrame.Panel)
        self.origLabel.setFrameShadow(QFrame.Sunken)
        self.origLabel.setScaledContents(True)
        origLayout.addWidget(self.origLabel)
        originalBox.setLayout(origLayout)

        centralWidget = QWidget()
        hLayout = QHBoxLayout(centralWidget)
        hLayout.addWidget(originalBox)
        self.setCentralWidget(centralWidget)

    def __init_core(self):
        self.core.init_api(app_path='./')
        # Subscribe to a video resource "local-image-RGB-2"
        self.video = CoreVideo('local-image-RGB-2',
                               self.__on_frame_arrived)
        self.video.start()

    def __on_frame_arrived(self, resource, frame):
        print("New frame arrived: " +
              "{0} x {1} - ".format(frame.width, frame.height) +
              "Format: {0}".format(CorePython.Video.VideoFormat(frame.format)))

        image = QImage(frame.pixel_data.tobytes(),
                       frame.width,
                       frame.height,
                       QImage.Format_RGB888)
        pixmap = QPixmap.fromImage(image)
        self.origLabel.setPixmap(pixmap.scaled(frame.width,
                                               frame.height,
                                               Qt.KeepAspectRatio,
                                               Qt.SmoothTransformation))



def main():
    app = QApplication(sys.argv)

    global window
    window = MainWindow()
    window.show()
    app.exec()



if __name__ == '__main__':
    sys.exit(main())