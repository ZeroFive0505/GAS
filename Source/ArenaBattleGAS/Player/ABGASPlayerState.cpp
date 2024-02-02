// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABGASPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterAttributeSet.h"
#include "Attribute/ABCharacterSkillAttributeSet.h"

AABGASPlayerState::AABGASPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AbilitySystemComponent->SetIsReplicated(true);
	AttributeSet = CreateDefaultSubobject<UABCharacterAttributeSet>(TEXT("AttributeSet"));
	SkillAttributeSet = CreateDefaultSubobject<UABCharacterSkillAttributeSet>(TEXT("SkillAttributeSet"));
}

UAbilitySystemComponent* AABGASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
