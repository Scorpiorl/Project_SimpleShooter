// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* GunMesh;
	// UPROPERTY(VisibleAnywhere)
	// USceneComponent* ProjectileSpwanPiont;
	
	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;
	
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	FVector ViewPointEnd;
	APawn* Shooter;
	AController* ShooterController;
	float MaxRange = 1000.0f;

	UPROPERTY(EditAnywhere)
	float DamageAmuont = 10.0f;
	
};
