# ms-proj-sharedavatar/Experiment1/TouchDesigner
<img src="https://img.shields.io/badge/License-Confidential-red">


ZED Miniからの映像をTouchDesignerで処理し，ストリーミングします．


# Requirement
- TouchDesigner         2021.13610
- Spout                 2.007f
- SpoutToNDI            2.007


# Installation
1. 対応するバージョンのSpoutをインストールする (https://github.com/leadedge/Spout2/releases)


# Usage
## 共通設定
1. ZED_Spout.toeを起動する
1. startup.batを実行すると，Spout to NDIとNDI to Spoutというアプリが2つずつ立ち上がる．
1. それぞれのアプリで，File -> Openからソースを指定する（それぞれ右目，左目映像に対応する）
1. ここまででNDIに映像がストリームされる
1. 別のPCにストリームする場合は，受け取り側のPCでNDI to Spoutを2つ開き，ソースを指定する


# Description
- 基本的には`ExManager.py`と`RobotControlManager.py`のメンバ変数を適宜書き換えることで様々な動作を行う．
- より詳細な設定を変更した場合は，それぞれのクラスを参照する．
- また，各クラスのメソッドの詳細についてはそれぞれのdocstringを参照してください．
- 以下，メインで使用するもののみ詳細を記述します．


# Note
### **WARNING**
- 2つの映像で同期が取れない

# Versions
- 1.0: 2021/6/26
    - Initial prototype

# Author

- Takayoshi Hagiwara
    - Graduate School of Media Design, Keio University
    - hagiwara@kmd.keio.ac.jp


# License

This project is Confidential.