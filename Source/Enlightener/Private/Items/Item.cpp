// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Item.h"
#include "Enlightener/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Characters/EnlightenerCharacter.h"
#include "NiagaraComponent.h"

// Sets default values. Initialising via initialisation lists at the constructor.
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	RunningTime = 0;
	Amplitude = 0.25f;
	TimeConstant = 5.f;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	EmbersEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	EmbersEffect->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

float AItem::TransformedSin()
{
    return Amplitude * FMath::Sin(RunningTime * TimeConstant);	// period = 2*pi/K (TimeConstant);
}

float AItem::TransformedCos()
{
    return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::OnSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AEnlightenerCharacter* EnlightenerCharacter = Cast<AEnlightenerCharacter>(OtherActor);
	if (EnlightenerCharacter)
	{
		EnlightenerCharacter->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	AEnlightenerCharacter* EnlightenerCharacter = Cast<AEnlightenerCharacter>(OtherActor);
	if (EnlightenerCharacter)
	{
		EnlightenerCharacter->SetOverlappingItem(nullptr);
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_HOVERING)
	{	
		float RotationInDegrees = 90;
		AddActorLocalRotation(FRotator(0.f, RotationInDegrees * DeltaTime, 0.f));
		AddActorLocalOffset(FVector(0.f, 0.f, TransformedSin()));
	}
}
