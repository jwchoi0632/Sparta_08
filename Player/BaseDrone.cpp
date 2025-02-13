// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BaseDrone.h"
#include "Components/BoxComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <EnhancedInputSubsystems.h>

ABaseDrone::ABaseDrone()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	CollisionComponent->SetCollisionProfileName("Pawn");
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	CollisionComponent->SetGenerateOverlapEvents(true);
	SetRootComponent(CollisionComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	MeshComponent->SetCollisionProfileName("CharacterMesh");
	MeshComponent->SetNotifyRigidBodyCollision(true);
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement Component"));
	MovementComponent->SetUpdatedComponent(CollisionComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCamera->SetupAttachment(SpringArm);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentRef(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Shapes/Shape_Cube.Shape_Cube'"));
	if (MeshComponentRef.Object) MeshComponent->SetStaticMesh(MeshComponentRef.Object);

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> PlayerMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Drone.IMC_Drone'"));
	if (PlayerMappingContextRef.Object) PlayerMappingContext = PlayerMappingContextRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveForwardBackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_MoveForwardBack.IA_MoveForwardBack'"));
	if (MoveForwardBackActionRef.Object) MoveForwardBackAction = MoveForwardBackActionRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveUpDownActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_MoveUpDown.IA_MoveUpDown'"));
	if (MoveUpDownActionRef.Object) MoveUpDownAction = MoveUpDownActionRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveLeftRightActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_MoveLeftRight.IA_MoveLeftRight'"));
	if (MoveLeftRightActionRef.Object) MoveLeftRightAction = MoveLeftRightActionRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> TurnYawActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_TurnYaw.IA_TurnYaw'"));
	if (TurnYawActionRef.Object) TurnYawAction = TurnYawActionRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> TurnPitchActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_TurnPitch.IA_TurnPitch'"));
	if (TurnPitchActionRef.Object) TurnPitchAction = TurnPitchActionRef.Object;
}

void ABaseDrone::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = Cast<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerMappingContext, 0);
	}

	Acceleration = 0;
	ForwardAcceleration = 0;
	LeftAcceleration = 0;

	MoveSpeed = 100;
	RotateSpeed = 100;
	MoveMeshDirection = FVector::ZeroVector;
	//Gravity = GetWorld()->GetGravityZ() * 0.f;
}

void ABaseDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseDrone::MoveFB(const FInputActionValue& Value)
{
	float MoveValue = Value.Get<float>();
	float MeshDirection = 0;

	if (MoveValue < 0) MeshDirection = -RevisionMoveForwardMesh * (1 - ForwardAcceleration);
	else MeshDirection = RevisionMoveForwardMesh * (1 - ForwardAcceleration);

	if (ForwardAcceleration < 1.0f) ForwardAcceleration += 0.01f;
	
	MoveValue *= GetWorld()->DeltaTimeSeconds * MoveSpeed;
	MoveMeshDirection.X = MeshDirection;

	const FRotator YawRotation(Controller->GetControlRotation().Pitch, Controller->GetControlRotation().Yaw, 0);
	const FRotator PitchRotation(Controller->GetControlRotation().Pitch, 0, 0);
	const FVector DirectionX = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	AddMovementInput(DirectionX, MoveValue);
	MeshComponent->AddLocalRotation(FRotator(MoveMeshDirection.X, 0, 0));
}

void ABaseDrone::MoveLR(const FInputActionValue& Value)
{
	float MoveValue = Value.Get<float>();
	float MeshDirection = 0;

	if (MoveValue < 0) MeshDirection = -RevisionMoveLeftMesh * (1 - LeftAcceleration);
	else MeshDirection = RevisionMoveLeftMesh * (1 - LeftAcceleration);

	if (LeftAcceleration < 1.0f) LeftAcceleration += 0.01f;

	MoveValue *= GetWorld()->DeltaTimeSeconds * MoveSpeed * LeftAcceleration;
	MoveMeshDirection.Y = MeshDirection;

	const FRotator Rotation(0, Controller->GetControlRotation().Yaw, 0);
	const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Direction, MoveValue);
	MeshComponent->AddLocalRotation(FRotator(0, MoveMeshDirection.Y, 0));
}

