/*
Subscriber Test
This example shows how to:
- Initialize avatarCore SDK
- Create a video resource 
- Periodically update the image contents every 30ms
- Destroy the SDK when pressing ctrl+c

Author: yamen@avatarin.com
*/

const CoreSDK = require("../CoreJS/CoreAPI");
const { CoreVideoWrapper } = require("../CoreJS/CoreVideoWrapper");

//setup input library
const readline = require('readline');
readline.emitKeypressEvents(process.stdin);
process.stdin.setRawMode(true);


//Initialize Core
CoreSDK.InitAPI(master = false, appPath = "../")

//Create a video wrapper object, this will handle image resources
let videoWrapper = new CoreVideoWrapper("local-image", (frame) => { });


let stopped = false
let width = 640
let height = 480
let index = 0

//create a buffer holder for an RGB image data (3 channels)
let videoData = new Uint8Array(width * height * 3)
setInterval(() => {
    if (stopped) return;
    //generate a random image
    for (let x = 0; x < width; x++) {
        for (let y = 0; y < height; y++) {
            let index = y * height + x
            videoData[index * 3 + 0] = Math.random() * 255;
            videoData[index * 3 + 1] = Math.random() * 255;
            videoData[index * 3 + 2] = Math.random() * 255;
        }
    }
    //write video frame resource
    console.log(index, " - Sending Frame [", videoWrapper.resource, "]: ", width, "x", height)
    index++;
    videoWrapper.WriteFrame(width, height, CoreSDK.VideoFormat.RGB, videoData)
}, 30)

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

