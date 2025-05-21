#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BirdAIController.generated.h"

UCLASS()
class SPM_API ABirdAIController : public AAIController
{
	GENERATED_BODY()

protected:

	ABirdAIController();
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UBehaviorTreeComponent* BehaviorComp;

	virtual void OnPossess(APawn* InPawn) override;

};
