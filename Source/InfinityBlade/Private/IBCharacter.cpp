// Fill out your copyright notice in the Description page of Project Settings.

#include "IBCharacter.h"
#include "IBAnimInstance.h"
#include "DrawDebugHelpers.h"

// Sets default values
AIBCharacter::AIBCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Pawn에 캡슐, 메쉬, 무브먼트를 가져올 수 있는 함수가 있기 때문에 스프링 암과 카메라만 따로 만들어준다.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CardBoard(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Bladed.SK_CharM_Bladed"));
	if (CardBoard.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CardBoard.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	//애니메이션 인스턴스 가져오기
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Book/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	//무기 부착하기
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_HeroSword22/SK_Blade_HeroSword22.SK_Blade_HeroSword22"));
		if (SK_WEAPON.Succeeded())
		{
			Weapon->SetSkeletalMesh(SK_WEAPON.Object);
		}

		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}

	//카메라 모드 전환
	SetControlMode(EControlMode::GTA);
	ArmLengthSpeed = 3.0f;
	ArmLocationSpeed = 5.0f;
	CameraLocationSpeed = 1.5f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	//걷기 모드 전환
	IsRun = false;
	CurrentShiftButtonOn = false;
	GetCharacterMovement()->MaxWalkSpeed = 400;

	//공격 모션 관리
	IsAttacking = false;

	//공격 콤포 관리
	MaxCombo = 4;
	AttackEndComboState();

	//콜리전 프리셋 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("IBCharacter"));

	//공격 범위 디버그
	AttackRange = 250.0f;
	AttackRadius = 90.0f;
}

// Called when the game starts or when spawned
void AIBCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AIBCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;
	
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		ArmLengthTo = 450.0f;
		CameraLocationTo = FVector(0.0f, 0.0f, 0.0f);
		//ArmLocationTo = FVector(0.0f, 0.0f, 0.0f);
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		//Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		break;
	case EControlMode::DEFENSE:
		ArmLengthTo = 180.0f;
		CameraLocationTo = FVector(0.0f, -30.0f, 50.0f);
		//ArmLocationTo = FVector(0.0f, -30.0f, 50.0f);
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		//Camera->SetRelativeLocation(FVector(0.0f, -30.0f, 50.0f));
		break;
	}

}


// Called every frame
void AIBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);
	//SpringArm->RelativeLocation = FMath::VInterpTo(SpringArm->RelativeLocation, ArmLocationTo, DeltaTime, ArmLocationSpeed);
	Camera->RelativeLocation = FMath::VInterpTo(Camera->RelativeLocation, CameraLocationTo, DeltaTime, CameraLocationSpeed);

	switch (CurrentControlMode)
	{
	case AIBCharacter::EControlMode::GTA:
		break;
	case AIBCharacter::EControlMode::DEFENSE:
		break;
	}

	RunChange();

}

void AIBCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	IBAnim = Cast<UIBAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != IBAnim);
	//매크로를 활용해 애님 인스턴스의 OnMontageEnded 델리게이트와 우리가 선언한 OnAttackMontageEnded를 연결한다.
	IBAnim->OnMontageEnded.AddDynamic(this, &AIBCharacter::OnAttackMontageEnded);

	IBAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		ABLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			IBAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	IBAnim->OnAttackHitCheck.AddUObject(this, &AIBCharacter::AttackCheck);
}

float AIBCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("%s , %f"), *GetName(), FinalDamage);

	if (FinalDamage > 0.0f)
	{
		IBAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	}

	return FinalDamage;
}

// Called to bind functionality to input
void AIBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AIBCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AIBCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AIBCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AIBCharacter::Turn);
	PlayerInputComponent->BindAction(TEXT("DefenseMode"), EInputEvent::IE_Pressed, this, &AIBCharacter::ModeChange);
	PlayerInputComponent->BindAction(TEXT("DefenseMode"), EInputEvent::IE_Released, this, &AIBCharacter::ModeChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AIBCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Runing"), EInputEvent::IE_Pressed, this, &AIBCharacter::ShiftButtonChange);
	PlayerInputComponent->BindAction(TEXT("Runing"), EInputEvent::IE_Released, this, &AIBCharacter::ShiftButtonChange);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AIBCharacter::Attack);

}
bool AIBCharacter::GetIsRun()
{
	return IsRun;
}
void AIBCharacter::UpDown(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case AIBCharacter::EControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
		break;
	case AIBCharacter::EControlMode::DEFENSE:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
		break;
	}

}
void AIBCharacter::LeftRight(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case AIBCharacter::EControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
		break;
	case AIBCharacter::EControlMode::DEFENSE:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
		break;
	}

}
void AIBCharacter::LookUp(float NewAxisValue)
{
	//Y축
	AddControllerPitchInput(NewAxisValue);
}
void AIBCharacter::Turn(float NewAxisValue)
{
	//Z축
	AddControllerYawInput(NewAxisValue);
}
void AIBCharacter::ModeChange()
{
	if (CurrentControlMode == EControlMode::GTA)
	{
//GetController()->SetControlRotation(GetActorRotation());
SetControlMode(EControlMode::DEFENSE);
	}
	else if (CurrentControlMode == EControlMode::DEFENSE)
	{
	//GetController()->SetControlRotation(SpringArm->RelativeRotation);
	SetControlMode(EControlMode::GTA);
	}
}

void AIBCharacter::RunChange()
{
	if (CurrentShiftButtonOn && this->GetVelocity().Size() > 0)
	{
		IsRun = true;
		GetCharacterMovement()->MaxWalkSpeed = 600;
	}
	else if (!CurrentShiftButtonOn && this->GetVelocity().Size() > 0)
	{
		IsRun = false;
		GetCharacterMovement()->MaxWalkSpeed = 400;
	}
	else
	{
		IsRun = false;
		GetCharacterMovement()->MaxWalkSpeed = 400;
	}
}

void AIBCharacter::ShiftButtonChange()
{
	if (!CurrentShiftButtonOn)
	{
		CurrentShiftButtonOn = true;
	}
	else if (CurrentShiftButtonOn)
	{
		CurrentShiftButtonOn = false;
	}
}

void AIBCharacter::Attack()
{
	if (IsAttacking)
	{
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		ABCHECK(CurrentCombo == 0);
		AttackStartComboState();
		IBAnim->PlayAttackMontage();
		IBAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void AIBCharacter::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	ABCHECK(IsAttacking);
	ABCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
}

void AIBCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AIBCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AIBCharacter::AttackCheck()
{
	
	FCollisionQueryParams Params(NAME_None, false, this);
	TArray<FHitResult> HitResults;
	bool bResults = GetWorld()->SweepMultiByChannel(HitResults,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResults ? FColor::Green : FColor::Red;
	float DebugLifeTime = 2.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif
	if (bResults)
	{
		for (FHitResult HitResult : HitResults)
		{
			ABLOG(Warning, TEXT("hit! %s"), *HitResult.Actor->GetName());

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(50.0f, DamageEvent, GetController(), this);
		}
	}
}



