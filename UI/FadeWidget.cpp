// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FadeWidget.h"

void UFadeWidget::Action()
{
	Super::Action();

	PlayAnimation(FadeIn);
}

void UFadeWidget::Update()
{
	Super::Update();

	PlayAnimation(FadeOut);
}
