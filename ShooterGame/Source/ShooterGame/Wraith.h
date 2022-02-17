// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Wraith.generated.h"

UCLASS()
class SHOOTERGAME_API AWraith : public ACharacter
{
	// ����Ʈ Ŭ������ cpp������� ���� �������ƮŬ���� ��ü
	// EditDefulatOnly Ŭ������ü�������� �ٲܼ� ����
	// �ν��Ͻ��� �� Ŭ������ ���忡 ��ġ�ؼ� ������ ������
	// EditInstanceOnly �ν��Ͻ��θ� �ٲܼ� ����

	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWraith();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	// Ű���� ����Ű�� ȭ�� ����
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	
	// ���콺���� ��ǲ������Ʈ���� �ٷ� �־���
	// ������ ĳ���Ϳ��� �����ϴ� ������� �ٷ� ����

	// �ݹ�
	void FireWeapon();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	// Ű���� ����Ű
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* FireWeaponMontage;
};
