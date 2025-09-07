// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Items/Weapons/Weapons.h"
#include "Components/BoxComponent.h"
#include "Component/AttributeComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Components/CapsuleComponent.h"

ABaseCharacter::ABaseCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	AttributeComp = CreateDefaultSubobject<UAttributeComponent>("AttributeComponent");

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (IsAlived() && Hitter)
	{
		DirectionalHitReact(Hitter->GetActorLocation());
	}
	else Die();

	PlayHitSound(ImpactPoint);

	SpawnHitParticle(ImpactPoint);
}

void ABaseCharacter::Die_Implementation()
{
	Tags.Add(FName("Dead"));
	PlayMontageDie();
}

void ABaseCharacter::Attacked()
{
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
	{
		CombatTarget = nullptr;
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	//让命中向量与地面平行，需要更改z的位置
	const FVector ImpactPointLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactPointLowered - GetActorLocation()).GetSafeNormal();

	//Forward * ToHit=|Forward||ToHit| * cos(theta)
	//|Forward|=1,|ToHit|=1,Forward * ToHit=cos(theta);
	const double Costheta = FVector::DotProduct(Forward, ToHit);
	//取反余弦得到theta
	double theta = FMath::Acos(Costheta);
	//将弧度变为度数
	theta = FMath::RadiansToDegrees(theta);

	//交叉乘积获取Z向量位置，如果向下则theta取负值
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0.f)
	{
		theta *= -1.f;
	}

	FName Section("ReactFromBack");

	if (theta >= -45.f && theta < 45.f)
	{
		Section = "ReactFromFront";
	}
	else if (theta >= 45.f && theta < 135.f)
	{
		Section = "ReactFromRight";
	}
	else if (theta >= -135.f && theta < -45.f)
	{
		Section = "ReactFromLeft";
	}

	PlayReactMontage(Section);
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (AttributeComp)
	{
		AttributeComp->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint);
	}
}

void ABaseCharacter::SpawnHitParticle(const FVector& ImpactPoint)
{
	if (HitParticle && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticle,
			ImpactPoint,
			GetActorRotation(),
			true);
	}
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

bool ABaseCharacter::IsAlived()
{
	return AttributeComp && AttributeComp->BIsAlive();
}

void ABaseCharacter::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::PlayReactMontage(const FName& SelectionName)
{
	UAnimInstance* ReactInstance = GetMesh()->GetAnimInstance();

	if (ReactInstance && ReactMontage)
	{
		ReactInstance->Montage_Play(ReactMontage);

		ReactInstance->Montage_JumpToSection(SelectionName, ReactMontage);
	}
}

int32 ABaseCharacter::PlayMontageAttack()
{
	return PlayRandomMontageSection(AttackMontage,AttackSelectionName);
}

int32 ABaseCharacter::PlayMontageTwoHandAttack()
{
	return PlayRandomMontageSection(TwoHandAttackMontage,AttackSelectionName);
}

int32 ABaseCharacter::PlayMontageDie()
{
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DieSelectionName);

	TEnumAsByte<EDeathPose> Pose(Selection);

	if (Pose < EDeathPose::EDP_MAX)
	{
		DeadPose = Pose;
	}

	return Selection;
}

void ABaseCharacter::PlayMontageDodge()
{
	PlayMontageSection(DodgeMontage, FName("Default"));
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AttackInstance = GetMesh()->GetAnimInstance();

	if (AttackInstance && AttackMontage)
	{
		AttackInstance->Montage_Stop(0.25f, AttackMontage);
	}
}

FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if (CombatTarget == nullptr)return FVector();
	
	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;

	return CombatTargetLocation + TargetToMe;
}

FVector ABaseCharacter::GetRotationWarpTarget()
{
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::DodgeEnd()
{
}

void ABaseCharacter::SetWeaponCollisionEnable(ECollisionEnabled::Type EnableCollision)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(EnableCollision);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SelectionName)
{
	UAnimInstance* AttackInstance = GetMesh()->GetAnimInstance();

	if (AttackInstance && Montage)
	{
		AttackInstance->Montage_Play(Montage);

		AttackInstance->Montage_JumpToSection(SelectionName, Montage);
	}
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* AnimMontage, const TArray<FName>& SelectionName)
{
	if (SelectionName.Num() <= 0)return -1;

	const int32 Selection = FMath::RandRange(0, SelectionName.Num() - 1);

	PlayMontageSection(AnimMontage, SelectionName[Selection]);

	return Selection;
}