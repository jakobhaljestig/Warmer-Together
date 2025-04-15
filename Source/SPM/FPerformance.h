#pragma once

#include "CoreMinimal.h"
#include "FPerformance.generated.h"

USTRUCT(BlueprintType)
struct SPM_API FPerformance
{
	GENERATED_BODY()

public:
	FPerformance()
		: PlayerDeaths(0), AvgPuzzleSolveTime(0.0f), TimesFrozen(0), CoopActions(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	int32 PlayerDeaths;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	float AvgPuzzleSolveTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	int32 TimesFrozen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	int32 CoopActions;

	// Returnerar ett värde mellan 0 (dålig prestation) och 1 (bra prestation)
	float RecentPerformanceScore() const
	{
		float rawScore = 0.0f;
		rawScore -= PlayerDeaths * 2.0f;
		rawScore -= TimesFrozen * 1.5f;
		rawScore -= AvgPuzzleSolveTime * 0.5f;
		rawScore += CoopActions * 2.0f;

		// Normalisera till ett värde mellan 0.0 och 1.0
		return FMath::Clamp((rawScore + 50.0f) / 100.0f, 0.0f, 1.0f); 
	}
};
