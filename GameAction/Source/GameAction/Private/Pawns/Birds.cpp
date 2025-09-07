// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Birds.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


ABirds::ABirds()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComp->SetCapsuleHalfHeight(20.f);
	CapsuleComp->SetCapsuleRadius(15.f);
	RootComponent = CapsuleComp;

	BridMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BridMesh->SetupAttachment(CapsuleComp);
	//�Զ����ƽ�ɫ����Ϊ��ɫ0
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->SetupAttachment(CapsuleComp);
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

}


void ABirds::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABirds::MoveForwardInput(float Value)
{
	if (Controller && Value != 0.f)
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward,Value);
	}
}

void ABirds::TurnInput(float Value)
{
	AddControllerYawInput(Value);
}

void ABirds::LookUpInput(float Value)
{
	AddControllerPitchInput(Value);
}


void ABirds::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABirds::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("ForwardMove"), this, &ABirds::MoveForwardInput);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABirds::TurnInput);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABirds::LookUpInput);
}

