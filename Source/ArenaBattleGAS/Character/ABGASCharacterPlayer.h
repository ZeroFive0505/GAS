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
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	void SetUpGASInputComponent();
	void GASInputPressed(int32 InputID);
	void GASInputReleased(int32 InputID);

	UFUNCTION()
	virtual void OnOutOfHealth();

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TMap<int32, TSubclassOf<UGameplayAbility>> StartInputAbilities;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UABGASWidgetComponent> HpBar;
};
