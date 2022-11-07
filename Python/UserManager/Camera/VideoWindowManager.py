
import os
import sys
from PySide6 import QtCore, QtGui, QtWidgets

class VideoWidgetManager:
    def __init__(self) -> None:
        pass

    def StartWindow(self):
        self.app = QtWidgets.QApplication(sys.argv) 
        self.mainWin = MainWin()
        self.mainWin.show()
        self.app.exec_()

    def ChangeNewFrame(self, frame):
        image = QtGui.QImage(frame.pixelData.tobytes(),
                                frame.width,
                                frame.height,
                                QtGui.QImage.Format_RGB888)
        pixmap = QtGui.QPixmap.fromImage(image)
        self.mainWin.robotVeiw_video.setPixmap(pixmap.scaled(self.mainWin.new_frame_geometry.width(),
                                                            self.mainWin.new_frame_geometry.height(),
                                                            QtCore.Qt.KeepAspectRatio,
                                                            QtCore.Qt.SmoothTransformation))

class MainWin(QtWidgets.QMainWindow):
    def __init__(self, parent=None):
        super().__init__(parent)      
        
        self.setWindowTitle('RemoteCamera')
        self.win_width = 1200
        self.win_ratio = {'width':16, 'height':9}
        self.resize(self.win_width, (self.win_width * self.win_ratio['height']) / self.win_ratio['width'])
        self.first_frame_geometry = self.geometry()

        self.loading_fig = QtGui.QPixmap("/Users/yuzu/Documents/GitHub/ms-sharedavatar/Python/UserManager/Camera/Image/loading.png")
        self.robotVeiw_geometry = {'x':0, 'y':0, 'size':1}
        self.robotVeiw_video = self.__set_video_widget('RobotView', self.robotVeiw_geometry)

    def __set_video_widget(self, name:str, geometry:dict):
        video_widget = QtWidgets.QLabel(name, self)
        video_widget.setObjectName(name)
        video_widget.setScaledContents(True)
        video_widget.setGeometry(self.first_frame_geometry.width()*geometry['x'],
                                 self.first_frame_geometry.height()*geometry['y'],
                                 self.first_frame_geometry.width()*geometry['size'],
                                 self.first_frame_geometry.height()*geometry['size'])
        video_widget.setPixmap(self.loading_fig.scaled(self.first_frame_geometry.width()*geometry['size'],
                                                       self.first_frame_geometry.height()*geometry['size'],
                                                       QtCore.Qt.KeepAspectRatio,
                                                       QtCore.Qt.SmoothTransformation))

        return video_widget

    def __resize_video_widget(self, widget, geometry):
        widget.setGeometry(self.new_frame_geometry.width()*geometry['x'],
                           self.new_frame_geometry.height()*geometry['y'],
                           self.new_frame_geometry.width()*geometry['size'],
                           self.new_frame_geometry.height()*geometry['size'])

    def resizeEvent(self, event):
        self.new_frame_geometry = self.frameGeometry()
        self.__resize_video_widget(self.robotVeiw_video, self.robotVeiw_geometry)

    # def keyPressEvent(self, event: QtGui.QKeyEvent) -> None:
    #     print(QtGui.QKeySequence(event.key()).toString())

    # def closeEvent(self, event):
    #     self.core.stop_core()
    #     event.accept()
    #     sys.exit()
        

if __name__ == '__main__':
    # app = QtWidgets.QApplication(sys.argv) 
    # window = MainWin()
    # window.show()
    video = VideoWidgetManager()
    # app.exec_()