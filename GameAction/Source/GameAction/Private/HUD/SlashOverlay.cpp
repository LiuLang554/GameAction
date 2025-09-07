// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USlashOverlay::SetHealthProgressBar(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void USlashOverlay::SetStaminaProgressBar(float Percent)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void USlashOverlay::SetGoldCountText(int32 Gold)
{
	if (GoldCountText)
	{
		const FString GoldString = FString::Printf(TEXT("%d"), Gold);
		const FText GoldCount = FText::FromString(GoldString);
		GoldCountText->SetText(GoldCount);
	}
}

void USlashOverlay::SetSoulsCountText(int32 Souls)
{
	if (SoulsCountText)
	{
		const FString SoulsString = FString::Printf(TEXT("%d"), Souls);
		const FText SoulsCount = FText::FromString(SoulsString);
		SoulsCountText->SetText(SoulsCount);
	}
}
