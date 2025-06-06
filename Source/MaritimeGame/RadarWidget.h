#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RadarComponent.h"
#include "RadarWidget.generated.h"

class UImage;
class UCanvasPanel;

UCLASS()
class MARITIMEGAME_API URadarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Call this from PlayerVessel to bind the component
	UFUNCTION(BlueprintCallable)
	void SetRadarComponent(URadarComponent* RadarComp);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	// Widgets from UMG
	UPROPERTY(meta = (BindWidget))
	UImage* BearingLine;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* BlipContainer;

private:
	URadarComponent* RadarComponent;

	void DrawRadarBlips();
	void UpdateBearingLine();

};