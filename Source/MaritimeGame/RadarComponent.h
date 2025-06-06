#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Vessel.h"
#include "RadarComponent.generated.h"

USTRUCT(BlueprintType)
struct FRadarBlip {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    AVessel* Vessel;

    UPROPERTY(BlueprintReadOnly)
    float Distance; // in cm

    UPROPERTY(BlueprintReadOnly)
    float RelativeBearing; // in degrees (-180 to 180)

    UPROPERTY(BlueprintReadOnly)
    FVector2D RadarPosition;

    UPROPERTY(BlueprintReadOnly)
    FLinearColor BlipColor;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MARITIMEGAME_API URadarComponent : public UActorComponent {
    GENERATED_BODY()

public:
    URadarComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
    float RadarRange;

    UPROPERTY(BlueprintReadOnly, Category = "Radar")
    TArray<FRadarBlip> Blips;

    UPROPERTY(BlueprintReadOnly, Category = "Radar")
    float MarkedBearing;

    UFUNCTION(BlueprintCallable)
    void MarkBearing(float Bearing);

    float GetMarkedBearing() const { return MarkedBearing; }

    UFUNCTION(BlueprintCallable)
    TArray<FRadarBlip> GetRadarBlips() const { return Blips; }
};
