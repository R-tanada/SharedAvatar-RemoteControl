// -----------------------------------------------------------------------
// Author:  Takayoshi Hagiwara (KMD)
// Created: 2021/6/12
// Summary: CyberneticAvatarの動きの制御マネージャー
// -----------------------------------------------------------------------

using System.Text;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Valve.VR;

public class CyberneticAvatarMotionManager : SingletonMonoBehaviour<CyberneticAvatarMotionManager>
{
    // ---------- Managers ---------- //
    DeviceInputManager deviceInputManager;

    // ---------- Behaviour ---------- //
    CyberneticAvatarMotionBehaviour cyberneticAvatarMotionBehaviour = new CyberneticAvatarMotionBehaviour();


    // ---------- General Settings ---------- //
    [SerializeField, Header("General Settings"), Tooltip("Number of reader controller (0, 1, ...)")]
    private int participantNumber = 0;
    public int ParticipantNumber { get { return participantNumber; } }

    public enum SharingMethod
    {
        WEIGHT,         // 重み付け
        DIVISION_OF_ROLES,     // 役割分担
    }
    [Tooltip("共有方法")]
    public SharingMethod sharingMethod;

    [SerializeField, Tooltip("共有割合"), Range(0, 1)]
    private List<float> sharingWeights;
    public List<float> SharingWeights { get { return sharingWeights; } }

    [SerializeField, Tooltip("回転のみ反映させるかどうか")]
    private bool isRotationOnly = false;

    [SerializeField, Tooltip("相対座標を使用するかどうか")]
    private bool isRelativePosition = true;
    [SerializeField, Tooltip("相対回転を使用するかどうか")]
    private bool isRelativeRotation = false;

    // ---------- Avatar GameObjects Settings ---------- //
    [SerializeField, Header("Avatar GameObjects Settings"), Tooltip("Cybernetic AvatarのGameObjectを設定する")]
    private List<GameObject> cyberneticAvatarObjs = new List<GameObject>();
    public List<GameObject> CyberneticAvatarObjs
    {
        get { return cyberneticAvatarObjs; }
        set { cyberneticAvatarObjs = value; AverageWeightAdjustment(); }
    }
    [SerializeField, Tooltip("参加者のGameObjectを設定する")]
    private List<GameObject> participantObjs = new List<GameObject>();
    public List<GameObject> ParticipantObjs
    {
        get { return participantObjs; }
        set { participantObjs = value; AverageWeightAdjustment(); }
    }

    // ---------- Variables ---------- //
    private List<Vector3> relativePositions = new List<Vector3>();
    private float triggerValue;
    public float TriggerValue { get { return triggerValue; } }

    // ---------- Internal flags ---------- //
    private bool isSetOrigin;
    public bool IsSetOrigin { get { return isSetOrigin; } }
    private bool isStopRobotArm;
    public bool IsStopRobotArm { get { return isStopRobotArm; } }



    // #################################################### //
    // ###-------------------- Main --------------------### //
    // Start is called before the first frame update
    void Start()
    {
        Init();   
    }

    // Update is called once per frame
    void Update()
    {
        // ----- 実際にAvatarを動かす ----- //
        cyberneticAvatarMotionBehaviour.Animate(sharingMethod, CyberneticAvatarObjs, ParticipantObjs, sharingWeights, isRotationOnly, isRelativePosition, isRelativeRotation);

        // ----- 相対座標、相対回転の原点の設定 ----- //
        if (deviceInputManager.deviceInputProvider.IsPressSelectButton() && !isSetOrigin)
        {
            cyberneticAvatarMotionBehaviour.SetOriginPositions();
            cyberneticAvatarMotionBehaviour.SetInversedMatrix();
            isStopRobotArm = false;
            isSetOrigin = true;
        }

        if (deviceInputManager.deviceInputProvider.IsPressCancelButton())
        {
            isStopRobotArm = true;
            isSetOrigin = false;
        }


        // ----- トリガーの押し込み (即席) ----- //
        if (isSetOrigin)
        {
            float leftTrigger = SteamVR_Actions.cyberneticAvatar_TriggerValue.GetAxis(SteamVR_Input_Sources.LeftHand);
            float rightTrigger = SteamVR_Actions.cyberneticAvatar_TriggerValue.GetAxis(SteamVR_Input_Sources.RightHand);
            float fusedTriggerValue = cyberneticAvatarMotionBehaviour.SumOfTrigger(leftTrigger, rightTrigger, sharingWeights[0]);
            triggerValue = fusedTriggerValue;
        }

        // ----- デバッグ用 ----- //
        Vector3 relativeRot0 = cyberneticAvatarMotionBehaviour.QuaternionToEulerAngles(cyberneticAvatarMotionBehaviour.GetRelativeRotation()[0]);
        Vector3 relativeRot1 = cyberneticAvatarMotionBehaviour.QuaternionToEulerAngles(cyberneticAvatarMotionBehaviour.GetRelativeRotation()[1]);
        Vector3 relativeRot = cyberneticAvatarMotionBehaviour.QuaternionToEulerAngles(Quaternion.Lerp(cyberneticAvatarMotionBehaviour.GetRelativeRotation()[0], cyberneticAvatarMotionBehaviour.GetRelativeRotation()[1], 0.5f));
        StringBuilder sb = new StringBuilder();
        //Debug.Log(sb.Append("MyRot..... x > ").Append(relativeRot.x).Append(", y > ").Append(relativeRot.y).Append(", z > ").Append(relativeRot.z));

        Vector3 relativeRotUseUnityFunc = Quaternion.Lerp(cyberneticAvatarMotionBehaviour.GetRelativeRotation()[0], cyberneticAvatarMotionBehaviour.GetRelativeRotation()[1], 0.5f).eulerAngles;
        StringBuilder sbUniFunc = new StringBuilder();
        //Debug.Log(sbUniFunc.Append("UnityRot..... x > ").Append(relativeRotUseUnityFunc.x).Append(", y > ").Append(relativeRotUseUnityFunc.y).Append(", z > ").Append(relativeRotUseUnityFunc.z));

        StringBuilder sbPos = new StringBuilder();
        Vector3 caPos = CyberneticAvatarObjs[0].transform.localPosition * 1000;
        Debug.Log(sbPos.Append("Position [mm] >> x = ").Append(caPos.x).Append("   y = ").Append(caPos.y).Append("   z = ").Append(caPos.z));
    }



    // #################################################### //
    // ###---------- Methods ----------### //
    public void Init()
    {
        deviceInputManager = FindObjectOfType<DeviceInputManager>();

        cyberneticAvatarMotionBehaviour = new CyberneticAvatarMotionBehaviour();
        cyberneticAvatarMotionBehaviour.Init(ParticipantObjs);

        if (participantNumber > participantObjs.Count - 1)
            participantNumber = participantObjs.Count - 1;

        isStopRobotArm = false;
        isSetOrigin = false;

        AverageWeightAdjustment();
    }

    /// <summary>
    /// 参加者数に応じて，重み割合を平均して設定する．
    /// </summary>
    public void AverageWeightAdjustment()
    {
        sharingWeights = new List<float>();
        for (int i = 0; i < CyberneticAvatarObjs.Count; i++)
            sharingWeights.Add(1f / ParticipantObjs.Count);
    }
}
