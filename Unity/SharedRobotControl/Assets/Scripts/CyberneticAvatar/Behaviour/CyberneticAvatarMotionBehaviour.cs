// -----------------------------------------------------------------------
// Author:  Takayoshi Hagiwara (KMD)
// Created: 2021/6/12
// Summary: CyberneticAvatar�̓����̐���
// -----------------------------------------------------------------------

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;

public class CyberneticAvatarMotionBehaviour
{
    // ---------- Variables ---------- //
    private List<GameObject> participantObjs = new List<GameObject>();
    private List<Vector3> originPositions = new List<Vector3>();
    private List<Vector3> relativePositions = new List<Vector3>();

    private List<Matrix4x4> inversedMatrixes = new List<Matrix4x4>();
    private List<Quaternion> relativeRotations = new List<Quaternion>();

    private List<Vector3> eulerAngles = new List<Vector3>();
    


    // ###---------- Methods ----------### //
    public void Init(List<GameObject> participantObjects)
    {
        participantObjs = new List<GameObject>();
        participantObjs = participantObjects;

        eulerAngles = new List<Vector3>();

        SetZeroOriginPositions();
        SetZeroRelativePositions();

        SetInversedMatrix();
        SetZeroRelativeRotations();
    }


    public void Animate(CyberneticAvatarMotionManager.SharingMethod sharingMethod, List<GameObject> cyberneticAvatars, List<GameObject> participants, List<float> weights, bool isRotationOnly = false, bool isRelativePosition = true, bool isRelativeRotation = false)
    {
        List<Transform> participantsTransform = new List<Transform>();
        foreach (GameObject participant in participants)
            participantsTransform.Add(participant.transform);

        switch (sharingMethod)
        {
            case CyberneticAvatarMotionManager.SharingMethod.WEIGHT:
                for (int iCA = 0; iCA < cyberneticAvatars.Count; iCA++)
                    SumOfWeight(cyberneticAvatars[iCA].transform, participantsTransform, weights[iCA], isRotationOnly, isRelativePosition, isRelativeRotation);
                break;

            case CyberneticAvatarMotionManager.SharingMethod.DIVISION_OF_ROLES:
                break;
        }

        /*
        for (int i = 0; i < relativePositions.Count; i++)
            Debug.Log("Controller > " + i + "  Relative Pos > " + relativePositions[i]);
        */
    }

    /// <summary>
    /// CA�ɏd�ݕt�������^���𔽉f����B3�l�ȏ�͎����I�ɕ���
    /// </summary>
    /// <param name="ca">CyberneticAvatar</param>
    /// <param name="participants">Participants</param>
    /// <param name="weight">Weight</param>
    /// <param name="isRotationOnly">��]�݂̂̔��f�ɂ��邩</param>
    /// <param name="isRelativePosition">���錴�_����̑��΍��W�Ő��䂷�邩</param>
    public void SumOfWeight(Transform ca, List<Transform> participants, float weight, bool isRotationOnly = false, bool isRelativePosition = true, bool isRelativeRotation = false)
    {
        if (participants.Count < 2)
        {
            if (!isRotationOnly)
            {
                if(isRelativePosition)
                    ca.localPosition = GetRelativePositions()[0];
                else
                    ca.localPosition = participants[0].localPosition;
            }

            if (isRelativeRotation)
                ca.localRotation = GetRelativeRotation()[0];
            else
                ca.localRotation = participants[0].localRotation;
        }
        if (participants.Count == 2)
        {
            if (!isRotationOnly)
            {
                if(isRelativePosition)
                    ca.localPosition = GetRelativePositions()[0] * weight + GetRelativePositions()[1] * (1 - weight);
                else
                    ca.localPosition = participants[0].localPosition * weight + participants[1].localPosition * (1 - weight);
            }

            if (isRelativeRotation)
                ca.localRotation = Quaternion.Lerp(GetRelativeRotation()[0], GetRelativeRotation()[1], 1 - weight);
            else
                ca.localRotation = Quaternion.Lerp(participants[0].localRotation, participants[1].localRotation, 1 - weight);
        }
        else if (participants.Count > 2)
        {
            float averageWeight = 1f / participants.Count;

            // Position
            if (!isRotationOnly)
            {
                Vector3 sumPos = new Vector3();
                if (isRelativePosition)
                    for (int i = 0; i < participants.Count; i++)
                        sumPos += GetRelativePositions()[i] * averageWeight;
                else
                    foreach (Transform participant in participants)
                        sumPos += participant.localPosition * averageWeight;

                ca.localPosition = sumPos;
            }

            // Rotation
            List<Quaternion> rot = new List<Quaternion>();
            if(isRelativeRotation)
                for(int i = 0; i < participantObjs.Count; i++)
                    rot.Add(GetRelativeRotation()[i]);
            else
                foreach (Transform participant in participants)
                    rot.Add(participant.localRotation);

            Quaternion resutlRot = new Quaternion();
            Vector4 avgr = Vector4.zero;
            foreach (Quaternion singleRotation in rot)
                resutlRot = Math3d.AverageQuaternion(ref avgr, singleRotation, rot[0], rot.Count);

            ca.localRotation = resutlRot;
        }

        for (int iChild = 0; iChild < ca.childCount; iChild++)
        {
            List<Transform> participantsTransformChild = new List<Transform>();
            foreach (Transform participant in participants)
                participantsTransformChild.Add(participant.GetChild(iChild));

            SumOfWeight(ca.GetChild(iChild), participantsTransformChild, weight);
        }
    }

