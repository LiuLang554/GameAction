// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Items.h"
#include "Weapons.generated.h"

class USoundBase;
class UBoxComponent;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	SingleHand UMETA(DisplayName = "SingleHand"),
	TwoHandSingle UMETA(DisplayName = "TwoHandSingle")
};

UCLASS()
class GAMEACTION_API AWeapons : public AItems
{
	GENERATED_BODY()

public:

	AWeapons();

	void Equip(USceneComponent* InParent,FName SocketName,AActor* NewOwner,APawn* NewInstigator);

	void DeactivateEmbers();

	void DisableSphereCollision();

	void EquipSoundPlay();

	void AttachMeshToSocket(USceneComponent* InParent, const FName& SocketName);

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool ActorIsSameType(AActor* OtherActor);

	void ExecuteGetHit(FHitResult& BoxHitResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CreatFeilds(const FVector& Location);

private:

	void BoxTrace(FHitResult& HitResult);

	UPROPERTY(EditAnywhere, Category = "Weapon BoxTrace")
	FVector BoxTraceExent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon BoxTrace")
	bool bShowBoxDebug = false;

	UPROPERTY(EditAnywhere, Category = "Weapon Sound")
	TObjectPtr<USoundBase> EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "BoxComponent")
	TObjectPtr<UBoxComponent> WeaponBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxStartComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxEndComp;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float DamageAmount;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType = EWeaponType::SingleHand;

public:

	FORCEINLINE UBoxComponent* GetWeaponBox()const { return WeaponBox; }
	TArray<AActor*> IgnoreActors;
	FORCEINLINE EWeaponType GetWeaponType()const { return WeaponType; }
};
