#include "SGameInstance.h"

USGameInstance::USGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void USGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score : %d"), TotalScore)

}