// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABGASUserWidget.h"

#include "AbilitySystemBlueprintLibrary.h"

void UABGASUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if(IsValid(InOwner))
	{
		AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}
}

UAbilitySystemComponent* UABGASUserWidget::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
