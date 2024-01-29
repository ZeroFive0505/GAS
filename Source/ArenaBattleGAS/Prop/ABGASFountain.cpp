// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ABGASFountain.h"

#include "AbilitySystemComponent.h"
#include "ArenaBattleGAS.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Tag/ABGameplayTag.h"

AABGASFountain::AABGASFountain()
{
	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotateMovement"));
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ActionPeriod = 3.0f;
}

UAbilitySystemComponent* AABGASFountain::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AABGASFountain::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RotatingMovementComponent->bAutoActivate =false;
	RotatingMovementComponent->Deactivate();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);


	for(const auto& StartAbility : StartAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(StartAbility);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

void AABGASFountain::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &AABGASFountain::TimerAction, ActionPeriod, true, 0.0f);
}

void AABGASFountain::TimerAction()
{
	ABGAS_LOG(LogABGAS, Log, TEXT("Begin"));

	FGameplayTagContainer TargetTag(ABTAG_ACTOR_ROTATE);

	if(!AbilitySystemComponent->HasMatchingGameplayTag(ABTAG_ACTOR_ISROTATING))
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(TargetTag);
	}
	else
	{
		AbilitySystemComponent->CancelAbilities(&TargetTag);
	}
}
