// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterPlayer.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "ABGASCharacterPlayer.generated.h"

class UABGASWidgetComponent;
/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABGASCharacterPlayer : public AABCharacterPlayer, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AABGASCharacterPlayer();
	FORCEINLINE virtual class UAnimMontage* GetSkillActionMontage() const { return SkillActionMontage; }
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	void SetUpGASInputComponent();
	void GASInputPressed(int32 InputID);
	void GASInputReleased(int32 InputID);

	UFUNCTION()
	virtual void OnOutOfHealth();

	void EquipWeapon(const FGameplayEventData* EventData);
	void UnEquipWeapon(const FGameplayEventData* EventData);

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TMap<int32, TSubclassOf<UGameplayAbility>> StartInputAbilities;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UABGASWidgetComponent> HpBar;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float WeaponRange;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float WeaponAttackRate;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayAbility> SkillAbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> SkillActionMontage;
};
