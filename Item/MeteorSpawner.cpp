// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MeteorSpawner.h"
#include "Components/BoxComponent.h"
#include "System/BaseGameState.h"

// Sets default values
AMeteorSpawner::AMeteorSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxCollision->SetupAttachment(SceneComp);
}

void AMeteorSpawner::StartSpawn(int32 LevelValue)
{
	LevelNum = LevelValue;
	SpawnMeteor();
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AMeteorSpawner::SpawnMeteor, SpawnTime - LevelNum, true);
}

void AMeteorSpawner::StopSpawn()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
}

// Called when the game starts or when spawned
void AMeteorSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	TObjectPtr<ABaseGameState> BaseGameState = Cast<ABaseGameState>(GetWorld()->GetGameState());

	if (!IsValid(BaseGameState))
	{
		return;
	}

	BaseGameState->SetCurrentMeteorSpawner(this);
}

FVector AMeteorSpawner::CalculateRandomPos()
{
	const FVector CollisionExtent = BoxCollision->GetScaledBoxExtent();
	const FVector CollisionPos = BoxCollision->GetComponentLocation();

	const float RandPosX = FMath::FRandRange(-CollisionExtent.X, CollisionExtent.X);
	const float RandPosY = FMath::FRandRange(-CollisionExtent.Y, CollisionExtent.Y);
	const float RandPosZ = FMath::FRandRange(-CollisionExtent.Z, CollisionExtent.Z);

	return FVector(CollisionPos.X + RandPosX, CollisionPos.Y + RandPosY, CollisionPos.Z + RandPosZ);
}

void AMeteorSpawner::SpawnMeteor()
{
	for (int32 i = 0; i <= LevelNum; i++)
	{
		FVector Location = CalculateRandomPos();
		FActorSpawnParameters SpawnParam;
		GetWorld()->SpawnActor<AActor>(Meteor, Location, FRotator::ZeroRotator, SpawnParam);
	}
}

