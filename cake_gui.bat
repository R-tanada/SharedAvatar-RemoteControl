@echo off
call %USERPROFILE%\envs\cake\Scripts\activate.bat
cd core_set
call activate.bat
python ../docs/cake_gui_v2.py
