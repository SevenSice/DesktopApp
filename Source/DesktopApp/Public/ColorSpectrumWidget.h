// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/NativeWidgetHost.h"
#include "ColorSpectrumWidget.generated.h"

/**
 *
 */
UCLASS()
class DESKTOPAPP_API UColorSpectrumWidget : public UNativeWidgetHost
{
	GENERATED_BODY()

public:
	UColorSpectrumWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor SelectedColorHSV = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor SelectedColorRGB = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
};
