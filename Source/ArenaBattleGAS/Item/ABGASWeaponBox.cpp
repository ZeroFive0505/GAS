// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABGASWeaponBox.h"

#include "AbilitySystemBlueprintLibrary.h"

void AABGASWeaponBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, WeaponEventTag, FGameplayEventData());
}
