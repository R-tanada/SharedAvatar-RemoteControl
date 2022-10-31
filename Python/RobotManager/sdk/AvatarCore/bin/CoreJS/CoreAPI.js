const avatarCoreAPI = require("../AvatarJS.node");
const path = require("path");

let __self = {
  updateLoop: 0,
};

let CoreAPIWrapper = null;

let core_inited = false;
let core_initing = false;

let _service_status = {};

function ServiceStatusReplyCB(service, path, data) {
  let res = data.split(":");
  _service_status[res[0]] = res[1] == "1" ? true : false;
  /*
  process.stdout.write("ServiceStatusReplyCB:  " + res[0] + " = ");
  process.stdout.write(_service_status[res[0]].toString());
  process.stdout.write("\n");*/
}

//predefined callback types
const DefaultCallbacks = {
  InitCallbackType: "Init",
  DestroyCallbackType: "Destroy",
  PreLoadServices: "PreLoadServices",
  OnLog: "OnLog",
};

let callbacks = {};

//Add a callback function with the format
// function(){}
function AddCallbackFunction(callbackType, func) {
  if (!(callbackType in callbacks)) {
    callbacks[callbackType] = [];
  }
  let index = callbacks[callbackType].indexOf(func);
  if (index == -1) {
    callbacks[callbackType].push(func);
  }
}

//Remove an existing callback function
function RemoveCallback(callbackType, func) {
  if (!(callbackType in callbacks)) {
    return;
  }
  let index = callbacks[callbackType].indexOf(func);
  if (index != -1) {
    callbacks[callbackType].splice(index, 1);
  }
}

function TriggerCallback(callbackType, data) {
  //console.log("------> TriggerCallback [" + callbackType + "]");
  if (callbackType in callbacks) {
    callbacks[callbackType].forEach(function (value) {
      value(data);
    });
  }
}

//predefined list of services
const ServiceNames = {
  CloudProxy: "Management/proxy",
  VideoCapture: "Capture/video/front",
  AudioCapture: "Capture/audio",
  MediaSender: "RTP/sender/media",
  MediaReceiver: "RTP/receiver/media",
  DataSender: "RTP/sender/data",
  DataReceiver: "RTP/receiver/data",
  AudioPlayback: "Capture/remote/audio",
};
const LogLevel = {
  Emergency: 0,
  Alert: 1,
  Critical: 2,
  Error: 3,
  Warning: 4,
  Notice: 5,
  Info: 6,
  Debug: 7,
};

/*
This function invokes a message to check whether a certain service is running or not, and will return its response in an asynced manner
Sample usage of this function:
checkServiceStatus(service, function (res) {
  console.log(service + " : " + res);
});
*/
function CheckServiceStatus(service, callback, timeout = 10) {
  _service_status[service] = false;
  CoreAPIWrapper.PublishMessage("", "service/status/get", service);
  setTimeout(function () {
    callback(_service_status[service]);
  }, timeout);
}

//const __LogLevelType = ["Info", "Success", "Warning", "Error"];
function LogCallback(source, message, level, verbose) {
  /*let msg = "[" + source + "]: " + message;
  if (logType == 2) console.warn(msg);
  else if (logType == 3) console.error(msg);*/
  TriggerCallback(DefaultCallbacks.OnLog, {
    timeStamp: new Date(),
    source: source,
    message: message,
    logType: "CoreSDK", //__LogLevelType[logType],
    logLevel: level, //verbose,
  });
}

//Initialize core API, master should be true as it indicates to start the service coordinator as a master service
function InitAPI(
  master,
  appPath = "",
  port = 0,
  preloadServices = [],
  defaultServices = [],
  serviceMode = true,
  logPath = "."
) {
  if (core_inited || core_initing) return true;
  process.stdout.write("Javascript- InitAPI!\n");
  __self.appPath = appPath;
  //Create Core API Wrapper instance here, its a singleton
  CoreAPIWrapper = new avatarCoreAPI.NapiCoreAPIWrapper();
  core_initing = true;
  //Initialize Core
  if (!CoreAPIWrapper.InitAPI(master, appPath, port, logPath, LogCallback)) {
    console.log("Failed to initialize avatar core!");
    DestroyAPI();
    core_initing = false;
    return false;
  }
  CoreAPIWrapper.SetServiceMode(serviceMode)
  core_inited = true;

  TriggerCallback(DefaultCallbacks.PreLoadServices, {});
  //Load Services, two approaches are possible:
  // LoadServices --> Would load the services to the same process, still have some issues
  // RefService --> Load the services via a subprocess, more stable but have the risk of the subprocess be active even when the main process is closed.
  preloadServices.forEach((service) => {
    CoreAPIWrapper.RefService(service);
  });
  defaultServices.forEach((service) => {
    CoreAPIWrapper.LoadServices(service);
  });
  CoreAPIWrapper.StartDefaultServices();

  //subscribe to the status reply messages, used to keep track of the status of the services when queried via "checkServiceStatus" function
  CoreAPIWrapper.SubscribeToMessage(
    "Application",
    "service/status/reply",
    ServiceStatusReplyCB
  );

  core_initing = false;

  UpdateLoop();
  //console.log(GetCameraList());
  //Trigger init callback function for subscribed functions
  TriggerCallback(DefaultCallbacks.InitCallbackType, {});
  return true;
}

