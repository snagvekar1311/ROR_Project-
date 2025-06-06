#pragma once

#include "CoreMinimal.h"
#include "VesselTypes.h"
#include "GameFramework/Pawn.h"
#include "Components/SpotLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CollisionAvoidanceComponent.h"

// Include the VesselTypes enum here
#include "Vessel.generated.h"

USTRUCT(BlueprintType)
struct FLightConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lights")
    FColor Color = FColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lights")
    float VisibilityRange = 3.0f; // Nautical miles

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lights")
    float ArcDegrees = 360.0f; // 360 = all-round light

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lights")
    FVector RelativeLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lights")
    FRotator RelativeRotation = FRotator::ZeroRotator;
};

UCLASS()
class MARITIMEGAME_API AVessel : public APawn
{
    GENERATED_BODY()

public:
    AVessel();

    UFUNCTION(BlueprintCallable, Category = "Vessel")
    EVesselType GetVesselType() const { return VesselType; }


    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** Vessel type, used for logic and radar filtering */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vessel")
    EVesselType VesselType = EVesselType::Unknown;

    /** Speed in cm/s */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vessel")
    float Speed = 1000.0f;

    /** Light configurations for this vessel */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lights")
    TArray<FLightConfig> Lights;

    /** Update light visibility (e.g., night mode) */
    UFUNCTION(BlueprintCallable, Category = "Lights")
    void UpdateLights(bool bIsNight);

protected:
    /** Visual mesh of the vessel */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* VesselMesh;

    /** Runtime created light components */
    UPROPERTY()
    TArray<ULightComponent*> LightComponents;

    /** Collision avoidance logic component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCollisionAvoidanceComponent* CollisionAvoidanceComponent;
};
