#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "PlayerVessel.h"

class SCompassWidget : public SCompoundWidget
{
    SLATE_BEGIN_ARGS(SCompassWidget)
    {}
    SLATE_ARGUMENT(APlayerVessel*, PlayerVessel)
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& InArgs);
    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
        const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
        int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
    virtual FVector2D ComputeDesiredSize(float) const override;

private:
    TWeakObjectPtr<APlayerVessel> PlayerVesselPtr;
    void DrawCompassBackground(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;
    void DrawCompassHeading(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;
};