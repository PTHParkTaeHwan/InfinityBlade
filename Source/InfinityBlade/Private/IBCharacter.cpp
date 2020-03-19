// Fill out your copyright notice in the Description page of Project Settings.

#include "IBCharacter.h"


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

	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Book/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	SetControlMode(EControlMode::GTA);

	ArmLengthSpeed = 3.0f;
	ArmLocationSpeed = 5.0f;
	CameraLocationSpeed = 1.5f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	IsRun = false;
	CurrentShiftButtonOn = false;
	GetCharacterMovement()->MaxWalkSpeed = 400;

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
	else if(!CurrentShiftButtonOn && this->GetVelocity().Size() > 0)
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



