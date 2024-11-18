// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"

class USoundBase;

/**
 * 
 */
UCLASS()
class ENLIGHTENER_API ATreasure : public AItem
{
	GENERATED_BODY()

public:

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PickUpSound;

	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	int32 Gold;
};
