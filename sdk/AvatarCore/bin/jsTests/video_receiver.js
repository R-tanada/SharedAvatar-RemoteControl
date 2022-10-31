/*
Video receiver
This example shows how to:
- Initialize avatarCore SDK
- Create a core video wrapper object for resource [local-camera]
- Start receiving images via a callback
- Destroy the SDK when pressing ctrl+c

Author: yamen@avatarin.com
*/

const CoreSDK = require("../CoreJS/CoreAPI");
const { CoreVideoWrapper } = require("../CoreJS/CoreVideoWrapper");

//setup input library
const readline = require('readline');
readline.emitKeypressEvents(process.stdin);
process.stdin.setRawMode(true);
let index = 0


//when image frames arrives, the following callback will be triggered
function cb_onFrameArrived(resource, frame) {
    console.log(index, " - Received Frame [" + resource + "]: ", frame.width, "x", frame.height, " - Format: " + CoreSDK.VideoFormatString[frame.format])
    index++
    //we can process the image here, make sure to know which format you are handling (Core uses format I420)
    //let graysale = frame.subarray(frame.width * frame.height)
}

//Initialize Core
CoreSDK.InitAPI(master = false, appPath = "../")

//Create a video wrapper object, this will handle image resources
let videoWrapper = new CoreVideoWrapper("remote-image", cb_onFrameArrived);


//Start reading the image resource
videoWrapper.Start();


const ExitApp = () => {
    //destroy and clean up the core
    videoWrapper.Stop();
    videoWrapper.Destroy();
    videoWrapper = undefined
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

