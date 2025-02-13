// Fill out your copyright notice in the Description page of Project Settings.


#include "System/BaseGameState.h"
#include "System/BaseGameInstance.h"
#include "System/BasePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "System/EnumSetClass.h"
#include "Item/ItemSpawner.h"
#include "Item/MeteorSpawner.h"
#include "Item/ObstaclesSpawner.h"
#include "Player/PlayerCharacter.h"

ABaseGameState::ABaseGameState() : Super()
{
	
}

void ABaseGameState::BeginDestroy()
{
	if (IsValid(BaseGameInstance))
	{
		BaseGameInstance->DestroyedGameState();
	}

	Super::BeginDestroy();
}

void ABaseGameState::BeginPlay()
{
	Super::BeginPlay();

	Score = 0;
	CurrentCoin = 0;
	StageTime = 0;
	WaveTime = 0;
	WaveNum = 0;
	BaseGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
}

void ABaseGameState::BeginPlayPlayer(APlayerCharacter* PlayerCharacter)
{
	if (!IsValid(BaseGameInstance))
	{
		return;
	}

	PlayerPawn = PlayerCharacter;
	BasePlayerController = Cast<ABasePlayerController>(PlayerPawn->GetController());
	BaseGameInstance->StartedGameState();

	UpdatedHp();
	NotifyCurrentScore();
	//SetPlayerAudioComp();
	StartStage();
}

void ABaseGameState::StartStage()
{
	if (!IsValid(BaseGameInstance))
	{
		return;
	}

	ESceneName CurrentScene = BaseGameInstance->GetCurrentSceneName();

	if (CurrentScene == ESceneName::EasyLevelScene)
	{
		StageNum = 0;
	}
	else if (CurrentScene == ESceneName::NormalLevelScene)
	{
		StageNum = 1;
	}
	else if (CurrentScene == ESceneName::HardLevelScene)
	{
		StageNum = 2;
	}
	else
	{
		return;
	}

	NotifyStageNum(StageNum + 1);
	NotifyStageTime(StageTime);
	UE_LOG(LogTemp, Warning, TEXT("Wave1 시작."));
	ChangeWave();

	GetWorld()->GetTimerManager().SetTimer(StageTimer, FTimerDelegate::CreateLambda([&]()
		{
			if (!bIsPause)
			{
				++StageTime;
				++WaveTime;
				NotifyStageTime(StageTime);

				if (WaveTime >= WaveChangeTime)
				{
					WaveTime = 0;
					ChangeWave();
				}
			}
		}), 1.0f, true);
}

void ABaseGameState::ChangeWave()
{
	++WaveNum;
	
	if (WaveNum > MaxWave)
	{
		if (WaveNum == MaxWave + 1)
		{
			FVector SpawnPos = PlayerPawn->GetActorForwardVector() * 500 + PlayerPawn->GetActorLocation();
			FRotator SpawnRotation = PlayerPawn->GetActorRotation() - FRotator(0, 90, 0);
			FActorSpawnParameters SpawnParam;
			GetWorld()->SpawnActor<AActor>(Portal, SpawnPos, SpawnRotation, SpawnParam);
		}
		return;
	}
	NotifyWaveNum(WaveNum);

	UE_LOG(LogTemp, Warning, TEXT("아이템이 생성됩니다."));
	SpawnBaseItem();

	if (WaveNum == 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Wave2에 진입. 장애물이 생성됩니다."));
		SpawnObstacles();
	}

	if (WaveNum == 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("Wave3에 진입. 하늘에서 운석이 떨어집니다."));
		SpawnRandomExplosion();
	}
}

void ABaseGameState::SpawnBaseItem()
{
	if (!IsValid(ItemSpawner))
	{
		return;
	}

	ItemSpawner->SetLevelValue(StageNum);
	ItemSpawner->SpawnItems(WaveNum);
	TotalCoin = ItemSpawner->GetCoinItemCount();
}

void ABaseGameState::SpawnObstacles()
{
	if (!IsValid(ObstaclesSpawner))
	{
		return;
	}
	ObstaclesSpawner->SpawnObstacles(StageNum);
}

void ABaseGameState::SpawnRandomExplosion()
{
	if (!IsValid(MeteorSpawner))
	{
		return;
	}
	MeteorSpawner->StartSpawn(StageNum);
}

