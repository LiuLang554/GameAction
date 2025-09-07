// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Birds.generated.h"

class UCapsuleComponent;
class USeletalMeshComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class GAMEACTION_API ABirds : public APawn
{
	GENERATED_BODY()

public:

	ABirds();

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UCapsuleComponent> CapsuleComp;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> BridMesh;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArmComp;

	void MoveForwardInput(float Value);

	void TurnInput(float Value);

	void LookUpInput(float Value);
};
