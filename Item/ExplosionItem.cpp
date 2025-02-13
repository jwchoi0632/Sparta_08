// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ExplosionItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AExplosionItem::AExplosionItem() : Super()
{
	ItemType = "Mine";

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Explosion Collision"));
	ExplosionCollision->SetupAttachment(RootComponent);
	ExplosionCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ExplosionCollision->SetGenerateOverlapEvents(true);
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
}

void AExplosionItem::ActionOverlapEvent(AActor* User)
{
	Super::ActionOverlapEvent(User);

	if (!IsValid(BaseGameState))
	{
		return;
	}
	BaseGameState->PlayItemEffectSound(2, AudioComp);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::Explosion, ExplosionDelay, false);
}

void AExplosionItem::Explosion()
{
	TArray<AActor*> HitActors;
	ExplosionCollision->GetOverlappingActors(HitActors);
	BaseGameState->PlayItemEffectSound(3, AudioComp);

	for (AActor* Actor : HitActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(Actor, ExplosionDamage, nullptr, this, UDamageType::StaticClass());
		}
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::DestroyItem, 0.3f, false);
}
