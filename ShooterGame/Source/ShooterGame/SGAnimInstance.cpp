// Fill out your copyright notice in the Description page of Project Settings.


#include "SGAnimInstance.h"
#include "Wraith.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USGAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	// NativeInitializeAnimation 에서 wraith를 불러오지 못했다면
	if (nullptr == Wraith)
	{
		Wraith = Cast<AWraith>(TryGetPawnOwner());
	}

	if (Wraith)
	{
		// 캐릭터의 속도
		FVector Velocity{ Wraith->GetVelocity() };
		Velocity.Z = 0; // 점프하거나 떨어지거나 할때의 속도는 넣지 않기 위해 0으로 설정
		Speed = Velocity.Size();

		// 떠있는지 체크
		bIsInAir = Wraith->GetCharacterMovement()->IsFalling();

		// 가속하고있는지 체크
		if (Wraith->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
		}

		//// 쳐다보고 있는 방향
		//FRotator AimRotation =
		//Wraith->GetBaseAimRotation();

		//FRotator MovementRotation =
		//	UKismetMathLibrary::MakeRotFromX(
		//		Wraith->GetVelocity());

		//MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(
		//	MovementRotation,
		//	AimRotation).Yaw;

		//if (Wraith->GetVelocity().Size() > 0)
		//{
		//	LastMovementOffsetYaw = MovementOffsetYaw;
		//}

		//bAiming = Wraith->GetAiming();
	}
}

void USGAnimInstance::NativeInitializeAnimation()
{
	Wraith = Cast<AWraith>(TryGetPawnOwner()); // 폰이 리턴되는 함수니깐 캐스트
}
