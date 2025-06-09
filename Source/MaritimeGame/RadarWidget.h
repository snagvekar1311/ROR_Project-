#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "RadarComponent.h"

class MARITIMEGAME_API SRadarWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SRadarWidget)
        : _RadarComponent(nullptr)
    {}
    SLATE_ARGUMENT(URadarComponent*, RadarComponent)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    
    virtual int32 OnPaint(
        const FPaintArgs& Args,
        const FGeometry& AllottedGeometry,
        const FSlateRect& MyCullingRect,
        FSlateWindowElementList& OutDrawElements,
        int32 LayerId,
        const FWidgetStyle& InWidgetStyle,
        bool bParentEnabled) const override;

    virtual FVector2D ComputeDesiredSize(float) const override;

private:
    URadarComponent* RadarComponent;
    
    void DrawRadarBackground(
        const FGeometry& AllottedGeometry,
        FSlateWindowElementList& OutDrawElements,
        int32 LayerId) const;
        
    void DrawBlips(
        const FGeometry& AllottedGeometry,
        FSlateWindowElementList& OutDrawElements,
        int32 LayerId) const;
        
    void DrawBearingLine(
        const FGeometry& AllottedGeometry,
        FSlateWindowElementList& OutDrawElements,
        int32 LayerId) const;
};
