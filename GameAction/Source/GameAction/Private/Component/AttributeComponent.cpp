// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttributeComponent::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, StaminaMax);
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health-Damage, 0.f, HealthMax);
}

void UAttributeComponent::UseStamina(float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, StaminaMax);
}

float UAttributeComponent::GetHealthParcent() const
{
	return Health / HealthMax;
}

float UAttributeComponent::GetStaminaParcent() const
{
	return Stamina / StaminaMax;
}

bool UAttributeComponent::BIsAlive()const
{
	return Health>0.f;
}

void UAttributeComponent::AddSouls(int32 NumberofSouls)
{
	Souls += NumberofSouls;
}

void UAttributeComponent::AddGold(int32 NumberofGold)
{
	Gold += NumberofGold;
}

void UAttributeComponent::AddHealth(int32 NumberofHealth)
{
	if (Health == HealthMax)return;
		
	Health += NumberofHealth;
}

