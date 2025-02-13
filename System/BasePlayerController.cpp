// Fill out your copyright notice in the Description page of Project Settings.


#include "System/BasePlayerController.h"
#include <EnhancedInputSubsystems.h>
#include "System/UIHandle.h"
#include "System/BaseGameInstance.h"
#include "System/BaseGameState.h"
#include "Player/PlayerCharacter.h"

ABasePlayerController::ABasePlayerController()
	: PlayerMappingContext(nullptr),
	  MoveAction(nullptr),
	  JumpAction(nullptr),
	  LookAction(nullptr),
	  PauseAction(nullptr),
	  MoveNextAction(nullptr)
{
	
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	ULocalPlayer* LocalPlayer = GetLocalPlayer();

	if (IsValid(LocalPlayer))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (PlayerMappingContext)
			{
				Subsystem->AddMappingContext(PlayerMappingContext, 0);
			}
		}
	}
}