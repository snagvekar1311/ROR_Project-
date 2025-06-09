#include "GameHUDWidget.h"
#include "PlayerVessel.h"
#include "CompassWidget.h"
#include "RadarWidget.h"
#include "RadarComponent.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SGameHUDWidget::Construct(const FArguments& InArgs)
{
    PlayerVesselPtr = InArgs._PlayerVessel;
    bIsRadarVisible = false;

    ChildSlot
    [
        SNew(SOverlay)
        + SOverlay::Slot()
        .HAlign(HAlign_Right)
        .VAlign(VAlign_Top)
        .Padding(FMargin(0, 20, 20, 0))
        [
            SAssignNew(CompassWidget, SCompassWidget)
            .PlayerVessel(PlayerVesselPtr.Get())
        ]

        + SOverlay::Slot()
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
        [
            SAssignNew(RadarWidget, SRadarWidget)
            .RadarComponent(PlayerVesselPtr.IsValid() ? PlayerVesselPtr->GetRadarComponent() : nullptr)
            .Visibility(EVisibility::Hidden)
        ]

        + SOverlay::Slot()
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Bottom)
        .Padding(FMargin(20, 0, 0, 20))
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(5)
            [
                SNew(STextBlock)
                .Text(FText::FromString(TEXT("Telegraph")))
            ]
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(5)
            [
                SAssignNew(TelegraphSlider, SSlider)
                .Value(0.0f)
                .OnValueChanged(this, &SGameHUDWidget::OnTelegraphValueChanged)
            ]
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(5)
            [
                SNew(STextBlock)
                .Text(FText::FromString(TEXT("Rudder")))
            ]
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(5)
            [
                SAssignNew(RudderSlider, SSlider)
                .Value(0.5f)  // Center position
                .OnValueChanged(this, &SGameHUDWidget::OnRudderValueChanged)
            ]
        ]

        + SOverlay::Slot()
        .HAlign(HAlign_Right)
        .VAlign(VAlign_Bottom)
        .Padding(FMargin(0, 0, 20, 20))
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(5)
            [
                SNew(SButton)
                .OnClicked(this, &SGameHUDWidget::OnRadarButtonClicked)
                .Content()
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(TEXT("Toggle Radar")))
                ]
            ]
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(5)
            [
                SAssignNew(ScoreText, STextBlock)
                .Text(FText::FromString(TEXT("Score: 0")))
            ]
        ]
    ];
}

FReply SGameHUDWidget::OnRadarButtonClicked()
{
    bIsRadarVisible = !bIsRadarVisible;
    if (RadarWidget.IsValid())
    {
        RadarWidget->SetVisibility(bIsRadarVisible ? EVisibility::Visible : EVisibility::Hidden);
    }
    return FReply::Handled();
}

void SGameHUDWidget::OnTelegraphValueChanged(float NewValue)
{
    if (PlayerVesselPtr.IsValid())
    {
        PlayerVesselPtr->SetTelegraph(NewValue);
    }
}

void SGameHUDWidget::OnRudderValueChanged(float NewValue)
{
    if (PlayerVesselPtr.IsValid())
    {
        // Convert from 0-1 range to -1 to 1 range
        float RudderValue = (NewValue * 2.0f) - 1.0f;
        PlayerVesselPtr->SetRudder(RudderValue);
    }
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION