var CoreAPI = require("./CoreAPI");

class CoreDataWrapper {
  constructor(resource, channel, onDataArrived) {
    this.avatarCoreDataWrapper = CoreAPI.CreateDataWrapper();

    this.resource = resource;
    this.channel = channel;
    this.onDataArrived = onDataArrived;
  }

  Destroy() {
    this.Stop();
    if (this.avatarCoreDataWrapper) {
      this.avatarCoreDataWrapper.destroy();
      this.avatarCoreDataWrapper = null;
    }
  }

  cb_onDataArrived(resource, channel, data) {
    if (this.onDataArrived !== undefined) {
      this.onDataArrived(resource, channel, data);
    }
  }

  // Other
  Start() {
    if (!this.avatarCoreDataWrapper) {
      this.avatarCoreDataWrapper = CoreAPI.CreateDataWrapper();
    }
    // console.log("Starting listining for: ", this.resource, ":", this.channel)
    this.avatarCoreDataWrapper.startStreaming(
      this.cb_onDataArrived.bind(this),
      this.resource,
      this.channel
    );
  }

  Stop() {
    if (this.avatarCoreDataWrapper) {
      this.avatarCoreDataWrapper.stopStreaming(function () { });
    }
  }

  SendData(message) {
    if (this.avatarCoreDataWrapper) {
      this.avatarCoreDataWrapper.sendData(this.resource, this.channel, message);
    }
  }
  SendDataToChannel(channel, message) {
    if (this.avatarCoreDataWrapper) {
      this.avatarCoreDataWrapper.sendData(this.resource, channel, message);
    }
  }
  SendDataToResourceChannel(resource, channel, message) {
    if (this.avatarCoreDataWrapper) {
      this.avatarCoreDataWrapper.sendData(resource, channel, message);
    }
  }
}

module.exports =
{
  CoreDataWrapper
}