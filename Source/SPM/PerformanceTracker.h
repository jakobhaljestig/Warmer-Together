#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FPerformance.h"
#include "WeatherUpdaterInterface.h"
#include "PerformanceTracker.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_API UPerformanceTracker : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPerformanceTracker();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//i characterbase
	void RegisterDeath();
	//i en framtida puzzle actor class
	void RegisterPuzzleSolved(float TimeToSolve);
	
	
	FORCEINLINE const FPerformance& GetPerformance() const { return Performance; }


private:
	int32 TotalPuzzles = 0;
	float TotalPuzzleTime = 0.0f;
	float TimeNearHeat = 0.0f;
	bool bIsNearHeat = false;

	FPerformance Performance;

	IWeatherUpdaterInterface* WeatherUpdater = nullptr;

};

