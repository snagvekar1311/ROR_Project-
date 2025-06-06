#include "RadarWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Blueprint/WidgetTree.h"

void URadarWidget::SetRadarComponent(URadarComponent* RadarComp)
{
	RadarComponent = RadarComp;
}

void URadarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!RadarComponent) return;

	DrawRadarBlips();
	UpdateBearingLine();

}

void URadarWidget::DrawRadarBlips()
{
	if (!BlipContainer || !RadarComponent) return;
    // Clear previous blips
    BlipContainer->ClearChildren();

    TArray<FRadarBlip> Blips = RadarComponent->GetRadarBlips();

    for (const FRadarBlip& Blip : Blips)
    {
        UImage* BlipDot = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());

        // Optional: Use a custom brush texture for the dot here
        BlipDot->SetBrushFromTexture(nullptr); // Replace with your texture if desired
        BlipDot->SetColorAndOpacity(Blip.BlipColor);
        BlipDot->SetDesiredSizeOverride(FVector2D(6.f, 6.f));

        UCanvasPanelSlot* BlipSlot = Cast<UCanvasPanelSlot>(BlipContainer->AddChild(BlipDot));
        if (BlipSlot)
        {
            FVector2D RadarCenter = FVector2D(200.f, 200.f); // Assumes 400x400 radar
            FVector2D ScreenPosition = RadarCenter + Blip.RadarPosition;

            BlipSlot->SetAutoSize(true);
            BlipSlot->SetAlignment(FVector2D(0.5f, 0.5f));
            BlipSlot->SetPosition(ScreenPosition);
        }
    }

}

void URadarWidget::UpdateBearingLine()
{
    if (!BearingLine || !RadarComponent) return;

    FWidgetTransform Transform;
    Transform.Angle = RadarComponent->GetMarkedBearing();
    BearingLine->SetRenderTransform(Transform);

}