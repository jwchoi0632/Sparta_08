// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ObstaclesSpawner.h"
#include "System/BaseGameState.h"

AObstaclesSpawner::AObstaclesSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);
}

void AObstaclesSpawner::SpawnObstacles(int32 LevelNum)
{
	switch (LevelNum)
	{
	case 0:
		SpawnObstaclesLevel1();
		break;

	case 1:
		SpawnObstaclesLevel2();
		break;

	case 2:
		SpawnObstaclesLevel3();
		break;
	}

	bIsStart = true;
}

void AObstaclesSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();
	BaseGameState = Cast<ABaseGameState>(GetWorld()->GetGameState());

	if (!IsValid(BaseGameState))
	{
		return;
	}

	BaseGameState->SetCurrentObstaclesSpawner(this);
}

void AObstaclesSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsStart)
	{
		return;
	}

	if ((StartLocation + GetActorUpVector() * TargetDestance).Z <= GetActorLocation().Z) //?
	{
		SetActorTickEnabled(false);
		return;
	}

	AddActorWorldOffset(GetActorUpVector() * UpSpeed);
}

void AObstaclesSpawner::SpawnObstaclesLevel1()
{
	FActorSpawnParameters SpawnParam;

	for (int32 i = 0; i < 4; i++)
	{
		FVector SpawnLocation = StartLocation - GetActorUpVector() * TargetDestance;
		FRotator SpawnRotation = FRotator::ZeroRotator;

		if (i % 2 == 0)
		{
			SpawnLocation += FVector(-3000, 3000 - 1200 * (i + 1), 0);
		}
		else
		{
			SpawnLocation += FVector(-2000, 3000 - 1200 * (i + 1), 0);
		}

		TObjectPtr<AActor> NewObstacle = GetWorld()->SpawnActor<AActor>(Obstacle, SpawnLocation, SpawnRotation, SpawnParam);
		NewObstacle->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		NewObstacle->SetActorScale3D(FVector(10, 1, 1));
	}
}

void AObstaclesSpawner::SpawnObstaclesLevel2()
{
	for (int32 i = 0; i < 4; i++)
	{
		FActorSpawnParameters SpawnParam;
		TObjectPtr<AActor> NewActor = GetWorld()->SpawnActor<AActor>(Empty, StartLocation, FRotator::ZeroRotator, SpawnParam);
		NewActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

		for (int32 j = 0; j < 8; j++)
		{
			FVector SpawnLocation = StartLocation - GetActorUpVector() * TargetDestance;
			FRotator SpawnRotation = FRotator::ZeroRotator;

			float RotY = 0;
			float LocX = 1975 - 500 * (j / 2);
			float LocY = -2000 + 500 * (j / 2);

			if (j % 2 != 0)
			{
				RotY += 90;
			}

			SpawnRotation.Yaw = RotY;
			SpawnLocation += FVector(LocX, LocY, 0);
			TObjectPtr<AActor> NewObstacle = GetWorld()->SpawnActor<AActor>(Obstacle, SpawnLocation, SpawnRotation, SpawnParam);
			NewObstacle->AttachToActor(NewActor, FAttachmentTransformRules::KeepWorldTransform);
		}

		NewActor->SetActorRotation(FRotator(0, i * 90, 0));
	}
}

void AObstaclesSpawner::SpawnObstaclesLevel3()
{
	FActorSpawnParameters SpawnParam;
	FVector SpawnLocation = StartLocation - GetActorUpVector() * TargetDestance;

	for (int32 i = 0; i < 4; i++)
	{
		TObjectPtr<AActor> FirstActor = GetWorld()->SpawnActor<AActor>(Empty, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParam);
		FirstActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

		for (int32 j = 0; j < 3; j++)
		{
			FVector SecondLocation = FVector::ZeroVector;

			TObjectPtr<AActor> SecondActor = GetWorld()->SpawnActor<AActor>(Empty, SecondLocation, FRotator::ZeroRotator, SpawnParam);
			SecondActor->AttachToActor(FirstActor, FAttachmentTransformRules::KeepWorldTransform);

			for (int32 k = 0; k < 4; k++)
			{
				TObjectPtr<AActor> NewObstacle = GetWorld()->SpawnActor<AActor>(Obstacle, SpawnLocation, FRotator(0, k * 90, 0), SpawnParam);
				NewObstacle->AttachToActor(SecondActor, FAttachmentTransformRules::KeepWorldTransform);
			}

			if (j == 0)
			{
				SecondLocation = FVector(0, 750, 0);
			}
			else if (j == 1)
			{
				SecondLocation = FVector(750, 1500, 0);
			}
			else
			{
				SecondLocation = FVector(1500, 750, 0);
			}

			SecondActor->SetActorLocation(SecondLocation);
		}
		FirstActor->SetActorRotation(FRotator(0, i * 90, 0));
	}
}

