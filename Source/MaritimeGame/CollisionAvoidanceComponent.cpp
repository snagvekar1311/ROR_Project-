#include "CollisionAvoidanceComponent.h"
#include "Vessel.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

UCollisionAvoidanceComponent::UCollisionAvoidanceComponent() {
    PrimaryComponentTick.bCanEverTick = true;
    SafeDistance = 370400.0f;     // 2 nm in cm
    CriticalDistance = 92600.0f;  // 0.5 nm in cm
    bCollisionRiskDetected = false;
}

void UCollisionAvoidanceComponent::BeginPlay() {
    Super::BeginPlay();
}

void UCollisionAvoidanceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    EvaluateCollisionRisk();
}

bool UCollisionAvoidanceComponent::IsCollisionRiskDetected() {
    return bCollisionRiskDetected;
}

void UCollisionAvoidanceComponent::EvaluateCollisionRisk() {
    bCollisionRiskDetected = false;
    AVessel* OwnerVessel = Cast<AVessel>(GetOwner());
    if (!OwnerVessel) return;

    TArray<AActor*> NearbyActors;
    GetOwner()->GetOverlappingActors(NearbyActors, AVessel::StaticClass());

    for (AActor* Actor : NearbyActors) {
        AVessel* OtherVessel = Cast<AVessel>(Actor);
        if (!OtherVessel || OtherVessel == OwnerVessel) continue;

        FVector ToOther = OtherVessel->GetActorLocation() - OwnerVessel->GetActorLocation();
        float Distance = ToOther.Size();
        FVector RelativeVelocity = OtherVessel->GetVelocity() - OwnerVessel->GetVelocity();
        FVector BearingVector = ToOther.GetSafeNormal();
        float BearingChange = FMath::Acos(FVector::DotProduct(RelativeVelocity.GetSafeNormal(), BearingVector));

        if (Distance < SafeDistance && FMath::Abs(BearingChange) < FMath::DegreesToRadians(5.0f)) {
            bCollisionRiskDetected = true;
            FString Situation;

            FString Msg = FString::Printf(TEXT("⚠️ COLLISION RISK DETECTED with %s (%.0f m)"),
                *OtherVessel->GetName(), Distance / 100.0f);
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Msg);

            if (IsGiveWayVessel(OtherVessel, Situation)) {
                Msg = FString::Printf(TEXT("You are the GIVE-WAY vessel (%s). Maneuvering."), *Situation);
                if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, Msg);
                ExecuteGiveWayManeuver(OtherVessel, Situation);
            }
            else {
                Msg = FString::Printf(TEXT("You are the STAND-ON vessel (%s). Holding course."), *Situation);
                if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Msg);
                ExecuteStandOnManeuver(OtherVessel);
            }
        }
    }
}

bool UCollisionAvoidanceComponent::IsGiveWayVessel(AVessel* OtherVessel, FString& Situation) {
    AVessel* OwnerVessel = Cast<AVessel>(GetOwner());
    Situation = "None";

    FVector ToOther = OtherVessel->GetActorLocation() - OwnerVessel->GetActorLocation();
    FVector Forward = OwnerVessel->GetActorForwardVector();
    float Angle = FMath::Acos(FVector::DotProduct(ToOther.GetSafeNormal(), Forward));

    if (Angle > FMath::DegreesToRadians(112.5f)) {
        Situation = "Overtaking";
        return true;
    }

    TMap<EVesselType, int> Priority = {
        {EVesselType::NotUnderCommand, 1},
        {EVesselType::FishingTrawling, 2},
        {EVesselType::Sailing, 3},
        {EVesselType::PowerDriven, 4}
    };

    if (Priority.Contains(OwnerVessel->VesselType) && Priority.Contains(OtherVessel->VesselType) &&
        Priority[OwnerVessel->VesselType] > Priority[OtherVessel->VesselType]) {
        Situation = "Hierarchy";
        return true;
    }

    if (OwnerVessel->VesselType == EVesselType::PowerDriven &&
        OtherVessel->VesselType == EVesselType::PowerDriven) {

        float RelativeAngle = FMath::Acos(FVector::DotProduct(OwnerVessel->GetActorForwardVector(), OtherVessel->GetActorForwardVector()));
        if (FMath::Abs(RelativeAngle - PI) < FMath::DegreesToRadians(6.0f)) {
            Situation = "HeadOn";
            return true;
        }

        FVector ToOtherNormal = ToOther.GetSafeNormal();
        float SideAngle = FMath::Acos(FVector::DotProduct(ToOtherNormal, OwnerVessel->GetActorRightVector()));
        if (SideAngle < FMath::DegreesToRadians(90.0f)) {
            Situation = "CrossingStarboard";
            return true;
        }
    }

    return false;
}

void UCollisionAvoidanceComponent::ExecuteGiveWayManeuver(AVessel* OtherVessel, const FString& Situation) {
    AVessel* OwnerVessel = Cast<AVessel>(GetOwner());
    if (!OwnerVessel) return;

    OwnerVessel->AddActorWorldRotation(FRotator(0, 30.0f, 0)); // Turn to starboard
}

void UCollisionAvoidanceComponent::ExecuteStandOnManeuver(AVessel* OtherVessel) {
    // Intentionally left empty to maintain course
}
