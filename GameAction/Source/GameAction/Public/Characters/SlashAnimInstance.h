// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "SlashAnimInstance.generated.h"

class ASlashCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class GAMEACTION_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation()override;

	virtual void NativeUpdateAnimation(float DeltaTime)override;

protected:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ASlashCharacter> Characters;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | CharacterState")
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly,Category="Combat")
	EActionState ActionState;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TEnumAsByte<EDeathPose> DeathPose;
};
