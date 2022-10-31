// -----------------------------------------------------------------------
// Author:  Takayoshi Hagiwara (KMD)
// Created: 2021/6/12
// Summary: CyberneticAvatar�̓����̐���}�l�[�W���[
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
        WEIGHT,         // �d�ݕt��
        DIVISION_OF_ROLES,     // �������S
    }
    [Tooltip("���L���@")]
    public SharingMethod sharingMethod;

    [SerializeField, Tooltip("���L����"), Range(0, 1)]
    private List<float> sharingWeights;
    public List<float> SharingWeights { get { return sharingWeights; } }

    [SerializeField, Tooltip("��]�̂ݔ��f�����邩�ǂ���")]
    private bool isRotationOnly = false;

    [SerializeField, Tooltip("���΍��W���g�p���邩�ǂ���")]
    private bool isRelativePosition = true;
    [SerializeField, Tooltip("���Ή�]���g�p���邩�ǂ���")]
    private bool isRelativeRotation = false;

    // ---------- Avatar GameObjects Settings ---------- //
    [SerializeField, Header("Avatar GameObjects Settings"), Tooltip("Cybernetic Avatar��GameObject��ݒ肷��")]
    private List<GameObject> cyberneticAvatarObjs = new List<GameObject>();
    public List<GameObject> CyberneticAvatarObjs
    {
        get { return cyberneticAvatarObjs; }
        set { cyberneticAvatarObjs = value; AverageWeightAdjustment(); }
    }
    [SerializeField, Tooltip("�Q���҂�GameObject��ݒ肷��")]
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
        // ----- ���ۂ�Avatar�𓮂��� ----- //
        cyberneticAvatarMotionBehaviour.Animate(sharingMethod, CyberneticAvatarObjs, ParticipantObjs, sharingWeights, isRotationOnly, isRelativePosition, isRelativeRotation);

        // ----- ���΍��W�A���Ή�]�̌��_�̐ݒ� ----- //
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


        // ----- �g���K�[�̉������� (����) ----- //
        if (isSetOrigin)
        {
            float leftTrigger = SteamVR_Actions.cyberneticAvatar_TriggerValue.GetAxis(SteamVR_Input_Sources.LeftHand);
            float rightTrigger = SteamVR_Actions.cyberneticAvatar_TriggerValue.GetAxis(SteamVR_Input_Sources.RightHand);
            float fusedTriggerValue = cyberneticAvatarMotionBehaviour.SumOfTrigger(leftTrigger, rightTrigger, sharingWeights[0]);
            triggerValue = fusedTriggerValue;
        }

        // ----- �f�o�b�O�p ----- //
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
    /// �Q���Ґ��ɉ����āC�d�݊����𕽋ς��Đݒ肷��D
    /// </summary>
    public void AverageWeightAdjustment()
    {
        sharingWeights = new List<float>();
        for (int i = 0; i < CyberneticAvatarObjs.Count; i++)
            sharingWeights.Add(1f / ParticipantObjs.Count);
    }
}