void ABaseGameState::AddScore(int32 AddValue)
{
	Score += AddValue;
	NotifyCurrentScore();
}

int32 ABaseGameState::GetScore()
{
	return Score;
}

void ABaseGameState::IncreaseCurrentCoin()
{
	++CurrentCoin;
}

void ABaseGameState::EndStage(bool bIsDie)
{
	if (!IsValid(BaseGameInstance))
	{
		return;
	}
	GetWorld()->GetTimerManager().ClearTimer(StageTimer);

	if (IsValid(MeteorSpawner))
	{
		MeteorSpawner->StopSpawn();
	}
	
	BaseGameInstance->AddTotalScore(Score);
	BaseGameInstance->AddPlayTime(StageTime);
	NotifyTotalScore();
	BaseGameInstance->EndStage();
}

void ABaseGameState::SetCurrentItemSpawner(AItemSpawner* Spawner)
{
	ItemSpawner = Spawner;
}

void ABaseGameState::SetCurrentMeteorSpawner(AMeteorSpawner* Spawner)
{
	MeteorSpawner = Spawner;
}

void ABaseGameState::SetCurrentObstaclesSpawner(AObstaclesSpawner* Spawner)
{
	ObstaclesSpawner = Spawner;
}

void ABaseGameState::UpdatedHp()
{
	int32 CurrentHp = PlayerPawn->GetCurrentHp();

	NotifyMaxHp(PlayerPawn->GetMaxHp());
	NotifyCurrentHp(CurrentHp);

	if (CurrentHp <= 0)
	{
		EndStage(true);
	}
}

void ABaseGameState::InputPauseKey()
{
	if (!IsValid(BaseGameInstance))
	{
		return;
	}
	BaseGameInstance->OnPause();
}

void ABaseGameState::ChangedCursorMode(bool bIsVisible) // InputMode까지 입력받는 방법 or InputMode 입력 처리 따로 하는 방법도 괜찮을지도
{
	if (!IsValid(BasePlayerController))
	{
		return;
	}

	if (bIsVisible)
	{
		BasePlayerController->bShowMouseCursor = true;
		BasePlayerController->SetInputMode(FInputModeUIOnly());
		BasePlayerController->FlushPressedKeys();
		bIsPause = true;
		//SetInputMode(FInputModeGameAndUI());
		//BasePlayerController->SetPause(true);
	}
	else
	{
		BasePlayerController->bShowMouseCursor = false;
		BasePlayerController->SetInputMode(FInputModeGameOnly());
		BasePlayerController->FlushPressedKeys();
		bIsPause = false;
		//BasePlayerController->SetPause(true);
	}
}

void ABaseGameState::SetPlayerAudioComp()
{
	if (!IsValid(BaseGameInstance))
	{
		return;
	}

	if (!IsValid(PlayerPawn))
	{
		return;
	}

	BaseGameInstance->InitActorAudioComp(EAudioCategory::Player, PlayerPawn);
}

void ABaseGameState::SetItemAudioComp(AActor* Item)
{
	if (!IsValid(BaseGameInstance))
	{
		return;
	}

	if (!IsValid(Item))
	{
		return;
	}

	BaseGameInstance->InitActorAudioComp(EAudioCategory::Item, Item);
}

void ABaseGameState::PlayItemEffectSound(int32 index, UAudioComponent* AudioComp)
{
	if (!IsValid(BaseGameInstance))
	{
		return;
	}

	if (AudioComp == nullptr)
	{
		return;
	}

	BaseGameInstance->PlaySoundAtAudioComp(EAudioCategory::Item, AudioComp, index);
}

void ABaseGameState::PlayPlayerEffectSound(int32 index)
{
	if (!IsValid(BaseGameInstance))
	{
		return;
	}
	
	//if (PlayerPawn->GetComponentByClass<UAudioComponent>() == nullptr)
	{
		//return;
	}

	//BaseGameInstance->PlaySoundAtAudioComp(EAudioCategory::Player, PlayerPawn->GetComponentByClass<UAudioComponent>(), index);
}

AActor* ABaseGameState::GetPlayer()
{
	return PlayerPawn;
}

