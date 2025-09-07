// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;


UCLASS()
class GAMEACTION_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	/*<AActor>*/
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	virtual void Destroyed()override;
	/*</AActor>*/

	/*</IHitInterface>*/
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)override;
	/*</IHitInterface>*/

protected:
	/*<AActor>*/
	virtual void BeginPlay() override;
	/*<AActor>*/

	/*<ABaseCharacter>*/
	virtual void Die_Implementation()override;
	void SpawnSoulsAtLocation();
	virtual bool CanAttack()override;
	virtual void HandleDamage(float DamageAmount)override;
	virtual void Attacked()override;
	virtual void AttackEnd()override;
	/*</ABaseCharacter>*/

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private:

	/*<AI Behavior>*/
	void InitializeEnemy();
	void CheckPatrolTarget();
	void CheckCombatActor();
	void PatrolTimerFinshed();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterset();
	void StartPatrolling();
	void StartChasing();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacked();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	void StartAttackedTime();
	void ClearAttackedTime();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChooseTarget();
	void SpawnDefaultWeapon();

	UFUNCTION()
	void PawnSeen(APawn* Pawn);//视觉感应组件的回调函数
	
	UPROPERTY(VisibleAnywhere, Category = "HUD")
	TObjectPtr<UHealthBarComponent> WidgetComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UPawnSensingComponent> PawnSensingComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapons> WeaponClass;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere)
	double AcceptanceRadius = 50.f;

	UPROPERTY()
	TObjectPtr<class AAIController> EnemyController;

	//导航
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TObjectPtr<AActor> PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	//计时器
	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float MinTimer = 5.0f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float MaxTimer = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float PatrolWalkSpeed = 125.f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Attackmin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChasingWalkSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float LifeSpawn = 8.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class ASouls> SoulsClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class ASouls> HealthSoulsClass;
};
