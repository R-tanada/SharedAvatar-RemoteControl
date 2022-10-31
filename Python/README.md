# ms-proj-sharedavatar/Experiment1/Python
<img src="https://img.shields.io/badge/Python-3.9.5-blue?&logo=Python">
<img src="https://img.shields.io/badge/License-Confidential-red">


Moonshot 融合アバタープロジェクトの開発リポジトリです．
複数人の操作者の手の動きを1つのxArmに反映させ，作業を行うシステムです．
今後，実験が増えていくと考え，実験ごとにディレクトリを切っています．


# Requirement

- Python                3.9.5
- numpy                 1.20.3
- numpy-quaternion      2021.6.9.13.34.11
- scipy                 1.6.3
- scikit-surgerycore    0.6.9
- simpleaudio           1.0.4



# Installation

```bash
pip install numpy
pip install numpy-quaternion
pip install scipy
pip install scikit-surgerycore
pip install simpleaudio
```

- `pip install simpleaudio` に失敗する場合
    - https://visualstudio.microsoft.com/ja/downloads/ からBuild Tools for Visual Studio 2019をダウンロードし，「C++ Build Tools」もしくは「C++によるデスクトップ開発」をインストールする




# Usage
### **IMPORTANT**
IPアドレスをプログラム上で直接入力する方式は廃止しました．
今後は，ExManager.pyと同じ階層に，settings.csvを作り，そこから読み込むようにしました．
```settings.csv
settings.csv
# 書き方のサンプル

xArmIPAddress,192.168.1.223
motiveServerIPAddress,192.168.1.1
motiveLocalIPAddress,192.168.1.2
wirelessIPAddress,192.168.80.202
localIPAddress,127.0.0.1
```


## 共通設定
1. `RobotControlManager.py`の`bendingSensorPortParticipant1` (曲げセンサからのデータを受け取るポート番号．曲げセンサを2つ使う場合は`bendingSensorPortParticipant2`も適宜変更)，`bendingSensorCount` (使用する曲げセンサの個数) を任意の値に変更する
1. OptiTrack Motive側で，参加者に対応するRigidBodyのStreaming IDが1から始まるように設定してください
    - 参加者2人の場合，Streaming IDはそれぞれ1と2に対応
    - このプログラムでは参加者の剛体を，Motiveの剛体Streaming IDで識別しています

## OptiTrackからの参加者2人の運動を反映させ，1つの曲げセンサを用いる場合
1. `RobotControlManager.py`のメンバ変数を以下のように設定する
    - bendingSensorCount = 1
    - recordedParticipantMotionCount = 0
    - recordedGripperValueCount = 0
    - motionDataInputMode = "optitrack"
    - gripperDataInputMode = "bendingsensor"
1. `ExManager.py`で，`robotControlManager.SendDataToRobot(participantNum=2, executionTime=30)`を記述
1. プログラムを実行する．`python ExManager.py`
1. Sキーを押し，Enterを押したときの剛体の座標と回転を原点とし，運動反映を開始する．
1. 30秒間運動が反映されたあと，参加者の運動データと曲げセンサからのデータを`ExportData`フォルダに書き出して終了する


<br>

- 3人以上の参加者，2つ以上の曲げセンサにも，プログラム上対応しています（動作未確認）
    - `participantNum`，`bendingSensorCount`，`bendingSensorPortParticipantXX`といった対応する数値を変更してください
    - ただし，3人以上で回転を融合する場合はデフォルトで平均になります．
        - 後述するNote - IMPORTANTも参照ください．
    - 曲げセンサの値も平均になります．



## あらかじめ記録された1人の参加者の運動と1つの曲げセンサの値を用いる場合
1. 上述の「OptiTrackからの参加者2人の運動を反映させ，1つの曲げセンサを用いる場合」で書き出した30秒間のデータを用意する（このサンプルで使用するのは参加者1のデータのみ)．
1. `ExManager.py`と同じ階層に`RecordedMotion`フォルダ作成し，その中に格納する．
1. `RobotControlManager.py`のメンバ変数を以下のように設定する
    - bendingSensorCount = 0
    - recordedParticipantMotionCount = 1
    - recordedGripperValueCount = 1
    - motionDataInputMode = "optitrack"
    - gripperDataInputMode = "bendingsensor"
1. `ExManager.py`で，`robotControlManager.SendDataToRobot(participantNum=0, executionTime=30, isExportData=False)`を記述
1. プログラムを実行する．`python ExManager.py`
1. Sキーを押し，運動反映を開始する．
1. 30秒間運動が反映されたあと終了する

<br>

- 記録データが，`SendDataToRobot`で指定した秒数よりも少ない場合，記録データの最後のフレームデータを返し続ける (ループはしない)．