void ABaseGameState::StartSlow(float SlowRate)
{
	if (!IsValid(PlayerPawn))
	{
		return;
	}
	PlayerPawn->SetMoveSpeed(PlayerPawn->GetMoveSpeed() * SlowRate);
	NotifyDebuff(EDebuffState::Slow, true);
}

void ABaseGameState::EndSlow()
{
	if (!IsValid(PlayerPawn))
	{
		return;
	}
	PlayerPawn->SetMoveSpeed(PlayerPawn->GetMoveSpeed());
	NotifyDebuff(EDebuffState::Slow, false);
}

void ABaseGameState::StartReverse()
{
	if (!IsValid(PlayerPawn))
	{
		return;
	}
	PlayerPawn->SetIsReverse(true);
	NotifyDebuff(EDebuffState::Reverse, true);
}

void ABaseGameState::EndReverse()
{
	if (!IsValid(PlayerPawn))
	{
		return;
	}
	PlayerPawn->SetIsReverse(false);
	NotifyDebuff(EDebuffState::Reverse, false);
}

void ABaseGameState::OnTriggeredPortal()
{
	bIsOverlapedPortal = true;
}

void ABaseGameState::EndTriggeredPortal()
{
	bIsOverlapedPortal = false;
}

void ABaseGameState::UsePortal()
{
	if (!bIsOverlapedPortal)
	{
		return;
	}
	EndStage(false);
}

//UAudioComponent* ABaseGameState::PlayItemEffectSound(int32 index, AActor* Owner)
//{
//	if (!IsValid(BaseGameInstance))
//	{
//		return nullptr;
//	}
//
//	if (!IsValid(Owner))
//	{
//		return nullptr;
//	}
//
//	return BaseGameInstance->PlayEffectSound(EAudioCategory::Item, index, Owner);
//}
//
//void ABaseGameState::StopItemEffectSound(UAudioComponent* AudioComp)
//{
//	if (!IsValid(BaseGameInstance))
//	{
//		return;
//	}
//
//	if (AudioComp == nullptr)
//	{
//		return;
//	}
//
//	BaseGameInstance->StopEffectSound(AudioComp);
//}

void ABaseGameState::RegisterGameStateObserver(TScriptInterface<IGameStateObserver> Observer)
{
	GameStateObservers.Add(Observer);
}

void ABaseGameState::UnregisterGameStateObserver(TScriptInterface<IGameStateObserver> Observer)
{
	GameStateObservers.Remove(Observer);
}

void ABaseGameState::NotifyCurrentScore()
{
	//for (TScriptInterface<IGameStateObserver> Observer : CurrentScoreObservers)
	for (TScriptInterface<IGameStateObserver> Observer : GameStateObservers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedCurrentScoreNotify(Score);
	}
}

void ABaseGameState::NotifyTotalScore()
{
	for (TScriptInterface<IGameStateObserver> Observer : GameStateObservers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedTotalScoreNotify(BaseGameInstance->GetTotalScore());
	}
}

void ABaseGameState::NotifyCurrentHp(int32 Hp)
{
	for (TScriptInterface<IGameStateObserver> Observer : GameStateObservers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedCurrentHpNotify(Hp);
	}
}

void ABaseGameState::NotifyMaxHp(int32 Hp)
{
	for (TScriptInterface<IGameStateObserver> Observer : GameStateObservers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedMaxHpNotify(Hp);
	}
}

void ABaseGameState::NotifyStageTime(int32 Time)
{
	for (TScriptInterface<IGameStateObserver> Observer : GameStateObservers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedWaveTime(Time);
	}
}

void ABaseGameState::NotifyStageNum(int32 Num)
{
	for (TScriptInterface<IGameStateObserver> Observer : GameStateObservers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedStage(Num);
	}
}

void ABaseGameState::NotifyWaveNum(int32 Num)
{
	for (TScriptInterface<IGameStateObserver> Observer : GameStateObservers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedWave(Num);
	}
}

void ABaseGameState::NotifyDebuff(EDebuffState TargetDebuff, bool bIsActive)
{
	for (TScriptInterface<IGameStateObserver> Observer : GameStateObservers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedPlayerState(TargetDebuff, bIsActive);
	}
}
