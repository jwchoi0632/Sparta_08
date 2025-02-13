// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Obstacle.h"

AObstacle::AObstacle()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	MeshComp->SetupAttachment(SceneComp);
}

void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}


