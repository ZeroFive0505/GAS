// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABGASCharacterNonPlayer.h"

#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterAttributeSet.h"

AABGASCharacterNonPlayer::AABGASCharacterNonPlayer()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UABCharacterAttributeSet>(TEXT("Attribute"));
}

UAbilitySystemComponent* AABGASCharacterNonPlayer::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AABGASCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(InitStatEffect, Level, EffectContextHandle);

	if(EffectSpecHandle.IsValid())
	{
		AbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}
