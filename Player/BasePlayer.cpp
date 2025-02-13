// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BasePlayer.h"
#include <EnhancedInputSubsystems.h>
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
//#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimInstance.h"

ABasePlayer::ABasePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	//bUseControllerRotationPitch = false;
	//bUseControllerRotationYaw = false;
	//bUseControllerRotationRoll = false;
	ColliderComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Component"));
	ColliderComponent->SetCollisionProfileName(TEXT("Pawn"));
	ColliderComponent->SetNotifyRigidBodyCollision(true);
	SetRootComponent(ColliderComponent);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->SetUpdatedComponent(ColliderComponent);
	//MovementComponent = CreateDefaultSubobject<UPawnMovementComponent>(TEXT("MovementComponent"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	//SpringArm->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionProfileName(TEXT("CharacterMesh"));
	MeshComponent->SetNotifyRigidBodyCollision(true);
	MeshComponent->SetupAttachment(RootComponent);

	FootCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Foot Collision"));
	FootCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	FootCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECR_Overlap);
	FootCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECR_Overlap);
	FootCollision->SetGenerateOverlapEvents(true);
	FootCollision->SetupAttachment(RootComponent);
	
	//->bOrientRotationToMovement = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Characters/Meshes/SKM_Quinn.SKM_Quinn'"));
	if (SkeletalMeshRef.Object) MeshComponent->SetSkeletalMesh(SkeletalMeshRef.Object);

	//static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Script/Engine.AnimBlueprint'/Game/BluePrints/BP_PlayerAnimInstance.BP_PlayerAnimInstance'"));
	//if (AnimInstanceRef.Succeeded()) MeshComponent->SetAnimInstanceClass(AnimInstanceRef.Class);
	//else UE_LOG(LogTemp, Warning, TEXT("Not Find AnimInstance"));
	//static ConstructorHelpers::FObjectFinder<UAnimInstance> AnimInstanceRef(TEXT("/Script/Engine.AnimBlueprint'/Game/BluePrints/BP_PlayerAnimInstance.BP_PlayerAnimInstance'"));
	//if (AnimInstanceRef.Object) MeshComponent->SetAnimInstanceClass((AnimInstance = AnimInstanceRef.Object)->GetClass());
	//else UE_LOG(LogTemp, Warning, TEXT("Not Find AnimInstance"));

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/MC_PlayerInput.MC_PlayerInput'"));
	if (InputMappingContextRef.Object != nullptr) PlayerMappingContext = InputMappingContextRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'"));
	if (MoveActionRef.Object != nullptr) MoveAction = MoveActionRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Look.IA_Look'"));
	if (LookActionRef.Object != nullptr) LookAction = LookActionRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'"));
	if (JumpActionRef.Object != nullptr) JumpAction = JumpActionRef.Object;

}

void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	ColliderComponent->OnComponentHit.AddDynamic(this, &ABasePlayer::OnHit);
	FootCollision->OnComponentBeginOverlap.AddDynamic(this, &ABasePlayer::StartTrigger);
	FootCollision->OnComponentEndOverlap.AddDynamic(this, &ABasePlayer::EndTrigger);

	bIsHit = false;
	bIsWalkable = true;
	bIsJumping = false;
	RotateSpeed = 30;
	MoveSpeed = 70;
	JumpSpeed = 300;
	JumpStrength = 1.0f;
	Acceleration = 0;
	Gravity = GetWorld()->GetGravityZ();
	CurrentHp = MaxHp = 100;

	PlayerController = Cast<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerMappingContext, 0);
	}

	PlayerController->PlayerCameraManager->ViewPitchMin = -30;
	PlayerController->PlayerCameraManager->ViewPitchMax = 30;
}

void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsWalkable)
	{
		AddMovementInput(MoveForwardDirectionOnJump, MoveSpeedOnJumpX);
		AddMovementInput(MoveRightDirectionOnJump, MoveSpeedOnJumpY);
	}
	else
	{
		//MoveToGroundLocation();
	}
	AddMovementInput(GetActorUpVector(), Gravity * DeltaTime * 0.1f);
}

void ABasePlayer::Move(const FInputActionValue& Value)
{
	MovementVector = Value.Get<FVector2D>() * GetWorld()->DeltaTimeSeconds * MoveSpeed;

	if (bIsWalkable)
	{
		if (Acceleration < 1.0f) Acceleration += 0.01f;

		FVector Velocity(GetVelocity().X, GetVelocity().Y, 0);
		float MoveDirection = CalculateDirection(Velocity, GetActorRotation());

		if (MoveDirection >= -45 && MoveDirection <= 45)
		{
			MeshComponent->SetWorldRotation(GetActorRotation() + FRotator(0, -90 + MoveDirection, 0));
			MovementVector *= Acceleration;
		}
		else if (MoveDirection <= -70 || MoveDirection >= 70)
		{
			MeshComponent->SetWorldRotation(GetActorRotation() + FRotator(0, -90, 0));
			MovementVector *= 0.5f;
		}
		else if (MoveDirection <= -90 || MoveDirection >= 90)
		{
			MeshComponent->SetWorldRotation(GetActorRotation() + FRotator(0, -90, 0));
			MovementVector *= 0.1f;
		}
		else MovementVector *= Acceleration;
	}
	else MovementVector *= 0.2f;

	//const FRotator Rotation = Controller->GetControlRotation();
	//const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	//const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	//const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(GetActorForwardVector(), MovementVector.X);
	AddMovementInput(GetActorRightVector(), MovementVector.Y);

	//AddMovementInput(ForwardDirection, MovementVector.X);
	//AddMovementInput(RightDirection, MovementVector.Y);
}

