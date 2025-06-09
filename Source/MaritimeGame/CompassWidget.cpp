#include "CompassWidget.h"
#include "SlateOptMacros.h"
#include "Rendering/DrawElements.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCompassWidget::Construct(const FArguments& InArgs)
{
    PlayerVesselPtr = InArgs._PlayerVessel;
}

FVector2D SCompassWidget::ComputeDesiredSize(float) const
{
    return FVector2D(200.0f, 200.0f); // Fixed size compass display
}

int32 SCompassWidget::OnPaint(
    const FPaintArgs& Args,
    const FGeometry& AllottedGeometry,
    const FSlateRect& MyCullingRect,
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId,
    const FWidgetStyle& InWidgetStyle,
    bool bParentEnabled) const
{
    const bool bIsEnabled = ShouldBeEnabled(bParentEnabled);
    const ESlateDrawEffect DrawEffects = bIsEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

    // Draw compass background
    DrawCompassBackground(AllottedGeometry, OutDrawElements, LayerId);
    
    // Draw heading
    DrawCompassHeading(AllottedGeometry, OutDrawElements, LayerId + 1);

    return LayerId + 2;
}

void SCompassWidget::DrawCompassBackground(
    const FGeometry& AllottedGeometry,
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId) const
{
    const FVector2D Center = AllottedGeometry.GetLocalSize() * 0.5f;
    const float Radius = FMath::Min(Center.X, Center.Y) - 10.0f;

    // Draw outer circle
    TArray<FVector2D> Points;
    const int32 NumPoints = 360;
    for (int32 i = 0; i <= NumPoints; ++i)
    {
        const float Angle = 2.0f * PI * i / NumPoints;
        Points.Add(Center + Radius * FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)));
    }

    FSlateDrawElement::MakeLines(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(),
        Points,
        ESlateDrawEffect::None,
        FLinearColor::White,
        true,
        2.0f
    );

    // Draw cardinal points
    const float TextRadius = Radius * 0.8f;
    const FVector2D TextSize(20.0f, 20.0f);
    const TArray<TPair<FString, float>> CardinalPoints = {
        {TEXT("N"), 0.0f},
        {TEXT("E"), 90.0f},
        {TEXT("S"), 180.0f},
        {TEXT("W"), 270.0f}
    };

    for (const auto& Point : CardinalPoints)
    {
        const float Angle = FMath::DegreesToRadians(Point.Value);
        const FVector2D Position = Center + TextRadius * FVector2D(FMath::Sin(Angle), -FMath::Cos(Angle));

        FSlateDrawElement::MakeText(
            OutDrawElements,
            LayerId,
            AllottedGeometry.ToPaintGeometry(Position - TextSize * 0.5f, TextSize),
            Point.Key,
            FCoreStyle::GetDefaultFontStyle("Bold", 12),
            ESlateDrawEffect::None,
            FLinearColor::White
        );
    }
}

void SCompassWidget::DrawCompassHeading(
    const FGeometry& AllottedGeometry,
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId) const
{
    if (!PlayerVesselPtr.IsValid()) return;

    const FVector2D Center = AllottedGeometry.GetLocalSize() * 0.5f;
    const float Yaw = PlayerVesselPtr->GetActorRotation().Yaw;
    
    // Draw heading text
    const FString HeadingText = FString::Printf(TEXT("%d°"), FMath::RoundToInt(FMath::Fmod(360.0f + Yaw, 360.0f)));
    const FVector2D TextSize(60.0f, 30.0f);

    FSlateDrawElement::MakeText(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(Center - TextSize * 0.5f, TextSize),
        HeadingText,
        FCoreStyle::GetDefaultFontStyle("Bold", 16),
        ESlateDrawEffect::None,
        FLinearColor::White
    );
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION