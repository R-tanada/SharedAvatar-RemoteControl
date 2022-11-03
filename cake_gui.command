cd
source envs/cake/bin/activate
MY_DIRNAME=$(dirname $0)
cd $MY_DIRNAME
cd core_set
source activate.sh
python ../docs/cake_gui_v2.py