//Destroy core API and close the active services
function DestroyAPI() {
  if (core_inited == false) return;
  //Trigger on destroyed callback event
  TriggerCallback(DefaultCallbacks.DestroyCallbackType, {});

  if (__self.updateLoop) {
    clearInterval(__self.updateLoop);
    __self.updateLoop = 0;
  }

  //remove subscribed messages
  CoreAPIWrapper.UnsubscribeToMessage("Application", "service/status/reply");

  process.stdout.write("Javascript- DestroyAPI!\n");
  core_inited = false;

  //Destroy the Core API instance
  CoreAPIWrapper.DestroyAPI();

  //avatarCoreAPI.KillProcess();
  // __self.subCallback.stopMethod();
  //__self.subCallback = 0;

  process.stdout.write("Javascript- DestroyAPI - Done!\n");
  CoreAPIWrapper = null;
}

//Shutdown the cloud process
function DestroyCloud() {
  process.stdout.write("Javascript- Destroy Cloud\n");
  avatarCoreAPI.CloseCloud();
  process.stdout.write("Javascript- Cloud Closed - Done!\n");
}

//Formats for video
const VideoFormatString = {

  0: "ALPHA",
  1: "I420",
  2: "YUY2",
  3: "RGB",
  4: "RGBA",
  5: "BGR",
  6: "BGRA",
  7: "xRGB",
  8: "xBGR",
  9: "NV12",
  10: "UNKOWN"
};
//Formats for video
const VideoFormat = {
  ALPHA: 0,
  I420: 1,
  YUY2: 2,
  RGB: 3,
  RGBA: 4,
  BGR: 5,
  BGRA: 6,
  xRGB: 7,
  xBGR: 8,
  NV12: 9,
  UNKOWN: 10
};
///Formats for audio
const AudioFormatString = {
  0: "Unkown",
  1: "U8", //single byte, unsigned
  2: "S8", //single byte, signed
  3: "U16", //two bytes
  4: "S16", //two bytes
  5: "U24", //three bytes
  6: "S24", //three bytes
  7: "U32", //four bytes
  8: "S32", //four bytes
  9: "F32", //four bytes, floating point
};
const AudioSampleSize = {
  0: 1,
  1: 1, //single byte, unsigned
  2: 1, //single byte, signed
  3: 2, //two bytes
  4: 2, //two bytes
  5: 3, //three bytes
  6: 3, //three bytes
  7: 4, //four bytes
  8: 4, //four bytes
  9: 4, //four bytes, floating point
};

function CreateVideoWrapper() {
  return new avatarCoreAPI.NapiVideoWrap();
}

function CreateAudioWrapper() {
  return new avatarCoreAPI.NapiAudioWrap();
}
function CreateDataWrapper() {
  return new avatarCoreAPI.NapiDataWrap();
}

function LoadServices(path) {
  if (core_inited == false) {
    process.stdout.write("LoadServices: CoreAPI is not inited!\n");
    return;
  }
  CoreAPIWrapper.LoadServices(path);
}
function RefService(service) {
  if (core_inited == false) {
    process.stdout.write("RefService: CoreAPI is not inited!\n");
    return;
  }
  CoreAPIWrapper.RefService(service);
}
function UnrefService(service) {
  if (core_inited == false) {
    process.stdout.write("UnrefService: CoreAPI is not inited!\n");
    return;
  }
  CoreAPIWrapper.UnrefService(service);
}

function SubscribeToMessage(name, path, callback) {
  if (core_inited == false) {
    process.stdout.write("SubscribeToMessage: CoreAPI is not inited!\n");
    return;
  }
  CoreAPIWrapper.SubscribeToMessage(name, path, callback);
}

function UnsubscribeToMessage(name, path) {
  if (core_inited == false) {
    process.stdout.write("UnsubscribeToMessage: CoreAPI is not inited!\n");
    return;
  }
  CoreAPIWrapper.UnsubscribeToMessage(name, path);
}