void ABasePlayer::MoveStop(const FInputActionValue& Value)
{
	Acceleration = 0;
	MeshComponent->SetWorldRotation(GetActorRotation() + FRotator(0, -90, 0));
}

void ABasePlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X * GetWorld()->DeltaTimeSeconds * RotateSpeed);
	AddControllerPitchInput(LookAxisVector.Y * GetWorld()->DeltaTimeSeconds * RotateSpeed);

	FRotator NewRotation = GetControlRotation();
	FRotator CameraRotation = FRotator(NewRotation.Pitch, NewRotation.Yaw, 0);
	FRotator PawnRotation = FRotator(0, NewRotation.Yaw, 0);

	AddActorLocalRotation(PawnRotation - GetActorRotation());
	SpringArm->SetWorldRotation(CameraRotation);
}

void ABasePlayer::StartJump()
{
	if (!bIsWalkable) return;

	//const FRotator Rotation = Controller->GetControlRotation();
	//const FRotator YawRotation(0, Rotation.Yaw, 0);

	//MoveForwardDirectionOnJump = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	//MoveRightDirectionOnJump = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	MoveForwardDirectionOnJump = GetActorForwardVector();
	MoveRightDirectionOnJump = GetActorRightVector();

	MoveSpeedOnJumpX = MovementVector.X;
	MoveSpeedOnJumpY = MovementVector.Y;

	bIsJumping = true;
}

void ABasePlayer::Jump(const FInputActionValue& Value)
{
	if (!bIsJumping) return;

	if (JumpStrength > 0) JumpStrength -= 0.02f;
	
	AddMovementInput(GetActorUpVector(), JumpSpeed * GetWorld()->DeltaTimeSeconds * JumpStrength);
}

void ABasePlayer::StopJump(const FInputActionValue& Value)
{
	bIsJumping = false;
	JumpStrength = 1.0f;
}

float ABasePlayer::CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation)
{
	if (!Velocity.IsNearlyZero())
	{
		FMatrix RotMatrix = FRotationMatrix(BaseRotation);
		FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		FVector NormalizedVel = Velocity.GetSafeNormal2D();

		// get a cos(alpha) of forward vector vs velocity
		float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedVel);
		// now get the alpha and convert to degree
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		// depending on where right vector is, flip it
		float RightCosAngle = FVector::DotProduct(RightVector, NormalizedVel);
		if (RightCosAngle < 0)
		{
			ForwardDeltaDegree *= -1;
		}

		return ForwardDeltaDegree;
	}

	return 0.f;
}

void ABasePlayer::MoveToGroundLocation()
{
	if (CurrentGround == nullptr) return;

	//FVector Current = FVector(CurrentGround->GetActorLocation().X, CurrentGround->GetActorLocation().Y, 0);
	//FVector Current = CurrentGround->GetActorLocation();
	//AddActorWorldOffset(Current - GroundActorVector);
	//GroundActorVector = Current;
	//AddActorLocalOffset(Velocity);
}

void ABasePlayer::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this)
	{
		//bIsJumpable = true;
		//bIsJumping = false;
	}
}

void ABasePlayer::StartTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherComp != ColliderComponent)
	{
		if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldDynamic || (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic))
		{
			if (CurrentGround == OtherActor) return;
			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			bIsWalkable = true;
			bIsFalling = false;
			bIsJumping = false;
			AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
			CurrentGround = OtherActor;
			//GroundActorVector = FVector(OtherActor->GetActorLocation().X, OtherActor->GetActorLocation().Y, 0);
			//GroundActorVector = OtherActor->GetActorLocation();
		}
	}
}

void ABasePlayer::EndTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherComp != ColliderComponent)
	{
		if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldDynamic || (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic))
		{
			if (CurrentGround != OtherActor) return;
			bIsWalkable = false;
			bIsFalling = true;
			CurrentGround = nullptr;
			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
	}
}

void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayer::Move);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ABasePlayer::MoveStop);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayer::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABasePlayer::StartJump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABasePlayer::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABasePlayer::StopJump);
	
}

float ABasePlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHp = FMath::Clamp(CurrentHp - DamageAmount, 0, MaxHp);

	if (CurrentHp <= 0)
	{
		//EndGame
	}

	return DamageAmount;
}

