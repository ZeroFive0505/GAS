// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/ABCharacterAttributeSet.h"

#include "ArenaBattleGAS.h"
#include "GameplayEffectExtension.h"

UABCharacterAttributeSet::UABCharacterAttributeSet() :
	AttackRange(100.0f),
	MaxAttackRange(300.0f),
	AttackRadius(50.0f),
	MaxAttackRadius(150.0f),
	AttackRate(30.0f),
	MaxAttackRate(100.0f),
	Health(100.0f),
	MaxHealth(100.0f),
	Damage(0.0f)
{
	InitHealth(GetMaxHealth());
}

void UABCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}

bool UABCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void UABCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		ABGAS_LOG(LogABGAS, Warning, TEXT("Direct Heatlh Access : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if(Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		ABGAS_LOG(LogABGAS, Log, TEXT("Damage : %f"), GetDamage());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), 0.0f, GetMaxHealth()));
		SetDamage(0.0f);
	}
}
