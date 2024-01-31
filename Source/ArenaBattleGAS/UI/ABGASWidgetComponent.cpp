// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABGASWidgetComponent.h"

#include "ABGASUserWidget.h"

void UABGASWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UABGASUserWidget* UserWidget = Cast<UABGASUserWidget>(GetWidget());
	if(UserWidget)
	{
		UserWidget->SetAbilitySystemComponent(GetOwner());
	}
}
