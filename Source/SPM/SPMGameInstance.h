#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AdaptiveWeatherSystem.h" // Inkludera din vädersystemklass
#include "SPMGameInstance.generated.h"

UCLASS()
class SPM_API USPMGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USPMGameInstance();

protected:
	// Detta kallas när GameInstance initieras
	virtual void Init() override;

public:

};
