/*
Data receiver
This example shows how to:
- Initialize avatarCore SDK
- Create a core data wrapper object for resource [test-resource]
- Start receiving data via a callback
- Destroy the SDK when pressing ctrl+c

Author: yamen@avatarin.com
*/

const CoreSDK = require("../CoreJS/CoreAPI");
const { CoreDataWrapper } = require("../CoreJS/CoreDataWrapper");

//setup input library
const readline = require('readline');
readline.emitKeypressEvents(process.stdin);
process.stdin.setRawMode(true);


//Initialize Core
CoreSDK.InitAPI(master = false, appPath = "../")

//when a data frame arrived, it will call this function
function cb_onFrameArrived(resource, channel, frame) {
    console.log("Received data frame: ", resource, ":", channel, " --> ", frame)

}

//Create a data wrapper object, this will handle data resources
let dataWrapper = new CoreDataWrapper("local-data", "app", cb_onFrameArrived);


//Start reading the data resource
dataWrapper.Start();


const ExitApp = () => {
    //destroy and clean up resources
    dataWrapper.Stop();
    dataWrapper.Destroy();
    dataWrapper = undefined
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

