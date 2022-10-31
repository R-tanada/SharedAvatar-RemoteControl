// -----------------------------------------------------------------------
// Author:  Takayoshi Hagiwara (KMD)
// Created: 2021/6/12
// Summary: UDP通信の管理
// -----------------------------------------------------------------------

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UDPManager : SingletonMonoBehaviour<UDPManager>
{
    // ---------- Managers ---------- //
    CyberneticAvatarMotionManager cyberneticAvatarMotionManager;

    // ---------- General Settings ---------- //
    public string ipAddress     = "127.0.0.1";
    private string multiIPAddr  = "239.255.0.1";

    public int sendPort     = 8000;
    private int recvPort    = 8001;

    // 2021/6/19: MulticastとReceivingがなぜかできないので、一旦触らないようにする。こちらからsingleでPythonに送ることはできる。
    private bool isMulticast = false;    // マルチキャスト設定
    private bool isReceiving = false;    // データを受け取り可能にするか (起動時のみ設定可能、途中変更不可)
    
    [SerializeField]
    private bool isStreaming = false;

    UDPSender udpSender     = new UDPSender();
    UDPReceiver udpReceiver = new UDPReceiver();

    public List<GameObject> sendObjects;

    // ---------- Internal flags ---------- //
    private bool isResetRobotArm;
    private bool isSetOrigin;

    // Start is called before the first frame update
    void Start()
    {
        Init();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
            udpSender.SendText("quit");

        if (cyberneticAvatarMotionManager.IsSetOrigin && !isSetOrigin)
        {
            udpSender.SendMultipleData("setorigin", sendObjects, cyberneticAvatarMotionManager.TriggerValue, useEuler: false);
            isSetOrigin = true;
        } 

        // トリガーをクリックして、相対座標原点を設定した後にループし続ける
        if (cyberneticAvatarMotionManager.IsSetOrigin && isStreaming)
        {
            udpSender.SendMultipleData("motionData", sendObjects, cyberneticAvatarMotionManager.TriggerValue, useEuler: false, isTriggerOnly: false);
            isResetRobotArm = false;
        }   

        // エラーで停止後、サイドボタンをクリックした後に1度だけ通る
        if(!isResetRobotArm && cyberneticAvatarMotionManager.IsStopRobotArm)
        {
            udpSender.SendText("reset");
            isResetRobotArm = true;
            isSetOrigin = false;
        }
    }

    private void Init()
    {
        cyberneticAvatarMotionManager = FindObjectOfType<CyberneticAvatarMotionManager>();

        if (isMulticast)
            ipAddress = multiIPAddr;

        udpSender.Connect(ipAddress, sendPort, isMulticast);

        if (isReceiving)
            udpReceiver.Connect(ipAddress, recvPort, isMulticast);

        isResetRobotArm = false;
        isSetOrigin = false;
    }

    private void OnApplicationQuit()
    {
        udpSender.SendText("quit");
        udpSender.Disconnect();
        udpReceiver.Disconnect();
    }
}