    /// <summary>
    /// ���΍��W�̌��_�ƂȂ���W���L�^����B�A�o�^�[�ȂǊK�w�\���ɂȂ��Ă���I�u�W�F�N�g�͑z�肵�Ă��Ȃ�
    /// </summary>
    public void SetOriginPositions()
    {
        for (int i = 0; i < participantObjs.Count; i++)
            originPositions[i] = participantObjs[i].transform.localPosition;
    }

    /// <summary>
    /// ���΍��W�̌��_���[���ŏ���������
    /// </summary>
    public void SetZeroOriginPositions()
    {
        originPositions = new List<Vector3>();
        for (int i = 0; i < participantObjs.Count; i++)
            originPositions.Add(new Vector3(0, 0, 0));
    }

    /// <summary>
    /// ���΍��W���X�g�̃[��������
    /// </summary>
    public void SetZeroRelativePositions()
    {
        relativePositions = new List<Vector3>();
        for (int i = 0; i < participantObjs.Count; i++)
            relativePositions.Add(new Vector3(0, 0, 0));
    }

    /// <summary>
    /// ���΍��W���v�Z����B�A�o�^�[�ȂǊK�w�\���ɂȂ��Ă���I�u�W�F�N�g�͑z�肵�Ă��Ȃ�
    /// </summary>
    /// <returns>���΍��W���X�g</Vector3></returns>
    public List<Vector3> GetRelativePositions()
    {
        for (int i = 0; i < relativePositions.Count; i++)
            relativePositions[i] = participantObjs[i].transform.localPosition - originPositions[i];

        return relativePositions;
    }

    /// <summary>
    /// �g���K�[���������l��C�ӂ̊����ŗZ������
    /// </summary>
    /// <param name="trigger1"></param>
    /// <param name="trigger2"></param>
    /// <param name="weight"></param>
    /// <returns></returns>
    public float SumOfTrigger(float trigger1, float trigger2, float weight)
    {
        return trigger1 * weight + trigger2 * (1 - weight);
    }

    /// <summary>
    /// ���Ή�]�̌v�Z���s�����߂̕ϊ��s���ݒ肷��
    /// </summary>
    public void SetInversedMatrix()
    {
        inversedMatrixes = new List<Matrix4x4>();

        for(int i = 0; i < participantObjs.Count; i++)
        {
            Quaternion q = participantObjs[i].transform.localRotation;

            // xArm�p�ɍ��W�ϊ�
            Quaternion armRot = new Quaternion(q.y, q.z, q.x, q.w);

            Matrix4x4 mat4x4 = new Matrix4x4();
            mat4x4[0, 0] = armRot.w;
            mat4x4[0, 1] = -armRot.y;
            mat4x4[0, 2] = armRot.x;
            mat4x4[0, 3] = armRot.z;

            mat4x4[1, 0] = armRot.y;
            mat4x4[1, 1] = armRot.w;
            mat4x4[1, 2] = -armRot.z;
            mat4x4[1, 3] = armRot.x;

            mat4x4[2, 0] = -armRot.x;
            mat4x4[2, 1] = armRot.z;
            mat4x4[2, 2] = armRot.w;
            mat4x4[2, 3] = armRot.y;

            mat4x4[3, 0] = -armRot.z;
            mat4x4[3, 1] = -armRot.x;
            mat4x4[3, 2] = -armRot.y;
            mat4x4[3, 3] = armRot.w;

            inversedMatrixes.Add(mat4x4.inverse);
        }
    }

