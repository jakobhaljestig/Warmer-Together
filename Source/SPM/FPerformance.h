#pragma once

#include "CoreMinimal.h"
#include "FPerformance.generated.h"

USTRUCT(BlueprintType)
struct FPerformance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DeathCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AveragePuzzleTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeNearHeat = 0.0f;

	UPROPERTY(Transient)
	float TotalPuzzleTime;
	
	UPROPERTY(Transient)
	int32 TotalPuzzles;

	// Returnerar värde mellan 0 och 1 (där 1 är "bra")
	float RecentPerformanceScore() const
	{
		// Enkel viktad modell (du kan utveckla den!)
		float score = 1.0f;

		score -= DeathCount * 0.2f;
		score -= AveragePuzzleTime * 0.05f;
		score += TimeNearHeat * 0.1f;

		return FMath::Clamp(score, 0.0f, 1.0f);
	}
};

