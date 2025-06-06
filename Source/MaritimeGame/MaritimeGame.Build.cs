using UnrealBuildTool;

public class MaritimeGame : ModuleRules
{
    public MaritimeGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG","Slate",
    "SlateCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}