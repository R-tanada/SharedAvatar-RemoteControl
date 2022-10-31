var CoreAPI = require("./CoreAPI");

class CoreVideoWrapper {
  constructor(resource, onFrameArrived) {
    this.resource = resource;
    this.onFrameArrived = onFrameArrived;
    this.avatarCoreVideoWrapper = CoreAPI.CreateVideoWrapper();
  }

  Destroy() {
    this.Stop();
    if (this.avatarCoreVideoWrapper !== null) {
      this.avatarCoreVideoWrapper.destroy();
      this.avatarCoreVideoWrapper = null; // This would help gc to regain memory and c++ object destructor}
    }
  }

  cb_onFrameArrived(resource, frame) {
    if (this.onFrameArrived !== undefined) {
      this.onFrameArrived(resource, frame);
    }
  }

  Start() {
    if (this.avatarCoreVideoWrapper === null) {
      this.avatarCoreVideoWrapper = CoreAPI.CreateVideoWrapper();
    }

    this.avatarCoreVideoWrapper.startStreaming(
      this.cb_onFrameArrived.bind(this),
      this.resource
    );
  }

  Stop() {
    if (this.avatarCoreVideoWrapper !== null) {
      this.avatarCoreVideoWrapper.stopStreaming(function () { });
    }
  }

  WriteFrame(width, height, format, videoData) {
    if (this.avatarCoreVideoWrapper !== null) {
      this.avatarCoreVideoWrapper.WriteVideoFrame(this.resource, width, height, format, videoData)
    }
  }

  WriteFrameToResource(resource, width, height, format, videoData) {
    if (this.avatarCoreVideoWrapper !== null) {
      this.avatarCoreVideoWrapper.WriteVideoFrame(resource, width, height, format, videoData)
    }
  }
}

module.exports =
{
  CoreVideoWrapper
}