function StartService(service) {
  if (core_inited == false) {
    process.stdout.write("StartService: CoreAPI is not inited!\n");
    return;
  }
  CoreAPIWrapper.RunService(service);
}
function StartDefaultServices() {
  if (core_inited == false) {
    process.stdout.write("StartDefaultServices: CoreAPI is not inited!\n");
    return;
  }
  CoreAPIWrapper.StartDefaultServices();
}
function StopService(service) {
  if (core_inited == false) {
    process.stdout.write("StopService: CoreAPI is not inited!\n");
    return;
  }
  CoreAPIWrapper.StopService(service);
}
function PublishMessage(target, path, data) {
  if (core_inited == false) {
    process.stdout.write("PublishMessage: CoreAPI is not inited!\n");
    return;
  }
  CoreAPIWrapper.PublishMessage(target, path, data);
}

function SetAppConfigurationFilePath(path, isRelative) {
  if (core_inited == false) {
    process.stdout.write(
      "SetAppConfigurationFilePath: CoreAPI is not inited!\n"
    );
    return;
  }
  CoreAPIWrapper.SetAppConfigurationFilePath(path, isRelative);
}

function SetAppConfiguation(name, value) {
  if (core_inited == false) {
    process.stdout.write("SetAppConfiguation: CoreAPI is not inited!\n");
    return;
  }
  CoreAPIWrapper.SetAppConfiguation(name, value);
}

function GetAppConfiguation(name, defaultValue) {
  if (core_inited == false) {
    process.stdout.write("GetAppConfiguation: CoreAPI is not inited!\n");
    return undefined;
  }
  let val = CoreAPIWrapper.GetAppConfiguation(name, defaultValue);
  return val;
}

function IsCoreInited() {
  return core_inited;
}

function RefreshDevices() {
  if (core_inited == false) {
    process.stdout.write("RefreshDevices: CoreAPI is not inited!\n");
    return undefined;
  }
  return CoreAPIWrapper.RefreshDevices();
}

function GetCameraList() {
  if (core_inited == false) {
    process.stdout.write("GetCameraList: CoreAPI is not inited!\n");
    return undefined;
  }
  return CoreAPIWrapper.GetCameraList();
}

function GetSpeakerList() {
  if (core_inited == false) {
    process.stdout.write("GetSpeakerList: CoreAPI is not inited!\n");
    return undefined;
  }
  return CoreAPIWrapper.GetSpeakerList();
}

function GetMicList() {
  if (core_inited == false) {
    process.stdout.write("GetMicList: CoreAPI is not inited!\n");
    return undefined;
  }
  return CoreAPIWrapper.GetMicList();
}

function UpdateLoop() {
  if (core_inited == false) {
    return;
  }

  if (__self.updateLoop) {
    clearInterval(__self.updateLoop);
    __self.updateLoop = 0;
  }

  __self.updateLoop = setInterval(() => {
    CoreAPIWrapper.UpdateLoop();
  }, 50);
}

function GetDeviceID() {
  if (core_inited == false) {
    process.stdout.write("GetDeviceID: CoreAPI is not inited!\n");
    return undefined;
  }
  return CoreAPIWrapper.GetDeviceID();
}
function GetIPAddress(iface) {
  if (core_inited == false) {
    process.stdout.write("GetIPAddress: CoreAPI is not inited!\n");
    return undefined;
  }
  return CoreAPIWrapper.GetIPAddress(iface);
}
function GetMACAddress(iface) {
  console.log("Getting MAC address");
  if (core_inited == false) {
    process.stdout.write("GetMACAddress: CoreAPI is not inited!\n");
    return undefined;
  }
  return CoreAPIWrapper.GetMACAddress(iface);
}
function GetSDKVersion(sdk) {
  if (core_inited == false) {
    process.stdout.write("GetSDKVersion: CoreAPI is not inited!\n");
    return undefined;
  }
  return CoreAPIWrapper.GetSDKVersion(sdk);
}

function GetApplicationPath() {
  return __self.appPath;
}

function LogMessage(category, message, level, verbose) {
  if (core_inited == false) {
    process.stdout.write("LogMessage: CoreAPI is not inited!\n");
    return false;
  }
  CoreAPIWrapper.LogMessage(category, message, level, verbose);
  return true;
}
module.exports = {
  InitAPI,
  DestroyAPI,
  IsCoreInited,
  DestroyCloud,
  CheckServiceStatus,
  CreateVideoWrapper,
  CreateAudioWrapper,
  CreateDataWrapper,
  LoadServices,
  StartDefaultServices,
  RefService,
  UnrefService,
  SubscribeToMessage,
  UnsubscribeToMessage,
  StartService,
  StopService,
  PublishMessage,
  ServiceNames,
  AddCallbackFunction,
  DefaultCallbacks,
  VideoFormat,
  VideoFormatString,
  AudioFormatString,
  AudioSampleSize,
  SetAppConfigurationFilePath,
  SetAppConfiguation,
  GetAppConfiguation,
  RefreshDevices,
  GetCameraList,
  GetSpeakerList,
  GetMicList,
  GetDeviceID,
  GetIPAddress,
  GetMACAddress,
  GetSDKVersion,
  GetApplicationPath,
  LogMessage,
  LogLevel
};
