/*
Cloud Client
This example shows how to:
- Initialize avatarCore SDK
- Load CloudClient services for streaming purpose
- Set the peerID for the client
- Connect to a master peer
- Start streaming/receiving data via cloud
- Destroy the SDK when pressing 'ctrl+c'

Make sure to run CloudMaster on a different machine before running this sample

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
CoreSDK.LoadServices("services/CloudClient-services.json");
//start default services
CoreSDK.StartDefaultServices();


CoreSDK.SubscribeToMessage(
    "avatarin",
    "cloud/status",
    (name, path, data) => {
        console.log("Cloud status: ", data)
    }
);

//set the service name alias (found in the CloudClient-services.json)
CoreSession.InitSession("Client/cloud")
//setup the peerID for this 
CoreSession.SetPeerID(509091)
//startup cloud services
CoreSDK.StartService("Client/.*")

setTimeout(() => {
    //connect to the master node
    CoreSession.ConnectToPeer(509090)
}, 2000)

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

