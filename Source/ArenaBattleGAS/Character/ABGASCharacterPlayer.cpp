// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABGASCharacterPlayer.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "Attribute/ABCharacterAttributeSet.h"
#include "Player/ABGASPlayerState.h"
#include "Tag/ABGameplayTag.h"
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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blunt/Blunt_Hellhammer/SK_Blunt_HellHammer.SK_Blunt_HellHammer'"));

	if(WeaponMeshRef.Object)
	{
		WeaponMesh = WeaponMeshRef.Object;
	}

	WeaponRange = 75.0f;
	WeaponAttackRate = 100.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SkillActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_SkillAttack.AM_SkillAttack'"));

	if(SkillActionMontageRef.Object)
	{
		SkillActionMontage = SkillActionMontageRef.Object;
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

		AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(ABTAG_EVENT_CHARACTER_WEAPONEQUIP).AddUObject(this, &ThisClass::EquipWeapon);
		AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(ABTAG_EVENT_CHARACTER_WEAPONUNEQUIP).AddUObject(this, &ThisClass::UnEquipWeapon);

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
		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &AABGASCharacterPlayer::GASInputPressed, 2);
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

void AABGASCharacterPlayer::EquipWeapon(const FGameplayEventData* EventData)
{
	if(Weapon)
	{
		Weapon->SetSkeletalMesh(WeaponMesh);

		FGameplayAbilitySpec NewSKillSpec(SkillAbilityClass);
		NewSKillSpec.InputID = 2;

		if(!AbilitySystemComponent->FindAbilitySpecFromClass(SkillAbilityClass))
		{
			AbilitySystemComponent->GiveAbility(NewSKillSpec);
		}

		const float CurrentAttackRange = AbilitySystemComponent->GetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRangeAttribute());
		const float CurrentAttackRate = AbilitySystemComponent->GetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRateAttribute());

		AbilitySystemComponent->SetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRangeAttribute(), CurrentAttackRange + WeaponRange);
		AbilitySystemComponent->SetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRateAttribute(), CurrentAttackRate + WeaponAttackRate);
	}
}

void AABGASCharacterPlayer::UnEquipWeapon(const FGameplayEventData* EventData)
{
	if(Weapon)
	{
		Weapon->SetSkeletalMesh(nullptr);

		const float CurrentAttackRange = AbilitySystemComponent->GetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRangeAttribute());
		const float CurrentAttackRate = AbilitySystemComponent->GetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRateAttribute());

		AbilitySystemComponent->SetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRangeAttribute(), CurrentAttackRange - WeaponRange);
		AbilitySystemComponent->SetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRateAttribute(), CurrentAttackRate - WeaponAttackRate);

		FGameplayAbilitySpec* SkillAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(SkillAbilityClass);

		if(SkillAbilitySpec)
		{
			AbilitySystemComponent->ClearAbility(SkillAbilitySpec->Handle);
		}
	}
}
