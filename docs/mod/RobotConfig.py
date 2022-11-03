import json

ENABLE_XARM = True

settings_file = open('../settings.json', 'r')
settings_data = json.load(settings_file)

AVATAR_PEER_ID = settings_data['avatarPeerID']
SLIDER_PORT = settings_data['sliderPort']
TABLE_PORT = settings_data['tablePort']
XARM_IP = settings_data['xArmIPAddress']

INIT_X, INIT_Y, INIT_Z = 200, 0, 400
INIT_ROLL, INIT_PITCH, INIT_YAW = 180, 0, 0
MAX_X, MAX_Y, MAX_Z = 460, 400, 600
MIN_X, MIN_Y, MIN_Z = 180, -300, 270
INIT_GRIPPER_POS = 850

# INIT_X, INIT_Y, INIT_Z = settings_data['initX'], settings_data['initY'], settings_data['initZ']
# INIT_ROLL, INIT_PITCH, INIT_YAW = settings_data['initRoll'], settings_data['initPitch'], settings_data['initYaw']
# MAX_X, MAX_Y, MAX_Z = settings_data['maxX'], settings_data['maxY'], settings_data['maxZ']
# MIN_X, MIN_Y, MIN_Z = settings_data['minX'], settings_data['minY'], settings_data['minZ']
# INIT_GRIPPER_POS = settings_data['initGripperPos']

core = None
xarm = None
table_serial = None
pos = None
gripper_pos = None
loadcell_val = None
