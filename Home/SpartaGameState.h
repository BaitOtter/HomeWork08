#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpartaGameState.generated.h"



UCLASS()
class SP_API ASpartaGameState : public AGameState
{
	GENERATED_BODY()

public:

	ASpartaGameState();

	virtual void BeginPlay() override;



	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;

	//FTimerHandle LevelTimerHandle;
	FTimerHandle WaveTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;


	// �ʼ� �׸� 1��, ���̺�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentWaveIndex;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 MaxWaves;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	float WaveDuration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<int32> ItemSpawnWave;
	UPROPERTY()
	TArray<AActor*> PreWaveItems;


	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	void OnCoinCollected();
	void StartLevel();

	// �ʼ� ���� 1��, ���̺�
	void StartWave();
	void OnWaveTimeUp();
	//void OnLevelTimeUp();
	void EndWave();
	void EndLevel();

	void EnableWave2();
	void EnableWave3();


	void UpdateHUD();
};
