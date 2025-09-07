
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;
class ATreasures;
class UCapsuleComponent;

UCLASS()
class GAMEACTION_API ABreakableActor : public AActor,public IHitInterface
{
	GENERATED_BODY()
	
public:	
	
	ABreakableActor();

	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter)override;

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> CapsuleComp;

private:

	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<ATreasures>> TreasuresClasses;

	bool bBroken = false;
};
