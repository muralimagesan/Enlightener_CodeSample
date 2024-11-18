// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnlightenerAnimInstance.h"
#include "Characters/EnlightenerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UEnlightenerAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    EnlightenerCharacter = Cast<AEnlightenerCharacter>(TryGetPawnOwner());
    if (EnlightenerCharacter)
    {
        EnlightenerCharacterMovement = EnlightenerCharacter->GetCharacterMovement();
    }    

}

void UEnlightenerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (EnlightenerCharacterMovement)
    {        
        GroundSpeed = UKismetMathLibrary::VSizeXY(EnlightenerCharacterMovement->Velocity);
        bIsFalling = EnlightenerCharacterMovement->IsFalling();
        CharacterState = EnlightenerCharacter->GetCharacterState();
    }
}