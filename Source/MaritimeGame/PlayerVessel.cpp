#include "PlayerVessel.h"
#include "RadarComponent.h"
#include "RadarWidget.h"
#include "GameHUDWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

APlayerVessel::APlayerVessel() {
    PrimaryActorTick.bCanEverTick = true;

    TelegraphSetting = 0.0f;
    RudderAngle = 0.0f;
    Speed = 0.0f;
    LastSpeed = 0.0f;
    LastRudder = 0.0f;

    RadarComponent = CreateDefaultSubobject<URadarComponent>(TEXT("RadarComponent"));
}

void APlayerVessel::BeginPlay()
{
    Super::BeginPlay();

    switch (PlayerVesselType) {
    case EVesselType::Container:
        Maneuvering = FManeuveringCharacteristics(4620.0f, 50.0f, 1.0f, 60000.0f);
        break;
    case EVesselType::Bulker:
        Maneuvering = FManeuveringCharacteristics(2772.0f, 30.0f, 0.8f, 80000.0f);
        break;
    case EVesselType::Tanker:
        Maneuvering = FManeuveringCharacteristics(3148.4f, 25.0f, 0.7f, 100000.0f);
        break;
    case EVesselType::LNG_LPG:
        Maneuvering = FManeuveringCharacteristics(3518.8f, 40.0f, 0.9f, 70000.0f);
        break;
    default:
        Maneuvering = FManeuveringCharacteristics(3000.0f, 40.0f, 0.8f, 70000.0f);
        break;
    }

    VesselType = EVesselType::PowerDriven;
    VesselLength = EVesselLength::Over50m;
    CreateGameHUD();
}

void APlayerVessel::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Only bind the bearing adjustment and radar toggle inputs
    PlayerInputComponent->BindAxis("AdjustBearing", this, &APlayerVessel::AdjustBearing);
    PlayerInputComponent->BindAction("ToggleRadar", IE_Pressed, this, &APlayerVessel::ToggleRadar);
}

void APlayerVessel::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    float TargetSpeed = TelegraphSetting * Maneuvering.MaxSpeed;
    float InterpSpeed = Maneuvering.Acceleration / FMath::Max(Maneuvering.MaxSpeed, 1.0f);
    Speed = FMath::FInterpTo(Speed, TargetSpeed, DeltaTime, InterpSpeed);

    float TurnRate = (RudderAngle / 35.0f) * Maneuvering.TurningRate;
    AddActorWorldRotation(FRotator(0.0f, TurnRate * DeltaTime, 0.0f));
    AddActorWorldOffset(GetActorForwardVector() * Speed * DeltaTime, true);

    FString SpeedTrend = FMath::IsNearlyEqual(Speed, LastSpeed, 0.01f)
        ? TEXT("Stable")
        : (Speed > LastSpeed ? TEXT("Increasing") : TEXT("Decreasing"));

    FString DebugMessage = FString::Printf(TEXT("Telegraph: %.2f | Rudder: %.2f°\nSpeed: %.2f knots (%s)"),
        TelegraphSetting, RudderAngle, Speed, *SpeedTrend);

    if (GEngine) {
        GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Green, DebugMessage);
    }

    LastSpeed = Speed;
    LastRudder = RudderAngle;
}

void APlayerVessel::SetTelegraph(float Value) {
    float OldValue = TelegraphSetting;
    TelegraphSetting = FMath::Clamp(Value, 0.0f, 1.0f);

    if (GEngine && !FMath::IsNearlyEqual(OldValue, TelegraphSetting, 0.01f)) {
        FString Msg = FString::Printf(TEXT("Telegraph set to %.2f"), TelegraphSetting);
        GEngine->AddOnScreenDebugMessage(2, 1.5f, FColor::Yellow, Msg);
    }
}

void APlayerVessel::SetRudder(float Value) {
    float OldRudder = RudderAngle;
    RudderAngle = FMath::Clamp(Value * 35.0f, -35.0f, 35.0f);

    if (GEngine && !FMath::IsNearlyEqual(OldRudder, RudderAngle, 0.5f)) {
        FString Msg = FString::Printf(TEXT("Rudder set to %.2f°"), RudderAngle);
        GEngine->AddOnScreenDebugMessage(3, 1.5f, FColor::Cyan, Msg);
    }
}

void APlayerVessel::AdjustBearing(float Value) {
    if (RadarComponent && FMath::Abs(Value) > KINDA_SMALL_NUMBER) {
        float NewBearing = RadarComponent->GetMarkedBearing() + Value * 10.0f;
        RadarComponent->MarkBearing(NewBearing);
        UE_LOG(LogTemp, Log, TEXT("AdjustBearing input detected: Value=%.2f | New MarkedBearing: %.2f°"), Value, NewBearing);
    }
}

void APlayerVessel::ToggleRadar()
{
    if (!RadarComponent)
        return;

    if (RadarWidget.IsValid() && GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->RemoveViewportWidgetContent(RadarWidget.ToSharedRef());
        RadarWidget.Reset();
    }
    else
    {
        // Create the Slate widget
        RadarWidget = SNew(SRadarWidget)
            .RadarComponent(RadarComponent);

        // Add it to the viewport
        if (GEngine && GEngine->GameViewport)
        {
            GEngine->GameViewport->AddViewportWidgetContent(
                SNew(SWeakWidget)
                .PossiblyNullContent(RadarWidget),
                0
            );
        }
    }
}

void APlayerVessel::CreateGameHUD()
{
    if (GameHUDWidget.IsValid() || !GEngine || !GEngine->GameViewport) return;

    GameHUDWidget = SNew(SGameHUDWidget)
        .PlayerVessel(this);

    GEngine->GameViewport->AddViewportWidgetContent(
        SNew(SWeakWidget)
        .PossiblyNullContent(GameHUDWidget),
        0
    );
}

void APlayerVessel::RemoveGameHUD()
{
    if (!GameHUDWidget.IsValid() || !GEngine || !GEngine->GameViewport) return;

    GEngine->GameViewport->RemoveViewportWidgetContent(GameHUDWidget.ToSharedRef());
    GameHUDWidget.Reset();
}