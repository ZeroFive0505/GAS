// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/ABTA_SphereTrace.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ArenaBattleGAS.h"
#include "Attribute/ABCharacterSkillAttributeSet.h"
#include "GameFramework/Character.h"
#include "Physics/ABCollision.h"

FGameplayAbilityTargetDataHandle AABTA_SphereTrace::MakeTargetData() const
{
	ABGAS_LOG(LogABGAS, Log, TEXT("Begin"));

	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* AbilitySystemComponent =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);

	if (!AbilitySystemComponent)
	{
		ABGAS_LOG(LogABGAS, Error, TEXT("ASC not Found!"));
		return FGameplayAbilityTargetDataHandle();
	}


	const UABCharacterSkillAttributeSet* SkillAttributeSet = AbilitySystemComponent->GetSet<UABCharacterSkillAttributeSet>();

	if(!SkillAttributeSet)
	{
		ABGAS_LOG(LogABGAS, Error, TEXT("SkillAttribute not found"));
		return FGameplayAbilityTargetDataHandle();
	}
	
	TArray<FOverlapResult> OverlapResults;
	const float SkillRadius = SkillAttributeSet->GetSkillRange();

	FVector Origin = Character->GetActorLocation();
	FCollisionQueryParams Params(SCENE_QUERY_STAT(AABTA_SphereTrace), false, Character);
	GetWorld()->OverlapMultiByChannel(OverlapResults, Origin, FQuat::Identity, CCHANNEL_ABACTION,
	                                  FCollisionShape::MakeSphere(SkillRadius), Params);

	TArray<TWeakObjectPtr<AActor>> HitActors;

	for (const FOverlapResult& OVerlap : OverlapResults)
	{
		AActor* HitActor = OVerlap.OverlapObjectHandle.FetchActor<AActor>();

		if (HitActor && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
		}
	}

	FGameplayAbilityTargetData_ActorArray* Actors = new FGameplayAbilityTargetData_ActorArray();
	Actors->SetActors(HitActors);

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FColor DrawColor = HitActors.Num() > 0 ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), Origin, SkillRadius, 16, DrawColor, false, 5.0f);
	}

#endif
	return FGameplayAbilityTargetDataHandle(Actors);
}
