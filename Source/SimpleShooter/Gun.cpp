// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root Component");
	SetRootComponent(SceneComponent);
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Gun Mesh");
	GunMesh->SetupAttachment(SceneComponent);
	// ProjectileSpwanPiont = CreateDefaultSubobject<USceneComponent>("Projectile Spwan Piont");
	// ProjectileSpwanPiont->SetupAttachment(GunMesh);
}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunMesh, TEXT("MuzzleFlashSocket"));
	Shooter = Cast<APawn>(GetOwner());
	if (Shooter == nullptr) return;
	ShooterController = Shooter->GetController();
	if (ShooterController == nullptr) return;
	ShooterController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
	ViewPointEnd = ViewPointLocation + ViewPointRotation.Vector() * MaxRange;
	
	FHitResult Hit;
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, ViewPointLocation, ViewPointEnd, ECollisionChannel::ECC_GameTraceChannel1);
	if (bSuccess)
	{
		FVector ShooterDirection = -ViewPointRotation.Vector();
		// DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		//在击中位置生成特效
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShooterDirection.Rotation());
		// DrawDebugLine(GetWorld(), ViewPointLocation, Hit.Location, FColor::Red, true);
		
		//仅对击中敌人造成伤害
		// APawn* HitActor = Cast<APawn>(Hit.GetActor());
		
		//对世界Actor都能造成伤害
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(DamageAmuont, Hit, ShooterDirection, nullptr);
			HitActor->TakeDamage(DamageAmuont, DamageEvent, ShooterController, this);
		}
	}
	// DrawDebugCamera(GetWorld(), ViewPointLocation, ViewPointRotation, 90, 2, FColor::Red, true);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

