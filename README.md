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
	Gun->AttachToComponent()
}
```
	FAttachmentTransformRules:
	- KeepRelativeTransform（保持两个物体之间的相对位置不变）
	- KeepWorldTransform（保持两个物体在世界的位置不变）
	- SnapToTargetNotIncludingScale（保持物体的缩放对齐到目标上）
	- SnapToTargetIncludingScale（随目标的缩放）
参考:https://www.cnblogs.com/CodeWithMe/p/12930819.html

	
