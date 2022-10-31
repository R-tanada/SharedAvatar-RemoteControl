// -----------------------------------------------------------------------
// Author:  Takayoshi Hagiwara (KMD)
// Created: 2021/6/12
// Summary: UDP�ʐM�̊Ǘ�
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

    // 2021/6/19: Multicast��Receiving���Ȃ����ł��Ȃ��̂ŁA��U�G��Ȃ��悤�ɂ���B�����炩��single��Python�ɑ��邱�Ƃ͂ł���B
    private bool isMulticast = false;    // �}���`�L���X�g�ݒ�
    private bool isReceiving = false;    // �f�[�^���󂯎��\�ɂ��邩 (�N�����̂ݐݒ�\�A�r���ύX�s��)
    
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

        // �g���K�[���N���b�N���āA���΍��W���_��ݒ肵����Ƀ��[�v��������
        if (cyberneticAvatarMotionManager.IsSetOrigin && isStreaming)
        {
            udpSender.SendMultipleData("motionData", sendObjects, cyberneticAvatarMotionManager.TriggerValue, useEuler: false, isTriggerOnly: false);
            isResetRobotArm = false;
        }   

        // �G���[�Œ�~��A�T�C�h�{�^�����N���b�N�������1�x�����ʂ�
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
