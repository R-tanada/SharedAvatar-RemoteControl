// -----------------------------------------------------------------------
// Author:  Takayoshi Hagiwara (KMD)
// Created: 2021/4/26
// Summary: デバイス入力制御用インタフェース
// -----------------------------------------------------------------------

using UnityEngine;

public interface IDeviceInputProvider
{
    void Init();

    // Axis2d (as Touchpad, Joystick...)
    Vector2 Axis2d();
    bool IsPressAxis2d();
    bool IsLongPressAxis2d();
    bool IsTouchAxis2d();
    //float PressedPowerAxis2d();

    // Button
    bool IsPressSelectButton();
    bool IsLongPressSelectButton();
    bool IsUpSelectButton();
    bool IsPressCancelButton();
    bool IsLongPressCancelButton();

    // Button (float)
    float SelectButtonValue();
}
