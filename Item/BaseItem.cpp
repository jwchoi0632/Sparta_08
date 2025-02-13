// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/BaseItem.h"
#include "Components/SphereComponent.h"
// Sets default values
ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionComp->SetGenerateOverlapEvents(true);
	CollisionComp->SetupAttachment(SceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(SceneComp);

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComp->SetupAttachment(SceneComp);
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnOverlapItem);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnEndOverlapItem);
	BaseGameState = Cast<ABaseGameState>(GetWorld()->GetGameState());
}

void ABaseItem::OnOverlapItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag("Player"))
	{
		if (bIsTriggered)
		{
			return;
		}
		bIsTriggered = true;
		ActionOverlapEvent(OtherActor);
	}
}

void ABaseItem::OnEndOverlapItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag("Player"))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player End Overlap"));
	}
}

void ABaseItem::ActionOverlapEvent(AActor* User)
{
	//UE_LOG(LogTemp, Warning, TEXT("Player Begin Overlap"));

	//DestroyItem();
}

FName ABaseItem::GetItemType() const
{
	return ItemType;
}

void ABaseItem::DestroyItem()
{
	if (!IsValid(this))
	{
		return;
	}
	Destroy();
}

