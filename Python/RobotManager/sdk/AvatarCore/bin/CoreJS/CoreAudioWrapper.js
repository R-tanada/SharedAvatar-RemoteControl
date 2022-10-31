var CoreAPI = require("./CoreAPI");

class CoreAudioWrapper {
  constructor(resource, onFrameArrived) {
    this.resource = resource;
    this.onFrameArrived = onFrameArrived;
    this.avatarCoreAudioWrapper = CoreAPI.CreateAudioWrapper();
  }

  Destroy() {
    this.Stop();
    if (this.avatarCoreAudioWrapper !== null) {
      this.avatarCoreAudioWrapper.destroy();
      this.avatarCoreAudioWrapper = null; // This would help gc to regain memory and c++ object destructor}
    }
  }

  cb_onFrameArrived(resource, frame) {
    if (this.onFrameArrived !== undefined) {
      this.onFrameArrived(resource, frame);
    }
  }

  Start() {
    if (this.avatarCoreAudioWrapper === null) {
      this.avatarCoreAudioWrapper = CoreAPI.CreateAudioWrapper();
    }

    this.avatarCoreAudioWrapper.startStreaming(
      this.cb_onFrameArrived.bind(this),
      this.resource
    );
  }

  Stop() {
    if (this.avatarCoreAudioWrapper !== null) {
      this.avatarCoreAudioWrapper.stopStreaming(function () { });
    }
  }

  WriteFrame(channels, rate, length, format, data) {
    if (this.avatarCoreAudioWrapper !== null) {
      this.avatarCoreAudioWrapper.WriteAudioFrame(this.resource, channels, rate, length, format, data)
    }
  }
  WriteFrameToResource(resource, channels, rate, length, format, data) {
    if (this.avatarCoreAudioWrapper !== null) {
      this.avatarCoreAudioWrapper.WriteAudioFrame(resource, channels, rate, length, format, data)
    }
  }
}

module.exports =
{
  CoreAudioWrapper
}