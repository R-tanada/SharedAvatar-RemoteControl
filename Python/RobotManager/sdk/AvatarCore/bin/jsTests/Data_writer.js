/*
Data Writer
This example shows how to:
- Initialize avatarCore SDK
- Create a core data wrapper object for resource [test-resource]:channel[app]
- Send key strokes via the data channel
- Destroy the SDK when pressing 'ctrl+c'

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

//Create a data wrapper object, this will handle data resources
let dataWrapper = new CoreDataWrapper("local-data", "app", (frame) => { });


const ExitApp = () => {
    //destroy and clean up the core
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
    } else {
        process.stdout.write(key.sequence)
        dataWrapper.SendData(key.sequence)
    }
});

