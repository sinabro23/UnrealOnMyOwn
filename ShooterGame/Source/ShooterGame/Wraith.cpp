// Fill out your copyright notice in the Description page of Project Settings.


#include "Wraith.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AWraith::AWraith()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.f, -80.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.f, 0.f));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.f; // 봉 길이.
	CameraBoom->bUsePawnControlRotation = true; // 컨트롤러에서 입력되는 움직임에 따라 회전함
	CameraBoom->SocketOffset = FVector(0.f, 60.f, 70.f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // attach camera to end of boom
	Camera->bUsePawnControlRotation = false; // camera does not rotate relative to arm

	
}

// Called when the game starts or when spawned
void AWraith::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWraith::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Value);
	}
}

void AWraith::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		// 외적?과 관련된것 같은데 잘 모르겠다.
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Value);
	}
}

void AWraith::Turn(float Value)
{
	//float TurnScaleFactor{};

	//if (bAiming)
	//{
	//	TurnScaleFactor = MouseAimingTurnRate;
	//}
	//else
	//{
	//	TurnScaleFactor = MouseHipTurnRate;
	//}
	AddControllerYawInput(Value );
}

void AWraith::LookUp(float Value)
{
	//float LookUpScaleFactor{};

	//if (bAiming)
	//{
	//	LookUpScaleFactor = MouseAimingLookUpRate;
	//}
	//else
	//{
	//	LookUpScaleFactor = MouseHipLookUpRate;
	//}
	AddControllerPitchInput(Value);
}

// Called every frame
void AWraith::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWraith::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AWraith::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AWraith::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AWraith::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AWraith::Turn);
}

