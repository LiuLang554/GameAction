// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlashOverlay.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class GAMEACTION_API USlashOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetHealthProgressBar(float Percent);
	void SetStaminaProgressBar(float Percent);
	void SetGoldCountText(int32 Gold);
	void SetSoulsCountText(int32 Souls);

private:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaProgressBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> GoldCountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SoulsCountText;
};
