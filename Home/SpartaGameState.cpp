#include "SpartaGameState.h"
#include "SGameInstance.h"
#include "SPC.h"
#include "Kismet/GamePlayStatics.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"


ASpartaGameState::ASpartaGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;

	// 필수 과제 1번, 웨이브
	CurrentWaveIndex = 0;
	MaxWaves = 2;
	WaveDuration = 20.0f;
	ItemSpawnWave = {20, 30, 40};
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ASpartaGameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 ASpartaGameState::GetScore() const
{
	return Score;
}

void ASpartaGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USGameInstance* SGameInstance = Cast<USGameInstance>(GameInstance);
		if (SGameInstance)
		{
			SGameInstance->AddToScore(Amount);
		}
	}

	//Score += Amount;
	//UE_LOG(LogTemp, Warning, TEXT("Score : %d"), Score)
}

void ASpartaGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASPC* SPC = Cast<ASPC>(PlayerController))
		{
			SPC->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USGameInstance* SGameInstance = Cast<USGameInstance>(GameInstance);
		if (SGameInstance)
		{
			CurrentLevelIndex = SGameInstance->CurrentLevelIndex;
		}
	}

	/*GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ASpartaGameState::OnLevelTimeUp,
		LevelDuration,
		false);*/

	StartWave();
}

//	SpawnedCoinCount = 0;
//	CollectedCoinCount = 0;
//
//	TArray<AActor*> FoundVolume;
//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolume);
//
//	const int32 ItemToSpawn = 40;
//
//	for (int32 i = 0; i < ItemToSpawn; i++)
//	{
//		if (FoundVolume.Num() > 0)
//		{
//			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolume[0]);
//			if (SpawnVolume)
//			{
//				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
//				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
//				{
//					SpawnedCoinCount++;
//				}
//			}
//		}
//	}
//}

// 필수 과제 1번, 웨이브
void ASpartaGameState::StartWave()
{
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolume;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolume);

	for (AActor* Item : PreWaveItems)
	{
		if (Item && IsValid(Item))
		{
			Item->Destroy();
		}
	}
	PreWaveItems.Empty();

	int32 ItemToSpawn = 20;

	if (ItemSpawnWave.IsValidIndex(CurrentWaveIndex))
	{
		ItemToSpawn = ItemSpawnWave[CurrentWaveIndex];
	}

	if (FoundVolume.Num() > 0)
	{
		for (int32 i = 0; i < ItemToSpawn; i++)
		{
			if (FoundVolume.Num() > 0)
			{
				ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolume[0]);
				if (SpawnVolume)
				{
					AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
					if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
					{
						SpawnedCoinCount++;
					}

					PreWaveItems.Add(SpawnedActor);
				}
			}
		}
	}

	if (CurrentWaveIndex == 1)
	{
		EnableWave2();
	}
	else if (CurrentWaveIndex == 2)
	{
		EnableWave3();
	}

	GetWorldTimerManager().SetTimer(
		WaveTimerHandle,
		this,
		&ASpartaGameState::OnWaveTimeUp,
		WaveDuration,
		false);
}

void ASpartaGameState::EnableWave2()
{
	FString Msg = TEXT("Enable Wave 2");
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Msg);
	}
}

void ASpartaGameState::EnableWave3()
{
	FString Msg = TEXT("Enable Wave 3");
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Msg);
	}
}

void ASpartaGameState::OnWaveTimeUp()
{
	EndWave();
}

void ASpartaGameState::EndWave()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	if (CurrentWaveIndex == MaxWaves)
	{
		EndLevel();
	}
	else
	{
		++CurrentWaveIndex;
		StartWave();
	}
}


//void ASpartaGameState::OnLevelTimeUp()
//{
//	EndLevel();
//}

void ASpartaGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected : %d / %d"),
		CollectedCoinCount,
		SpawnedCoinCount);

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
}

void ASpartaGameState::EndLevel()
{
	//GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USGameInstance* SGameInstance = Cast<USGameInstance>(GameInstance);
		if (SGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			SGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}
	
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}

}

void ASpartaGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASPC* SPC = Cast<ASPC>(PlayerController))
		{
			SPC->SetPause(true);
			SPC->ShowMainMenu(true);
		}
	}
}

void ASpartaGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASPC* SPC = Cast<ASPC>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SPC->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					//float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time : %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						USGameInstance* SGameInstance = Cast<USGameInstance>(GameInstance);
						if (SGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score : %d"), SGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level : %d"), CurrentLevelIndex + 1)));
				}

				if (UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
				{
					WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave : %d / %d"), CurrentWaveIndex + 1, MaxWaves + 1)));
				}
			}
		}
	}
}