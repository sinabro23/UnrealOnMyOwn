// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Wraith.generated.h"

UCLASS()
class SHOOTERGAME_API AWraith : public ACharacter
{
	// 디폴트 클래스는 cpp기반으로 만든 블루프린트클래스 자체
	// EditDefulatOnly 클래스자체값에서만 바꿀수 있음
	// 인스턴스는 그 클래스로 월드에 배치해서 선택한 개개인
	// EditInstanceOnly 인스턴스로만 바꿀수 있음

	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWraith();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	// 키보드 방향키로 화면 무빙
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	
	// 마우스턴은 인풋컴포넌트에서 바로 넣었음
	// 점프도 캐릭터에서 제공하는 기능으로 바로 했음

	// 격발
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

	// 키보드 방향키
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
