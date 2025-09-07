// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Items.h"
#include "Items/Weapons/Weapons.h"
#include "Animation/AnimMontage.h"
#include <HUD/SlashHUD.h>
#include <HUD/SlashOverlay.h>
#include "Component/AttributeComponent.h"
#include "Items/Souls.h"
#include "Items/Treasures.h"

ASlashCharacter::ASlashCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArm"));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

	Hair=CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AttributeComp && SlashOverlay)
	{
		AttributeComp->RegenStamina(DeltaTime);
		SlashOverlay->SetStaminaProgressBar(AttributeComp->GetStaminaParcent());
	}
}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("ForwardMove"), this, &ASlashCharacter::ForwardMove);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ASlashCharacter::TurnMove);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ASlashCharacter::LookupMove);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ASlashCharacter::MovedRight);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ASlashCharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ASlashCharacter::EkeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ASlashCharacter::Attacked);
	PlayerInputComponent->BindAction(FName("Dodge"), IE_Pressed, this, &ASlashCharacter::Dodge);

}

void ASlashCharacter::Jump()
{
	if (IsUnoccuppied())
	{
		Super::Jump();
	}
}

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint,Hitter);

	SetWeaponCollisionEnable(ECollisionEnabled::NoCollision);

	if (AttributeComp && AttributeComp->GetHealthParcent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

void ASlashCharacter::SetOverlappingItems(AItems* Item)
{
	OverlappingItems = Item;
}

void ASlashCharacter::AddSouls(ASouls* Souls)
{
	if (AttributeComp && SlashOverlay)
	{
		AttributeComp->AddSouls(Souls->GetSouls());
		SlashOverlay->SetSoulsCountText(AttributeComp->GetSouls());
	}
}

void ASlashCharacter::AddGold(ATreasures* Treasures)
{
	if (AttributeComp && SlashOverlay)
	{
		AttributeComp->AddGold(Treasures->GetGold());
		SlashOverlay->SetGoldCountText(AttributeComp->GetGold());
	}
}

void ASlashCharacter::AddHealth(ASouls* Souls)
{
	if (AttributeComp && SlashOverlay)
	{
		AttributeComp->AddHealth(Souls->GetRecovers());
		SlashOverlay->SetHealthProgressBar(AttributeComp->GetHealthParcent());
	}
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(FName("EngageableTarget"));

	InitializeSlashOverlay();
}

void ASlashCharacter::ForwardMove(float value)
{
	if (!IsUnoccuppied())return;
	if (Controller&&value != 0.f)
	{
		//找到随着控制器旋转的向前向量
		const FRotator ControllerRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControllerRotation.Yaw, 0.f);

		const FVector Direction=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}

void ASlashCharacter::TurnMove(float value)
{
	AddControllerYawInput(value);
}

void ASlashCharacter::LookupMove(float value)
{
	AddControllerPitchInput(value);
}

void ASlashCharacter::MovedRight(float value)
{
	if (!IsUnoccuppied())return;
	if (Controller && value != 0.f)
	{
		//找到随着控制器旋转的向右向量
		const FRotator ControllerRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControllerRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}

void ASlashCharacter::EkeyPressed()
{
	AWeapons* Weapon = Cast<AWeapons>(OverlappingItems);

	if (Weapon)
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->Destroy();
		}
		EquipWeapon(Weapon);
	}
	else {
		if (CanDisarm())
		{
			CanDisArm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}
}

void ASlashCharacter::Dodge()
{
	if (IsOccupied() || !HasEnoughStamina())return;
	
	PlayMontageDodge();
	ActionState = EActionState::EAS_Dodge;
	if (AttributeComp)
	{
		AttributeComp->UseStamina(AttributeComp->GetDodgeCost());
		SlashOverlay->SetStaminaProgressBar(AttributeComp->GetStaminaParcent());
	}
}

void ASlashCharacter::EquipWeapon(AWeapons* Weapon)
{
	if (Weapon == nullptr)return;
	if (IsSingleHandWeapon(Weapon))
	{
		SingleWeaponEquip(Weapon);
	}
	if(!IsSingleHandWeapon(Weapon))
	{
		TwoHandWeaponEquip(Weapon);
	}
}

void ASlashCharacter::TwoHandWeaponEquip(AWeapons*& Weapon)
{
	Weapon->Equip(GetMesh(), FName("TwoHandWeaponSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
	OverlappingItems = nullptr;
	EquippedWeapon = Weapon;
}

void ASlashCharacter::SingleWeaponEquip(AWeapons*& Weapon)
{
	Weapon->Equip(GetMesh(), FName("WeaponHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItems = nullptr;
	EquippedWeapon = Weapon;
}

bool ASlashCharacter::IsSingleHandWeapon(AWeapons* Weapon)
{
	return Weapon->GetWeaponType() == EWeaponType::SingleHand;
}

void ASlashCharacter::Attacked()
{
	Super::Attacked();

	if (CanAttack())
	{
		if (IsSingleHandWeapon(EquippedWeapon))
		{
			PlayMontageAttack();

			ActionState = EActionState::EAS_Attacking;
		}

		if (!IsSingleHandWeapon(EquippedWeapon))
		{
			PlayMontageTwoHandAttack();

			ActionState = EActionState::EAS_Attacking;
		}
	}
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccuppied;
}

void ASlashCharacter::DodgeEnd()
{
	ActionState = EActionState::EAS_Unoccuppied;
}

bool ASlashCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccuppied && 
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanDisarm()
{
	return CharacterState != ECharacterState::ECS_Unequipped && 
		ActionState == EActionState::EAS_Unoccuppied;
}

bool ASlashCharacter::CanArm()
{
	return CharacterState == ECharacterState::ECS_Unequipped && 
		ActionState == EActionState::EAS_Unoccuppied &&
		EquippedWeapon;
}

void ASlashCharacter::CanDisArm()
{
	PlayEquipMontage(FName("UnEquip"));

	CharacterState = ECharacterState::ECS_Unequipped;

	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));

	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;

	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::PlayEquipMontage(const FName& SelectionName)
{
	UAnimInstance* EquipInstance = GetMesh()->GetAnimInstance();

	if (EquipInstance && EquipMontage)
	{
		EquipInstance->Montage_Play(EquipMontage);

		EquipInstance->Montage_JumpToSection(SelectionName, EquipMontage);
	}
}

void ASlashCharacter::Die_Implementation()
{
	Super::Die_Implementation();
	ActionState = EActionState::EAS_Dead;
	Tags.Add(FName("Dead"));
	DisableMeshCollision();
}

bool ASlashCharacter::HasEnoughStamina()
{
	return AttributeComp && AttributeComp->GetDodge() > AttributeComp->GetDodgeCost();
}

bool ASlashCharacter::IsOccupied()
{
	return ActionState != EActionState::EAS_Unoccuppied;
}

void ASlashCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineScoket"));
	}
}

void ASlashCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("WeaponHandSocket"));
	}
}

void ASlashCharacter::FinshEquipping()
{
	ActionState = EActionState::EAS_Unoccuppied;
}

void ASlashCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccuppied;
}

bool ASlashCharacter::IsUnoccuppied()
{
	return ActionState == EActionState::EAS_Unoccuppied;
}

void ASlashCharacter::InitializeSlashOverlay()
{
	APlayerController* PlayController = Cast<APlayerController>(GetController());
	if (PlayController)
	{
		ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayController->GetHUD());
		if (SlashHUD)
		{
			SlashOverlay = SlashHUD->GetSlashOverlay();
			if (SlashOverlay && AttributeComp)
			{
				SlashOverlay->SetHealthProgressBar(AttributeComp->GetHealthParcent());
				SlashOverlay->SetStaminaProgressBar(1.0f);
				SlashOverlay->SetGoldCountText(0);
				SlashOverlay->SetSoulsCountText(0);
			}
		}
	}
}

void ASlashCharacter::SetHUDHealth()
{
	if (SlashOverlay && AttributeComp)
	{
		SlashOverlay->SetHealthProgressBar(AttributeComp->GetHealthParcent());
	}
}