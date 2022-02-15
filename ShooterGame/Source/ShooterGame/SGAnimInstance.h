// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SGAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API USGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	// �������Ʈ���� �����ؼ� ������Ʈ(Tick) �Լ�ó�� �� �Լ�
	UFUNCTION(BlueprintCallable)
		void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;


private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class AWraith* Wraith;

	// ĳ���� ���ǵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float Speed;

	// ĳ���Ͱ� ���߿� �ִ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool bIsInAir;

	// ĳ���Ͱ� �����̰� �ִ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool bIsAccelerating;

	// �͸����� �ϱ� ���� Yaw offset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float MovementOffsetYaw;

	// stop�ִϸ��̼ǿ� �� �������� ����ϰ� �ֱ����� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float LastMovementOffsetYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool bAiming;

};
