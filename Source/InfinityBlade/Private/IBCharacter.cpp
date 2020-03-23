// Fill out your copyright notice in the Description page of Project Settings.

#include "IBCharacter.h"
#include "IBAnimInstance.h"
#include "IBWeapon.h"
#include "IBCharacterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "IBCharacterWidget.h"

// Sets default values
AIBCharacter::AIBCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Pawn�� ĸ��, �޽�, �����Ʈ�� ������ �� �ִ� �Լ��� �ֱ� ������ ������ �ϰ� ī�޶� ���� ������ش�.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UIBCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CardBoard(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Bladed.SK_CharM_Bladed"));
	if (CardBoard.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CardBoard.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	//�ִϸ��̼� �ν��Ͻ� ��������
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Book/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	//ĳ�� �����Ǹ鼭 ���� �����ϱ�
	/*FName WeaponSocket(TEXT("hand_rSocket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_HeroSword22/SK_Blade_HeroSword22.SK_Blade_HeroSword22"));
		if (SK_WEAPON.Succeeded())
		{
			Weapon->SetSkeletalMesh(SK_WEAPON.Object);
		}

		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}*/

	//ī�޶� ��� ��ȯ
	SetControlMode(EControlMode::GTA);
	ArmLengthSpeed = 3.0f;
	ArmLocationSpeed = 5.0f;
	CameraLocationSpeed = 1.5f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	//�ȱ� ��� ��ȯ
	IsRun = false;
	CurrentShiftButtonOn = false;
	GetCharacterMovement()->MaxWalkSpeed = 400;
	
	

	//���� ��� ����
	IsAttacking = false;

	//���� �޺� ����
	MaxCombo = 4;
	AttackEndComboState();

	//�ݸ��� ������ ����
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("IBCharacter"));

	//���� ���� �����
	AttackRange = 250.0f;
	AttackRadius = 90.0f;

	//��ƼŬ �ý���
	FirstHitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HITEFFECT"));
	FirstHitEffect->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HIT(TEXT("/Game/InfinityBladeEffects/Effects/FX_Combat_Base/Impact/P_ImpactSpark.P_ImpactSpark"));
	if (P_HIT.Succeeded())
	{
		FirstHitEffect->SetTemplate(P_HIT.Object);
		FirstHitEffect->bAutoActivate = false;
	}

	//HP, SE UI
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/Book/UI/UI_HPUIBar.UI_HPUIBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}
}

// Called when the game starts or when spawned
void AIBCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	////���� ���ͷ� �����ϱ�
	//FName WeaponSocket(TEXT("hand_rSocket"));
	//auto CurWeapon = GetWorld()->SpawnActor<AIBWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	//if (nullptr != CurWeapon)
	//{
	//	CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	//}
}

void AIBCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;
	
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		IsDefense = false;
		ArmLengthTo = 450.0f;
		CameraLocationTo = FVector(0.0f, 0.0f, 0.0f);
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::DEFENSE:
		IsDefense = true;
		ArmLengthTo = 180.0f;
		CameraLocationTo = FVector(0.0f, -30.0f, 50.0f);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	}
}

// Called every frame
void AIBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);
	Camera->RelativeLocation = FMath::VInterpTo(Camera->RelativeLocation, CameraLocationTo, DeltaTime, CameraLocationSpeed);

	RunChange();

	switch (CurrentControlMode)
	{
	case AIBCharacter::EControlMode::DEFENSE:
		break;
	}
	//ABLOG(Warning, TEXT("%d"), GetCharacterMovement()->);
	
}

void AIBCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	IBAnim = Cast<UIBAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != IBAnim);
	//��ũ�θ� Ȱ���� �ִ� �ν��Ͻ��� OnMontageEnded ��������Ʈ�� �츮�� ������ OnAttackMontageEnded�� �����Ѵ�.
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

	CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
		ABLOG(Warning, TEXT("OnHPIsZero"));
		IBAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	});

	auto CharacterWidget = Cast<UIBCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}

}

float AIBCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("%s , %f"), *GetName(), FinalDamage);
	FirstHitEffect->Activate(true);
	
	switch (CurrentControlMode)
	{
	case AIBCharacter::EControlMode::GTA:
		CharacterStat->SetDamage(FinalDamage);
		break;
	case AIBCharacter::EControlMode::DEFENSE:
		CharacterStat->SetDamage(FinalDamage/2.0f);
		break;
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
bool AIBCharacter::GetIsDefense()
{
	return IsDefense;
}
bool AIBCharacter::CanSetWeapon()
{
	return (nullptr == CurrentWeapon);
}
void AIBCharacter::SetWeapon(AIBWeapon * NewWeapon)
{
	ABCHECK(nullptr != NewWeapon && nullptr == CurrentWeapon);
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}

}
void AIBCharacter::UpDown(float NewAxisValue)
{
	if(!IsAttacking) AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
	else AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue/100);
}
void AIBCharacter::LeftRight(float NewAxisValue)
{
	if (!IsAttacking) AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
	else  AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue/100);
}
void AIBCharacter::LookUp(float NewAxisValue)
{
	//Y��
	AddControllerPitchInput(NewAxisValue);
}
void AIBCharacter::Turn(float NewAxisValue)
{
	//Z��
	AddControllerYawInput(NewAxisValue);
}
void AIBCharacter::ModeChange()
{
	if (CurrentControlMode == EControlMode::GTA)
	{
		SetControlMode(EControlMode::DEFENSE);
	}
	else if (CurrentControlMode == EControlMode::DEFENSE)
	{
		SetControlMode(EControlMode::GTA);
	}
}

void AIBCharacter::RunChange()
{
	switch (CurrentControlMode)
	{
	case AIBCharacter::EControlMode::GTA:
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
		break;
	case AIBCharacter::EControlMode::DEFENSE:
		GetCharacterMovement()->MaxWalkSpeed = 200;
		break;
	}
	
}

void AIBCharacter::ShiftButtonChange()
{
	if (!CurrentShiftButtonOn)
	{
		CurrentShiftButtonOn = true;
		IsAttacking = false;
		AttackEndComboState();
		IBAnim->StopAttackMontage();
	}
	else if (CurrentShiftButtonOn)
	{
		CurrentShiftButtonOn = false;
	}
}

void AIBCharacter::Attack()
{
	switch (CurrentControlMode)
	{
	case AIBCharacter::EControlMode::GTA:
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
		break;
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
			if (CurrentCombo < 4)
			{
				FDamageEvent DamageEvent;
				HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
			}
			else
			{
				FDamageEvent DamageEvent;
				HitResult.Actor->TakeDamage(CharacterStat->GetAttack()*2, DamageEvent, GetController(), this);
			}
		}
	}
}



