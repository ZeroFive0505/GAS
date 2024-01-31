// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterNonPlayer.h"
#include "AbilitySystemInterface.h"
#include "ABGASCharacterNonPlayer.generated.h"

class UGameplayEffect;
class UABCharacterAttributeSet;
class UABGASWidgetComponent;
/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABGASCharacterNonPlayer : public AABCharacterNonPlayer, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AABGASCharacterNonPlayer();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION()
	virtual void OnOutOfHealth();

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UABCharacterAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> InitStatEffect;

	UPROPERTY(EditAnywhere, Category = "GAS")
	float Level = 1.0f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UABGASWidgetComponent> HpBar;
};
