// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "TestItem.generated.h"

class UWidgetComponent;
class UCanvasPanel;

UCLASS()
class SPARTAPRJ06_API ATestItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	virtual void OnEndOverlapItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndext) override;
	virtual void ActionOverlapEvent(AActor* User) override;
	virtual void BeginPlay() override;
	ATestItem();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> WidgetComp;
};
