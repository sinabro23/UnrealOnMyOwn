// Fill out your copyright notice in the Description page of Project Settings.


#include "SGAnimInstance.h"
#include "Wraith.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USGAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (nullptr == Wraith)
	{
		Wraith = Cast<AWraith>(TryGetPawnOwner());
	}

	if (Wraith)
	{
		// Get the lateral speed of the character from velocity
		FVector Velocity{ Wraith->GetVelocity() };
		Velocity.Z = 0; // 점프하거나 떨어지거나 할때의 속도는 넣지 않기 위해 0으로 설정
		Speed = Velocity.Size();

		// Is the Character in the air?
		bIsInAir = Wraith->GetCharacterMovement()->IsFalling();

		// Is The Character accelerating?
		if (Wraith->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
		}

		// 쳐다보고 있는 방향
		FRotator AimRotation =
			Wraith->GetBaseAimRotation();

		FRotator MovementRotation =
			UKismetMathLibrary::MakeRotFromX(
				Wraith->GetVelocity());

		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(
			MovementRotation,
			AimRotation).Yaw;

		if (Wraith->GetVelocity().Size() > 0)
		{
			LastMovementOffsetYaw = MovementOffsetYaw;
		}

		//bAiming = Wraith->GetAiming();
	}
}

void USGAnimInstance::NativeInitializeAnimation()
{
	Wraith = Cast<AWraith>(TryGetPawnOwner());

}

