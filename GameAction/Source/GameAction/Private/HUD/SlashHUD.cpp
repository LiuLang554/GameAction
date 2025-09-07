// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"

void ASlashHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		APlayerController* PlayController=GetWorld()->GetFirstPlayerController();
		if (PlayController && SlashOverlayClass)
		{
			Slash = CreateWidget<USlashOverlay>(PlayController, SlashOverlayClass);
			Slash->AddToViewport();
		}
	}
}