## OptiTrackからの参加者1人の運動とあらかじめ記録された1人の参加者の運動を融合して反映させ，1つの曲げセンサを用いる場合
1. 上述の「OptiTrackからの参加者2人の運動を反映させ，1つの曲げセンサを用いる場合」で書き出した30秒間のデータを用意する（このサンプルで使用するのは参加者1のデータのみ)．
1. `ExManager.py`と同じ階層に`RecordedMotion`フォルダ作成し，その中に格納する．
1. `RobotControlManager.py`のメンバ変数を以下のように設定する
    - bendingSensorCount = 1
    - recordedParticipantMotionCount = 1
    - recordedGripperValueCount = 0
    - motionDataInputMode = "optitrack"
    - gripperDataInputMode = "bendingsensor"
1. `ExManager.py`で，`robotControlManager.SendDataToRobot(participantNum=1, executionTime=30, isExportData=False)`を記述
1. プログラムを実行する．`python ExManager.py`
1. Sキーを押し，Enterを押したときの剛体の座標と回転を原点とし，運動反映を開始する．
1. 30秒間記録データと実際の参加者の運動が融合して反映されたあと終了する

<br>
<br>

### Default parameters
- Motion data input:    `OptiTrack` 
- Gripper input:        `Bending sensor`
- `RobotControlManager.py`
    - xArm IP address:              `192.168.1.223`
    - Bending sensor IP address:    `192.168.80.142`
    - Bending sensor port:          `9000`, `9001`
    - Number of bending sensors:    1
- `OptiTrackStreamingManager.py`
    - OptiTrack server:     `192.168.1.1`
    - OptiTrack client:     `192.168.1.2`
    - Multicast address:    `239.255.42.99`

# Description
- 基本的には`ExManager.py`と`RobotControlManager.py`のメンバ変数を適宜書き換えることで様々な動作を行う．
- より詳細な設定を変更した場合は，それぞれのクラスを参照する．
- また，各クラスのメソッドの詳細についてはそれぞれのdocstringを参照してください．
- 以下，メインで使用するもののみ詳細を記述します．


## Main contents
### ExManager.py
- 実験管理プログラム．プログラム全体を管理する中央司令塔．
- このプログラムから任意のマネージャーを呼び出し，実行する．

### RobotControlManager.py
- ロボットアームを制御するマネージャー．
- ロボットアームとのデータ通信や実験制御，エラー管理を行う．
- このプログラムから，操作者のデータへのアクセスや融合運動生成プログラムへアクセスする．

#### Parameters
- xArmIpAddress
    - 制御するxArmのIPアドレス
- localIpAddress
    - ローカルストリーミングでデータを受け取る場合のIPアドレス
    - Unityからデータを受け取る場合に使用
- bendingSensorIpAddress
    - 曲げセンサからのデータをWi-Fi経由で受け取る際のIPアドレス


- localPort
    - ローカルストリーミングでデータを受け取る場合のポート番号
    - Unityからデータを受け取る場合に使用
- bendingSensorPortParticipant1, bendingSensorPortParticipant2
    - 曲げセンサからのデータをWi-Fi経由で受け取る際のポート番号
    - さらにセンサを増やす場合は，適宜追加し，`bendingSensorPorts`に加える
- bendingSensorPorts
    - すべての曲げセンサのポートを格納するリスト


- bendingSensorCount
    - 実際に使用する曲げセンサの個数
- otherRigidBodyCount
    - 参加者以外の剛体の個数
    - ロボットアームの実空間での先端座標を取得したい場合に，アームに剛体を定義するときなどに使用
    - この値は融合運動には全く影響しない
    - 単なる座標取得，記録用


- recordedParticipantMotionCount
    - あらかじめ記録された参加者の運動データを使用する場合に使用
    - 0の場合，特に何も読み込まない（リアルタイムの運動反映のみ）
    - 1以上の場合，`ParticipantMotionManager.py`のメンバ変数，`recordedMotionPath`の場所からCSVファイルを読み込む
    - ファイル名は`recordedMotionFileName`で指定する
- recordedGripperValueCount
    - あらかじめ記録されたグリッパーデータを使用する場合に使用
    - 0の場合，特に何も読み込まない（リアルタイムの反映のみ）
    - 1以上の場合，`ParticipantMotionManager.py`のメンバ変数，`recordedMotionPath`の場所からCSVファイルを読み込む
    - ファイル名は`recordedGripperValueFileName`で指定する


- motionDataInputMode
    - リアルタイムの参加者の運動データ入力に使用するシステム
    - `optitrack`, `unity`を指定可能
    - `optitrack`の場合，後述の`OptiTrackStreamingManager.py`のIPアドレスも指定してください
    - `unity`の場合，別途Unityを実行し，指定の形式でUDPストリーミングしてください．
- gripperDataInputMode
    - リアルタイムのグリッパー入力に使用するシステム
    - `bendingsensor`, `unity`, `debug`を指定可能
    - `bendingsensor`の場合，`bendingSensorIpAddress`と`bendingSensorPorts`を指定してください
    - `unity`の場合，別途Unityを実行し，指定の形式でUDPストリーミングしてください．
    - `debug`の場合，常に`targetMax / 2` (Default = 425) を返し続けます．

