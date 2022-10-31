let CoreAPI = require("./CoreAPI");

let __internalData = { cloudAlias: "Cloud/proxy" };

function InitSession(cloudAlias) {
  __internalData.cloudAlias = cloudAlias
}
//setup session related callbacks
function SetupSessionCallbacks(props) {
  //props:
  //CloudStatus
  //PeerJoining
  //SessionCreated
  //SessionReceived
  //PeerJoined
  //PeerDisconnected

  CoreAPI.SubscribeToMessage(
    "application",
    "cloud/status",
    (name, path, data) => {
      console.log("SetupSessionCallbacks::Cloud status is: " + data);
      if (props.CloudStatus !== undefined) props.CloudStatus(data);
    }
  );
  CoreAPI.SubscribeToMessage(
    "application",
    "cloud/peer/joining",
    (name, path, data) => {
      if (props.PeerJoining !== undefined) props.PeerJoining(data);
      this.setState({ cloud: "Peer is trying to join" });
    }
  );
  CoreAPI.SubscribeToMessage(
    "application",
    "cloud/session/created",
    (name, path, data) => {
      if (props.SessionCreated !== undefined) props.SessionCreated(data);
      console.log("Session was created");
    }
  );

  CoreAPI.SubscribeToMessage(
    "application",
    "cloud/session/closed",
    (name, path, data) => {
      if (props.SessionClosed !== undefined) props.SessionClosed(data);
      console.log("Session has been closed");
    }
  );
  CoreAPI.SubscribeToMessage(
    "application",
    "cloud/session/received",
    (name, path, data) => {
      if (props.SessionReceived !== undefined) props.SessionReceived(data);
      console.log("Session successfuly received");
    }
  );
  CoreAPI.SubscribeToMessage(
    "application",
    "cloud/peer/joined",
    (name, path, data) => {
      if (props.PeerJoined !== undefined) props.PeerJoined();
      console.log("Remote peer successfuly joined our session");
    }
  );

  CoreAPI.SubscribeToMessage(
    "application",
    "cloud/peer/disconnected",
    (name, path, data) => {
      if (props.PeerDisconnected !== undefined) props.PeerDisconnected(data);
      console.log("Remote peer disconnected");
    }
  );
}

//removes previously registered callbacks
function ClearSessionCallbacks() {
  CoreAPI.UnsubscribeToMessage("application", "cloud/status");
  CoreAPI.UnsubscribeToMessage("application", "cloud/peer/joining");
  CoreAPI.UnsubscribeToMessage("application", "cloud/session/created");
  CoreAPI.UnsubscribeToMessage("application", "cloud/session/closed");
  CoreAPI.UnsubscribeToMessage("application", "cloud/peer/joined");
  CoreAPI.UnsubscribeToMessage("application", "cloud/peer/disconnected");
  CoreAPI.UnsubscribeToMessage("application", "cloud/session/received");
}


//tell the cloud our peer ID so it can be registered
function SetPeerID(peerID) {
  console.log("Setting up peerID: " + peerID.toString());
  let peerIDObj = {
    peerID: peerID.toString(),
  };
  __internalData.localPeerID = peerID;
  CoreAPI.PublishMessage(__internalData.cloudAlias, "cloud/peer/set", JSON.stringify(peerIDObj));
  CoreAPI.PublishMessage(__internalData.cloudAlias, "cloud/status/get", "");
}


//connect to a remote peer provided their ID
function ConnectToPeer(remotePeerID) {
  console.log("Connect to peer: " + remotePeerID);
  let thisPeerID = __internalData.localPeerID;
  __internalData.remotePeerID = remotePeerID;
  if (remotePeerID !== undefined && thisPeerID != remotePeerID)
    CoreAPI.PublishMessage("", "cloud/joinpeer", remotePeerID.toString());
}
//disconnect from a remote peer provided their ID
function DisconnectFromPeer(remotePeerID) {
  CoreAPI.UnsubscribeToMessage("application", "cloud/peer/disconnected");
  let thisPeerID = __internalData.localPeerID;
  if (remotePeerID == 0) {
    remotePeerID = __internalData.remotePeerID;
  }
  if (remotePeerID !== undefined && thisPeerID != remotePeerID)
    CoreAPI.PublishMessage("", "cloud/disconnectpeer", remotePeerID.toString());
}

module.exports = {
  InitSession,
  SetupSessionCallbacks,
  ClearSessionCallbacks,
  SetPeerID,
  ConnectToPeer,
  DisconnectFromPeer,
};
