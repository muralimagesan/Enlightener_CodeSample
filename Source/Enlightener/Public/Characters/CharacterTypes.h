#pragma once

// class makes it a scoped enum - so to access the enum, we would need to use the qualifier ECharacterState::
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_UNEQUIPPED UMETA(DisplayName = "Unequipped"),
	ECS_EQUIPPED_ONE_HANDED_WEAPON UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EQUIPPED_TWO_HANDED_WEAPON UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_UNOCCUPIED UMETA(DisplayName = "Unoccupied"),
	EAS_ARM_DISARM_WEAPON UMETA(DisplayName = "Arm / Disarm Weapon"),
	EAS_ATTACKING UMETA(DisplayName = "Attacking"),
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_DEAD UMETA(DisplayName = "Dead"),
	EES_PATROLLING UMETA(DisplayName = "Patrolling"),
	EES_CHASING UMETA(DisplayName = "Chasing"),
	EES_ATTACKING UMETA(DisplayName = "Attacking"),
	EES_ENGAGED UMETA(DisplayName = "Engaged"),
	
	EES_NOSTATE UMETA(DisplayName = "NoState")
};