// -----------------------------------------------------------------------
// Author:  Takayoshi Hagiwara (KMD)
// Created: 2021/4/26
// Summary: Vive controller入力用Provider
// -----------------------------------------------------------------------

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Valve.VR;


// -------------------- 注意 -------------------- //
// 2021/4/26
// 未完成．形だけ作成
// -------------------- 注意 -------------------- //

public class ViveControllerInputProvider : IDeviceInputProvider
{
    SteamVR_ActionSet[] actionSets;
    private SteamVR_Input_Sources handType;
    private string actionSetName = "cyberneticAvatar";

    float longPressTimeDurationAxis2d;
    float longPressTimeDurationSelectButton;
    float longPressDetectTime = 1f;

    public void Init()
    {
        // 入力を受け取るコントローラーの設定
        handType = SteamVR_Input_Sources.Any;

        // 登録されているactionSetを検索し、actionSetNameと一致する名前のactionSetをアクティブにする
        actionSets = SteamVR_Input.actionSets;
        foreach (SteamVR_ActionSet actionSet in actionSets)
            if (actionSet.GetShortName() == actionSetName)
                actionSet.Activate(handType);

        longPressTimeDurationAxis2d = 0f;
        longPressTimeDurationSelectButton = 0f;

        Debug.Log("Init Input Device Success > Vive controller");
    }

    // Axis2d
    public Vector2 Axis2d()
    {
        return SteamVR_Actions.cyberneticAvatar_Axis2d.GetAxis(handType);
    }

    // Axis2dに対応するキーを押したかどうか (単発)
    public bool IsPressAxis2d()
    {
        return SteamVR_Actions.cyberneticAvatar_IsPressAxis2d.GetStateDown(handType);
    }

    // Axis2dに対応するキーを押したかどうか (長押し)
    public bool IsLongPressAxis2d()
    {
        bool isLongPress = false;
        if (SteamVR_Actions.cyberneticAvatar_IsPressAxis2d.GetState(handType))
            longPressTimeDurationAxis2d += Time.deltaTime;
        else
            longPressTimeDurationAxis2d = 0f;

        if (longPressTimeDurationAxis2d > longPressDetectTime)
            isLongPress = true;

        return isLongPress;
    }

    // タッチパッドを触っているかどうか
    public bool IsTouchAxis2d()
    {
        return SteamVR_Actions.cyberneticAvatar_IsTouchAxis2d.GetState(handType);
    }

    // Button
    public bool IsPressSelectButton()
    {
        return SteamVR_Actions.cyberneticAvatar_IsPressSelectButton.GetStateDown(handType);
    }

    public bool IsLongPressSelectButton()
    {
        bool isLongPress = false;
        if (SteamVR_Actions.cyberneticAvatar_IsPressSelectButton.GetState(handType))
            longPressTimeDurationSelectButton += Time.deltaTime;
        else
            longPressTimeDurationSelectButton = 0f;

        if (longPressTimeDurationSelectButton > longPressDetectTime)
            isLongPress = true;

        return isLongPress;
    }

    public bool IsUpSelectButton()
    {
        return SteamVR_Actions.cyberneticAvatar_IsPressSelectButton.GetStateUp(handType);
    }

    public bool IsPressCancelButton()
    {
        return SteamVR_Actions.cyberneticAvatar_IsPressCancelButton.GetStateDown(handType);
    }

    public bool IsLongPressCancelButton()
    {
        // これだけシステムの閾値。0.75??　内部値がわからない。SteamVRはくそ
        return SteamVR_Actions.cyberneticAvatar_IsLongPressCancelButton.GetState(handType);
    }

    public float SelectButtonValue()
    {
        return SteamVR_Actions.cyberneticAvatar_TriggerValue.GetAxis(handType);
    }
}
