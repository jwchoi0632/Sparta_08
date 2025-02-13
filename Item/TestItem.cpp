// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/TestItem.h"
#include "Components/WidgetComponent.h"
#include "Components/CanvasPanel.h"
//#include "System/BaseGameState.h"

void ATestItem::OnEndOverlapItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndext)
{
	Super::OnEndOverlapItem(OverlappedComp, OtherActor, OtherComp, OtherBodyIndext);

	bIsTriggered = false;

	if (!IsValid(BaseGameState))
	{
		return;
	}
	BaseGameState->EndTriggeredPortal();
	WidgetComp->SetHiddenInGame(true);
}

void ATestItem::ActionOverlapEvent(AActor* User)
{
	Super::ActionOverlapEvent(User);

	if (!IsValid(BaseGameState))
	{
		return;
	}
	BaseGameState->OnTriggeredPortal();
	WidgetComp->SetHiddenInGame(false);
}

void ATestItem::BeginPlay()
{
	Super::BeginPlay();

	WidgetComp->SetHiddenInGame(true);
}

ATestItem::ATestItem() : Super()
{
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComp->SetupAttachment(MeshComp);
	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
}
