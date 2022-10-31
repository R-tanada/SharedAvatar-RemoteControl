# -----------------------------------------------------------------------
# Author:   Takumi Katagiri, Takumi Nishimura (Nagoya Institute of Technology)
# Created:  2021
# Summary:  振動フィードバック制御マネージャー
# -----------------------------------------------------------------------

from typing import List
import pyaudio
import numpy as np
from scipy import signal
import winsound
from typing import List
import math

from CyberneticAvatarMotion.CyberneticAvatarMotionBehaviour import CyberneticAvatarMotionBehaviour
from MotionFilter.MotionFilter import MotionFilter
from VibrotactileFeedback.AudioDeviceIndexes import AudioDeviceIndexes
from FileIO.FileIO import FileIO

class VibrotactileFeedbackManager:
    def __init__(self, condition: str = ''):
        """
        重い
        """
        # ----- listIndexNum from settings.csv ----- #
        fileIO = FileIO()
        dat = fileIO.Read('settings.csv',',')
        listIndexNum = [addr for addr in dat if 'listIndexNum' in addr[0]] 
        self.listIndexNum = listIndexNum
        self.listIndexNum[0].remove('listIndexNum')
        listIndexNumstr = self.listIndexNum[0]
        listIndexNum = list(map(int,listIndexNumstr))

        # ----- Find audio device indexes ----- #
        audioDeviceIndexes = AudioDeviceIndexes()
        ListIndexNum = audioDeviceIndexes.Find(host_api='MME', name='スピーカー (3- Sound Blaster Play! 3')
        ListIndexNum = listIndexNum
        OutputDeviceNum = len(ListIndexNum)

        # self.condition = condition
        self.condition = 'B'

        self.caBehaviour = CyberneticAvatarMotionBehaviour()

        self.p = pyaudio.PyAudio()
        self.format = pyaudio.paInt16
        self.channels = 1
        self.rate = 44100
        self.amp = 30

        self.freq = 200
        self.CHUNK = int(self.rate / self.freq)
        self.sin = np.sin(2.0 * np.pi * np.arange(self.CHUNK) * float(self.freq) / float(self.rate))
        self.square = signal.square(2.0 * np.pi * np.arange(self.CHUNK) * float(self.freq) / float(self.rate))

        # ----- Initialize the parameter of data_out according as OutputDeviceNum ----- #
        for i in range(OutputDeviceNum):
            data_out_command = 'self.data_out_' + str(i+1) + '= 0.0'
            exec(data_out_command)

        # ----- Define streamming command according as OutputDeviceNum ----- #
        for i in range(OutputDeviceNum):
            stream_command = 'self.stream' + str(i+1) + '= self.p.open('\
                + 'rate = self.rate,'\
                + 'channels = self.channels,'\
                + 'format = self.format,'\
                + 'output = True,'\
                + 'output_device_index = ListIndexNum[' + str(i) + '],'\
                + 'frames_per_buffer = self.CHUNK,'\
                + 'stream_callback = self.callback' + str(i+1)\
            + ')'
            exec(stream_command)

            start_streamming_command = 'self.stream' + str(i+1) + '.start_stream()'
            exec(start_streamming_command)

        n = 2
        fp = 10
        fs = 180
        self.filter_FB = MotionFilter()
        self.filter_FB.InitLowPassFilterWithOrder(fs, fp, n)
        self.dt = round(1/fs, 4)

        self.initGripperPosition = np.array([0, 0, -200])

        self.get_pos_r_box = [[0, 0, 0, 0, 0, 0]] * n
        self.get_pos_1_box = [[0, 0, 0, 0, 0, 0]] * n
        self.get_pos_2_box = [[0, 0, 0, 0, 0, 0]] * n
        self.get_pos_r_filt_box = [[0, 0, 0, 0, 0, 0]] * n
        self.get_pos_1_filt_box = [[0, 0, 0, 0, 0, 0]] * n
        self.get_pos_2_filt_box = [[0, 0, 0, 0, 0, 0]] * n

        self.beforelpf_vel    = [0] * n
        self.afterlpf_vel     = [0] * n

        self.listParticipantPos1 = []
        self.listParticipantPos2 = []
        self.listEndEffectorPos  = []

        self.listParticipantRot1 = []
        self.listParticipantRot2 = []
        self.listEndEffectorRot  = []

        self.data_out_1 = 0
        self.data_out_2 = 0
        self.data_out_3 = 0
        self.data_out_4 = 0

    def callback1(self, in_data, frame_count, time_info, status):
        # out_data_1 = (self.data_out_1 * float(self.amp) * (self.sin)).astype(np.int16)
        out_data_1 = ((503.0463*math.exp((self.data_out_1*self.amp*0.00008)/0.3752)-500) * self.sin).astype(np.int16)
        # out_data_1 = ((math.exp(3.3738 / 0.494) * math.exp(self.data_out_1 * 32 / (0.494 * 16384)) - math.exp(3.3738 / 0.494)) * self.sin).astype(np.int16)
        return (out_data_1, pyaudio.paContinue)


    def callback2(self, in_data, frame_count, time_info, status):
        # out_data_2 = (self.data_out_2 * float(self.amp) * (self.sin)).astype(np.int16)
        out_data_2 = ((503.0463*math.exp((self.data_out_2*self.amp*0.00008)/0.3752)-500) * self.sin).astype(np.int16)
        # out_data_2 = ((math.exp(3.3738 / 0.494) * math.exp(self.data_out_2 * 32 / (0.494 * 16384)) - math.exp(3.3738 / 0.494)) * self.sin).astype(np.int16)
        return (out_data_2, pyaudio.paContinue)


    def callback3(self, in_data, frame_count, time_info, status):
        out_data_3 = (self.data_out_3 * float(self.amp) * (self.sin)).astype(np.int16)
        return (out_data_3, pyaudio.paContinue)


    def callback4(self, in_data, frame_count, time_info, status):
        out_data_4 = (self.data_out_4 * float(self.amp) * (self.sin)).astype(np.int16)
        return (out_data_4, pyaudio.paContinue)


    def close(self):
        self.p.terminate()

    def forIntegration(self, position: dict, rotation: dict, weight: float):
        posParticipant1 = position['participant1'] * 1000
        posParticipant2 = position['participant2'] * 1000
        # posEndEffector  = position['endEffector'] * 1000
        rotParticipant1 = np.rad2deg(self.caBehaviour.Quaternion2Euler(rotation['participant1']))
        rotParticipant2 = np.rad2deg(self.caBehaviour.Quaternion2Euler(rotation['participant2']))
        # rotEndEffector  = np.rad2deg(self.caBehaviour.Quaternion2Euler(rotation['endEffector']))

        self.get_pos_1_box.append(np.concatenate([posParticipant1, rotParticipant1], 0))
        get_pos_1_filt = self.filter_FB.lowpass2(self.get_pos_1_box, self.get_pos_1_filt_box)
        self.get_pos_1_filt_box.append(get_pos_1_filt)
        del self.get_pos_1_box[0]
        del self.get_pos_1_filt_box[0]

        self.get_pos_2_box.append(np.concatenate([posParticipant2, rotParticipant2], 0))
        get_pos_2_filt = self.filter_FB.lowpass2(self.get_pos_2_box, self.get_pos_2_filt_box)
        self.get_pos_2_filt_box.append(get_pos_2_filt)
        del self.get_pos_2_box[0]
        del self.get_pos_2_filt_box[0]

        # self.get_pos_r_box.append(np.concatenate([posEndEffector, rotEndEffector], 0))
        # get_pos_r_filt = self.filter_FB.lowpass2(self.get_pos_r_box, self.get_pos_r_filt_box)
        # self.get_pos_r_filt_box.append(get_pos_r_filt)
        # del self.get_pos_r_box[0]
        # del self.get_pos_r_filt_box[0]

        self.listParticipantPos1.append(get_pos_1_filt[0:3])
        self.listParticipantPos2.append(get_pos_2_filt[0:3])
        # self.listEndEffectorPos.append(get_pos_r_filt[0:3])

        self.listParticipantRot1.append(get_pos_1_filt[3:7])
        self.listParticipantRot2.append(get_pos_2_filt[3:7])
        # self.listEndEffectorRot.append(get_pos_r_filt[3:7])

        if len(self.listParticipantPos1) == 2:
            velPosP1 = np.linalg.norm((np.diff(self.listParticipantPos1, n=1, axis=0)/self.dt))
            velPosP2 = np.linalg.norm((np.diff(self.listParticipantPos2, n=1, axis=0)/self.dt))
            # velPosEf = np.linalg.norm((np.diff(self.listEndEffectorPos, n=1, axis=0)/self.dt))

            velRotP1 = np.linalg.norm((np.diff(self.listParticipantRot1, n=1, axis=0)/self.dt))
            velRotP2 = np.linalg.norm((np.diff(self.listParticipantRot2, n=1, axis=0)/self.dt))
            # velRotEf = np.linalg.norm((np.diff(self.listEndEffectorRot, n=1, axis=0)/self.dt))

            # # # FB
            # # # 位置FB
            # # # 相手との差
            # af.data_out_1 = np.linalg.norm((get_pos_r_filt-get_pos_1_filt)[1:3])
            # af.data_out_2 = np.linalg.norm((get_pos_r_filt-get_pos_2_filt)[1:3])

            # # 速度FB
            p_r_gain = 0
            vel_gain = 1.0
            # fb_vel_r = (velPosEf+velRotEf*p_r_gain)*vel_gain
            fb_vel_1 = (velPosP1+velRotP1*p_r_gain)*vel_gain
            fb_vel_2 = (velPosP2+velRotP2*p_r_gain)*vel_gain

            if self.condition == 'A':
                self.data_out_1 = 0
                self.data_out_2 = 0
            elif self.condition == 'B':
                self.data_out_1 = fb_vel_1 * weight
                self.data_out_2 = fb_vel_2 * (1-weight)
            elif self.condition == 'C':
                self.data_out_1 = fb_vel_r * weight
                self.data_out_2 = fb_vel_r * (1-weight)
            else:
                print('!!!condition-error!!!')

            # print('data_out_1   ',self.data_out_1,'   data_out_2   ',self.data_out_2,sep=',')

            # 相手
            # self.data_out_1 = fb_vel_1 * weight
            # self.data_out_2 = fb_vel_2 * (1-weight)
            # self.data_out_3 = fb_vel_r
            # self.data_out_4 = fb_vel_r
            # # # 相手との差(小)
            # af.data_out_1 = abs(fb_vel_1 - fb_vel_2)
            # af.data_out_3 = abs(fb_vel_1 - fb_vel_2)
            # # # 相手との差(大)
            # if fb_vel_1<50 or fb_vel_2<50:
            # 	af.data_out_1 = 0
            # 	af.data_out_3 = 0
            # else:
            # 	if abs(fb_vel_1 - fb_vel_2)*1.4<900:
            # 		af.data_out_1 = abs(900 - abs(fb_vel_1 - fb_vel_2)*1.6)
            # 		af.data_out_3 = abs(900 - abs(fb_vel_1 - fb_vel_2)*1.6)
            # 	elif abs(fb_vel_1 - fb_vel_2)*1.4>900:
            # 		af.data_out_1 = 100
            # 		af.data_out_2 = 100
            # # # ロボットと自分の差
            # af.data_out_1 = (fb_vel_r - fb_vel_1*fb_vel_r/(fb_vel_1+fb_vel_2))*2.7
            # af.data_out_3 = (fb_vel_r - fb_vel_2*fb_vel_r/(fb_vel_1+fb_vel_2))*2.7
            # # # ロボットと相手の差
            # af.data_out_1 = fb_vel_r - fb_vel_2*fb_vel_r/(fb_vel_1+fb_vel_2)*2.7
            # af.data_out_3 = fb_vel_r - fb_vel_1*fb_vel_r/(fb_vel_1+fb_vel_2)*2.7
            # # 割合(自分)
            # self.data_out_2 = fb_vel_r*2
            # self.data_out_1 = fb_vel_1*fb_vel_r/(fb_vel_1+fb_vel_2)*2
            # self.data_out_4 = fb_vel_r*2
            # self.data_out_3 = fb_vel_2*fb_vel_r/(fb_vel_1+fb_vel_2)*2
            # # # 割合(相手)
            # af.data_out_2 = fb_vel_r*2
            # af.data_out_3 = fb_vel_2*fb_vel_r/(fb_vel_1+fb_vel_2)*2
            # af.data_out_4 = fb_vel_r*2
            # af.data_out_1 = fb_vel_1*fb_vel_r/(fb_vel_1+fb_vel_2)*2
            # # 割合
            # if fb_vel_1<5 and fb_vel_2<5:
            # 	af.data_out_1 = 0
            # 	af.data_out_3 = 0
            # else:
            # 	af.data_out_1 = (fb_vel_1*fb_vel_r/(fb_vel_1+fb_vel_2))/fb_vel_r*800
            # 	af.data_out_3 = (fb_vel_2*fb_vel_r/(fb_vel_1+fb_vel_2))/fb_vel_r*800

            # # # 心拍
            # af.data_out_1 = raspi.pulse1
            # af.data_out_3 = raspi.pulse2

            del self.listParticipantPos1[0]
            del self.listParticipantPos2[0]
            # del self.listEndEffectorPos[0]

            del self.listParticipantRot1[0]
            del self.listParticipantRot2[0]
            # del self.listEndEffectorRot[0]

        return self.data_out_1, self.data_out_2

    def forIROS(self, position, rotation):
        # ----- Position control feedback ----- #
        posParticipant1 = self.caBehaviour.GetRelativePosition(position)['participant2'] * 1000
        self.listParticipantPos1.append(posParticipant1)

        if len(self.listParticipantPos1) == 4:
            self.vel  = np.linalg.norm(np.diff(self.listParticipantPos1, n=1, axis=0)[2] / self.dt)

            # ----- Lowpass filter ----- #
            self.beforelpf_vel.append(self.vel)
            self.vel  = self.filter_FB.lowpass(self.beforelpf_vel, self.afterlpf_vel)
            self.afterlpf_vel.append(self.vel)
            del self.beforelpf_vel[0]
            del self.afterlpf_vel[0]

            self.acc  = np.linalg.norm(np.diff(self.listParticipantPos1, n=2, axis=0)[1] / self.dt**2)
            self.jerk = np.linalg.norm(np.diff(self.listParticipantPos1, n=3, axis=0)[0] / self.dt**3)

            self.data_out_1 = self.vel
            # self.data_out_2 = 0

            del self.listParticipantPos1[0]

    def __forCustomWeight(self):
        pass

    def __beep(self, freq, dur=100):
        """
            ビープ音を鳴らす
            @param freq 周波数[Hz]
            @param dur  継続時間[ms]
        """
        winsound.Beep(freq, dur)

    def __GenerateVibrotactileFeedback(self, position, rotation, weightReader: float = 0.5, weightFollower: float = 0.5):
        posParticipant1 = position['participant1']
        posParticipant2 = position['participant2']

        rotParticipant1 = rotation['participant1']
        rotParticipant2 = rotation['participant2']

        # ----- move_reader_FB ----- #
        diff_pos_reader_x = abs(posParticipant1[0] -  posParticipant2[0])
        diff_pos_reader_y = abs(posParticipant1[1] -  posParticipant2[1])
        diff_pos_reader_z = abs(posParticipant1[2] -  posParticipant2[2])
        FB_rot_w = 0.2
        diff_rot_reader_roll    = abs(rotParticipant1[0] - rotParticipant2[0])*FB_rot_w
        diff_rot_reader_pitch   = abs(rotParticipant1[1] - rotParticipant2[1])*FB_rot_w
        diff_rot_reader_yaw     = abs(rotParticipant1[2] - rotParticipant2[2])*FB_rot_w

        diff_move_reader = np.power(diff_pos_reader_x*diff_pos_reader_y*diff_pos_reader_z*diff_rot_reader_roll*diff_rot_reader_pitch*diff_rot_reader_yaw,1/6)
        self.data_out_1 = (np.tanh(diff_move_reader)/(1+2*np.exp(-diff_move_reader))) * weightReader * 5000

        # ----- move_follower_FB ----- #
        diff_pos_follower_x = abs(posParticipant1[0] -  posParticipant2[0])
        diff_pos_follower_y = abs(posParticipant1[1] -  posParticipant2[1])
        diff_pos_follower_z = abs(posParticipant1[2] -  posParticipant2[2])

        diff_rot_follower_roll = abs(rotParticipant1[0] - rotParticipant2[0])*FB_rot_w
        diff_rot_follower_pitch = abs(rotParticipant1[1] - rotParticipant2[1])*FB_rot_w
        diff_rot_follower_yaw = abs(rotParticipant1[2] - rotParticipant2[2])*FB_rot_w

        diff_move_follower = np.power(diff_pos_follower_x*diff_pos_follower_y*diff_pos_follower_z*diff_rot_follower_roll*diff_rot_follower_pitch*diff_rot_follower_yaw,1/6)
        self.data_out_2 = (np.tanh(diff_move_follower)/(1+2*np.exp(-diff_move_follower))) * weightFollower * 5000

    def __GenerateVibrotactileFeedback(self, position, rotation, weight):
        """
        2021/12/4
        REGACY system
        """

        posParticipant1 = position['participant1'] * 1000
        posParticipant2 = position['participant2'] * 1000
        posEndEffector  = position['endEffector'] * 1000
        rotParticipant1 = np.rad2deg(self.caBehaviour.Quaternion2Euler(rotation['participant1']))
        rotParticipant2 = np.rad2deg(self.caBehaviour.Quaternion2Euler(rotation['participant2']))
        rotEndEffector = np.rad2deg(self.caBehaviour.Quaternion2Euler(rotation['endEffector']))

        get_pos_1 = posParticipant1 + rotParticipant1
        self.get_pos_1_box.append(get_pos_1)
        get_pos_1_filt = self.filter_FB.lowpass2(self.get_pos_1_box, self.get_pos_1_filt_box)
        self.get_pos_1_filt_box.append(get_pos_1_filt)
        del self.get_pos_1_box[0]
        del self.get_pos_1_filt_box[0]
        get_pos_2 = posParticipant2 + rotParticipant2
        self.get_pos_2_box.append(get_pos_2)
        get_pos_2_filt = self.filter_FB.lowpass2(self.get_pos_2_box, self.get_pos_2_filt_box)
        self.get_pos_2_filt_box.append(get_pos_2_filt)
        del self.get_pos_2_box[0]
        del self.get_pos_2_filt_box[0]
        get_pos_r = np.hstack([posEndEffector,rotEndEffector])  # これだけなんでhstack？
        self.get_pos_r_box.append(get_pos_r)
        get_pos_r_filt = self.filter_FB.lowpass2(self.get_pos_r_box, self.get_pos_r_filt_box)
        self.get_pos_r_filt_box.append(get_pos_r_filt)
        del self.get_pos_r_box[0]
        del self.get_pos_r_filt_box[0]

        self.listParticipantPos1.append(posParticipant1)
        self.listParticipantPos2.append(posParticipant2)
        self.listEndEffectorPos.append(posEndEffector)

        self.listParticipantRot1.append(rotParticipant1)
        self.listParticipantRot2.append(rotParticipant2)
        self.listEndEffectorRot.append(rotEndEffector)

        if len(self.listParticipantPos1) == 2:
            velPosP1 = np.linalg.norm((np.diff(self.listParticipantPos1, n=1, axis=0)/self.dt))
            velPosP2 = np.linalg.norm((np.diff(self.listParticipantPos2, n=1, axis=0)/self.dt))
            velPosEf = np.linalg.norm((np.diff(self.listEndEffectorPos, n=1, axis=0)/self.dt))

            velRotP1 = np.linalg.norm((np.diff(self.listParticipantRot1, n=1, axis=0)/self.dt))
            velRotP2 = np.linalg.norm((np.diff(self.listParticipantRot2, n=1, axis=0)/self.dt))
            velRotEf = np.linalg.norm((np.diff(self.listEndEffectorRot, n=1, axis=0)/self.dt))

            # # # FB
            # # # 位置FB
            # # # 相手との差
            # af.data_out_1 = np.linalg.norm((get_pos_r_filt-get_pos_1_filt)[1:3])
            # af.data_out_2 = np.linalg.norm((get_pos_r_filt-get_pos_2_filt)[1:3])
            # # 速度FB
            delta = 1
            p_r_gain = 0
            vel_gain = 0.02 * 5
            fb_vel_r = (velPosEf+velRotEf*p_r_gain)*vel_gain
            fb_vel_1 = (velPosP1+velRotP1*p_r_gain)*vel_gain
            fb_vel_2 = (velPosP2+velRotP2*p_r_gain)*vel_gain
            # 相手
            self.data_out_1 = fb_vel_1 * (1-weight)
            self.data_out_3 = fb_vel_2 * weight
            self.data_out_2 = fb_vel_r

            # af.data_out_1 = fb_vel_r
            # af.data_out_3 = fb_vel_r
            # # # 相手との差(小)
            # af.data_out_1 = abs(fb_vel_1 - fb_vel_2)
            # af.data_out_3 = abs(fb_vel_1 - fb_vel_2)
            # # # 相手との差(大)
            # if fb_vel_1<50 or fb_vel_2<50:
            # 	af.data_out_1 = 0
            # 	af.data_out_3 = 0
            # else:
            # 	if abs(fb_vel_1 - fb_vel_2)*1.4<900:
            # 		af.data_out_1 = abs(900 - abs(fb_vel_1 - fb_vel_2)*1.6)
            # 		af.data_out_3 = abs(900 - abs(fb_vel_1 - fb_vel_2)*1.6)
            # 	elif abs(fb_vel_1 - fb_vel_2)*1.4>900:
            # 		af.data_out_1 = 100
            # 		af.data_out_2 = 100
            # # # ロボットと自分の差
            # af.data_out_1 = (fb_vel_r - fb_vel_1*fb_vel_r/(fb_vel_1+fb_vel_2))*2.7
            # af.data_out_3 = (fb_vel_r - fb_vel_2*fb_vel_r/(fb_vel_1+fb_vel_2))*2.7
            # # # ロボットと相手の差
            # af.data_out_1 = fb_vel_r - fb_vel_2*fb_vel_r/(fb_vel_1+fb_vel_2)*2.7
            # af.data_out_3 = fb_vel_r - fb_vel_1*fb_vel_r/(fb_vel_1+fb_vel_2)*2.7
            # # 割合(自分)

            """
            self.data_out_2 = fb_vel_r*2
            self.data_out_1 = fb_vel_1*fb_vel_r/(fb_vel_1+fb_vel_2)*2
            self.data_out_4 = fb_vel_r*2
            self.data_out_3 = fb_vel_2*fb_vel_r/(fb_vel_1+fb_vel_2)*2
            """

            # # # 割合(相手)
            # af.data_out_2 = fb_vel_r*2
            # af.data_out_3 = fb_vel_2*fb_vel_r/(fb_vel_1+fb_vel_2)*2
            # af.data_out_4 = fb_vel_r*2
            # af.data_out_1 = fb_vel_1*fb_vel_r/(fb_vel_1+fb_vel_2)*2
            # # 割合
            # if fb_vel_1<5 and fb_vel_2<5:
            # 	af.data_out_1 = 0
            # 	af.data_out_3 = 0
            # else:
            # 	af.data_out_1 = (fb_vel_1*fb_vel_r/(fb_vel_1+fb_vel_2))/fb_vel_r*800
            # 	af.data_out_3 = (fb_vel_2*fb_vel_r/(fb_vel_1+fb_vel_2))/fb_vel_r*800

            # # # 心拍
            # af.data_out_1 = raspi.pulse1
            # af.data_out_3 = raspi.pulse2

            #self.plot_1.append(self.data_out_1)
            #self.plot_2.append(self.data_out_1)

            # Data.write()

            del self.listParticipantPos1[0]
            del self.listParticipantPos2[0]
            del self.listEndEffectorPos[0]

            del self.listParticipantRot1[0]
            del self.listParticipantRot2[0]
            del self.listEndEffectorRot[0]

    def forDivisionOfRoles(self, position, rotation):
        posParticipant1 = self.caBehaviour.GetRelativePosition(position)['participant1'] * 1000
        rot = self.caBehaviour.GetRelativeRotation(rotation)['participant2']
        RQ = np.array([[rot[0]**2-rot[1]**2-rot[2]**2+rot[3]**2, 2*(rot[0]*rot[1]-rot[2]*rot[3]), 2*(rot[0]*rot[2]+rot[1]*rot[3])],
            [2*(rot[0]*rot[1]+rot[2]*rot[3]), -rot[0]**2+rot[1]**2-rot[2]**2+rot[3]**2, 2*(rot[1]*rot[2]-rot[0]*rot[3])],
            [2*(rot[0]*rot[2]-rot[1]*rot[3]), 2*(rot[1]*rot[2]+rot[0]*rot[3]), -rot[0]**2-rot[1]**2+rot[2]**2+rot[3]**2]])
        rotParticipant2 = np.dot(RQ, self.initGripperPosition)

        self.listParticipantPos1.append(posParticipant1)
        self.listParticipantRot2.append(rotParticipant2)

        if len(self.listParticipantPos1) == 4:
            vel  = np.linalg.norm(np.diff(self.listParticipantPos1, n=1, axis=0)[2] / self.dt)
            acc  = np.linalg.norm(np.diff(self.listParticipantPos1, n=2, axis=0)[1] / self.dt**2)
            jerk = np.linalg.norm(np.diff(self.listParticipantPos1, n=3, axis=0)[0] / self.dt**3)

            self.data_out_1 = vel / 5
            print(self.data_out_1)

            del self.listParticipantPos1[0]

        if len(self.listParticipantRot2) == 3:
            anglevel = np.linalg.norm(np.diff(self.listParticipantRot2, n=1, axis=0)[1] / self.dt)
            angleacc = np.linalg.norm(np.diff(self.listParticipantRot2, n=2, axis=0)[0] / self.dt**2)

            self.data_out_2 = anglevel

            del self.listParticipantRot2[0]


    def __forCustomWeight(self):
        pass