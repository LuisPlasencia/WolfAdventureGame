// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "Actor/BaseProjectile.h"
#include "BaseFireBall.generated.h"

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API ABaseFireBall : public ABaseProjectile
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeline();

	UPROPERTY(BLueprintReadOnly)
	TObjectPtr<AActor> ReturnToActor;

	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams ExplosionDamageParams;

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnHit() override;


};
