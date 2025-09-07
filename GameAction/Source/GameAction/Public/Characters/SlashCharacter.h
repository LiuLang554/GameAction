// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Interfaces/PickUpInterface.h"
#include "SlashCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UGroomComponent;
class AItems;
class UAnimMontage;
class USlashOverlay;
class ASouls;
class ATreasures;

UCLASS()
class GAMEACTION_API ASlashCharacter : public ABaseCharacter , public IPickUpInterface
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump()override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)override;
	virtual void SetOverlappingItems(AItems* Item)override;
	virtual void AddSouls(ASouls* Souls)override;
	virtual void AddGold(ATreasures* Treasures)override;
	virtual void AddHealth(ASouls* Souls)override;

protected:
	virtual void BeginPlay() override;

	//Call back Functions
	void ForwardMove(float value);
	void TurnMove(float value);
	void LookupMove(float value);
	void MovedRight(float value);
	void EkeyPressed();
	void Dodge();

	void EquipWeapon(AWeapons* Weapon);
	void TwoHandWeaponEquip(AWeapons*& Weapon);
	void SingleWeaponEquip(AWeapons*& Weapon);
	bool IsSingleHandWeapon(AWeapons* Weapon);
	virtual void Attacked()override;
	virtual void AttackEnd()override;
	virtual void DodgeEnd()override;
	virtual bool CanAttack()override;
	bool CanDisarm();
	bool CanArm();
	void CanDisArm();
	void Arm();
	void PlayEquipMontage(const FName& SelectionName);
	virtual void Die_Implementation()override;
	bool HasEnoughStamina();
	bool IsOccupied();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinshEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

private:
	bool IsUnoccuppied();
	void InitializeSlashOverlay();
	void SetHUDHealth();

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleDefaultsOnly,Category="Hair")
	TObjectPtr<UGroomComponent> Hair;

	UPROPERTY(VisibleDefaultsOnly, Category = "Hair")
	TObjectPtr<UGroomComponent> Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItems> OverlappingItems;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	EActionState ActionState = EActionState::EAS_Unoccuppied;

	UPROPERTY(EditDefaultsOnly, Category = "Montaged")
	TObjectPtr <UAnimMontage> EquipMontage;

	UPROPERTY()
	TObjectPtr<USlashOverlay> SlashOverlay;

public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState()const { return ActionState; }
};
