// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prop/ABFountain.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "ABGASFountain.generated.h"

/**
 * 
 */
class URotatingMovementComponent;
UCLASS()
class ARENABATTLEGAS_API AABGASFountain : public AABFountain, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AABGASFountain();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void TimerAction();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	TObjectPtr<URotatingMovementComponent> RotatingMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Timer")
	float ActionPeriod;
	
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> StartAbilities;
	
	FTimerHandle ActionTimer;
};
