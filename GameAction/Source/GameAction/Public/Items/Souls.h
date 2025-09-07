// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Items.h"
#include "Souls.generated.h"

/**
 * 
 */
UCLASS()
class GAMEACTION_API ASouls : public AItems
{
	GENERATED_BODY()
	
protected:
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	virtual void BeginPlay()override;

private:
	UPROPERTY(EditAnywhere, Category = "Souls Properties")
	int32 Souls;

	float DesiredZ;

	UPROPERTY(EditAnywhere)
	float DriftRate=-15.f;

	UPROPERTY(EditAnywhere,Category="Health")
	int32 Recovers;

public:
	FORCEINLINE int32 GetSouls()const { return Souls; }
	FORCEINLINE void SetSouls(int32 NumberofSouls) { Souls = NumberofSouls; }
	FORCEINLINE int32 GetRecovers()const { return Recovers; }
	FORCEINLINE void SetRecovers(int32 NumberofRecovers) { Recovers = NumberofRecovers; }
	virtual void Tick(float DeltaTime)override;
};
