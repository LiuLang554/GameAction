// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapons.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/SlashCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"

AWeapons::AWeapons()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Ignore);

	BoxStartComp = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trance Start"));
	BoxStartComp->SetupAttachment(GetRootComponent());

	BoxEndComp = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trance End"));
	BoxEndComp->SetupAttachment(GetRootComponent());
}

void AWeapons::Equip(USceneComponent* InParent, FName SocketName, AActor* NewOwner, APawn* NewInstigator)
{
	ItemState = EItemState::EIS_Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, SocketName);
	EquipSoundPlay();
	DisableSphereCollision();
	DeactivateEmbers();
}

void AWeapons::DeactivateEmbers()
{
	if (ItemEffect)
	{
		//关闭Niagara粒子组件
		ItemEffect->Deactivate();
	}
}

void AWeapons::DisableSphereCollision()
{
	if (SphereComp)
	{
		SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapons::EquipSoundPlay()
{
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			EquipSound,
			GetActorLocation());
	}
}

void AWeapons::AttachMeshToSocket(USceneComponent* InParent, const FName& SocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, SocketName);
}

void AWeapons::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapons::OnBoxBeginOverlap);
}

void AWeapons::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActorIsSameType(OtherActor))return;

	FHitResult BoxHitResult;
	BoxTrace(BoxHitResult);

	if (BoxHitResult.GetActor())
	{
		if (ActorIsSameType(BoxHitResult.GetActor()))return;

		UGameplayStatics::ApplyDamage(BoxHitResult.GetActor(),DamageAmount,GetInstigator()->GetController(),this,UDamageType::StaticClass());
		ExecuteGetHit(BoxHitResult);
		CreatFeilds(BoxHitResult.ImpactPoint);
	}
}

bool AWeapons::ActorIsSameType(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

void AWeapons::ExecuteGetHit(FHitResult& BoxHitResult)
{
	IHitInterface* HitInter = Cast<IHitInterface>(BoxHitResult.GetActor());
	if (HitInter)
	{
		HitInter->Execute_GetHit(BoxHitResult.GetActor(), BoxHitResult.ImpactPoint,GetOwner());
	}
}

void AWeapons::BoxTrace(FHitResult& HitResult)
{
	const FVector StartLocation = BoxStartComp->GetComponentLocation();
	const FVector EndLocation = BoxEndComp->GetComponentLocation();
	TArray<AActor*> ActorToIgnore;
	ActorToIgnore.Add(this);
	ActorToIgnore.Add(GetOwner());
	for (AActor* Actor : IgnoreActors)
	{
		ActorToIgnore.AddUnique(Actor);
	}
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		StartLocation,
		EndLocation,
		BoxTraceExent,
		BoxStartComp->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResult,
		true);
	IgnoreActors.AddUnique(HitResult.GetActor());
}
