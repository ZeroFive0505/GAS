// Fill out your copyright notice in the Description page of Project Settings.


#include "GC/ABGC_AttackHit.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

UABGC_AttackHit::UABGC_AttackHit()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionRef(TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));

	if(ExplosionRef.Object)
	{
		ParticleSystem = ExplosionRef.Object;
	}
}

bool UABGC_AttackHit::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	bool bResult = Super::OnExecute_Implementation(MyTarget, Parameters);

	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();

	if(HitResult)
	{
		UGameplayStatics::SpawnEmitterAtLocation(MyTarget, ParticleSystem, HitResult->ImpactPoint, FRotator::ZeroRotator, true);
	}
	else
	{
		for(const auto& TargetActor : Parameters.EffectContext.Get()->GetActors())
		{
			if(TargetActor.Get())
			{
				UGameplayStatics::SpawnEmitterAtLocation(MyTarget, ParticleSystem, TargetActor.Get()->GetActorLocation(), FRotator::ZeroRotator, true);
			}
		}
	}
	
	return bResult;
}
