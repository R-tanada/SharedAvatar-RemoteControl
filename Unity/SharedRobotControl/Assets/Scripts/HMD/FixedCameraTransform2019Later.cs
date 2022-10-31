// --------------------------------------------------
// Author:      Takayoshi Hagiwara (Toyohashi University of Technology)
// Created:     2020/11/22
// Summary:     HMDを任意の場所に固定する
// Environment: Unity 2019 or Later
// Version:     1.0
// Issues:      None
// --------------------------------------------------

using UnityEngine;
using UnityEngine.XR;

public class FixedCameraTransform2019Later : MonoBehaviour
{
    [SerializeField, Tooltip("固定するポジション")]
    public Transform fixedTransform;
    [SerializeField, Tooltip("固定するポジションのオフセット")]
    public Vector3 offset;

    void Update()
    {
        Vector3 basePos = fixedTransform.position;

        Vector3 trackingPos;
        TryGetCenterEyePosition(out trackingPos);

        // 固定したい位置から hmd の位置を差し引いて実質 hmd の移動を無効化する
        transform.position = basePos - trackingPos;
        transform.position += offset;
    }

    bool TryGetCenterEyePosition(out Vector3 position)
    {
        InputDevice device = InputDevices.GetDeviceAtXRNode(XRNode.CenterEye);
        if (device.isValid)
        {
            if (device.TryGetFeatureValue(CommonUsages.centerEyePosition, out position))
                return true;
        }
        // This is the fail case, where there was no center eye was available.
        position = Vector3.zero;
        return false;
    }

    bool TryGetCenterEyeRotation(out Quaternion rotation)
    {
        InputDevice device = InputDevices.GetDeviceAtXRNode(XRNode.CenterEye);
        if (device.isValid)
        {
            if (device.TryGetFeatureValue(CommonUsages.centerEyeRotation, out rotation))
                return true;
        }
        // This is the fail case, where there was no center eye was available.
        rotation = Quaternion.identity;
        return false;
    }
}
