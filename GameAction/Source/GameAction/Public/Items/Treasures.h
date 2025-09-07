// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Items.h"
#include "Treasures.generated.h"

class USoundBase;
/**
 * 
 */
UCLASS()
class GAMEACTION_API ATreasures : public AItems
{
	GENERATED_BODY()
	
public:

	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;

	FORCEINLINE int32 GetGold()const { return Gold; }

private:

	UPROPERTY(EditAnywhere,Category="Treasures Properties")
	uint32 Gold;
};
