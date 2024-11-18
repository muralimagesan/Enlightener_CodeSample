// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"
#include "Components/CapsuleComponent.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector &ImpactPoint)
{
	if (bBroken) return;

	bBroken = true;
	// This would spawn an ATreasure C++ class but not the BP_Treasure (which has all the mesh information)
	// GetWorld()->SpawnActor<ATreasure>()
	// When checking an array - check the number of elements which exist in the array. While unpopulated it would return 0, therefore never entering the if statement
	if (GetWorld() && TreasureClasses.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);

		FTransform SpawnTransform = GetActorTransform();
		SpawnTransform.SetLocation(GetActorLocation() + FVector(0.f, 0.f, 75.f));
		GetWorld()->SpawnActor<ATreasure>(TreasureClasses[Selection], SpawnTransform);

		// FVector Location = GetActorLocation();
		// Location.Z += 75.f;
		// GetWorld()->SpawnActor<ATreasure>(TreasureClasses[Selection], Location, GetActorRotation());
	}	
}