    /// <summary>
    /// ���Ή�]���i�[���郊�X�g�̏�����
    /// </summary>
    public void SetZeroRelativeRotations()
    {
        relativeRotations = new List<Quaternion>();
        for (int i = 0; i < participantObjs.Count; i++)
            relativeRotations.Add(Quaternion.identity);
    }

    /// <summary>
    /// �ϊ��s����g�p���āA���Ή�]�����߂�
    /// </summary>
    /// <returns>���Ή�]</returns>
    /// <CAUTION>�����</CAUTION>
    public List<Quaternion> GetRelativeRotation()
    {
        for(int i = 0; i < relativeRotations.Count; i++)
        {
            Quaternion participantRotation = participantObjs[i].transform.localRotation;
            //Vector4 relativeRot = inversedMatrixes[i].transpose * new Vector4(participantRotation.x, participantRotation.y, participantRotation.z, participantRotation.w);
            Vector4 relativeRot = inversedMatrixes[i] * new Vector4(participantRotation.x, participantRotation.y, participantRotation.z, participantRotation.w);
            relativeRotations[i] = new Quaternion(relativeRot.x, relativeRot.y, relativeRot.z, relativeRot.w);
        }
        return relativeRotations;
    }


    /// <summary>
    /// Quaternion��EulerAngles�ɕϊ�����
    /// </summary>
    /// <param name="q">�ϊ�����Quaternion</param>
    /// <param name="useDegree">Degree���g�p����Dfalse�̏ꍇRad���g�p�����D</param>
    /// <returns></returns>
    public Vector3 QuaternionToEulerAngles(Quaternion q, bool useDegree = true)
    {
        /*
        ��]�s��
        | m00 m01 m02 0 |
        | m10 m11 m12 0 |
        | m20 m21 m22 0 |
        | 0   0   0  1 |
       */

        // 1 - 2y^2 - 2z^2
        float m00 = 1 - (2 * Mathf.Pow(q.y, 2)) - (2 * Mathf.Pow(q.z, 2));
        // 2xy + 2wz
        float m01 = (2 * q.x * q.y) + (2 * q.w * q.z);
        // 2xz - 2wy
        float m02 = (2 * q.x * q.z) - (2 * q.w * q.y);
        // 2xy - 2wz
        float m10 = (2 * q.x * q.y) - (2 * q.w * q.z);
        // 1 - 2x^2 - 2z^2
        float m11 = 1 - (2 * Mathf.Pow(q.x, 2)) - (2 * Mathf.Pow(q.z, 2));
        // 2yz + 2wx
        float m12 = (2 * q.y * q.z) + (2 * q.w * q.x);
        // 2xz + 2wy
        float m20 = (2 * q.x * q.z) + (2 * q.w * q.y);
        // 2yz - 2wx
        float m21 = (2 * q.y * q.z) - (2 * q.w * q.x);
        // 1 - 2x^2 - 2y^2
        float m22 = 1 - (2 * Mathf.Pow(q.x, 2)) - (2 * Mathf.Pow(q.y, 2));

        // ��]���̏��Ԃ�X->Y->Z�̃I�C���[�p(Rx*Ry*Rz)
        float tx, ty, tz;
        if(m02 == 1)
        {
            tx = Mathf.Atan2(m10, m11);
            ty = Mathf.PI / 2;
            tz = 0;
        }
        else if (m02 == -1)
        {
            tx = Mathf.Atan2(m21, m20);
            ty = -Mathf.PI / 2;
            tz = 0;
        }
        else
        {
            tx = -Mathf.Atan2(-m12, m22);
            ty = -Mathf.Asin(m02);
            tz = -Mathf.Atan2(-m01, m00);
        }

        if(useDegree)
        {
            tx *= Mathf.Rad2Deg;
            ty *= Mathf.Rad2Deg;
            tz *= Mathf.Rad2Deg;
        }

        return new Vector3(tx, ty, tz);
    }
}
