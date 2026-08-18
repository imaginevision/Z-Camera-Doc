using UnrealBuildTool;

public class ZCAMFreeD : ModuleRules
{
    public ZCAMFreeD(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "LiveLink",
                "LiveLinkInterface",
                "InputCore"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "Sockets",
                "Networking"
            }
        );
    }
} 