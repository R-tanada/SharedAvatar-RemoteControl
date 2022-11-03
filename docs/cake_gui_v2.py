import json
import os
import sys
import threading

import cv2
import numpy as np
from PySide6 import QtCore, QtGui, QtWidgets

import logs
import mod.ImgPath as IP
from logs import logger

sys.path.append('../')
from sdk.AvatarCore.bin.CorePython.CoreAPI import CoreAPI
from sdk.AvatarCore.bin.CorePython.CoreResourceManager import \
    CoreResourceManager
from sdk.AvatarCore.bin.CorePython.CoreSession import CoreSession
from sdk.AvatarCore.bin.CorePython.Timeout import set_timeout


class CoreManager:
    def __init__(self):
        self.user_ID = None

        self.cursor_pos = [0, 0]
        self.cursor_pressed = False

        self.grip_state = False
        self.cream_state = False
        self.open_state = False
        self.table_state = 0
        self.button_anim = False
        self.button_state = 'fruit'
        self.robot_height = 50
        self.pickup_cream_state = False

        self.new_frame = False
        self.frame_data = None
        self.resource_name = ''
        self.resource_mutex = threading.Lock()

        self.__init_core()

    def __init_core(self):
        self.core_api = CoreAPI()
        self.core_session = CoreSession(self.core_api)

        self.core_api.init_api(app_path='../',
                               master=True,
                               port=0,
                               app_name='UserClient')
        logger.info('Start the core_api')

        self.core_api.load_services('core_services/pancake_core_user_cloud_services.json')
        self.core_api.start_default_services()
        logger.info('Start the default services')

        self.core_api.subscribe_to_message('avatarin', 'cloud/status', self.__cloud_status)
        self.core_api.subscribe_to_message('avatarin', 'cloud/peer/disconnected', self.__peer_disconnected)

        self.core_session.init_session('User/cloud')
        self.core_session.set_peer_id(USER_PEER_ID)
        self.core_api.start_service('User/.*')
        logger.info('Start the services')

        self.__create_resource()
        set_timeout(self.__connect_to_session, 2000)

    def __cloud_status(self, name, path, data):
        print('Cloud status: {0}'.format(data))

    def __connect_to_session(self):
        self.core_session.connect_to_peer(AVATAR_PEER_ID)
        logger.info('Connect to session : %s'%AVATAR_PEER_ID)

    def __peer_disconnected(self, name, path, data):
        logger.info('Peer disconnected : %s'%data)
        sys.exit()

    def __create_resource(self):
        global core_resources

        core_resources = CoreResourceManager(self.__on_resource_frame_arrived)

        core_resources.subscribe_video('micro-camera-RGB')
        core_resources.subscribe_video('fruit-camera-RGB')
        core_resources.subscribe_video('cake-camera-RGB')

        core_resources.subscribe_data('feedback-to-user', 'id')
        core_resources.subscribe_data('feedback-to-user', 'cursor')
        core_resources.subscribe_data('feedback-to-user', 'cursor-pressed')
        core_resources.subscribe_data('feedback-to-user', 'robot-position')
        core_resources.subscribe_data('feedback-to-user', 'grip-state')
        core_resources.subscribe_data('feedback-to-user', 'table-state')
        core_resources.subscribe_data('feedback-to-user', 'button-state')
        core_resources.subscribe_data('feedback-to-user', 'robot-height')
        core_resources.subscribe_data('feedback-to-user', 'pickup_cream')

        core_resources.start(1)

    def __on_resource_frame_arrived(self, type, resource, channel, frame):
        if type == 'data':
            self.__on_data_frame_arrived(resource, channel, frame)

        elif type == 'video':
            self.__on_video_frame_arrived(resource, frame)

    def __on_data_frame_arrived(self, resource, channel, frame):
        logger.debug('receive data : %s - %s - %s'%(resource, channel, frame))

        data = json.loads(frame)

        if channel == 'id':
            if self.user_ID is None:
                if USER_PEER_ID == data['PeerID']:
                    self.user_ID = data['UserID']

        if 'feedback' in resource:
            if channel == 'cursor':
                if not data['user'] == str(self.user_ID):
                    self.cursor_pos = [data['x'], data['y']]

            elif channel == 'cursor-pressed':
                if not data['user'] == str(self.user_ID):
                    self.cursor_pressed = data['pressed']

            elif channel == 'grip-state':
                if data['Button'] == 'grip':
                    self.grip_state = data['state']

                elif data['Button'] == 'cream':
                    self.cream_state = data['state']

                elif data['Button'] == 'open':
                    self.open_state = data['state']

            elif channel == 'table-state':
                self.table_state = data['state']

            elif channel == 'button-state':
                if not data['user'] == str(self.user_ID):
                    self.button_anim = True
                    self.button_state = data['state']

            elif channel == 'robot-height':
                self.robot_height = data['per-z']

            elif channel == 'pickup_cream':
                self.pickup_cream_state = data

    def __on_video_frame_arrived(self, resource, frame):
        try:
            self.resource_mutex.acquire()

            image = QtGui.QImage(frame.pixelData.tobytes(),
                                 frame.width,
                                 frame.height,
                                 QtGui.QImage.Format_RGB888)
            pixmap = QtGui.QPixmap.fromImage(image)

            self.frame_data = pixmap
            self.resource_name = resource
            self.new_frame = True

            self.resource_mutex.release()
        except:
            logger.error('Error in handle new frame!!!')

    def core_write_data(self, channel, message):
        global core_resources

        message = json.dumps(message)
        resource = 'user-%s-data'%self.user_ID
        core_resources.write_data(resource, channel, message)

        logger.debug('send data : %s - %s - %s'%(resource, channel, message))

    def stop_core(self):
        global core_resources

        self.core_session.disconnect_from_peer(AVATAR_PEER_ID)
        self.core_session.clear_session_callbacks()
        logger.info('Stop the core_session')

        core_resources.destroy()
        core_resources = None
        logger.info('Stop the core_resources')

        self.core_api.destroy_api()
        self.core_finished = True
        logger.info('Stop the core_api')

class ColorSet:
    def __init__(self):
        self.main_window_background_color = QtGui.QColor(252, 250, 248)

        self.button_border_color = QtGui.QColor(249, 115, 21)
        self.button_border_width = 2
        self.button_border_pen = QtGui.QPen(self.button_border_color,
                                            self.button_border_width,
                                            QtCore.Qt.SolidLine)

        self.button_font_size = 14
        self.button_font = QtGui.QFont('Arial',
                                       self.button_font_size,
                                       weight=QtGui.QFont.Normal)
        self.button_font_color = QtGui.QColor(139, 116, 100)

        self.button_hover_color = QtGui.QColor.fromRgbF(255, 217, 127, 0.2)

        self.pos_btn_color = np.array([252, 250, 248])
        self.pos_btn_on_color = np.array([249, 115, 21])

        self.button_style = f"""
        QLabel#parent{{
            background-color: rgb%s;
            border-style: solid;
            border-width: %s;
            border-color: rgb%s;
        }}
        """%(self.main_window_background_color.getRgb()[:-1],
             self.button_border_width,
             self.button_border_color.getRgb()[:-1])

    def setBtnPress(self, widget:QtWidgets.QLabel, event):
        def __on_time():
            widget.setGraphicsEffect(None)

        self.color_effect = QtWidgets.QGraphicsColorizeEffect()
        self.color_effect.setColor(QtGui.QColor(self.pos_btn_on_color[0],
                                                self.pos_btn_on_color[1],
                                                self.pos_btn_on_color[2]))
        widget.setGraphicsEffect(self.color_effect)
        QtCore.QTimer.singleShot(150, __on_time)

