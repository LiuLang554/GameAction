// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "BaseCharacter.generated.h"

class AWeapons;
class UAnimMontage;
class UAttributeComponent;

UCLASS()
class GAMEACTION_API ABaseCharacter : public ACharacter,public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

	//战斗函数
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)override;

	UFUNCTION(BlueprintNativeEvent)
	void Die();

	virtual void Attacked();
	void DirectionalHitReact(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticle(const FVector& ImpactPoint);
	void DisableCapsule();
	virtual bool CanAttack();
	bool IsAlived();
	void DisableMeshCollision();

	//蒙太奇函数
	void PlayReactMontage(const FName& SelectionName);
	virtual int32 PlayMontageAttack();
	virtual int32 PlayMontageTwoHandAttack();
	virtual int32 PlayMontageDie();
	virtual void PlayMontageDodge();
	void StopAttackMontage();

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	virtual void DodgeEnd();

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnable(ECollisionEnabled::Type EnableCollision);

	//变量
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<AWeapons> EquippedWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Actor Attribute")
	TObjectPtr<UAttributeComponent> AttributeComp;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double WarpTargetDistance = 75.f;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeadPose;

private:
	void PlayMontageSection(UAnimMontage* AnimMontage, const FName& SelectionName);
	int32 PlayRandomMontageSection(UAnimMontage* AnimMontage, const TArray<FName>& SelectionName);

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UParticleSystem> HitParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Montaged")
	TObjectPtr <UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> ReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montaged")
	TObjectPtr <UAnimMontage> DodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montaged")
	TObjectPtr <UAnimMontage> TwoHandAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<FName> AttackSelectionName;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<FName> DieSelectionName;

public:

	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose()const { return DeadPose; }
};
