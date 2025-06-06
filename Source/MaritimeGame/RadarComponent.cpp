#include "RadarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerVessel.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

URadarComponent::URadarComponent() {
    PrimaryComponentTick.bCanEverTick = true;
    RadarRange = 1111200.0f; // ~6 NM in cm
    MarkedBearing = 0.0f;
}

void URadarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    Blips.Empty();

    if (GEngine) {
        GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("Radar Tick Running"));
    }

    APlayerVessel* PlayerVessel = Cast<APlayerVessel>(GetOwner());
    if (!PlayerVessel) return;

    FVector PlayerLocation = PlayerVessel->GetActorLocation();
    FVector PlayerForward = PlayerVessel->GetActorForwardVector();
    FVector PlayerRight = PlayerVessel->GetActorRightVector();

    TArray<AActor*> NearbyActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVessel::StaticClass(), NearbyActors);

    for (AActor* Actor : NearbyActors) {
        AVessel* OtherVessel = Cast<AVessel>(Actor);
        if (!OtherVessel || OtherVessel == PlayerVessel) continue;

        FVector ToTarget = OtherVessel->GetActorLocation() - PlayerLocation;
        float Distance = ToTarget.Size();

        if (Distance <= RadarRange) {
            FVector Direction = ToTarget.GetSafeNormal();

            float AngleRad = FMath::Acos(FVector::DotProduct(PlayerForward, Direction));
            if (FVector::DotProduct(PlayerRight, Direction) < 0) {
                AngleRad = -AngleRad;
            }

            float RelativeBearing = FMath::RadiansToDegrees(AngleRad);

            // Convert to radar screen coordinates
            float Radius = Distance / (RadarRange / 200.f); // 200 = radar radius in px
            float AngleRadUI = FMath::DegreesToRadians(RelativeBearing);
            FVector2D RadarPos(Radius * FMath::Sin(AngleRadUI), -Radius * FMath::Cos(AngleRadUI));

            FLinearColor DotColor = FLinearColor::White;
            EVesselType Type = OtherVessel->VesselType;

            switch (Type) {
            case EVesselType::PowerDriven:
                DotColor = FLinearColor::Green;
                break;
            case EVesselType::Sailing:
                DotColor = FLinearColor::Blue;
                break;
            case EVesselType::Fishing:
                DotColor = FLinearColor::Yellow;
                break;
            case EVesselType::Restricted:
                DotColor = FLinearColor::Red;
                break;
            default:
                DotColor = FLinearColor::Gray;
                break;
            }

            FRadarBlip Blip;
            Blip.Vessel = OtherVessel;
            Blip.Distance = Distance;
            Blip.RelativeBearing = RelativeBearing;
            Blip.RadarPosition = RadarPos;
            Blip.BlipColor = DotColor;

            Blips.Add(Blip);

            // Show debug message
            if (GEngine) {
                FString Msg = FString::Printf(TEXT("Blip: %s | %.0f cm | %.1f°"),
                    *OtherVessel->GetName(), Distance, RelativeBearing);
                GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, Msg);
            }
        }
    }
}

void URadarComponent::MarkBearing(float Bearing) {
    MarkedBearing = FMath::Clamp(Bearing, -180.0f, 180.0f);
}
