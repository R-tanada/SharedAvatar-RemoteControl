// -----------------------------------------------------------------------
// Author:  Takayoshi Hagiwara (KMD)
// Created: 2021/4/26
// Summary: キーボード入力用Provider
// -----------------------------------------------------------------------

using System.Collections.Generic;
using UnityEngine;

public class KeyInputProvider : IDeviceInputProvider
{
    List<KeyCode> keyCodes = new List<KeyCode>();
    float timeDurationAxis2d;
    float timeDurationButton;
    float longPressDetectTime = 1f;

    public void Init()
    {
        keyCodes.Add(KeyCode.RightArrow);
        keyCodes.Add(KeyCode.LeftArrow);
        keyCodes.Add(KeyCode.UpArrow);
        keyCodes.Add(KeyCode.DownArrow);

        timeDurationAxis2d = 0f;
        timeDurationButton = 0f;

        Debug.Log("Init Input Device Success > Keyboard");
    }

    // Axis2d
    public Vector2 Axis2d()
    {
        Vector2 axis = new Vector2();

        if (Input.GetKey(KeyCode.RightArrow)) axis.x += 1;
        if (Input.GetKey(KeyCode.LeftArrow)) axis.x -= 1;

        if (Input.GetKey(KeyCode.UpArrow)) axis.y += 1;
        if (Input.GetKey(KeyCode.DownArrow)) axis.y -= 1;

        return axis;
    }

    // Axis2dに対応するキーを押したかどうか (単発)
    public bool IsPressAxis2d()
    {
        bool isPress = false;

        foreach (KeyCode key in keyCodes)
            if (Input.GetKeyDown(key))
                isPress = true;

        return isPress;
    }

    // Axis2dに対応するキーを押したかどうか (長押し)
    public bool IsLongPressAxis2d()
    {
        bool isLongPress = false;

        foreach (KeyCode key in keyCodes)
            if (Input.GetKey(key))
                timeDurationAxis2d += Time.deltaTime;
            else
                timeDurationAxis2d = 0f;

        if (timeDurationAxis2d > longPressDetectTime)
            isLongPress = true;

        return isLongPress;
    }

    // キーボードインプットでは対応しない
    public bool IsTouchAxis2d()
    {
        return false;
    }

    // Button
    public bool IsPressSelectButton()
    {
        if (Input.GetKeyDown(KeyCode.Return))
            return true;
        else return false;
    }

    public bool IsLongPressSelectButton()
    {
        bool isLongPress = false;
        if (Input.GetKey(KeyCode.Return))
            timeDurationButton += Time.deltaTime;
        else
            timeDurationButton = 0f;

        if (timeDurationButton > longPressDetectTime)
            isLongPress = true;

        return isLongPress;
    }

    public bool IsUpSelectButton()
    {
        if (Input.GetKeyUp(KeyCode.Return))
            return true;
        else return false;
    }


    public bool IsPressCancelButton()
    {
        if (Input.GetKeyDown(KeyCode.C))
            return true;
        else return false;
    }

    public bool IsLongPressCancelButton()
    {
        bool isLongPress = false;
        if (Input.GetKey(KeyCode.C))
            timeDurationButton += Time.deltaTime;
        else
            timeDurationButton = 0f;

        if (timeDurationButton > longPressDetectTime)
            isLongPress = true;

        return isLongPress;
    }

    public float SelectButtonValue()
    {
        if (Input.GetKeyDown(KeyCode.Return))
            return 1;

        return 0;
    }
}
