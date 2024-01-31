// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABGASCharacterPlayer.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "Attribute/ABCharacterAttributeSet.h"
#include "Player/ABGASPlayerState.h"
#include "UI/ABGASWidgetComponent.h"

AABGASCharacterPlayer::AABGASCharacterPlayer()
{
	AbilitySystemComponent = nullptr;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

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
}

UAbilitySystemComponent* AABGASCharacterPlayer::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AABGASCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AABGASPlayerState* ABGASPlayerState = GetPlayerState<AABGASPlayerState>();

	if(ABGASPlayerState)
	{
		AbilitySystemComponent = ABGASPlayerState->GetAbilitySystemComponent();
		AbilitySystemComponent->InitAbilityActorInfo(ABGASPlayerState, this);

		const UABCharacterAttributeSet *AttributeSet = AbilitySystemComponent->GetSet<UABCharacterAttributeSet>();
		if(AttributeSet)
		{
			AttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);
		}
		
		for(const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			AbilitySystemComponent->GiveAbility(StartSpec);
		}

		for(const auto& StartInputAbility : StartInputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			AbilitySystemComponent->GiveAbility(StartSpec);
		}

		SetUpGASInputComponent();

		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}
}

void AABGASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetUpGASInputComponent();
}

void AABGASCharacterPlayer::SetUpGASInputComponent()
{
	if(IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AABGASCharacterPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AABGASCharacterPlayer::GASInputPressed, 1);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AABGASCharacterPlayer::GASInputReleased, 0);
	}
}

void AABGASCharacterPlayer::GASInputPressed(int32 InputID)
{
	FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromInputID(InputID);

	if(Spec)
	{
		Spec->InputPressed = true;

		if(Spec->IsActive())
		{
			AbilitySystemComponent->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			AbilitySystemComponent->TryActivateAbility(Spec->Handle);
		}
	}
}

void AABGASCharacterPlayer::GASInputReleased(int32 InputID)
{
	FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromInputID(InputID);

	if(Spec)
	{
		Spec->InputPressed = false;

		if(Spec->IsActive())
		{
			AbilitySystemComponent->AbilitySpecInputReleased(*Spec);
		}
		else
		{
			AbilitySystemComponent->TryActivateAbility(Spec->Handle);
		}
	}
}

void AABGASCharacterPlayer::OnOutOfHealth()
{
	SetDead();
}
