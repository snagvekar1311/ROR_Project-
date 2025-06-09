#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Widgets/SWeakWidget.h"
#include "Vessel.h"
#include "PlayerVessel.generated.h"

// Forward declarations
class URadarComponent;
class SRadarWidget;
class SGameHUDWidget;

UENUM(BlueprintType)
enum class EVesselLength : uint8 {
    Under50m UMETA(DisplayName = "Under 50m"),
    Over50m  UMETA(DisplayName = "Over 50m")
};

USTRUCT(BlueprintType)
struct FManeuveringCharacteristics {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TurningRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Acceleration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Displacement;

    FManeuveringCharacteristics()
        : MaxSpeed(0.0f), TurningRate(0.0f), Acceleration(0.0f), Displacement(0.0f) {
    }

    FManeuveringCharacteristics(float InMaxSpeed, float InTurningRate, float InAcceleration, float InDisplacement)
        : MaxSpeed(InMaxSpeed), TurningRate(InTurningRate), Acceleration(InAcceleration), Displacement(InDisplacement) {
    }
};

UCLASS()
class MARITIMEGAME_API APlayerVessel : public AVessel {
    GENERATED_BODY()

public:
    APlayerVessel();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void SetTelegraph(float Value);
    void SetRudder(float Value);
    void AdjustBearing(float Value);
    void ToggleRadar();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vessel")
    EVesselType PlayerVesselType = EVesselType::Unknown;

    void CreateGameHUD();
    void RemoveGameHUD();

    // Getter for RadarComponent
    FORCEINLINE URadarComponent* GetRadarComponent() const { return RadarComponent; }

private:
    // HUD Widget
    TSharedPtr<SGameHUDWidget> GameHUDWidget;

    // Movement
    float TelegraphSetting;
    float RudderAngle;
    float Speed;
    float LastSpeed;
    float LastRudder;

    // Vessel length category
    EVesselLength VesselLength;

    // Maneuvering data
    UPROPERTY()
    FManeuveringCharacteristics Maneuvering;

    // Radar
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Radar", meta = (AllowPrivateAccess = "true"))
    URadarComponent* RadarComponent;

    // Slate widget for radar display
    TSharedPtr<SRadarWidget> RadarWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UUserWidget> RadarWidgetClass;
};