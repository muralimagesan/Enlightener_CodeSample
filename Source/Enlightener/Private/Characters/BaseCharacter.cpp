// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AttributeComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack()
{

}

void ABaseCharacter::Die()
{
    
}

int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

int32 ABaseCharacter::PlayDeathMontage()
{
    return PlayRandomMontageSection(DeathMontage, DeathMontageSections);;
}

void ABaseCharacter::DisableCapsuleCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAnimMontage* ABaseCharacter::GetAttackMontage()
{
	return AttackMontage;
}

void ABaseCharacter::PlayHitReactMontage(const FName &SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector &ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	// Lower Impact Point to the Enemey's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// A . B = |A||B|cos(theta) - normalising the vector simplifies equation. 
	// As magnitudes are 1, the equation simplifies down to A.B = cos(theta) 
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cos (arc-cos) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	//If crossproduct points down, theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section = FName("HitFromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("HitFromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("HitFromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("HitFromRight");
	}

	PlayHitReactMontage(Section);
}

void ABaseCharacter::PlayHitSound(const FVector & ImpactPoint)
{
	if (HitSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
    }
}

void ABaseCharacter::SpawnHitParticles(const FVector &ImpactPoint)
{
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
	}
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName &SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName> &SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;

	const int32 MaxSectionsIndex = SectionNames.Num() - 1;
	const int32 Selection =  FMath::RandRange(0, MaxSectionsIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
    return Selection;
}

void ABaseCharacter::AttackEnd()
{

}

bool ABaseCharacter::CanAttack()
{
    return false;
}

bool ABaseCharacter::IsAlive()
{	
    return Attributes && Attributes->IsAlive();
}
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon)
	{
		// EquippedWeapon->GetWeaponBox()->SetWeaponCollisionEnabled(CollisionEnabled);
		EquippedWeapon->SetWeaponCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}
