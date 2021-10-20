## Project_Simple_Shooter
**实现功能:**

1. Player Movement
2. Animation
3. Shooting
4. Health/Death
5. Enemy AI
6. Win/Lose Condition
   
### Player Movement

![InputSetting](https://raw.githubusercontent.com/Scorpiorl/Project_SimpleShooter/master/README_PICTURE/InputSetting_AddGamePad.png)

```C++
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis("LookRightRate", this, &AShooterCharacter::LookRightRate);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotateRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotateRate * GetWorld()->GetDeltaSeconds());
}
```
## Animation
**Mesh-->Skeleton<--Animations**
- **Bland Space**
	1. Locomotion![BS_Locomotion](https://raw.githubusercontent.com/Scorpiorl/UrealProject_SimpleShooter/master/README_PICTURE/BS_Locomotion.png)
	2. Axis Setting![BS_AxisSetting](https://raw.githubusercontent.com/Scorpiorl/UrealProject_SimpleShooter/master/README_PICTURE/BS_AxisSetting.png)
- **Animation Blueprint**
	1. Animation Graph![ABP_AnimGraph](https://raw.githubusercontent.com/Scorpiorl/UrealProject_SimpleShooter/master/README_PICTURE/ABP_AnimGraph.png)
	2. Event Graph![ABP_EventGraph](https://raw.githubusercontent.com/Scorpiorl/UrealProject_SimpleShooter/master/README_PICTURE/ABP_EventGraph.png)

## Shooting
1. 创建一个Actor作为Gun
2. 设置Gun根节点以及Mesh
	```C++
	//Gun.h
	private:
		UPROPERTY(VisibleAnywhere)
		USceneComponent* SceneComponent;
		UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* GunMesh;
	```
	```C++
	//Gun.cpp
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
	```

3. 装备新Gun
	```C++
	//ShooterCharacter.cpp
	void AShooterCharacter::BeginPlay()
	{
		Super::BeginPlay();
		Gun = GetWorld()->SpawnActor<AGun>(GunClass);
		//将Character原有Gun隐藏
		GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
		//将新Gun附着到Character上
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weapon_rSocket"));
		//用来标识武器的主人
		Gun->SetOwner(this);
	}
	```
		FAttachmentTransformRules:
		- KeepRelativeTransform（保持两个物体之间的相对位置不变）
		- KeepWorldTransform（保持两个物体在世界的位置不变）
		- SnapToTargetNotIncludingScale（保持物体的缩放对齐到目标上）
		- SnapToTargetIncludingScale（随目标的缩放）
	参考:https://www.cnblogs.com/CodeWithMe/p/12930819.html

4. 设置Muzzle Flash
5. Player View Point & Line Trace By Channel

	**APlayerController::GetPlayerViewPoint & APawn::GetActorEyesViewPoint**

   - APlayerController::GetPlayerViewPoint
	
		Returns Player's Point of View. 

		For the AI, this means the Pawn's 'Eyes' ViewPoint. 

		For a Human player, this means the Camera's ViewPoint

   - APawn::GetActorEyesViewPoint
  	
		Returns the point of view of the actor.

		**注意:** 这并不意味着相机，而是演员的“眼睛”。例如，对于 Pawn，这将定义眼睛高度位置和视图旋转（这与具有零间距分量的 pawn 旋转不同）。相机第一人称视角通常会使用此视点。大多数跟踪（武器、AI）将从这个角度进行。
	
	**DrawDebugPoint & LineTraceSingleByChannel**
	```C++
	//Gun.h
	private:
		FVector ViewPointLocation;
		FRotator ViewPointRotation;
		FVector ViewPointEnd;
		APawn* Shooter;
		AController* ShooterController;
		float MaxRange = 1000.0f;
	//Gun.cpp
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
			DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		}
		// DrawDebugCamera(GetWorld(), ViewPointLocation, ViewPointRotation, 90, 2, FColor::Red, true);
	}
	```

	


