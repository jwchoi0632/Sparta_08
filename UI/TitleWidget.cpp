// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleWidget.h"
#include "Components/Button.h"
#include "System/UIHandle.h"

void UTitleWidget::OnStartButton()
{
	if (!IsValid(UIHandle))
	{
		return;
	}
	UIHandle->ClickedMoveToNext();
}

void UTitleWidget::OnOptionButton()
{
	if (!IsValid(UIHandle))
	{
		return;
	}
	UIHandle->OpenOptionUI();
}

void UTitleWidget::OnExitButton()
{
	UIHandle->ClickedQuitGame();
}

void UTitleWidget::HoveredStartButton()
{
	StartButton->SetRenderScale(FVector2D(1.05f, 1.05f));
}

void UTitleWidget::UnHoveredStartButton()
{
	StartButton->SetRenderScale(FVector2D(1.0f, 1.0f));
}

void UTitleWidget::HoveredOptionsButton()
{
	OptionsButton->SetRenderScale(FVector2D(1.05f, 1.05f));
}

void UTitleWidget::UnHoveredOptionsButton()
{
	OptionsButton->SetRenderScale(FVector2D(1.0f, 1.0f));
}

void UTitleWidget::HoveredExitButton()
{
	ExitButton->SetRenderScale(FVector2D(1.05f, 1.05f));
}

void UTitleWidget::UnHoveredExitButton()
{
	ExitButton->SetRenderScale(FVector2D(1.0f, 1.0f));
}

void UTitleWidget::InitWidget()
{
	Super::InitWidget();

	StartButton->OnHovered.AddDynamic(this, &UTitleWidget::HoveredStartButton);
	StartButton->OnUnhovered.AddDynamic(this, &UTitleWidget::UnHoveredStartButton);
	OptionsButton->OnHovered.AddDynamic(this, &UTitleWidget::HoveredOptionsButton);
	OptionsButton->OnUnhovered.AddDynamic(this, &UTitleWidget::UnHoveredOptionsButton);
	ExitButton->OnHovered.AddDynamic(this, &UTitleWidget::HoveredExitButton);
	ExitButton->OnUnhovered.AddDynamic(this, &UTitleWidget::UnHoveredExitButton);

	StartButton->OnClicked.AddDynamic(this, &UTitleWidget::OnStartButton);
	OptionsButton->OnClicked.AddDynamic(this, &UTitleWidget::OnOptionButton);
	ExitButton->OnClicked.AddDynamic(this, &UTitleWidget::OnExitButton);
}
