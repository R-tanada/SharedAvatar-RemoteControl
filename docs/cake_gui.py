import json
import sys
import threading
from functools import partial

from pynput import mouse
from PySide6 import QtCore, QtGui, QtWidgets

import logs
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
        self.table_state = False

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
        print(data)
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
                if data['Button'] == 'table':
                    self.table_state = data['state']

    def core_write_data(self, channel, message):
        global core_resources

        message = json.dumps(message)
        resource = 'user-%s-data'%self.user_ID
        core_resources.write_data(resource, channel, message)

        logger.debug('send data : %s - %s - %s'%(resource, channel, message))

    def __on_video_frame_arrived(self, resource, frame):
        print(resource, frame)
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

class MainWindow(QtWidgets.QMainWindow):
    def __init__(self, core, parent=None):
        global core_resources

        self.core = core

        super().__init__(parent)
        self.setWindowTitle('Pancake_GUI')

        self.window_width = 1400
        self.window_height = self.window_width*9/16
        self.resize(self.window_width, self.window_height)
        self.move(100, 200)
        self.first_frame_geometry = self.geometry()

        self.minimum_window_width = 500
        self.minimum_window_height = self.minimum_window_width*9/16
        self.setMinimumSize(self.minimum_window_width, self.minimum_window_height)

        def __on_click(x, y, button, pressed):
            if button == mouse.Button.left and pressed == True:
                message = {'pressed': True}
                self.core.core_write_data('cursor-pressed', message)

        self.mouse_listener = mouse.Listener(on_click=__on_click)
        self.mouse_listener.start()

        self.loading_fig = QtGui.QPixmap('../button_fig/test_fig.png')
        self.micro_widget_size = 0.52
        self.per_y = 0.55

        self.micro_widget_geometry = {'x':0.1, 'y':0.05, 'size':1}
        self.fruit_widget_geometry = {'x':0.67, 'y':0.05, 'size':0.5}
        self.cake_widget_geometry = {'x':0.67, 'y':0.35, 'size':0.5}

        self.micro_video = self.__set_video_widget('micro', self.micro_widget_geometry)
        self.fruit_video = self.__set_video_widget('fruit', self.fruit_widget_geometry)
        self.cake_video = self.__set_video_widget('cake', self.cake_widget_geometry)

        self.micro_video.mousePressEvent = partial(self.__press_video, self.micro_video)
        self.fruit_video.mousePressEvent = partial(self.__press_video, self.fruit_video)
        self.cake_video.mousePressEvent = partial(self.__press_video, self.cake_video)

        self.front_widget_geometry = {'x':0.6, 'y':0.62, 'size':0.15}
        self.back_widget_geometry = {'x':0.6, 'y':0.77, 'size':0.15}
        self.right_widget_geometry = {'x':0.7, 'y':0.7, 'size':0.15}
        self.left_widget_geometry = {'x':0.5, 'y':0.7, 'size':0.15}
        self.up_widget_geometry = {'x':0.38, 'y':0.62, 'size':0.15}
        self.down_widget_geometry = {'x':0.38, 'y':0.77, 'size':0.15}
        self.roll_plus_widget_geometry = {'x':0.28, 'y':0.62, 'size':0.15}
        self.roll_minus_widget_geometry = {'x':0.28, 'y':0.77, 'size':0.15}
        self.grip_widget_geometry = {'x':0.8, 'y':0.62, 'size':0.15}
        self.cream_widget_geometry = {'x':0.8, 'y':0.78, 'size':0.15}
        self.open_widget_geometry = {'x':0.9, 'y':0.72, 'size':0.15}
        self.table_widget_geometry = {'x':0.07, 'y':0.65, 'size':0.15}
        self.cream_grip_1_widget_geometry = {'x':0.17, 'y':0.62, 'size':0.15}
        self.cream_grip_2_widget_geometry = {'x':0.17, 'y':0.77, 'size':0.15}

        self.front_button = self.__set_button_widget('front',
                                                     '../button_fig/front.png',
                                                     self.front_widget_geometry)
        self.back_button = self.__set_button_widget('back',
                                                    '../button_fig/back.png',
                                                    self.back_widget_geometry)
        self.right_button = self.__set_button_widget('right',
                                                     '../button_fig/right.png',
                                                     self.right_widget_geometry)
        self.left_button = self.__set_button_widget('left',
                                                    '../button_fig/left.png',
                                                    self.left_widget_geometry)
        self.up_button = self.__set_button_widget('up',
                                                  '../button_fig/up.png',
                                                  self.up_widget_geometry)
        self.down_button = self.__set_button_widget('down',
                                                    '../button_fig/down.png',
                                                    self.down_widget_geometry)
        self.roll_plus_button = self.__set_button_widget('roll_plus',
                                                         '../button_fig/roll_1.png',
                                                         self.roll_plus_widget_geometry)
        self.roll_minus_button = self.__set_button_widget('roll_minus',
                                                          '../button_fig/roll_2.png',
                                                          self.roll_minus_widget_geometry)
        self.cream_grip_1_button = self.__set_button_widget('cream_1',
                                                            '../button_fig/cream_1.png',
                                                            self.cream_grip_1_widget_geometry)
        self.cream_grip_2_button = self.__set_button_widget('cream_2',
                                                            '../button_fig/cream_2.png',
                                                            self.cream_grip_2_widget_geometry)

        self.grip_button, self.grip_img, self.grip_move_img = self.__set_hund_button_widget('grip',
                                                                                            '../button_fig/grip.png',
                                                                                            '../button_fig/grip_move.png',
                                                                                            self.grip_widget_geometry)
        self.cream_button, self.cream_img, self.cream_move_img = self.__set_hund_button_widget('cream',
                                                                                               '../button_fig/cream.png',
                                                                                               '../button_fig/cream_move.png',self.cream_widget_geometry)
        self.open_button, self.open_img, self.open_move_img = self.__set_hund_button_widget('g_open',
                                                                                            '../button_fig/g_open.png',
                                                                                            '../button_fig/g_open_move.png',
                                                                                            self.open_widget_geometry)
        self.table_button, self.table_img, self.table_move_img = self.__set_hund_button_widget('table',
                                                                                               '../button_fig/table.png',
                                                                                               '../button_fig/table_move.png',
                                                                                               self.table_widget_geometry)


        self.crosshair_size = 15
        self.crosshair_img = QtGui.QPixmap('../button_fig/crosshair.png')
        self.crosshair_img = self.crosshair_img.scaled(self.crosshair_size,
                                                       self.crosshair_size,
                                                       QtCore.Qt.KeepAspectRatio,
                                                       QtCore.Qt.SmoothTransformation)
        self.crosshair = QtWidgets.QLabel('crosshair', self.micro_video)
        self.crosshair.setScaledContents(True)
        self.crosshair.setPixmap(self.crosshair_img)
        self.crosshair.setGeometry(self.micro_video.frameGeometry().width()*0.5-self.crosshair_img.width()/2,
                                   self.micro_video.frameGeometry().height()*self.per_y-self.crosshair_img.height()/2,
                                   self.crosshair_size,
                                   self.crosshair_size)

        self.cursor_size = 20
        self.cursor_pos = (50, 50)
        self.cursor_pressed = False
        self.partner_cursor_fig = QtGui.QPixmap('../button_fig/cursor.png')
        self.partner_cursor_pressed_fig = QtGui.QPixmap('../button_fig/cursor_pressed.png')
        self.partner_cursor_fig = self.partner_cursor_fig.scaled(self.cursor_size,
                                                                 self.cursor_size,
                                                                 QtCore.Qt.KeepAspectRatio,
                                                                 QtCore.Qt.SmoothTransformation)
        self.partner_cursor_pressed_fig = self.partner_cursor_pressed_fig.scaled(self.cursor_size,
                                                                                 self.cursor_size,
                                                                                 QtCore.Qt.KeepAspectRatio,
                                                                                 QtCore.Qt.SmoothTransformation)
        self.partner_cursor = QtWidgets.QLabel('cursor', self)
        self.partner_cursor.setScaledContents(True)
        self.partner_cursor.setGeometry(10, 10, self.cursor_size, self.cursor_size)
        self.partner_cursor.setPixmap(self.partner_cursor_fig)

        self.old_cursor_pos_per = (0, 0)

        self.update_timer = QtCore.QTimer()
        self.update_timer.timeout.connect(self.__update)
        self.update_timer.start(1)

    def resizeEvent(self, event):
        self.new_frame_geometry = self.frameGeometry()

        self.__resize_video()
        self.__resize_button()

        self.crosshair.setGeometry(self.micro_video.frameGeometry().width()*0.5-self.crosshair_img.width()/2,
                                   self.micro_video.frameGeometry().height()*self.per_y-self.crosshair_img.height()/2,
                                   15, 15)

        self.frame_per = self.new_frame_geometry.width()/self.first_frame_geometry.width()
        self.partner_cursor.resize(self.cursor_size*self.frame_per, self.cursor_size*self.frame_per)

    def __resize_video(self):
        self.__resize_video_widget(self.micro_video, self.micro_widget_geometry)
        self.__resize_video_widget(self.fruit_video, self.fruit_widget_geometry)
        self.__resize_video_widget(self.cake_video, self.cake_widget_geometry)

    def __resize_button(self):
        self.__resize_button_widget(self.front_button, self.front_widget_geometry)
        self.__resize_button_widget(self.back_button, self.back_widget_geometry)
        self.__resize_button_widget(self.right_button, self.right_widget_geometry)
        self.__resize_button_widget(self.left_button, self.left_widget_geometry)
        self.__resize_button_widget(self.up_button, self.up_widget_geometry)
        self.__resize_button_widget(self.down_button, self.down_widget_geometry)
        self.__resize_button_widget(self.roll_plus_button, self.roll_plus_widget_geometry)
        self.__resize_button_widget(self.roll_minus_button, self.roll_minus_widget_geometry)
        self.__resize_button_widget(self.grip_button, self.grip_widget_geometry)
        self.__resize_button_widget(self.cream_button, self.cream_widget_geometry)
        self.__resize_button_widget(self.open_button, self.open_widget_geometry)
        self.__resize_button_widget(self.table_button, self.table_widget_geometry)
        self.__resize_button_widget(self.cream_grip_1_button, self.cream_grip_1_widget_geometry)
        self.__resize_button_widget(self.cream_grip_2_button, self.cream_grip_2_widget_geometry)

    def __set_video_widget(self, name:str, geometry:dict):
        video_widget = QtWidgets.QLabel(name, self)
        video_widget.setObjectName(name)
        video_widget.setScaledContents(True)
        video_widget.setGeometry(self.first_frame_geometry.width()*geometry['x'],
                                 self.first_frame_geometry.height()*geometry['y'],
                                 self.first_frame_geometry.width()*self.micro_widget_size*geometry['size'],
                                 self.first_frame_geometry.width()*self.micro_widget_size*geometry['size']*9/16)
        video_widget.setPixmap(self.loading_fig.scaled(self.first_frame_geometry.width()*self.micro_widget_size*geometry['size'],
                                                       self.first_frame_geometry.width()*self.micro_widget_size*geometry['size']*9/16,
                                                       QtCore.Qt.KeepAspectRatio,
                                                       QtCore.Qt.SmoothTransformation))
        return video_widget

    def __resize_video_widget(self, widget, geometry):
        widget.setGeometry(self.new_frame_geometry.width()*geometry['x'],
                           self.new_frame_geometry.height()*geometry['y'],
                           self.new_frame_geometry.width()*self.micro_widget_size*geometry['size'],
                           self.new_frame_geometry.width()*self.micro_widget_size*geometry['size']*9/16)

    def __press_video(self, widget, event):
        name = widget.objectName()
        if name in ['fruit', 'cake']:
            message = {'Button':name, 'bool':True}
            self.core.core_write_data('command', message)

        elif name == 'micro':
            pos_micro_per_x = (event.position().x()-widget.width()/2)/(widget.width()/2)

            if event.position().y() < widget.height()*self.per_y:
                pos_micro_per_y = (widget.height()*self.per_y-event.position().y())/(widget.height()*self.per_y)

            elif event.position().y() >= widget.height()*self.per_y:
                pos_micro_per_y = -(1-(widget.height()*(1-self.per_y)-(event.position().y()-widget.height()*self.per_y))/(widget.height()*(1-self.per_y)))

            message = {'Button':name, 'bool':(pos_micro_per_x, pos_micro_per_y)}
            self.core.core_write_data('command', message)

    def __set_button_widget(self, name:str, path:str, geometry:dict):
        img = QtGui.QPixmap(path)
        img_on = QtGui.QPixmap(path)

        painter = QtGui.QPainter()
        painter.begin(img_on)
        painter.fillRect(0, 0, img.width(), img.height(), QtGui.QColor(253, 126, 0, 50))
        painter.end()

        button_label = QtWidgets.QLabel(name, self)
        button_label.setObjectName(name)
        button_label.setScaledContents(True)
        button_label.setPixmap(img)
        button_label.setGeometry(self.first_frame_geometry.width()*geometry['x'],
                                 self.first_frame_geometry.height()*geometry['y'],
                                 self.first_frame_geometry.width()*self.micro_widget_size*geometry['size'],
                                 self.first_frame_geometry.width()*self.micro_widget_size*geometry['size'])

        def __on_time():
            button_label.setPixmap(img)

        def __on_button(event):
            message = {'Button':name, 'bool':True}
            self.core.core_write_data('command', message)

            button_label.setPixmap(img_on)
            QtCore.QTimer.singleShot(150, __on_time)

        button_label.mousePressEvent = __on_button

        return button_label

    def __set_hund_button_widget(self, name:str, path:str, path_move:str, geometry:dict):
        img = QtGui.QPixmap(path)
        img_on = QtGui.QPixmap(path)

        painter = QtGui.QPainter()
        painter.begin(img_on)
        painter.setBrush(QtGui.QColor(253, 126, 0, 50))
        painter.drawEllipse(0, 0, img.width(), img.height())
        painter.end()

        img_move = QtGui.QPixmap(path_move)
        button_label = QtWidgets.QLabel(name, self)
        button_label.setObjectName(name)
        button_label.setScaledContents(True)
        button_label.setPixmap(img)
        button_label.setGeometry(self.first_frame_geometry.width()*geometry['x'],
                                 self.first_frame_geometry.height()*geometry['y'],
                                 self.first_frame_geometry.width()*self.micro_widget_size*geometry['size'],
                                 self.first_frame_geometry.width()*self.micro_widget_size*geometry['size'])

        def __on_time():
            button_label.setPixmap(img)

        def __on_button(event):
            message = {'Button':name, 'bool':True}
            self.core.core_write_data('command', message)

            button_label.setPixmap(img_on)
            QtCore.QTimer.singleShot(150, __on_time)

        button_label.mousePressEvent = __on_button

        return button_label, img, img_move

    def __resize_button_widget(self, widget, geometry):
        widget.setGeometry(self.new_frame_geometry.width()*geometry['x'],
                           self.new_frame_geometry.height()*geometry['y'],
                           self.new_frame_geometry.width()*self.micro_widget_size*geometry['size'],
                           self.new_frame_geometry.width()*self.micro_widget_size*geometry['size'])

    def __update(self):
        self.update()
        self.__get_cursor_pos()

        self.__update_data()
        self.__update_video()

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

    def __update_data(self):
        self.__update_cursor()
        self.__update_grip_state()

    def __update_cursor(self):
        def __on_time():
            self.partner_cursor.setPixmap(self.partner_cursor_fig)
            self.core.cursor_pressed = False

        self.partner_cursor.move(self.new_frame_geometry.width()*self.core.cursor_pos[0]-self.partner_cursor_fig.width()/2,
                                 self.new_frame_geometry.height()*self.core.cursor_pos[1]-self.partner_cursor_fig.height())

        if self.core.cursor_pressed:
            self.partner_cursor.setPixmap(self.partner_cursor_pressed_fig)
            QtCore.QTimer.singleShot(150, __on_time)

    def __update_grip_state(self):
        if self.core.grip_state == 1:
            self.grip_button.setPixmap(self.grip_move_img)

        elif self.core.grip_state == 0:
            self.grip_button.setPixmap(self.grip_img)

        if self.core.cream_state == 1:
            self.cream_button.setPixmap(self.cream_move_img)

        elif self.core.cream_state == 0:
            self.cream_button.setPixmap(self.cream_img)

        if self.core.open_state == 1:
            self.open_button.setPixmap(self.open_move_img)

        elif self.core.open_state == 0:
            self.open_button.setPixmap(self.open_img)

        if self.core.table_state == 1:
            self.table_button.setPixmap(self.table_move_img)

        elif self.core.table_state == 0:
            self.table_button.setPixmap(self.table_img)

    def __update_video(self):
        if self.core.new_frame:
            self.core.resource_mutex.acquire()
            self.__update_frame(self.core.resource_name, self.core.frame_data)
            self.core.new_frame = False
            self.core.resource_mutex.release()

    def __update_frame(self, resource, frame):
        if resource == 'micro-camera-RGB':
            self.micro_video.setPixmap(frame)

        elif resource == 'fruit-camera-RGB':
            self.fruit_video.setPixmap(frame)

        elif resource == 'cake-camera-RGB':
            self.cake_video.setPixmap(frame)

    def closeEvent(self, event):
        self.core.stop_core()
        event.accept()

if __name__ == '__main__':
    logs.set_file_handler(logger, '../gui.log', 'DEBUG')

    setting_file = open('../settings.json', 'r')
    settings_data = json.load(setting_file)

    AVATAR_PEER_ID = settings_data['avatarPeerID']
    USER_PEER_ID = settings_data['userPeerID']

    core_resources = None
    core = CoreManager()

    app = QtWidgets.QApplication(sys.argv)
    window = MainWindow(core)
    window.show()
    app.exec_()
