// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "MyBTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UMyBTTask_Attack : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
