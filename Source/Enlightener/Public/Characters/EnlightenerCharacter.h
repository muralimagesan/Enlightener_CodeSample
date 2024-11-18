// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "InputAction.h"
#include "Characters/CharacterTypes.h"
#include "EnlightenerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class UAnimMontage;

UCLASS()
class ENLIGHTENER_API AEnlightenerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnlightenerCharacter();

	/* <APawn> */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/* </APawn> */

	/* <ACharacter> */
	virtual void Jump() override;
	/* </ACharacter> */

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* EnlightenerContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AttackActionCombo;

	/* Callbacks for Enhanced Input	*/
	void Move(const FInputActionValue& Value);	
	void Look(const FInputActionValue& Value);
	void Equip(const FInputActionValue& Value);
	void PlayerAttack(const FInputActionValue& Value);
	void PlayerAttackCombo(const FInputActionValue& Value);

	/* Combat*/
	void EquipWeapon(AWeapon* Weapon);
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	bool CanArmWeapon();
	bool CanDisarmWeapon();
	void PlayArmWeaponMontage(const FName& SectionName);
	void PlayAttackComboMontage();

	UFUNCTION(BlueprintCallable)
	void DisarmWeapon();

	UFUNCTION(BlueprintCallable)
	void ArmWeapon();

	UFUNCTION(BlueprintCallable)
	void ArmDisarmWeaponComplete();

private:
	/* Character Components */
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Hair")
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category = "Hair")
	UGroomComponent* Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	/* Animation Montages */
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* ArmWeaponMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> AttackComboMontageSections;

	ECharacterState CharacterState = ECharacterState::ECS_UNEQUIPPED;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_UNOCCUPIED;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE void SetCharacterState(ECharacterState State) { CharacterState = State; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE void SetActionState(EActionState State) {  ActionState = State; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};

