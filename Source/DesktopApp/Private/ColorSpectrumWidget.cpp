// Fill out your copyright notice in the Description page of Project Settings.

#include "ColorSpectrumWidget.h"
#include "Colors/SColorSpectrum.h"
#include "Kismet/KismetMathLibrary.h"

UColorSpectrumWidget::UColorSpectrumWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TSharedRef<SColorSpectrum> colorPicker = SNew(SColorSpectrum)
		.SelectedColor_Lambda([this]()
	{
		return SelectedColorHSV;
	})
		.OnValueChanged_Lambda([this](FLinearColor c)
	{
		SelectedColorHSV = c;
		UKismetMathLibrary::HSVToRGB_Vector(SelectedColorHSV, SelectedColorRGB);
	});

	SetContent(colorPicker);
}