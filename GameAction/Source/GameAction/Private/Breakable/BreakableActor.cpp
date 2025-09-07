// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Engine/World.h"
#include "Items/Treasures.h"
#include "Components/CapsuleComponent.h"



ABreakableActor::ABreakableActor()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>("GeometryCollection");
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	CapsuleComp->SetupAttachment(GetRootComponent());
	CapsuleComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}


void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
}


void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter)
{
	if (bBroken)return;
	bBroken = true;
	UWorld* World = GetWorld();
	if (World && TreasuresClasses.Num()>0)
	{
		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		const int32 Index = FMath::RandRange(0, TreasuresClasses.Num()-1);
		World->SpawnActor<ATreasures>(TreasuresClasses[Index], Location, GetActorRotation());
	}
}

