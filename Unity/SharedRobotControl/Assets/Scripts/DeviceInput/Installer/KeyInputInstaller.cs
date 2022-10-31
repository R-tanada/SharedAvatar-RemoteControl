using Zenject;

public class KeyInputInstaller : Installer<KeyInputInstaller>
{
    public override void InstallBindings()
    {
        Container
            .Bind<IDeviceInputProvider>()
            .To<KeyInputProvider>()
            .AsSingle();
    }
}