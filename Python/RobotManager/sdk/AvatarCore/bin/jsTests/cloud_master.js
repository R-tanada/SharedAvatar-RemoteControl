/*
Cloud Client
This example shows how to:
- Initialize avatarCore SDK
- Load CloudClient services for streaming purpose
- Set the peerID for the client
- Start streaming/receiving data via cloud
- Destroy the SDK when pressing 'ctrl+c'

Run this sample on Machine 1 before running CloudClient Sample on Machine 2

Author: yamen@avatarin.com
*/

const CoreSDK = require("../CoreJS/CoreAPI");
const CoreSession = require("../CoreJS/CoreSesssion");

//setup input library
const readline = require('readline');
readline.emitKeypressEvents(process.stdin);
process.stdin.setRawMode(true);


//Initialize Core
CoreSDK.InitAPI(master = true, appPath = "../")

//load service file. It should be located under "targetFolder/services" path
CoreSDK.LoadServices("services/CloudMaster-services.json");
//start default services
CoreSDK.StartDefaultServices();


CoreSDK.SubscribeToMessage(
    "avatarin",
    "cloud/status",
    (name, path, data) => {
        console.log("Cloud status: ", data)
    }
);


//set the service name alias (found in the CloudMaster-services.json)
CoreSession.InitSession("Master/cloud")
//setup the peerID for this 
CoreSession.SetPeerID(509090)
//startup cloud services
CoreSDK.StartService("Master/.*")

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

