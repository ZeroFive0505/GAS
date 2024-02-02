// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Skill.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/ABGASCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

UABGA_Skill::UABGA_Skill()
{
}

void UABGA_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AABGASCharacterPlayer* TargetCharacter =  Cast<AABGASCharacterPlayer>(ActorInfo->AvatarActor.Get());

	if(!TargetCharacter)
	{
		return;
	}

	ActiveSkillActionMontage = TargetCharacter->GetSkillActionMontage();

	if(!ActiveSkillActionMontage)
	{
		return;
	}

	TargetCharacter->GetCharacterMovement()->SetMovementMode(MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SkillMontage"), ActiveSkillActionMontage, 1.0f);
	PlayMontageAndWait->OnCompleted.AddDynamic(this, &ThisClass::OnCompleteCallback);
	PlayMontageAndWait->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptCallback);

	PlayMontageAndWait->ReadyForActivation();
}

void UABGA_Skill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AABGASCharacterPlayer* TargetCharacter =  Cast<AABGASCharacterPlayer>(ActorInfo->AvatarActor.Get());

	if(!TargetCharacter)
	{
		return;
	}
	
	TargetCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UABGA_Skill::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}

void UABGA_Skill::OnInterruptCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}
