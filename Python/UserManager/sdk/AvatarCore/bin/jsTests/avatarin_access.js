/*
Avatarin desktop app resources access
This example shows how to:
- Initialize avatarCore SDK
- Read resources from the desktop app (video/data)
- Destroy the SDK when pressing 'ctrl+c'

Author: yamen@avatarin.com
*/


const CoreSDK = require("../CoreJS/CoreAPI");
const { CoreVideoWrapper } = require("../CoreJS/CoreVideoWrapper");
const { CoreDataWrapper } = require("../CoreJS/CoreDataWrapper");

//setup input library
const readline = require('readline');
readline.emitKeypressEvents(process.stdin);
process.stdin.setRawMode(true);


//when image frames arrives, the following callback will be triggered
function cb_onVideoFrameArrived(resource, frame) {
    console.log("[" + resource + "] New frame arrived: ", frame.width, "x", frame.height, " - Format: " + CoreSDK.VideoFormatString[frame.format])
    //we can process the image here, make sure to know which format you are handling (Core uses format I420)
    //let graysale = frame.subarray(frame.width * frame.height)
}

//when a data frame arrived, it will call this function
function cb_onDataFrameArrived(resource, channel, frame) {
    console.log("[" + resource + ":" + channel + "] Data Arrived: " + frame)
}


//Initialize Core
CoreSDK.InitAPI(master = false, appPath = "../")

//Create a video wrapper object, this will handle image resources
let localVideoWrapper = new CoreVideoWrapper("local-camera", cb_onVideoFrameArrived);
let remoteVideoWrapper = new CoreVideoWrapper("remote-camera", cb_onVideoFrameArrived);
let remoteBotVideoWrapper = new CoreVideoWrapper("remote-bottom-camera", cb_onVideoFrameArrived);

let localDataWrapper = new CoreDataWrapper("local-data", "app", cb_onDataFrameArrived);
let remoteDataWrapper = new CoreDataWrapper("remote-data", "app", cb_onDataFrameArrived);


//Start reading the image resource
localVideoWrapper.Start();
remoteVideoWrapper.Start();
remoteBotVideoWrapper.Start();


//Start reading the data resources
localDataWrapper.Start();
remoteDataWrapper.Start();

const ExitApp = () => {
    //destroy and clean up the resources
    localVideoWrapper.Destroy();
    localVideoWrapper = undefined

    remoteVideoWrapper.Destroy();
    remoteVideoWrapper = undefined

    remoteBotVideoWrapper.Destroy();
    remoteBotVideoWrapper = undefined

    localDataWrapper.Destroy();
    localDataWrapper = undefined

    remoteDataWrapper.Destroy();
    remoteDataWrapper = undefined


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

