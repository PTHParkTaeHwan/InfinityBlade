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
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const&DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;
	
	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UIBCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;
	
public:
	bool GetIsRun();
	bool GetIsDefense();
	
	//무기 아이템 습득
	bool CanSetWeapon();
	void SetWeapon(class AIBWeapon* NewWeapon);
		
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class AIBWeapon* CurrentWeapon;

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
	bool IsDefense;

	//공격 모션
	void Attack();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	//공격모션
	void AttackStartComboState();
	void AttackEndComboState();

	//공격 충돌처리
	void AttackCheck();


private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;
			
	UPROPERTY()
	class UIBAnimInstance* IBAnim;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;

//파티클 시스템
public:
	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* FirstHitEffect;

};
