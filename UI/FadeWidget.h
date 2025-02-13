// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "FadeWidget.generated.h"

UCLASS()
class SPARTAPRJ06_API UFadeWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public :
	virtual void Action() override;
	virtual void Update() override;


public:
	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeIn;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeOut;
};
