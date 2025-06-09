#include "RadarWidget.h"
#include "SlateOptMacros.h"
#include "Rendering/DrawElements.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/AppStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SRadarWidget::Construct(const FArguments& InArgs)
{
    RadarComponent = InArgs._RadarComponent;
}

FVector2D SRadarWidget::ComputeDesiredSize(float) const
{
    return FVector2D(400.0f, 400.0f); // Fixed size radar display
}

int32 SRadarWidget::OnPaint(
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

    // Draw radar background
    DrawRadarBackground(AllottedGeometry, OutDrawElements, LayerId);
    
    // Draw blips
    DrawBlips(AllottedGeometry, OutDrawElements, LayerId + 1);
    
    // Draw bearing line
    DrawBearingLine(AllottedGeometry, OutDrawElements, LayerId + 2);

    return LayerId + 3;
}

void SRadarWidget::DrawRadarBackground(
    const FGeometry& AllottedGeometry,
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId) const
{
    const FVector2D Center = AllottedGeometry.GetLocalSize() * 0.5f;
    const float Radius = FMath::Min(Center.X, Center.Y);

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
        FLinearColor::Green,
        true,
        1.0f
    );

    // Draw range rings
    for (float RingRadius = Radius * 0.25f; RingRadius < Radius; RingRadius += Radius * 0.25f)
    {
        Points.Empty(NumPoints + 1);
        for (int32 i = 0; i <= NumPoints; ++i)
        {
            const float Angle = 2.0f * PI * i / NumPoints;
            Points.Add(Center + RingRadius * FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)));
        }

        FSlateDrawElement::MakeLines(
            OutDrawElements,
            LayerId,
            AllottedGeometry.ToPaintGeometry(),
            Points,
            ESlateDrawEffect::None,
            FLinearColor(0.0f, 0.5f, 0.0f, 0.5f),
            true,
            1.0f
        );
    }
}

void SRadarWidget::DrawBlips(
    const FGeometry& AllottedGeometry,
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId) const
{
    if (!RadarComponent) return;

    const FVector2D Center = AllottedGeometry.GetLocalSize() * 0.5f;
    const float Radius = FMath::Min(Center.X, Center.Y);

    for (const FRadarBlip& Blip : RadarComponent->GetRadarBlips())
    {
        const FVector2D BlipPos = Center + Blip.RadarPosition * (Radius / 200.0f);
        
        FSlateDrawElement::MakeBox(
            OutDrawElements,
            LayerId,
            AllottedGeometry.ToPaintGeometry(BlipPos - FVector2D(3.0f, 3.0f), FVector2D(6.0f, 6.0f)),
            FAppStyle::GetBrush("WhiteBrush"),
            ESlateDrawEffect::None,
            Blip.BlipColor
        );
    }
}

void SRadarWidget::DrawBearingLine(
    const FGeometry& AllottedGeometry,
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId) const
{
    if (!RadarComponent) return;

    const FVector2D Center = AllottedGeometry.GetLocalSize() * 0.5f;
    const float Radius = FMath::Min(Center.X, Center.Y);
    
    const float Angle = FMath::DegreesToRadians(RadarComponent->GetMarkedBearing());
    const FVector2D EndPoint = Center + Radius * FVector2D(FMath::Sin(Angle), -FMath::Cos(Angle));

    TArray<FVector2D> Points = { Center, EndPoint };

    FSlateDrawElement::MakeLines(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(),
        Points,
        ESlateDrawEffect::None,
        FLinearColor::Yellow,
        true,
        2.0f
    );
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION