// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/TA/ABTA_Trace.h"
#include "ABTA_SphereTrace.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABTA_SphereTrace : public AABTA_Trace
{
	GENERATED_BODY()

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;
};
