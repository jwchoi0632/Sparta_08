// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Meteor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMeteor::AMeteor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionComp->SetGenerateOverlapEvents(true);
	CollisionComp->SetupAttachment(SceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeshComp->SetupAttachment(SceneComp);

	ExplosionComp = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ExplosionComp->SetGenerateOverlapEvents(true);
	ExplosionComp->SetupAttachment(SceneComp);
}

void AMeteor::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapItem);
}

void AMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!bIsFalling)
	{
		return;
	}

	AddActorWorldOffset(GetActorUpVector() * -FallingSpeed);
}

void AMeteor::OnOverlapItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	if (!OtherActor->ActorHasTag("Map"))
	{
		return;
	}

	TArray<AActor*> HitActors;
	ExplosionComp->GetOverlappingActors(HitActors);

	for (AActor* Actor : HitActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(Actor, Damage, nullptr, this, UDamageType::StaticClass());
		}
	}

	Destroy();
}
