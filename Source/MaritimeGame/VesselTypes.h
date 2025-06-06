#pragma once

#include "CoreMinimal.h"
#include "VesselTypes.generated.h"

UENUM(BlueprintType)
enum class EVesselType : uint8
{
    Unknown             UMETA(DisplayName = "Unknown"),
    Container           UMETA(DisplayName = "Container"),
    Bulker              UMETA(DisplayName = "Bulker"),
    Tanker              UMETA(DisplayName = "Tanker"),
    LNG_LPG             UMETA(DisplayName = "LNG / LPG"),

    // ADD these to avoid errors in CollisionAvoidanceComponent
    PowerDriven         UMETA(DisplayName = "Power Driven Vessel"),
    Sailing             UMETA(DisplayName = "Sailing Vessel"),
    Fishing             UMETA(DisplayName = "Fishing"),
    FishingTrawling      UMETA(DisplayName = "FishingTrawling"),
    Restricted          UMETA(DisplayName = "Restricted"),
    NotUnderCommand     UMETA(DisplayName = "Not Under Command"),
};
