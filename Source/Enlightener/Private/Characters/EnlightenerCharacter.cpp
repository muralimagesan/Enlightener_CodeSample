// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnlightenerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
// #include "Animation/AnimMontage.h"

AEnlightenerCharacter::AEnlightenerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 380.f, 0.f);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 200.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AEnlightenerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AEnlightenerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEnlightenerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AEnlightenerCharacter::Jump);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AEnlightenerCharacter::Equip);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AEnlightenerCharacter::PlayerAttack);
		EnhancedInputComponent->BindAction(AttackActionCombo, ETriggerEvent::Triggered, this, &AEnlightenerCharacter::PlayerAttackCombo);
	}
}

void AEnlightenerCharacter::Jump()
{
	if (GetActionState() == EActionState::EAS_ATTACKING) return;
	
	Super::Jump();	
}

void AEnlightenerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(EnlightenerContext, 0);
		}
	}	

	Tags.Add(FName("EnlightenerCharacter"));
}

void AEnlightenerCharacter::Move(const FInputActionValue &Value)
{
	if (GetActionState() != EActionState::EAS_UNOCCUPIED) return;

	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (GetController())
	{
		// Get Controller rotation and then the yaw rotation.
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		// Get the forward direction vector based on the rotation matrix's x-axis
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, MovementVector.Y);
		
		// Get the forward direction vector based on the rotation matrix's y-axis
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}	
}

void AEnlightenerCharacter::Look(const FInputActionValue &Value)
{
	const FVector2D LookingVector = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerPitchInput(LookingVector.Y);
		AddControllerYawInput(LookingVector.X);
	}
}

void AEnlightenerCharacter::Equip(const FInputActionValue &Value)
{	
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		CharacterState = ECharacterState::ECS_EQUIPPED_ONE_HANDED_WEAPON;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
	else
	{
		if (CanDisarmWeapon())
		{
			PlayArmWeaponMontage(FName("DisarmWeapon"));
			SetCharacterState(ECharacterState::ECS_UNEQUIPPED);
			SetActionState(EActionState::EAS_ARM_DISARM_WEAPON);
		}
		else if (CanArmWeapon())
		{
			PlayArmWeaponMontage(FName("ArmWeapon"));
			SetCharacterState(ECharacterState::ECS_EQUIPPED_ONE_HANDED_WEAPON);
			SetActionState(EActionState::EAS_ARM_DISARM_WEAPON);
		}
	}
}

void AEnlightenerCharacter::PlayerAttack(const FInputActionValue &Value)
{
	if (CanAttack())
	{
		PlayAttackMontage();
		SetActionState(EActionState::EAS_ATTACKING);
	}		
}

void AEnlightenerCharacter::PlayerAttackCombo(const FInputActionValue &Value)
{
	if (CanAttack())
	{
		PlayAttackComboMontage();
		SetActionState(EActionState::EAS_ATTACKING);
	}
}

void AEnlightenerCharacter::PlayAttackComboMontage()
{
	PlayRandomMontageSection(GetAttackMontage(), AttackComboMontageSections);
}

void AEnlightenerCharacter::PlayArmWeaponMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ArmWeaponMontage)
	{
		AnimInstance->Montage_Play(ArmWeaponMontage);
		AnimInstance->Montage_JumpToSection(SectionName, ArmWeaponMontage);
	}
}

void AEnlightenerCharacter::AttackEnd()
{
	SetActionState(EActionState::EAS_UNOCCUPIED);
}

bool AEnlightenerCharacter::CanAttack()
{
	bool bCanAttack = false;
	bCanAttack = GetCharacterState() != ECharacterState::ECS_UNEQUIPPED
				&& GetActionState() == EActionState::EAS_UNOCCUPIED;    
	return bCanAttack;
}

bool AEnlightenerCharacter::CanArmWeapon()
{
	bool bCanArm = false;
	bCanArm = (GetCharacterState() == ECharacterState::ECS_UNEQUIPPED)
		&& (GetActionState() == EActionState::EAS_UNOCCUPIED)
		&& (EquippedWeapon != nullptr);
    return bCanArm;
}

bool AEnlightenerCharacter::CanDisarmWeapon() 
{
	bool bCanDisarm = false;
	bCanDisarm = (GetCharacterState() != ECharacterState::ECS_UNEQUIPPED)
		&& (GetActionState() == EActionState::EAS_UNOCCUPIED)
		&& (EquippedWeapon != nullptr);
    return bCanDisarm;
}

void AEnlightenerCharacter::DisarmWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void AEnlightenerCharacter::ArmWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void AEnlightenerCharacter::ArmDisarmWeaponComplete()
{
	SetActionState(EActionState::EAS_UNOCCUPIED);
}