class FontSize:
    def __init__(self):
        self.pos_btn_font_size = 32

        self.height_widget_font_size = 20
        self.slider_value_font_size = 26
        self.height_push_btn_font_size = 38

        self.rotate_private1_font_size = 22
        self.rotate_private2_font_size = 32

        self.roll_private1_font_size = 20

        self.toggle_widget_font_size = 15

        self.fruit_font_size = 12

        self.cream_private1_font_size = 18
        self.cream_private2_font_size = 32

        self.squeeze_private1_font_size = 22
        self.squeeze_private2_font_size = 24

class MainWin(QtWidgets.QMainWindow):
    on_signal = QtCore.Signal(object)
    send_frame = QtCore.Signal(str, QtGui.QPixmap)
    send_table_state = QtCore.Signal(int)
    send_button_state = QtCore.Signal(str)
    send_height = QtCore.Signal(float)
    get_cursor = QtCore.Signal(dict)
    get_status = QtCore.Signal(dict)

    def __init__(self, core, parent=None):
        super().__init__()

        self.core = core

        self.setWindowTitle(os.path.basename(__file__))
        self.setCentralWidget(parent)
        self.setMouseTracking(True)
        self.setStyleSheet('background-color: rgb(%s,%s,%s)'%color_set.main_window_background_color.getRgb()[:-1])

        self.first_win_width = 1280
        self.first_win_ratio = 0.63
        self.resize(self.first_win_width,
                    self.first_win_width*self.first_win_ratio)

        screen = app.primaryScreen()
        self.move(screen.size().width()/2-self.first_win_width/2, 0)

        minimum_win_scale = 0.4
        self.setMinimumSize(self.first_win_width*minimum_win_scale,
                            self.first_win_ratio*self.first_win_ratio*minimum_win_scale)

        self.on_signal.connect(self.get_press_signal)

        self.old_cursor_pos_per = (0, 0)
        self.partner_cursor_size = 60
        self.cursor_size = self.partner_cursor_size
        self.partner_cursor = QtWidgets.QLabel(self)
        self.cursor_style = f"""
        QLabel{{
            background-color: rgba(83,191,245,100);
            min-width: %spx;
            min-height: %spx;
            max-width: %spx;
            max-height: %spx;
            border-radius: %spx;
        }}
        """%(self.partner_cursor_size,
             self.partner_cursor_size,
             self.partner_cursor_size,
             self.partner_cursor_size,
             self.partner_cursor_size/2)
        self.partner_cursor.setStyleSheet(self.cursor_style)
        self.partner_cursor.move(50, 50)

        self.cursor_update = QtCore.QTimer()
        self.cursor_update.timeout.connect(self.__update_cursor)
        self.cursor_update.start(5)

        self.update_timer = QtCore.QTimer()
        self.update_timer.timeout.connect(self.__update)
        self.update_timer.start(1)

    def get_press_signal(self, widget):
        self.core.core_write_data('command', widget.message)

    def mousePressEvent(self, event:QtGui.QMouseEvent):
        message = {'pressed': True}
        self.core.core_write_data('cursor-pressed', message)

    def resizeEvent(self, event:QtGui.QResizeEvent):
        scale_width = self.width()/self.first_win_width
        scale_height = self.height()/self.first_win_width*self.first_win_ratio
        scale = min(scale_width, scale_height)

        self.cursor_size = int(self.partner_cursor_size*scale)

    def __update(self):
        self.update()

        self.__update_data()
        self.__update_video()

    def __update_data(self):
        self.__update_state()
        self.__update_height()

    def __update_state(self):
        self.send_table_state.emit(self.core.table_state)
        self.get_status.emit(self.__get_state())

        if self.core.button_anim:
            self.send_button_state.emit(self.core.button_state)
            self.core.button_anim = False

    def __get_state(self):
        state_dict = {'grip': self.core.grip_state,
                      'cream': self.core.cream_state,
                      'open': self.core.open_state,
                      'pickup_cream': self.core.pickup_cream_state}
        return state_dict

    def __update_height(self):
        self.send_height.emit(self.core.robot_height)

    def __update_video(self):
        if self.core.new_frame:
            self.core.resource_mutex.acquire()
            self.__update_frame(self.core.resource_name, self.core.frame_data)
            self.core.new_frame = False
            self.core.resource_mutex.release()

    def __update_frame(self, resource, frame):
        self.send_frame.emit(resource, frame)

    def __update_cursor(self):
        self.__get_cursor_pos()
        self.__update_cursor_draw()

    def __get_cursor_pos(self):
        self.cursor_pos = self.mapFromGlobal(QtGui.QCursor.pos())
        self.window_size = self.geometry()

        if self.cursor_pos.x() > 0 and \
            self.cursor_pos.y() > 0:
            if self.cursor_pos.x() < self.window_size.width() and \
                self.cursor_pos.y() < self.window_size.height():
                    self.cursor_pos_per_x = self.cursor_pos.x()/self.window_size.width()
                    self.cursor_pos_per_y = self.cursor_pos.y()/self.window_size.height()

                    if not (self.cursor_pos_per_x == self.old_cursor_pos_per[0] and self.cursor_pos_per_y == self.old_cursor_pos_per[1]):
                        message = {'x':self.cursor_pos_per_x,
                                   'y':self.cursor_pos_per_y}
                        self.core.core_write_data('cursor', message)

                    self.old_cursor_pos_per = (self.cursor_pos_per_x, self.cursor_pos_per_y)

    def __update_cursor_draw(self):
        def __on_time():
            self.cursor_style = f"""
            QLabel{{
                background-color: rgba(83,191,245,100);
                min-width: %spx;
                min-height: %spx;
                max-width: %spx;
                max-height: %spx;
                border-radius: %spx;
            }}
            """%(self.cursor_size,
                 self.cursor_size,
                 self.cursor_size,
                 self.cursor_size,
                 int(self.cursor_size/2))
            self.partner_cursor.setStyleSheet(self.cursor_style)
            self.core.cursor_pressed = False

        self.cursor_style = f"""
        QLabel{{
            background-color: rgba(83,191,245,100);
            min-width: %spx;
            min-height: %spx;
            max-width: %spx;
            max-height: %spx;
            border-radius: %spx;
        }}
        """%(self.cursor_size,
                self.cursor_size,
                self.cursor_size,
                self.cursor_size,
                int(self.cursor_size/2))
        self.partner_cursor.setStyleSheet(self.cursor_style)
        self.partner_cursor.move(self.width()*self.core.cursor_pos[0]-self.partner_cursor.width()/2,
                                 self.height()*self.core.cursor_pos[1]-self.partner_cursor.height()/2)

        if self.core.cursor_pressed:
            self.cursor_style = f"""
            QLabel{{
                background-color: rgba(249,115,21,100);
                min-width: %spx;
                min-height: %spx;
                max-width: %spx;
                max-height: %spx;
                border-radius: %spx;
            }}
            """%(self.cursor_size,
                 self.cursor_size,
                 self.cursor_size,
                 self.cursor_size,
                 int(self.cursor_size/2))
            self.partner_cursor.setStyleSheet(self.cursor_style)
            QtCore.QTimer.singleShot(150, __on_time)

    def closeEvent(self, event):
        self.core.stop_core()
        event.accept()
        sys.exit()

