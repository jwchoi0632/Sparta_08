// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemSpawner.h"
#include "System/BaseGameState.h"
#include "Components/BoxComponent.h"
#include "Item/ExplosionItem.h"

// Sets default values
AItemSpawner::AItemSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxCollision->SetupAttachment(SceneComp);
}

void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	BaseGameState = GetWorld()->GetGameState<ABaseGameState>();

	if (IsValid(BaseGameState))
	{
		BaseGameState->SetCurrentItemSpawner(this);
	}
}

void AItemSpawner::SetLevelValue(int32 Value)
{
	LevelValue = Value;
}

void AItemSpawner::SpawnItems(int32 WaveValue)
{
	if (!IsValid(BoxCollision))
	{
		return;
	}

	if (!IsValid(BaseGameState))
	{
		return;
	}

	SpawnCount += 10 * LevelValue + 5 * WaveValue;

	for (int32 i = 0; i < SpawnCount; i++)
	{
		FVector SpawnLocation = CalculateRandomPos();
		TObjectPtr<AActor> SpawnActor = SelectSpawnItem(SpawnLocation);
		//SpawnActor->SetActorLocation(SpawnLocation);
		//BaseGameState->SetItemAudioComp(SpawnActor);
	}
}

int32 AItemSpawner::GetCoinItemCount()
{
	return CoinItemCount;
}

FVector AItemSpawner::CalculateRandomPos()
{
	const FVector CollisionExtent = BoxCollision->GetScaledBoxExtent();
	const FVector CollisionPos = BoxCollision->GetComponentLocation();

	const float RandPosX = FMath::FRandRange(-CollisionExtent.X, CollisionExtent.X);
	const float RandPosY = FMath::FRandRange(-CollisionExtent.Y, CollisionExtent.Y);
	const float RandPosZ = FMath::FRandRange(-CollisionExtent.Z, CollisionExtent.Z);

	return FVector(CollisionPos.X + RandPosX, CollisionPos.Y + RandPosY, CollisionPos.Z + RandPosZ);
}

AActor* AItemSpawner::SelectSpawnItem(FVector& Location)
{
	const float RandValue = FMath::FRandRange(0, 1.0);
	
	float SmallCoinRate = 0.3f - LevelValue * 0.1f;
	float BigCoinRate = CoinSpawnRate;
	float HealingRate = CoinSpawnRate + 0.3f - LevelValue * 0.1f;
	float SlowRate = HealingRate + 0.05f * (LevelValue + 1);
	float ReverseRate = SlowRate + 0.05f * (LevelValue + 1);

	FActorSpawnParameters SpawnParameters;

	if (RandValue <= SmallCoinRate)
	{
		++CoinItemCount;
		return GetWorld()->SpawnActor<AActor>(SmallCoin, Location, FRotator::ZeroRotator, SpawnParameters);
	}
	else if (RandValue <= BigCoinRate)
	{
		++CoinItemCount;
		return GetWorld()->SpawnActor<AActor>(BigCoin, Location, FRotator::ZeroRotator, SpawnParameters);
	}
	else if (RandValue <= HealingRate)
	{
		return GetWorld()->SpawnActor<AActor>(HealingPotion, Location, FRotator::ZeroRotator, SpawnParameters);
	}
	else if (RandValue <= SlowRate)
	{
		return GetWorld()->SpawnActor<AActor>(Slowing, Location, FRotator::ZeroRotator, SpawnParameters);
	}
	else if (RandValue <= ReverseRate)
	{
		return GetWorld()->SpawnActor<AActor>(Reverse, Location, FRotator::ZeroRotator, SpawnParameters);
	}
	else
	{	
		AActor* NewItem = GetWorld()->SpawnActor<AActor>(ExplosionMine, Location, FRotator::ZeroRotator, SpawnParameters);
		Cast<AExplosionItem>(NewItem)->ExplosionDelay -= LevelValue;
		return NewItem;
	}
}

