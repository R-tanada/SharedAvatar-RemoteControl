using UnityEngine;
using Zenject;

public class DeviceInputInstaller : MonoInstaller
{
    public DeviceInputManager deviceInputManager;

    public override void InstallBindings()
    {
        switch (deviceInputManager.inputMode)
        {
            case DeviceInputManager.InputMode.KEYINPUT:
                KeyInputInstaller.Install(Container);
                break;
            case DeviceInputManager.InputMode.VIVE_CONTROLLER:
                ViveControllerInputInstaller.Install(Container);
                break;
        }
    }
}