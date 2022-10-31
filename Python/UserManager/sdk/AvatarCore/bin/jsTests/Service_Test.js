/*
Service Test
This example shows how to:
- Initialize avatarCore SDK
- Subscribe to some messages coming from the core side
- Load services from JSON file
- Destroy the SDK when pressing ctrl+c

Simple Services file contains two services:
1) VideoCapture service, this one opens a camera connect to the PC, and outputs its image frames into "local-camera" resource object
2) VideoFormat service, this one converts an image resource "local-camera" to RGB format and writes the results into "local-camera-RGB" resource object

Please note that in CoreSDK, the default image format is I420 which is a YUYV format that is relatively much smaller than RGB frames. However, for most computer vision operations, RGB format might be preferable than I420 for the ease of access to the different color channels. Thus this sample shows how to get the RGB formatted frame.

Author: yamen@avatarin.com
*/

const CoreSDK = require("../CoreJS/CoreAPI");
//setup input library
const readline = require('readline');
readline.emitKeypressEvents(process.stdin);
process.stdin.setRawMode(true);


//Initialize Core
CoreSDK.InitAPI(master = false, appPath = "../")


function replyCB(service, path, data) {
  console.log("Subscriber callback in Javascript [" + service + "]--> " + path + ":" + data)
}

//subscribe to service/loaded messages. When the services are loaded or started, the callback will "replyCB" will be triggered from the native side
CoreSDK.SubscribeToMessage("application", "service/loaded", replyCB);
CoreSDK.SubscribeToMessage("application", "service/started", replyCB);

//load service file. It should be located under "targetFolder" path
//copy Simple-services.json to [bin/[arch]/services] folder
CoreSDK.LoadServices("services/Simple-services.json");
//start default services
CoreSDK.StartDefaultServices();


const ExitApp = () => {
  //destroy and clean up the core
  CoreSDK.DestroyAPI()
  console.log("Core Destroyed!")
  process.exit(0)
};



process.stdin.on('keypress', (str, key) => {
  if (key.ctrl && key.name === 'c') {
    ExitApp()
  }
});