- sharedMethod
    - 融合アバターを操作する方法
    - `integration`, `divisionofroles`を指定可能

- directionOfParticipants
    - 参加者の向いている方向
    - すべての参加者ロボットに対して同じ方向を向いている場合`same`
    - 1人でも対面の場合，`opposite`を指定
    - `same`, `opposite`を指定可能

- oppositeParticipants
    - ロボットに対して対面している参加者をリストで指定する
    - ここで指定した参加者のrotationのdictを参照する

- inversedAxes
    - oppositeParticipantsで指定した参加者の回転について，反転させる軸を文字列型リストで指定する
    - `['y', 'z']`: 自然な動作になる
    - `['x', 'z']`: 鏡に写った動作となる

- movingDifferenceLimit
    - ロボットアームがフレーム間でどのくらい移動した場合に動作を停止するか．
    - デフォルトは50[mm]

#### Methods
- SendDataToRobot<br>
    **Arguments**
    - participantNum
        - リアルタイムの参加者人数
    - executionTime
        - (Optional) 実験実行時間．秒．この時間を超えると自動的に終了する
    - isFixedFrameRate
        - (Optional) 固定フレームレートでプログラムを実行するかどうか．
        - デフォルトはPCスペックに依存
    - frameRate
        - (Optional) 指定のフレームレートでプログラムを実行する
    - isChangeOSTimer
        - (Optional, only for Windows) OSタイマーを変更するかどうか．
        - WindowsではOSタイマー以上に細かくフレームレートを設定できないため，タイマーを変更することでより詳細なフレームレート設定を実現する．
        - **注意: OSタイマーを変更するので，他のアプリケーションの動作に影響を及ぼす可能性がある．**
    - isExportData
        - (Optional) データを書き出すかどうか．
        - デフォルトでは`ExManager.py`と同じ階層に`ExportData`フォルダを作成し，データを書き出す
    - isEnablexArm
        - (Optional) xArmを起動するかどうか．
        - デバッグ用．xArmを動かす部分以外の挙動を確認する．


### OptiTrackStreamingManager.py
#### Parameters
- serverAddress
    - OptiTrackを動かしているPCのIPアドレス
- localAddress
    - OptiTrackからのデータを受け取るPCのIPアドレス
    - 特殊な状態でなければ，このPythonを動かしているPC

#### Methods
- stream_run
    - これをスレッドなどで実行することで，ストリーミングが走る


### xArmTransform.py
#### Parameters
- __initX, __initY, __initZ, __initRoll, __initPitch, __initYaw
    - xArmのInitial Position
- __minX, __minY, __minZ, __minRoll, __minPitch, __minYaw
    - xArmの動きの下限
    - この値以下には動かないように制限される
- __maxX, __maxY, __maxZ, __maxRoll, __maxPitch, __maxYaw
    - xArmの動きの上限
    - この値以上には動かないように制限される

#### Methods
- Transform
    - このメソッドを呼び出すことで，動きの制限を加えたxArmの座標と回転を取得する



# Note
### **WARNING**
- `RobotControlManager.SendDataToRobot`
    - Argument: `isChangeOSTimer`
    - only for Windows
    - Change the Windows OS timer
    - Since this option changes the OS timer, it will affect the performance of other programs.
    - https://python-ai-learn.com/2021/02/07/time/
    - https://docs.microsoft.com/en-us/windows/win32/api/timeapi/nf-timeapi-timebeginperiod


### **IMPORTANT**
- `CyberneticAvatarMotionBehaviour.py`
    - If you are using average rotations of three or more people, please cite the following paper
    - see also: https://github.com/UCL/scikit-surgerycore

```
Thompson S, Dowrick T, Ahmad M, et al.
SciKit-Surgery: compact libraries for surgical navigation.
International Journal of Computer Assisted Radiology and Surgery. May 2020.
DOI: 10.1007/s11548-020-02180-5
```


# Versions
Described when there is a major change or addition
- 1.0: 2021/10/16
    - Initial prototype
- 1.1: 2021/10/28
    - Support for pre-recorded data
    - Minor changes
- 1.2: 2021/11/17
    - Add Vibrotactile feedback manager and methods
    - Add Weight slider manager and methods
- 1.3: 2021/11/19
    - Support for division of role method
- 1.3.1: 2021/11/20
    - Added a function to amplitude modulate a sine wave from a load cell raw value.
- 1.3.2: 2021/11/25
    - Added a mode to control the robot face-to-face.
- 1.4.0: 2021/12/4
    - Merged `Tanakalab`

# Author

- Takayoshi Hagiwara
    - Graduate School of Media Design, Keio University
    - hagiwara@kmd.keio.ac.jp

- Takumi Katagiri
    - Nagoya Institute of Technology

- Takumi Nishimura
    - Nagoya Institute of Technology


# License

This project is Confidential.