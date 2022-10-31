using Zenject;

public class ViveControllerInputInstaller : Installer<ViveControllerInputInstaller>
{
    public override void InstallBindings()
    {
        Container
            .Bind<IDeviceInputProvider>()
            .To<ViveControllerInputProvider>()
            .AsSingle();
    }
}