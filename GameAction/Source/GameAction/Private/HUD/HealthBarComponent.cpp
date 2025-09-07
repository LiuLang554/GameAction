// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBarWidget.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if (HealthWidget == nullptr)
	{
		HealthWidget = Cast<UHealthBarWidget>(GetUserWidgetObject());
	}

	if (HealthWidget && HealthWidget->HealthBar)
	{
		HealthWidget->HealthBar->SetPercent(Percent);
	}
}
