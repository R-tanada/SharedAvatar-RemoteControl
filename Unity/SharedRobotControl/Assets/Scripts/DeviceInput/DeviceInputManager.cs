// -----------------------------------------------------------------------
// Author:  Takayoshi Hagiwara (KMD)
// Created: 2021/4/29
// Summary: デバイス入力マネージャー．入力デバイスの制御，入力方法の制御．
// -----------------------------------------------------------------------

using UnityEngine;
using Zenject;

public class DeviceInputManager : MonoBehaviour
{
    [Inject]
    public IDeviceInputProvider deviceInputProvider;

    public enum InputMode
    {
        KEYINPUT,
        VIVE_CONTROLLER,
    }
    [Header("入力方法")]
    public InputMode inputMode;

    public enum InputInteraction
    {
        DEVICE_DIRECT,
        //GUI,
    }
    [Header("入力の渡し方")]
    public InputInteraction inputInteraction;

    private void Awake()
    {
        deviceInputProvider.Init();
    }
}