void ABaseDrone::MoveUD(const FInputActionValue& Value)
{
	float MoveValue = Value.Get<float>();
	float MeshDirection = 0;

	if (MoveValue < 0) MeshDirection = -RevisionMoveUpMesh * (1 - Acceleration);
	else MeshDirection = RevisionMoveUpMesh * (1 - Acceleration);

	if (Acceleration < 1.0f) Acceleration += 0.01f;

	MoveValue *= GetWorld()->DeltaTimeSeconds * MoveSpeed * Acceleration;
	MoveMeshDirection.Z = MeshDirection;

	AddMovementInput(FVector::UpVector, MoveValue);
	MeshComponent->AddLocalRotation(FRotator(MoveMeshDirection.Z, 0, 0));
}

void ABaseDrone::MoveUpDownEnd()
{
	Acceleration = 0;
	MoveMeshDirection.Z = 0;
	MeshComponent->SetWorldRotation(GetActorRotation() + FRotator(MoveMeshDirection.X + MoveMeshDirection.Z, MoveMeshDirection.Y, 0));
}

void ABaseDrone::MoveForwardEnd()
{
	ForwardAcceleration = 0;
	MoveMeshDirection.X = 0;
	MeshComponent->SetWorldRotation(GetActorRotation() + FRotator(MoveMeshDirection.X + MoveMeshDirection.Z, MoveMeshDirection.Y, 0));
}

void ABaseDrone::MoveLeftEnd()
{
	LeftAcceleration = 0;
	MoveMeshDirection.Y = 0;
	MeshComponent->SetWorldRotation(GetActorRotation() + FRotator(MoveMeshDirection.X + MoveMeshDirection.Z, MoveMeshDirection.Y, 0));
}

void ABaseDrone::TurnYaw(const FInputActionValue& Value)
{
	float YawValue = Value.Get<float>();
	
	YawValue *= GetWorld()->DeltaTimeSeconds * RotateSpeed;

	FRotator YawRotation = FRotator(0, YawValue, 0);
	AddActorLocalRotation(FQuat(YawRotation));
	//AddControllerYawInput(YawValue);
	Controller->SetControlRotation(GetActorRotation());
}

void ABaseDrone::TurnPitch(const FInputActionValue& Value)
{
	float PitchValue = Value.Get<float>();
	PitchValue *= GetWorld()->DeltaTimeSeconds * RotateSpeed;

	FRotator PitchRotation = FRotator(PitchValue, 0, 0);
	AddActorLocalRotation(FQuat(PitchRotation));
	Controller->SetControlRotation(GetActorRotation());
	//AddControllerPitchInput(PitchValue);
}

void ABaseDrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveForwardBackAction, ETriggerEvent::Triggered, this, &ABaseDrone::MoveFB);
	EnhancedInputComponent->BindAction(MoveLeftRightAction, ETriggerEvent::Triggered, this, &ABaseDrone::MoveLR);
	EnhancedInputComponent->BindAction(MoveUpDownAction, ETriggerEvent::Triggered, this, &ABaseDrone::MoveUD);
	EnhancedInputComponent->BindAction(MoveForwardBackAction, ETriggerEvent::Completed, this, &ABaseDrone::MoveForwardEnd);
	EnhancedInputComponent->BindAction(MoveLeftRightAction, ETriggerEvent::Completed, this, &ABaseDrone::MoveLeftEnd);
	EnhancedInputComponent->BindAction(MoveUpDownAction, ETriggerEvent::Completed, this, &ABaseDrone::MoveUpDownEnd);
	EnhancedInputComponent->BindAction(TurnYawAction, ETriggerEvent::Triggered, this, &ABaseDrone::TurnYaw);
	EnhancedInputComponent->BindAction(TurnPitchAction, ETriggerEvent::Triggered, this, &ABaseDrone::TurnPitch);

}


