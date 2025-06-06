#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CollisionAvoidanceComponent.generated.h" // ✅ LAST include

class AVessel; // ✅ Forward declaration

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MARITIMEGAME_API UCollisionAvoidanceComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UCollisionAvoidanceComponent();
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avoidance")
    float SafeDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avoidance")
    float CriticalDistance;

    UFUNCTION(BlueprintCallable)
    bool IsCollisionRiskDetected();

protected:
    virtual void BeginPlay() override;

    void EvaluateCollisionRisk();
    bool IsGiveWayVessel(AVessel* OtherVessel, FString& Situation);
    void ExecuteGiveWayManeuver(AVessel* OtherVessel, const FString& Situation);
    void ExecuteStandOnManeuver(AVessel* OtherVessel);

private:
    bool bCollisionRiskDetected;
};
