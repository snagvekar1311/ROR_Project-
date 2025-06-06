#include "Vessel.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/SpotLightComponent.h"
#include "Components/PointLightComponent.h"

AVessel::AVessel()
{
    PrimaryActorTick.bCanEverTick = true;

    VesselMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VesselMesh"));
    RootComponent = VesselMesh;
    VesselMesh->SetSimulatePhysics(false);
    VesselMesh->SetMobility(EComponentMobility::Movable);
    VesselMesh->SetGenerateOverlapEvents(true);

    CollisionAvoidanceComponent = CreateDefaultSubobject<UCollisionAvoidanceComponent>(TEXT("CollisionAvoidanceComponent"));
}

void AVessel::BeginPlay()
{
    Super::BeginPlay();

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
            FString::Printf(TEXT("Vessel '%s' BeginPlay with CollisionAvoidanceComponent"), *GetName()));
    }

    // Setup lights based on Lights array
    for (const FLightConfig& Light : Lights)
    {
        float VisibilityInCm = Light.VisibilityRange * 185200.0f; // Convert nautical miles to cm (1 nm ≈ 185200 cm)

        ULightComponent* NewLight = nullptr;

        if (Light.ArcDegrees < 360.0f)
        {
            USpotLightComponent* SpotLight = NewObject<USpotLightComponent>(this);
            SpotLight->RegisterComponent();
            SpotLight->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            SpotLight->SetRelativeLocation(Light.RelativeLocation);
            SpotLight->SetRelativeRotation(Light.RelativeRotation);
            SpotLight->SetLightColor(Light.Color);
            SpotLight->SetAttenuationRadius(VisibilityInCm);
            SpotLight->SetOuterConeAngle(Light.ArcDegrees / 2.0f);
            SpotLight->SetIntensity(50000.0f);
            SpotLight->SetVisibility(true);
            NewLight = SpotLight;
        }
        else
        {
            UPointLightComponent* PointLight = NewObject<UPointLightComponent>(this);
            PointLight->RegisterComponent();
            PointLight->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            PointLight->SetRelativeLocation(Light.RelativeLocation);
            PointLight->SetLightColor(Light.Color);
            PointLight->SetAttenuationRadius(VisibilityInCm);
            PointLight->SetIntensity(20000.0f);
            PointLight->SetVisibility(true);
            NewLight = PointLight;
        }

        if (NewLight)
        {
            LightComponents.Add(NewLight);
        }
    }
}

void AVessel::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Speed > 0.0f)
    {
        FVector Move = GetActorForwardVector() * Speed * DeltaTime;
        AddActorWorldOffset(Move, true);
    }

    // Debug: visualize safe distance if avoidance component exists
    if (CollisionAvoidanceComponent)
    {
        DrawDebugSphere(GetWorld(), GetActorLocation(), CollisionAvoidanceComponent->SafeDistance, 32, FColor::Cyan, false, -1.f, 0, 2.0f);
    }
}

void AVessel::UpdateLights(bool bIsNight)
{
    for (ULightComponent* Light : LightComponents)
    {
        if (Light)
        {
            Light->SetVisibility(bIsNight);
        }
    }
}
