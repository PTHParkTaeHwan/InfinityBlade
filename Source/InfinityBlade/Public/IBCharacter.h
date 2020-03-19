// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "GameFramework/Character.h"
#include "IBCharacter.generated.h"



UCLASS()
class INFINITYBLADE_API AIBCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AIBCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class EControlMode
	{
		GTA,
		DEFENSE
	};

	void SetControlMode(EControlMode NewControlMode);
	EControlMode CurrentControlMode = EControlMode::GTA;
	FVector DirectionToMove = FVector::ZeroVector;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;
	
public:
	bool GetIsRun();

private:
	//캐릭터 움직임
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	//카메라 움직임
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	
	//캐릭터 걷기 모션
	void ModeChange();
	void RunChange();
	void ShiftButtonChange();

	float ArmLengthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.0f;

	FVector ArmLocationTo = FVector::ZeroVector;
	float ArmLocationSpeed = 0.0f;

	FVector CameraLocationTo = FVector::ZeroVector;
	float CameraLocationSpeed = 0.0f;

	bool IsRun;
	bool CurrentShiftButtonOn;

	//공격 모션
	void Attack();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY()
	class UIBAnimInstance* IBAnim;

};
