using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Valve.VR;
using System.Windows;

public class testMan : MonoBehaviour
{
    //public DeviceInputManager deviceInputManager;
    public GameObject targetObj;
    public Quaternion originalAngle;
    public Quaternion relativeAngle;

    bool isDebug;

    // Start is called before the first frame update
    void Start()
    {
        isDebug = false;

        InversedMatrix();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.A))
        {
            originalAngle = targetObj.transform.localRotation;
            isDebug = true;
        }

        if(isDebug)
        {
            relativeAngle = Quaternion.FromToRotation(originalAngle.eulerAngles, targetObj.transform.localRotation.eulerAngles);
            Debug.Log(relativeAngle.eulerAngles);
        }
            

        //Debug.Log(deviceInputManager.deviceInputProvider.IsPressSelectButton());
    }

    public void InversedMatrix()
    {
        Quaternion q = new Quaternion(0.1f, 0.2f, 0.4f, 1);
        Quaternion armRot = new Quaternion(q.y, q.z, q.x, q.w);
        Debug.Log("armRot > " + armRot);

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

        Debug.Log(mat4x4);
        Debug.Log(mat4x4.inverse);

        Quaternion myRotation = new Quaternion(0, 1, 0, 1);

        Vector4 relativeRot = mat4x4.inverse.transpose * new Vector4(myRotation.x, myRotation.y, myRotation.z, myRotation.w);
        Debug.Log("x: " + relativeRot.x + " y: " + relativeRot.y + " z: " + relativeRot.z + " w: " + relativeRot.w);
    }
}
