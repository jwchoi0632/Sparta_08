// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "System/BasePlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "System/BaseGameState.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->bUsePawnControlRotation = false;
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	CurrentHp = MaxHp = 100;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller)
	{
		return;
	}
	
	FVector2D MoveValue = Value.Get<FVector2D>();

	if (bIsReverse)
	{
		MoveValue *= -1;
	}

	if (!FMath::IsNearlyZero(MoveValue.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveValue.X);
	}

	if (!FMath::IsNearlyZero(MoveValue.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveValue.Y);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookValue = Value.Get<FVector2D>();

	AddControllerYawInput(LookValue.X * RotateSpeed);
	AddControllerPitchInput(LookValue.Y * RotateSpeed);
}

void APlayerCharacter::StartJump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		Jump();
	}
}

void APlayerCharacter::StopJump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		StopJumping();
	}
}

void APlayerCharacter::PauseGame(const FInputActionValue& Value)
{
	if (!IsValid(BaseGameState))
	{
		return;
	}

	if (Value.Get<bool>())
	{
		BaseGameState->InputPauseKey();
	}
}

void APlayerCharacter::MoveNextLevel(const FInputActionValue& Value)
{
	if (!IsValid(BaseGameState))
	{
		return;
	}

	if (Value.Get<bool>())
	{
		BaseGameState->UsePortal();
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHp = FMath::Clamp(CurrentHp - DamageAmount, 0, MaxHp);
	
	if (!IsValid(BaseGameState))
	{
		return 0.0f;
	}
	BaseGameState->UpdatedHp();

	if (CurrentHp <= 0)
	{
		//GetWorld()->GetGameState<ABaseGameState>()->EndStage(true);
	}

	return DamageAmount;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	BaseGameState = Cast<ABaseGameState>(GetWorld()->GetGameState());
	if (IsValid(BaseGameState))
	{
		BaseGameState->BeginPlayPlayer(this);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ABasePlayerController* PlayerController = Cast<ABasePlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
			}
			if (PlayerController->JumpAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartJump);
				EnhancedInputComponent->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJump);
			}
			if (PlayerController->PauseAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->PauseAction, ETriggerEvent::Triggered, this, &APlayerCharacter::PauseGame);
			}
			if (PlayerController->MoveNextAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->MoveNextAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveNextLevel);
			}
		}
	}
}

void APlayerCharacter::AddHp(int32 AddValue)
{
	CurrentHp = FMath::Clamp(CurrentHp + AddValue, 0, MaxHp);
	
	if (!IsValid(BaseGameState))
	{
		return;
	}

	BaseGameState->UpdatedHp();
}

int32 APlayerCharacter::GetMaxHp()
{
	return MaxHp;
}

int32 APlayerCharacter::GetCurrentHp()
{
	return CurrentHp;
}

void APlayerCharacter::SetMoveSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

float APlayerCharacter::GetMoveSpeed()
{
	return MoveSpeed;
}

void APlayerCharacter::SetIsReverse(bool Value)
{
	bIsReverse = Value;
}
