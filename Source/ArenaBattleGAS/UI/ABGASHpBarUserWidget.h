// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ABGASUserWidget.h"
#include "ABGASHpBarUserWidget.generated.h"

struct FGameplayTag;
struct FOnAttributeChangeData;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABGASHpBarUserWidget : public UABGASUserWidget
{
	GENERATED_BODY()

protected:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnInvincibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	void UpdateHpBar();
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PbHpBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TxtHpStat;
	
	float CurrentHealth;
	float CurrentMaxHealth = 0.1f;

	FLinearColor HealthColor = FLinearColor::Red;
	FLinearColor InvincibleColor = FLinearColor::Blue;
};
