

#include "Items/Items.h"
#include "Components/SphereComponent.h"
#include "Interfaces/PickUpInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AItems::AItems()
{
 
	PrimaryActorTick.bCanEverTick = true;
	Amplitupe = 0.25f;
	TimeConstant = 5.f;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComp->SetupAttachment(ItemMesh);

	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>("NiagaraSystemComponent");
	ItemEffect->SetupAttachment(GetRootComponent());

}

void AItems::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AItems::OnSphereBeginOverlap);

	SphereComp->OnComponentEndOverlap.AddDynamic(this, &AItems::OnSphereEndOverlap);

}

float AItems::TransformedSin()
{
	return Amplitupe * FMath::Sin(RunningTime * TimeConstant);
}

float AItems::TransformedCos()
{
	return Amplitupe * FMath::Cos(RunningTime * TimeConstant);
}

void AItems::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickupInterface=Cast<IPickUpInterface>(OtherActor);

	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItems(this);
	}
}

void AItems::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickUpInterface* PickupInterface = Cast<IPickUpInterface>(OtherActor);

	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItems(nullptr);
	}
}

void AItems::SetPickupEffects()
{
	if (PickupEffects)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickupEffects, GetActorLocation());
	}
}

void AItems::SpawnPickupSound()
{
	if (PickUpSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, PickUpSound, GetActorLocation());
	}
}

void AItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;
	
	if (ItemState != EItemState::EIS_Equipped)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
}

