#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckForPlayers.generated.h"

UCLASS(BlueprintType)
class SPM_API UBTService_CheckForPlayers : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckForPlayers();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
