#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

// Forward declarations
class APlayerVessel;
class SCompassWidget;
class SRadarWidget;
class SSlider;
class STextBlock;

class MARITIMEGAME_API SGameHUDWidget : public SCompoundWidget
{
    SLATE_BEGIN_ARGS(SGameHUDWidget)
    {}
    SLATE_ARGUMENT(APlayerVessel*, PlayerVessel)
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& InArgs);

private:
    TWeakObjectPtr<APlayerVessel> PlayerVesselPtr;
    TSharedPtr<SCompassWidget> CompassWidget;
    TSharedPtr<SRadarWidget> RadarWidget;
    TSharedPtr<SSlider> TelegraphSlider;
    TSharedPtr<SSlider> RudderSlider;
    TSharedPtr<STextBlock> ScoreText;
    bool bIsRadarVisible;

    FReply OnRadarButtonClicked();
    void OnTelegraphValueChanged(float NewValue);
    void OnRudderValueChanged(float NewValue);
};