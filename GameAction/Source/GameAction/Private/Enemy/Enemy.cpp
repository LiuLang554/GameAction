// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Component/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Characters/CharacterTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Items/Weapons/Weapons.h"
#include "Items/Souls.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	WidgetComp = CreateDefaultSubobject<UHealthBarComponent>("HealthBarComponent");
	WidgetComp->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensing");
	PawnSensingComp->SetPeripheralVisionAngle(45.f);
	PawnSensingComp->SightRadius = 4000.f;

}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead())return;

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatActor();
	}
	else
	{
		CheckPatrolTarget();
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);

	CombatTarget=EventInstigator->GetPawn();

	if (IsInsideAttackRadius())
	{
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if(IsOutsideAttackRadius())
	{
		StartChasing();
	}

	return DamageAmount;
}

void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint,Hitter);
	if(!IsDead())ShowHealthBar();
	ClearPatrolTimer();
	ClearAttackedTime();
	SetWeaponCollisionEnable(ECollisionEnabled::NoCollision);

	StopAttackMontage();
	if (IsInsideAttackRadius())
	{
		if(!IsDead())StartAttackedTime();
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensingComp)PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);

	InitializeEnemy();

	Tags.Add(FName("Enemy"));
}

void AEnemy::Die_Implementation()
{
	Super::Die_Implementation();

	EnemyState = EEnemyState::EES_Dead;
	ClearAttackedTime();
	HideHealthBar();
	DisableCapsule();
	SetLifeSpan(LifeSpawn);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetWeaponCollisionEnable(ECollisionEnabled::NoCollision);
	SpawnSoulsAtLocation();
}

void AEnemy::SpawnSoulsAtLocation()
{
	if (GetWorld() && SoulsClass && AttributeComp)
	{
		const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 125.f);
		ASouls* SpawnSouls=GetWorld()->SpawnActor<ASouls>(SoulsClass, SpawnLocation, GetActorRotation());
		SpawnSouls->SetSouls(AttributeComp->GetSouls());
		SpawnSouls->SetOwner(this);
	}

	if (GetWorld() && HealthSoulsClass && AttributeComp)
	{
		const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 150.f);
		ASouls* SpawnSouls = GetWorld()->SpawnActor<ASouls>(HealthSoulsClass, SpawnLocation, GetActorRotation());
		SpawnSouls->SetRecovers(AttributeComp->GetRecovers());
		SpawnSouls->SetOwner(this);
	}
}

bool AEnemy::CanAttack()
{
	const bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacked() &&
		!IsEngaged() &&
		!IsDead();

	return bCanAttack;
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);

	if (AttributeComp && WidgetComp)
	{
		WidgetComp->SetHealthPercent(AttributeComp->GetHealthParcent());
	}
}

void AEnemy::Attacked()
{
	Super::Attacked();

	if (CombatTarget == nullptr)return;

	EnemyState = EEnemyState::EES_Engaged;
	PlayMontageAttack();
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;

	CheckCombatActor();
}

void AEnemy::InitializeEnemy()
{
	EnemyController = Cast<AAIController>(GetController());

	MoveToTarget(PatrolTarget);

	HideHealthBar();

	SpawnDefaultWeapon();
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChooseTarget();

		const float WaitTimer = FMath::RandRange(MinTimer,MaxTimer);

		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinshed, WaitTimer);
	}
}

void AEnemy::CheckCombatActor()
{
	if (IsOutsideCombatRadius())
	{
		ClearAttackedTime();

		LoseInterset();

		if (!IsEngaged()) StartPatrolling();

	}

	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		ClearAttackedTime();
		//×·Öð²»¹¥»÷
		if (!IsEngaged()) StartChasing();

	}

	else if (CanAttack())
	{
		StartAttackedTime();
	}
}

void AEnemy::PatrolTimerFinshed()
{
	MoveToTarget(PatrolTarget);
}

void AEnemy::HideHealthBar()
{
	if (WidgetComp)
	{
		WidgetComp->SetVisibility(false);
	}
}

void AEnemy::ShowHealthBar()
{
	if (WidgetComp)
	{
		WidgetComp->SetVisibility(true);
	}
}

void AEnemy::LoseInterset()
{
	CombatTarget = nullptr;

	HideHealthBar();
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;

	GetCharacterMovement()->MaxWalkSpeed = PatrolWalkSpeed;

	MoveToTarget(PatrolTarget);
}

void AEnemy::StartChasing()
{
	EnemyState = EEnemyState::EES_Chasing;

	GetCharacterMovement()->MaxWalkSpeed = ChasingWalkSpeed;

	MoveToTarget(CombatTarget);
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsAttacked()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsDead()
{
	return EnemyState==EEnemyState::EES_Dead;
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::StartAttackedTime()
{
	EnemyState = EEnemyState::EES_Attacking;

	const float AttackedTime = FMath::RandRange(Attackmin, AttackMax);

	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attacked, AttackedTime);

}

void AEnemy::ClearAttackedTime()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr)return false;

	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr)return;

	FAIMoveRequest MoveRequest;

	MoveRequest.SetGoalActor(Target);

	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);

	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemy::ChooseTarget()
{
	TArray<AActor*> VaildTargets;

	for (AActor* PointTarget : PatrolTargets)
	{
		if (PointTarget != PatrolTarget)
		{
			VaildTargets.AddUnique(PointTarget);
		}
	}

	const int32 NumPatrolTarget = VaildTargets.Num();

	if (NumPatrolTarget > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTarget - 1);

		return VaildTargets[TargetSelection];

	}

	return nullptr;
}

void AEnemy::SpawnDefaultWeapon()
{
	if (GetWorld() && WeaponClass)
	{
		AWeapons* DefaultWeapon = GetWorld()->SpawnActor<AWeapons>(WeaponClass);

		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);

		EquippedWeapon = DefaultWeapon;
	}
}

void AEnemy::PawnSeen(APawn* Pawn)
{
	const bool bPawnDead = Pawn->ActorHasTag(FName("Dead"));

	if (bPawnDead)
	{
		StartPatrolling();
	}

	const bool bShouldchaseChasing =
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState < EEnemyState::EES_Attacking &&
		Pawn->ActorHasTag(FName("EngageableTarget"));

	if (bShouldchaseChasing)
	{
		CombatTarget = Pawn;

		ClearPatrolTimer();

		StartChasing();
	}
	
}