class VideoFrame(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        layout = QtWidgets.QHBoxLayout(self)
        layout.setContentsMargins(11, 11, 11, 0)

        left_layout = QtWidgets.QVBoxLayout()
        right_layout = QtWidgets.QVBoxLayout()

        layout.addLayout(left_layout, 2)
        layout.addLayout(right_layout, 1)

        self.per_y = 0.55
        self.micro_video = VideoWidget('micro', self.per_y, parent=self)
        self.fruit_video = VideoWidget('fruit', parent=self)
        self.cake_video = VideoWidget('cake', parent=self)

        left_layout.addWidget(self.micro_video)
        right_layout.addWidget(self.cake_video)
        right_layout.addWidget(self.fruit_video)

class VideoWidget(QtWidgets.QWidget):
    def __init__(self, name, per_y=None, parent=None):
        super().__init__(parent=parent)

        self.name = name
        self.per_y = per_y
        self.setWidgetFlag = False

    def resizeEvent(self, event):
        if not self.setWidgetFlag:
            self.private_widget = VideoWidgetPrivate(self.name, self.per_y, parent=self)
            self.setWidgetFlag = True

        self.private_widget.move(self.width()/2-self.private_widget.width()/2,
                                 self.height()/2-self.private_widget.height()/2)
        self.private_widget.resize(self.private_widget.loading_image.scaled(event.size(), QtCore.Qt.KeepAspectRatio).size())

class VideoWidgetPrivate(QtWidgets.QLabel, ColorSet):
    def __init__(self, name, per_y, parent=None):
        super().__init__(parent=parent)
        ColorSet.__init__(self)

        self.name = name
        self.per_y = per_y
        self.message = {}
        self.setScaledContents(True)
        self.setAlignment(QtCore.Qt.AlignCenter)

        self.loading_image = QtGui.QPixmap(IP.LOADING_FIG_PATH)
        self.loading_image = self.loading_image.scaled(self.parentWidget().size(),
                                                       QtCore.Qt.KeepAspectRatio,
                                                       QtCore.Qt.SmoothTransformation)

        self.resize(self.loading_image.size())
        self.setPixmap(self.loading_image)

        if self.name == 'micro':
            self.crosshair_widget = CrosshairWidget(self.per_y, parent=self)

        main_win.send_frame.connect(self.updateFrame)

    def mousePressEvent(self, event):
        def __on_time():
            self.setGraphicsEffect(None)

        self.color_effect = QtWidgets.QGraphicsColorizeEffect()
        self.color_effect.setColor(QtGui.QColor(self.pos_btn_on_color[0],
                                                self.pos_btn_on_color[1],
                                                self.pos_btn_on_color[2],
                                                50))
        self.setGraphicsEffect(self.color_effect)
        QtCore.QTimer.singleShot(150, __on_time)

        if self.name in ['fruit', 'cake']:
            self.message = {'Button': self.name, 'bool': True}

        elif self.name == 'micro':
            pos_micro_per_x = (event.position().x()-self.width()/2)/(self.width()/2)

            if event.position().y() < self.height()*self.per_y:
                pos_micro_per_y = (self.height()*self.per_y-event.position().y())/(self.height()*self.per_y)

            elif event.position().y() >= self.height()*self.per_y:
                pos_micro_per_y = -(1-(self.height()*(1-self.per_y)-(event.position().y()-self.height()*self.per_y))/(self.height()*(1-self.per_y)))

            self.message = {'Button': self.name, 'bool': (pos_micro_per_x, pos_micro_per_y)}

        main_win.on_signal.emit(self)
        main_win.mousePressEvent(event)

    def updateFrame(self, resource, frame):
        if self.name in resource:
            self.setPixmap(frame)

class CrosshairWidget(QtWidgets.QLabel, ColorSet):
    def __init__(self, per_y, parent=None):
        super().__init__(parent=parent)
        ColorSet.__init__(self)

        self.crosshair_size = self.parentWidget().width()*0.1
        self.crosshair_margin = 6
        self.per_y = per_y

        self.crosshair_style = f"""
        QLabel{{
            background-color: transparent;
            min-width: %spx;
            min-height: %spx;
            max-width: %spx;
            max-height: %spx;
        }}
        """%(self.parentWidget().width(),self.parentWidget().height(), self.parentWidget().width(), self.parentWidget().height())

        self.setStyleSheet(self.crosshair_style)

    def paintEvent(self, event:QtGui.QPaintEvent):
        painter = QtGui.QPainter(self)
        painter.setRenderHint(QtGui.QPainter.Antialiasing)

        painter.setPen(self.button_border_pen)

        self.crosshair_size = self.parentWidget().width()*0.02
        painter.drawLine(self.parentWidget().width()/2-self.crosshair_size/2,
                         self.parentWidget().height()*self.per_y,
                         self.parentWidget().width()/2+self.crosshair_size/2,
                         self.parentWidget().height()*self.per_y)
        painter.drawLine(self.parentWidget().width()/2,
                         self.parentWidget().height()*self.per_y-self.crosshair_size/2,
                         self.parentWidget().width()/2,
                         self.parentWidget().height()*self.per_y+self.crosshair_size/2)

class ButtonFrame(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        layout = QtWidgets.QHBoxLayout(self)
        layout.setContentsMargins(11, 0, 11, 11)

        self.pos_widget = PosBtnWidget(parent=self)
        self.height_widget = HeightBtnWidget(parent=self)
        self.rotate_widget = RotateBtnWidget(parent=self)
        self.status_widget = StatusWidget(parent=self)
        self.gripper_widget = GripperBtnWidget(parent=self)

        layout.addWidget(self.pos_widget, 3)
        layout.addWidget(self.height_widget, 2)
        layout.addWidget(self.rotate_widget, 2)
        layout.addWidget(self.rotate_widget, 2)
        layout.addWidget(self.status_widget, 2)
        layout.addWidget(self.gripper_widget, 2)

class LabelBtnWidget(QtWidgets.QLabel, ColorSet):
    def __init__(self, name, parent=None):
        super().__init__(parent=parent)
        ColorSet.__init__(self)

        self.setObjectName('parent')
        self.setStyleSheet(self.button_style)

        self.name = name
        self.message = {'Button': self.name, 'bool':True}
        self.base_btn_size = None

        self.btn_layout = QtWidgets.QGridLayout(self)
        self.btn_layout.setContentsMargins(0, 0, 0, 0)
        self.btn_layout.setSpacing(0)

    def resizeEvent(self, event):
        if not self.base_btn_size:
            self.base_btn_size = self.size()

        btn_width_scale = self.width()/self.base_btn_size.width()
        btn_height_scale = self.height()/self.base_btn_size.height()
        self.btn_scale = min(btn_width_scale, btn_height_scale)

    def mousePressEvent(self, event):
        self.setBtnPress(self, event)
        main_win.get_press_signal(self)
        main_win.mousePressEvent(event)

class LabelBtnWidgetPrivate(QtWidgets.QLabel, ColorSet):
    get_scale = QtCore.Signal(float)

    def __init__(self, parent=None):
        super().__init__()
        ColorSet.__init__(self)

        self.base_font_size = None
        self.font_size = self.base_font_size

        self.base_fig_scale = None
        self.fig_scale = None
        self.fig_pixmap = None

        self.setAlignment(QtCore.Qt.AlignCenter)
        self.setStyleSheet('color: rgb(%s,%s,%s)'%self.button_font_color.getRgb()[:-1])
        self.setFont(self.button_font)

        self.get_scale.connect(self.getParentScale)

    def setFontSize(self):
        if not self.font_size:
            self.font_size = self.base_font_size
        _font = self.button_font
        _font.setPointSizeF(self.font_size)
        self.setFont(_font)

    def setFontBold(self):
        _font = self.button_font
        _font.setBold(True)
        self.setFont(_font)

    def setFigSize(self):
        if not self.fig_scale:
            self.fig_scale = self.base_fig_scale
            self.base_fig_pixmap = self.fig_pixmap

        _fig_pixmap = self.base_fig_pixmap.scaled(self.base_fig_pixmap.size()*self.fig_scale,
                                                  QtCore.Qt.KeepAspectRatio,
                                                  QtCore.Qt.SmoothTransformation)

        self.setPixmap(_fig_pixmap)

    def getParentScale(self, scale):
        if bool(self.text()):
            self.font_size = self.base_font_size*scale
            self.setFontSize()

        elif bool(self.pixmap()):
            self.fig_scale = self.base_fig_scale*scale
            self.setFigSize()

class PosBtnWidget(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.pos_widget_private = PosBtnWidgetPrivate(parent=self)

    def paintEvent(self, event):
        painter = QtGui.QPainter(self)
        painter.setRenderHint(QtGui.QPainter.Antialiasing)
        self.pos_widget_private.draw(painter)

    def mousePressEvent(self, event):
        self.pos_widget_private.get_mouse_pos.emit(event)

class PosBtnWidgetPrivate(QtWidgets.QLabel, ColorSet):
    get_mouse_pos = QtCore.Signal(object)

    def __init__(self, parent=None):
        super().__init__(parent=parent)
        ColorSet.__init__(self)

        self.first_widget_size = None

        self.btn_size = 0.3
        self.front_penta = PentaPoints(name='front',
                                       text='前',
                                       scale=self.btn_size,
                                       translate=(0.5, 0.05),
                                       rotate=0,
                                       parent=parent)
        self.right_penta = PentaPoints(name='right',
                                       text='右',
                                       scale=self.btn_size,
                                       translate=(0.95, 0.5),
                                       rotate=90,
                                       parent=parent)
        self.back_penta = PentaPoints(name='back',
                                      text='後',
                                      scale=self.btn_size,
                                      translate=(0.5, 0.95),
                                      rotate=180,
                                      parent=parent)
        self.left_penta = PentaPoints(name='left',
                                      text='左',
                                      scale=self.btn_size,
                                      translate=(0.05, 0.5),
                                      rotate=270,
                                      parent=parent)

        self.get_mouse_pos.connect(self.press_event)

    def draw(self, painter:QtGui.QPainter):
        if not self.first_widget_size:
            self.first_widget_size = self.parentWidget().size()

        widget_size = self.parentWidget().size()

        self.front_penta.draw_penta(widget_size, painter)
        self.right_penta.draw_penta(widget_size, painter)
        self.back_penta.draw_penta(widget_size, painter)
        self.left_penta.draw_penta(widget_size, painter)

    def press_event(self, event):
        self.front_penta.determine_inside(event)
        self.right_penta.determine_inside(event)
        self.back_penta.determine_inside(event)
        self.left_penta.determine_inside(event)

class PentaPoints(QtCore.QObject, ColorSet, FontSize):
    def __init__(self, name, text, scale, translate, rotate, parent):
        super().__init__(parent=parent)
        ColorSet.__init__(self)
        FontSize.__init__(self)

        self.name = name
        self.message = {'Button': self.name, 'bool': True}

        self.btn_name = name
        self.btn_text = text
        self.btn_scale = scale
        self.btn_translate = translate
        self.btn_rotate = rotate
        self.btn_parent = parent
        self.btn_base_font_size = self.pos_btn_font_size
        self.base_widget_size = None

        self._animValue = 0.0
        self.anim = QtCore.QPropertyAnimation(self)
        self.anim.setTargetObject(self)
        self.anim.setPropertyName(b'press')
        self.anim.setStartValue(0.0)
        self.anim.setEndValue(1.0)
        self.anim.setDuration(300)
        self.anim.setEasingCurve(QtCore.QEasingCurve.Linear)
        self.anim.finished.connect(self.btn_parent.update())

    def draw_penta(self, widget_size:QtCore.QSize, painter:QtGui.QPainter):
        if not self.base_widget_size:
            self.base_widget_size = widget_size

        scale_w = widget_size.width()/self.base_widget_size.width()
        scale_h = widget_size.height()/self.base_widget_size.height()
        scale = min(scale_w, scale_h)

        painter.save()

        painter.setPen(self.button_border_pen)
        diff_color = self.pos_btn_color - self.pos_btn_on_color
        painter.setBrush(QtGui.QColor(self.pos_btn_color[0]-diff_color[0]*self._animValue,
                                      self.pos_btn_color[1]-diff_color[1]*self._animValue,
                                      self.pos_btn_color[2]-diff_color[2]*self._animValue))

        self.widget_size = widget_size
        if self.btn_rotate in (0, 180):
            penta_width = widget_size.width()*self.btn_scale*0.85*(1+self._animValue*0.05)
            penta_height = widget_size.height()*self.btn_scale*(1+self._animValue*0.05)
        elif self.btn_rotate in (90, 270):
            penta_width = widget_size.height()*self.btn_scale*0.85*(1+self._animValue*0.05)
            penta_height = widget_size.width()*self.btn_scale*(1+self._animValue*0.05)

        q_points = [QtCore.QPointF(0, 0),
                    QtCore.QPointF(penta_width/2, 0),
                    QtCore.QPointF(penta_width/2, penta_height),
                    QtCore.QPointF(0, penta_height*1.4),
                    QtCore.QPointF(-penta_width/2, penta_height),
                    QtCore.QPointF(-penta_width/2, 0)]

        self.rotate_points = self.points_rotate(q_points, self.btn_rotate)
        self.translate_points = self.points_translate(self.rotate_points, self.btn_translate)
        self.poly = QtGui.QPolygonF(self.translate_points)
        painter.drawPolygon(self.poly)

        _font = self.button_font
        _font.setPointSizeF(self.btn_base_font_size*scale)
        _font.setBold(True)
        painter.setFont(self.button_font)

        button_font_color = np.array(self.button_font_color.getRgb()[:-1])
        font_diff_color = button_font_color - np.array([255, 255, 255])
        painter.setPen(QtGui.QColor(button_font_color[0]-font_diff_color[0]*self._animValue,
                                    button_font_color[1]-font_diff_color[1]*self._animValue,
                                    button_font_color[2]-font_diff_color[2]*self._animValue))

        if self.btn_text == '前':
            painter.drawText(self.widget_size.width()*0.46,
                             self.widget_size.height()*0.2,
                             self.btn_text)

        elif self.btn_text == '右':
            painter.drawText(self.widget_size.width()*0.8,
                             self.widget_size.height()*0.53,
                             self.btn_text)

        elif self.btn_text == '後':
            painter.drawText(self.widget_size.width()*0.46,
                             self.widget_size.height()*0.85,
                             self.btn_text)

        elif self.btn_text == '左':
            painter.drawText(self.widget_size.width()*0.13,
                             self.widget_size.height()*0.53,
                             self.btn_text)

        painter.restore()

    def points_translate(self, points, diff):
        translate_points = []
        for i in points:
            translate_x = i.x()+self.widget_size.width()*diff[0]
            translate_y = i.y()+self.widget_size.height()*diff[1]
            translate_points.append(QtCore.QPointF(translate_x, translate_y))
        return translate_points

    def points_rotate(self, points, angle):
        rotate_points = []
        for i in points:
            rotate_x = i.x()*np.cos(np.radians(angle))-i.y()*np.sin(np.radians(angle))
            rotate_y = i.x()*np.sin(np.radians(angle))+i.y()*np.cos(np.radians(angle))
            rotate_points.append(QtCore.QPointF(rotate_x, rotate_y))
        return rotate_points

    @QtCore.Property(float)
    def press(self):
        return self._animValue

    @press.setter
    def press(self, value):
        self._animValue = -4*value*(value-1)
        self.btn_parent.update()

    @QtCore.Slot(name='animate')
    def animate(self):
        self.anim.setDirection(QtCore.QPropertyAnimation.Forward)
        self.anim.start()

    def determine_inside(self, event):
        polygon = []
        for i in self.translate_points:
            polygon.append([int(i.x()), int(i.y())])
        polygon = np.array(polygon)

        mPos = (event.position().x(), event.position().y())

        if cv2.pointPolygonTest(polygon, mPos, False) > 0:
            main_win.get_press_signal(self)
            main_win.mousePressEvent(event)
            self.animate()

class HeightBtnWidget(QtWidgets.QWidget, ColorSet, FontSize):
    def __init__(self, parent=None):
        super().__init__(parent=parent)
        ColorSet.__init__(self)
        FontSize.__init__(self)

        layout = QtWidgets.QGridLayout(self)
        layout.setContentsMargins(0, 0, 15, 0)
        layout.setSpacing(15)

        self.base_widget_size = None
        self.base_font_size = self.height_widget_font_size

        self.text_label = QtWidgets.QLabel('高さ')
        _font = self.button_font
        _font.setPointSize(self.base_font_size)
        self.text_label.setFont(_font)
        style = f"""
        color: rgb(%s,%s,%s);
        border-bottom-style: solid;
        border-bottom-width: 2px;
        border-radius: 0px;
        border-color: rgba(139,116,100,50);
        """%self.button_font_color.getRgb()[:-1]
        self.text_label.setStyleSheet(style)
        self.text_label.setAlignment(QtCore.Qt.AlignCenter)

        self.height_slider = HeightSliderWidget(parent=self)
        self.height_push_btn = HeightPushBtnWidget(parent=self)

        layout.addWidget(self.text_label, 0, 0, 1, 2)
        layout.addWidget(self.height_slider, 1, 0, 30, 1)
        layout.addWidget(self.height_push_btn, 1, 1, 30, 1)

    def resizeEvent(self, event):
        if not self.base_widget_size:
            self.base_widget_size = self.size()

        btn_width_scale = self.width()/self.base_widget_size.width()
        btn_height_scale = self.height()/self.base_widget_size.height()
        self.btn_scale = min(btn_width_scale, btn_height_scale)

        _font_size = self.base_font_size*self.btn_scale
        _font = self.button_font
        _font.setPointSizeF(_font_size)
        self.text_label.setFont(_font)

class HeightSliderWidget(QtWidgets.QLabel):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.widget_layout = QtWidgets.QHBoxLayout(self)
        self.widget_layout.setContentsMargins(0, 0, 0, 0)

        self.first_value = 50
        self.value_widget = ValueWidget(parent=self)
        self.slider_widget = SliderWidget(parent=self)

        self.widget_layout.addWidget(self.value_widget)
        self.widget_layout.addWidget(self.slider_widget)

class SliderWidget(QtWidgets.QSlider):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        slider_style = f"""
        QSlider::add-page:vertical{{
            background-color: #E1D5CB;
            width: 5px;
            border-radius: 3px;
        }}

        QSlider::sub-page:vertical{{
            background-color: #F97315;
            width: 5px;
            border-radius: 3px;
        }}

        QSlider::groove:vertical{{
            background-color: transparent;
            width: 6px;
        }}

        QSlider::handle:vertical{{
            height: 10px;
            width: 15px;
            margin: 0px -15px;
            border-radius: 10px;
            background-color: rgb(139,116,100);
        }}
        """
        self.setStyleSheet(slider_style)

        self.parent_widet = parent

        self.setOrientation(QtCore.Qt.Vertical)
        self.setMinimum(0)
        self.setMaximum(100)
        self.setValue(self.parent_widet.first_value)
        self.setTickPosition(self.TicksBothSides)
        self.setTickInterval(10)

    def mousePressEvent(self, event:QtGui.QMouseEvent) -> None:
        main_win.mousePressEvent(QtGui.QMouseEvent)

        sliderValue = (1-event.position().y()/self.height())*100

        self.message = {'Button': 'height_slider', 'bool': sliderValue}
        main_win.on_signal.emit(self)

class ValueWidget(QtWidgets.QLabel, ColorSet, FontSize):
    def __init__(self, parent=None):
        super().__init__(parent=parent)
        ColorSet.__init__(self)
        FontSize.__init__(self)

        self.parent_widget = parent
        self.slider_value = self.parent_widget.first_value
        self.base_font_size = self.slider_value_font_size
        self.base_widget_size = None
        self.scale = 1

    def paintEvent(self, event):
        painter = QtGui.QPainter(self)
        painter.setRenderHint(QtGui.QPainter.Antialiasing)

        if not self.base_widget_size:
            self.base_widget_size = self.size()
            main_win.send_height.connect(self.__get_value)

        scale_width = self.width()/self.base_widget_size.width()
        scale_height = self.height()/self.base_widget_size.height()
        self.scale = min(scale_width, scale_height)

        painter.setPen(self.button_font_color)
        _font = self.button_font
        _font.setPointSizeF(self.base_font_size*self.scale)
        painter.setFont(self.button_font)

        painter.drawText(self.width()*0.25,
                         self.height()*0.9*(1-0.01*self.slider_value)+self.height()*0.09,
                         str(self.slider_value))

        self.update()

    def __get_value(self, value):
        self.slider_value = int(value)
        self.parent_widget.slider_widget.setValue(self.slider_value)

class HeightPushBtnWidget(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.widget_layout = QtWidgets.QVBoxLayout(self)
        self.widget_layout.setContentsMargins(0, 0, 0, 0)
        self.widget_layout.setSpacing(10)

        self.height_plus = HeightPushBtn(name='up',
                                         text='＋',
                                         parent=self)

        self.height_minus = HeightPushBtn(name='down',
                                          text='ー',
                                          parent=self)

        self.widget_layout.addStretch(1)
        self.widget_layout.addWidget(self.height_plus, 20)
        self.widget_layout.addWidget(self.height_minus, 20)
        self.widget_layout.addStretch(1)

class HeightPushBtn(LabelBtnWidget, FontSize):
    def __init__(self, name, text, parent=None):
        super().__init__(name=name, parent=parent)
        FontSize.__init__(self)

        self.private_1 = LabelBtnWidgetPrivate(parent=parent)
        self.private_1.base_font_size = self.height_push_btn_font_size
        self.private_1.setFontSize()
        self.setAlignment(QtCore.Qt.AlignCenter)
        self.private_1.setText(text)

        self.btn_layout.addWidget(self.private_1, 0, 0, 1, 1)

    def resizeEvent(self, event):
        super().resizeEvent(event)

        self.private_1.get_scale.emit(self.btn_scale)

class RotateBtnWidget(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.widget_layout = QtWidgets.QGridLayout(self)
        self.widget_layout.setContentsMargins(0, 0, 0, 0)

        self.rotate_table = RotateTableWidget('table', parent=self)
        self.roll_minus = RollBtnWidget('roll_minus',
                                       ['反時計回り', '↺'],
                                       IP.ROLL_MINUS_FIG_PATH,
                                       parent=self)
        self.roll_plus = RollBtnWidget('roll_plus',
                                        ['時計周り', '↻'],
                                        IP.ROLL_PLUS_FIG_PATH,
                                        parent=self)

        self.widget_layout.addWidget(self.rotate_table, 0, 0, 1, 2)
        self.widget_layout.addWidget(self.roll_minus, 1, 0, 2, 1)
        self.widget_layout.addWidget(self.roll_plus, 1, 1, 2, 1)

class RotateTableWidget(LabelBtnWidget, FontSize):
    def __init__(self, name, parent=None):
        super().__init__(name=name, parent=parent)
        FontSize.__init__(self)

        self.setSignal = False

        self.private_1 = LabelBtnWidgetPrivate(parent=self)
        self.private_1.base_font_size = self.rotate_private1_font_size
        self.private_1.setFontSize()
        self.private_1.setAlignment(QtCore.Qt.AlignBottom|QtCore.Qt.AlignHCenter)
        self.private_1.setText('テーブル回転')

        self.private_2 = LabelBtnWidgetPrivate(parent=self)
        self.private_2.base_font_size = self.rotate_private2_font_size
        self.private_2.setFontSize()
        self.private_2.setText('スタート')

        self.btn_layout.addWidget(self.private_1, 0, 0, 3, 1)
        self.btn_layout.addWidget(self.private_2, 3, 0, 4, 1)

    def resizeEvent(self, event):
        if not self.setSignal:
            main_win.send_table_state.connect(self.__get_table_state)
            self.setSignal = True

        super().resizeEvent(event)

        self.private_1.get_scale.emit(self.btn_scale)
        self.private_2.get_scale.emit(self.btn_scale)

    def mousePressEvent(self, event):
        super().mousePressEvent(event)

    def __get_table_state(self, state):
        if state == 0:
            self.private_2.setText('スタート')
        elif state == 1:
            self.private_2.setText('ストップ')

class RollBtnWidget(LabelBtnWidget, FontSize):
    def __init__(self, name, text, file_path, parent=None):
        super().__init__(name=name, parent=parent)
        FontSize.__init__(self)

        self.roll_state = False

        self.btn_layout.setSpacing(10)

        self.private_1 = LabelBtnWidgetPrivate(parent=self)
        self.private_1.setAlignment(QtCore.Qt.AlignBottom|QtCore.Qt.AlignHCenter)
        self.private_1.base_font_size = self.roll_private1_font_size
        self.private_1.setFontSize()
        self.private_1.setText(text[0])

        self.private_2 = LabelBtnWidgetPrivate(parent=self)
        self.private_2.setAlignment(QtCore.Qt.AlignTop|QtCore.Qt.AlignHCenter)
        self.private_2.base_fig_scale = 0.5
        self.private_2.fig_pixmap = QtGui.QPixmap(file_path)
        self.private_2.setFigSize()

        self.btn_layout.addWidget(self.private_1, 0, 0, 3, 1)
        self.btn_layout.addWidget(self.private_2, 3, 0, 5, 1)

    def resizeEvent(self, event):
        super().resizeEvent(event)

        self.private_1.get_scale.emit(self.btn_scale)
        self.private_2.get_scale.emit(self.btn_scale)

class StatusWidget(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.first_widget_size = None

        self.status_list = ['何も持っていません',
                            'フルーツを持っています',
                            'ホイップを持っています',
                            'ホイップ戻し中',
                            'ホイップ絞り中']

        self.status_layout = QtWidgets.QVBoxLayout(self)
        self.status_layout.setContentsMargins(0, 10, 0, 5)
        self.status_layout.setSpacing(10)

        for i, status in enumerate(self.status_list):
            status_label = StatusWidgetPrivate(status, self)
            self.status_layout.addWidget(status_label)

class StatusWidgetPrivate(QtWidgets.QLabel, ColorSet):
    def __init__(self, text, parent=None):
        super().__init__(parent=parent)
        ColorSet.__init__(self)

        self.base_font_size = 16
        self.base_widget_size = None

        self.setAlignment(QtCore.Qt.AlignCenter)
        self.setText(text)

    def resizeEvent(self, event:QtGui.QResizeEvent):
        if not self.base_widget_size:
            self.base_widget_size = self.size()
            main_win.get_status.connect(self.check_status_signal)

        scale = (self.width()/self.base_widget_size.width(),
                 self.height()/self.base_widget_size.height())
        scale = min(scale)

        self.status_off_style = f"""
        QLabel{{
            color: rgba(139, 116, 100, 130);
            background-color: rgba(235, 232, 230, 100);
            border-style: solid;
            border-width: 3px;
            border-color: rgba(235, 232, 230, 150);
            border-radius: %spx
        }}
        """%(self.height()*0.4)

        self.status_on_style = f"""
        QLabel{{
            color: rgb(85, 65, 51);
            background-color: rgba(255, 237, 213, 150);
            border-style: solid;
            border-width: 3px;
            border-color: rgba(255, 237, 213, 250);
            border-radius: %spx
        }}
        """%(self.height()*0.4)

        self.setStyleSheet(self.status_off_style)

        if self.text() == '何も持っていません':
            self.setStyleSheet(self.status_on_style)

        _font = self.button_font
        _font.setPointSizeF(self.base_font_size*scale)
        self.setFont(_font)

    def check_status_signal(self, data):
        if data['pickup_cream']:
            if data['pickup_cream']['state']:
                if self.text() == '何も持っていません':
                    self.setStyleSheet(self.status_off_style)
                if data['pickup_cream']['state'] == 'open':
                    if self.text() == 'ホイップを持っています':
                        self.setStyleSheet(self.status_on_style)
                    elif self.text() == 'ホイップ絞り中':
                        if data['cream']:
                            self.setStyleSheet(self.status_on_style)
                        else:
                            self.setStyleSheet(self.status_off_style)
                elif data['pickup_cream']['state'] == 'grip':
                    if self.text() == 'ホイップを持っています':
                        self.setStyleSheet(self.status_off_style)
                    elif self.text() == 'ホイップ戻し中':
                        self.setStyleSheet(self.status_on_style)
                    if self.text() == 'フルーツを持っています':
                        if data['grip'] == 0:
                            self.setStyleSheet(self.status_off_style)
                        elif data['grip'] == 1:
                            self.setStyleSheet(self.status_on_style)
            elif data['pickup_cream']['state'] == None:
                if self.text() == 'ホイップ戻し中':
                    self.setStyleSheet(self.status_off_style)
                if data['grip'] == 0:
                    if self.text() == 'フルーツを持っています':
                        self.setStyleSheet(self.status_off_style)
                    elif self.text() == '何も持っていません':
                        self.setStyleSheet(self.status_on_style)
                elif data['grip'] == 1:
                    if self.text() == 'フルーツを持っています':
                        self.setStyleSheet(self.status_on_style)
                    elif self.text() == '何も持っていません':
                        self.setStyleSheet(self.status_off_style)
        else:
            if data['grip'] == 0:
                    if self.text() == 'フルーツを持っています':
                        self.setStyleSheet(self.status_off_style)
                    elif self.text() == '何も持っていません':
                        self.setStyleSheet(self.status_on_style)
            elif data['grip'] == 1:
                if self.text() == 'フルーツを持っています':
                    self.setStyleSheet(self.status_on_style)
                elif self.text() == '何も持っていません':
                    self.setStyleSheet(self.status_off_style)


class GripperBtnWidget(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.widget_layout = QtWidgets.QGridLayout(self)
        self.widget_layout.setContentsMargins(0, 0, 0, 0)

        self.mode_toggle = ToggleBtnWidget(parent=self)
        self.grip_widget = GripperBtnStacked(parent=self)

        self.widget_layout.addWidget(self.mode_toggle, 0, 0, 1, 2)
        self.widget_layout.addWidget(self.grip_widget, 1, 0, 2, 2)

        self.mode_toggle.mode_signal.connect(self.__get_mode)

    def __get_mode(self, mode):
        self.grip_widget.check_mode.emit(mode)

class ToggleBtnWidget(QtWidgets.QLabel):
    mode_signal = QtCore.Signal(str)
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.toggle_widget_private = ToggleBtnWidgetPrivate(parent=self)

    def paintEvent(self, event):
        painter = QtGui.QPainter(self)
        painter.setRenderHints(QtGui.QPainter.Antialiasing)
        self.toggle_widget_private.draw(painter)

    def mousePressEvent(self, event):
        main_win.mousePressEvent(event)
        self.toggle_widget_private.get_mouse_pos.emit(event)

class ToggleBtnWidgetPrivate(QtWidgets.QWidget, ColorSet):
    get_mouse_pos = QtCore.Signal(object)

    def __init__(self, parent=None):
        super().__init__(parent=parent)
        ColorSet.__init__(self)

        self.first_widget_size = None
        self.parent_widget = parent

        self.toggle_anim = ToggleAnim(parent=parent)

        self.get_mouse_pos.connect(self.press_event)

    def draw(self, painter:QtGui.QPainter):
        if not self.first_widget_size:
            self.first_widget_size = self.parentWidget().size()

        widget_size = self.parentWidget().size()

        self.toggle_anim.draw_toggle(widget_size, painter)

    def press_event(self, event):
        _mode = self.toggle_anim.determine_inside(event)
        self.parent_widget.mode_signal.emit(_mode)

class ToggleAnim(QtCore.QObject, ColorSet,FontSize):
    def __init__(self, parent=None):
        super().__init__(parent=parent)
        ColorSet.__init__(self)
        FontSize.__init__(self)

        self.btn_parent = parent

        self.strawberry_brown_pixmap = QtGui.QPixmap(IP.STRAWBERRY_BROWN_FIG_PATH)
        self.base_strawberry_brown_pixmap = self.strawberry_brown_pixmap.scaled(self.strawberry_brown_pixmap.size()*0.3,
                                                                                QtCore.Qt.KeepAspectRatio,
                                                                                QtCore.Qt.SmoothTransformation)

        self.strawberry_white_pixmap = QtGui.QPixmap(IP.STRAWBERRY_WHITE_FIG_PATH)
        self.base_strawberry_white_pixmap = self.strawberry_white_pixmap.scaled(self.strawberry_white_pixmap.size()*0.3,
                                                                                QtCore.Qt.KeepAspectRatio,
                                                                                QtCore.Qt.SmoothTransformation)

        self.cupcake_brown_pixmap = QtGui.QPixmap(IP.CUPCAKE_BROWN_FIG_PATH)
        self.base_cupcake_brown_pixmap = self.cupcake_brown_pixmap.scaled(self.cupcake_brown_pixmap.size()*0.3,
                                                                          QtCore.Qt.KeepAspectRatio,
                                                                          QtCore.Qt.SmoothTransformation)

        self.cupcake_white_pixmap = QtGui.QPixmap(IP.CUPCAKE_WHITE_FIG_PATH)
        self.base_cupcake_white_pixmap = self.cupcake_white_pixmap.scaled(self.cupcake_white_pixmap.size()*0.3,
                                                                          QtCore.Qt.KeepAspectRatio,
                                                                          QtCore.Qt.SmoothTransformation)

        self._animValue = 0.0
        self.anim = QtCore.QPropertyAnimation(self)
        self.anim.setTargetObject(self)
        self.anim.setPropertyName(b'press')
        self.anim.setStartValue(0.0)
        self.anim.setEndValue(1.0)
        self.anim.setDuration(200)
        self.anim.setEasingCurve(QtCore.QEasingCurve.InOutQuad)
        self.anim.finished.connect(self.btn_parent.update())

        self.anim_off_background = QtGui.QColor(235, 232, 230)
        self.base_font_size = self.toggle_widget_font_size
        self.base_widget_size = None

        self._animColor = self.button_border_color
        self.color_anim = QtCore.QPropertyAnimation(self)
        self.color_anim.setTargetObject(self)
        self.color_anim.setPropertyName(b'color')
        self.color_anim.setStartValue(self.button_border_color)
        self.color_anim.setEndValue(self.anim_off_background)
        self.color_anim.setDuration(200)
        self.color_anim.setEasingCurve(QtCore.QEasingCurve.InOutQuad)
        self.color_anim.finished.connect(self.btn_parent.update())

        self._animColor_back = self.anim_off_background
        self.color_anim_back = QtCore.QPropertyAnimation(self)
        self.color_anim_back.setTargetObject(self)
        self.color_anim_back.setPropertyName(b'color_back')
        self.color_anim_back.setStartValue(self.anim_off_background)
        self.color_anim_back.setEndValue(self.button_border_color)
        self.color_anim_back.setDuration(200)
        self.color_anim_back.setEasingCurve(QtCore.QEasingCurve.InOutQuad)
        self.color_anim_back.finished.connect(self.btn_parent.update())

    def draw_toggle(self, widget_size:QtCore.QSize, painter:QtGui.QPainter):
        if not self.base_widget_size:
            self.base_widget_size = widget_size
            main_win.send_button_state.connect(self.__check_button_state)

        painter.save()

        self.widget_size = widget_size

        widget_ratio = (self.widget_size.width()/self.base_widget_size.width(),
                        self.widget_size.height()/self.base_widget_size.height())
        widget_ratio = min(widget_ratio)

        _font = self.button_font
        _font.setPointSize(int(self.base_font_size*widget_ratio))
        _font.setBold(True)
        painter.setFont(_font)

        painter.setPen(self._animColor)
        painter.setBrush(self._animColor)
        painter.drawRect(0,
                         widget_size.height()*0.1*(1-self._animValue),
                         widget_size.width()/2,
                         widget_size.height()*(1-0.1*(1-self._animValue)))

        painter.setPen(self._animColor_back)
        painter.setBrush(self._animColor_back)
        painter.drawRect(widget_size.width()/2,
                         widget_size.height()*0.1*self._animValue,
                         widget_size.width()/2,
                         widget_size.height()*(1-0.1*self._animValue))

        if self._animValue > 0.5:
            painter.drawPixmap(self.widget_size.width()*0.18,
                            self.widget_size.height()*0.18,
                            self.base_strawberry_brown_pixmap.scaled(self.base_strawberry_brown_pixmap.size()*widget_ratio,
                                                                     QtCore.Qt.KeepAspectRatio,
                                                                     QtCore.Qt.SmoothTransformation))
        else:
            painter.drawPixmap(self.widget_size.width()*0.18,
                               self.widget_size.height()*0.18,
                               self.base_strawberry_white_pixmap.scaled(self.base_strawberry_white_pixmap.size()*widget_ratio,
                                                                        QtCore.Qt.KeepAspectRatio,
                                                                        QtCore.Qt.SmoothTransformation))

        painter.setPen(self._animColor_back)
        painter.drawText(self.widget_size.width()*0.01,
                         widget_size.height()*0.9, 'フルーツモード')

        if self._animValue < 0.5:
            painter.drawPixmap(self.widget_size.width()*0.68,
                               self.widget_size.height()*0.18,
                               self.base_cupcake_brown_pixmap.scaled(self.base_cupcake_brown_pixmap.size()*widget_ratio,
                                                                     QtCore.Qt.KeepAspectRatio,
                                                                     QtCore.Qt.SmoothTransformation))
        else:
            painter.drawPixmap(self.widget_size.width()*0.68,
                               self.widget_size.height()*0.18,
                               self.base_cupcake_white_pixmap.scaled(self.base_cupcake_white_pixmap.size()*widget_ratio,
                                                                     QtCore.Qt.KeepAspectRatio,
                                                                     QtCore.Qt.SmoothTransformation))

        painter.setPen(self._animColor)
        painter.drawText(self.widget_size.width()*0.51,
                         widget_size.height()*0.9, 'ホイップモード')

        painter.restore()

    @QtCore.Property(float)
    def press(self):
        return self._animValue

    @press.setter
    def press(self, value):
        self._animValue = value
        self.btn_parent.update()

    @QtCore.Property(QtGui.QColor)
    def color(self):
        return self._animColor

    @color.setter
    def color(self, value):
        self._animColor = value

    @QtCore.Property(QtGui.QColor)
    def color_back(self):
        return self._animColor_back

    @color_back.setter
    def color_back(self, value):
        self._animColor_back = value

    @QtCore.Slot(bool, name='animate')
    def animate(self, check):
        self.color_anim.setDirection(QtCore.QPropertyAnimation.Forward if check else QtCore.QPropertyAnimation.Backward)
        self.color_anim.start()
        self.color_anim_back.setDirection(QtCore.QPropertyAnimation.Forward if check else QtCore.QPropertyAnimation.Backward)
        self.color_anim_back.start()
        self.anim.setDirection(QtCore.QPropertyAnimation.Forward if check else QtCore.QPropertyAnimation.Backward)
        self.anim.start()

    def determine_inside(self, event:QtCore.QEvent):
        if main_win.core.button_state == 'fruit':
            if event.position().x() > self.widget_size.width()/2:
                self.animate(True)

                main_win.core.button_state = 'cream'
                self.message = {'state': main_win.core.button_state}
                main_win.core.core_write_data('button-state', self.message)

                return main_win.core.button_state

        elif main_win.core.button_state == 'cream':
            if event.position().x() < self.widget_size.width()/2:
                self.animate(False)

                main_win.core.button_state = 'fruit'
                self.message = {'state': 'fruit'}
                main_win.core.core_write_data('button-state', self.message)

                return main_win.core.button_state

    def __check_button_state(self, state):
        main_win.core.button_state = state

        if state == 'fruit':
            self.animate(False)
        elif state == 'cream':
            self.animate(True)

        self.btn_parent.mode_signal.emit(state)

class GripperBtnStacked(QtWidgets.QWidget):
    check_mode = QtCore.Signal(str)

    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.setStacked = False

        self.mode_layout = QtWidgets.QStackedLayout(self)

        self.fruit_widget = FruitWidget(parent=self)
        self.cream_widget = CreamWidget(parent=self)

        self.mode_layout.addWidget(self.cream_widget)
        self.mode_layout.addWidget(self.fruit_widget)

        self.check_mode.connect(self.__check_grip_mode)

    def resizeEvent(self, event):
        if not self.setStacked:
            self.mode_layout.setCurrentWidget(self.fruit_widget)
            self.setStacked = True

    def __check_grip_mode(self, mode):
        self.__set_mode_widget(mode)

    def __set_mode_widget(self, mode):
        if mode == 'fruit':
            self.mode_layout.setCurrentWidget(self.fruit_widget)
        elif mode == 'cream':
            self.mode_layout.setCurrentWidget(self.cream_widget)

class FruitWidget(QtWidgets.QLabel):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.fruit_layout = QtWidgets.QHBoxLayout(self)
        self.fruit_layout.setContentsMargins(0, 0, 0, 0)

        self.open_widget = FruitBtnWidget('g_open',
                                          '開く',
                                          IP.OPEN_FIG_PATH,
                                          parent=self)
        self.close_widget = FruitBtnWidget('grip',
                                           '閉じる',
                                           IP.CLOSE_FIG_PATH,
                                           parent=self)

        self.fruit_layout.addWidget(self.open_widget)
        self.fruit_layout.addWidget(self.close_widget)

class FruitBtnWidget(LabelBtnWidget, FontSize):
    def __init__(self, name, text, file_path, parent=None):
        super().__init__(name=name, parent=parent)
        FontSize.__init__(self)

        self.private_1 = LabelBtnWidgetPrivate(parent=self)
        self.private_1.setAlignment(QtCore.Qt.AlignBottom|QtCore.Qt.AlignHCenter)
        self.private_1.base_fig_scale = 0.03
        self.private_1.fig_pixmap = QtGui.QPixmap(file_path)
        self.private_1.setFigSize()

        self.private_2 = LabelBtnWidgetPrivate(parent=self)
        self.private_2.setAlignment(QtCore.Qt.AlignTop|QtCore.Qt.AlignHCenter)
        self.private_2.base_font_size = self.fruit_font_size
        self.private_2.setFontSize()
        self.private_2.setText(text)

        self.btn_layout.setSpacing(15)
        self.btn_layout.addWidget(self.private_1, 0, 0, 5, 1)
        self.btn_layout.addWidget(self.private_2, 5, 0, 4, 1)

    def resizeEvent(self, event):
        super().resizeEvent(event)

        self.private_1.get_scale.emit(self.btn_scale)
        self.private_2.get_scale.emit(self.btn_scale)

class CreamWidget(QtWidgets.QLabel):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.cream_layout = QtWidgets.QGridLayout(self)
        self.cream_layout.setContentsMargins(0, 0, 0, 0)

        self.cream_1 = CreamBtnWidget('cream_1',
                                      ['ホイップ','1'],
                                      parent=self)
        self.cream_2 = CreamBtnWidget('cream_2',
                                      ['ホイップ','2'],
                                      parent=self)
        self.squeeze = SqueezeBtnWidget('squeeze',
                                        parent=self)

        self.cream_layout.addWidget(self.cream_1, 0, 0, 1, 1)
        self.cream_layout.addWidget(self.cream_2, 1, 0, 1, 1)
        self.cream_layout.addWidget(self.squeeze, 0, 1, 2, 1)

class CreamBtnWidget(LabelBtnWidget, FontSize):
    def __init__(self, name, text, parent=None):
        super().__init__(name=name, parent=parent)
        FontSize.__init__(self)

        self.private_1 = LabelBtnWidgetPrivate(parent=self)
        self.private_1.setAlignment(QtCore.Qt.AlignBottom|QtCore.Qt.AlignHCenter)
        self.private_1.base_font_size = self.cream_private1_font_size
        self.private_1.setFontSize()
        self.private_1.setText(text[0])

        self.private_2 = LabelBtnWidgetPrivate(parent=self)
        self.private_2.setAlignment(QtCore.Qt.AlignTop|QtCore.Qt.AlignHCenter)
        self.private_2.base_font_size = self.cream_private2_font_size
        self.private_2.setFontSize()
        self.private_2.setFontBold()
        self.private_2.setText(text[1])

        self.btn_layout.setSpacing(3)
        self.btn_layout.addWidget(self.private_1, 0, 0, 1, 1)
        self.btn_layout.addWidget(self.private_2, 1, 0, 1, 1)

    def resizeEvent(self, event):
        super().resizeEvent(event)

        self.private_1.get_scale.emit(self.btn_scale)
        self.private_2.get_scale.emit(self.btn_scale)

class SqueezeBtnWidget(LabelBtnWidget, FontSize):
    def __init__(self, name, parent=None):
        super().__init__(name, parent)
        FontSize.__init__(self)

        self.squeeze_state = False

        self.private_1 = LabelBtnWidgetPrivate(parent=self)
        self.private_1.setAlignment(QtCore.Qt.AlignBottom|QtCore.Qt.AlignHCenter)
        self.private_1.base_font_size = self.squeeze_private1_font_size
        self.private_1.setFontSize()
        self.private_1.setText('しぼる')

        self.private_2 = LabelBtnWidgetPrivate(parent=self)
        self.private_2.setAlignment(QtCore.Qt.AlignTop|QtCore.Qt.AlignHCenter)
        self.private_2.base_font_size = self.squeeze_private2_font_size
        self.private_2.setFontSize()
        self.private_2.setFontBold()
        self.private_2.setText('スタート')

        self.btn_layout.setSpacing(10)
        self.btn_layout.addWidget(self.private_1, 0, 0, 1, 1)
        self.btn_layout.addWidget(self.private_2, 1, 0, 1, 1)

    def resizeEvent(self, event):
        super().resizeEvent(event)

        self.private_1.get_scale.emit(self.btn_scale)
        self.private_2.get_scale.emit(self.btn_scale)

    def mousePressEvent(self, event):
        super().mousePressEvent(event)

        if self.squeeze_state:
            self.private_2.setText('スタート')
            self.squeeze_state = False
        else:
            self.private_2.setText('ストップ')
            self.squeeze_state = True

if __name__ == '__main__':
    logs.set_file_handler(logger, '../gui.log', 'INFO')

    setting_file = open('../settings.json', 'r')
    settings_data = json.load(setting_file)

    AVATAR_PEER_ID = settings_data['avatarPeerID']
    USER_PEER_ID = settings_data['userPeerID']

    core_resources = None
    core = CoreManager()

    color_set = ColorSet()

    app = QtWidgets.QApplication(sys.argv)

    panel = QtWidgets.QWidget()
    panel_layout = QtWidgets.QVBoxLayout(panel)
    panel_layout.setContentsMargins(0, 0, 0, 0)

    video_widget = VideoFrame(parent=panel)
    button_widget = ButtonFrame(parent=panel)

    panel_layout.addWidget(video_widget, 3)
    panel_layout.addWidget(button_widget, 2)

    main_win = MainWin(core=core, parent=panel)

    main_win.show()
    app.exec_()
