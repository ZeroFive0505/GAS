// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABGASPlayerState.h"

#include "AbilitySystemComponent.h"

AABGASPlayerState::AABGASPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AbilitySystemComponent->SetIsReplicated(true);
}

UAbilitySystemComponent* AABGASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
