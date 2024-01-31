// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABGASCharacterNonPlayer.h"

#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterAttributeSet.h"
#include "UI/ABGASWidgetComponent.h"

AABGASCharacterNonPlayer::AABGASCharacterNonPlayer()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UABCharacterAttributeSet>(TEXT("Attribute"));

	HpBar = CreateDefaultSubobject<UABGASWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_HpBar.WBP_HpBar_C"));

	if(HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(200.0f, 20.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	Level = 1.0f;
}

UAbilitySystemComponent* AABGASCharacterNonPlayer::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AABGASCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(InitStatEffect, Level, EffectContextHandle);

	if(EffectSpecHandle.IsValid())
	{
		AbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}

void AABGASCharacterNonPlayer::OnOutOfHealth()
{
	SetDead();
}
