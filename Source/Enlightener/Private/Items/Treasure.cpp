// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Characters/EnlightenerCharacter.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    // As I don't want the parent behaviour of this function, will not call super.
    AEnlightenerCharacter* EnlightenerCharacter = Cast<AEnlightenerCharacter>(OtherActor);
	if (EnlightenerCharacter)
	{
        if (PickUpSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickUpSound, GetActorLocation());
		    // Destroys the item after pickup.
            Destroy();
        }
        
	}
}