// Fill out your copyright notice in the Description page of Project Settings.


#include "Wraith.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"

// Sets default values
AWraith::AWraith() : 
	BaseTurnRate(40.f),
	BaseLookUpRate(40.f),
	RangeFromBarrel(50'000.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.f, -80.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.f, 0.f));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 150.f; // 봉 길이.
	CameraBoom->bUsePawnControlRotation = true; // 컨트롤러에서 입력되는 움직임에 따라 회전함
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // attach camera to end of boom
	Camera->bUsePawnControlRotation = false; // camera does not rotate relative to arm


	// 컨트롤러가 돌때 같이 돌지// 모두 false면 컨트롤러는 카메라에만 영향을줌
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // 인풋 넣어주는 방향으로 캐릭터가 움직임
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f); // 이 방향값만큼 속도로 돌아
	GetCharacterMovement()->JumpZVelocity = 500.f; // 점프 힘
	GetCharacterMovement()->AirControl = 0.2f; // 공중에서 움직일수있는 힘
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
		const FRotator Rotation{ Controller->GetControlRotation() }; // 컨트롤러의 로테이션 기준으로 앞으로가야함
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 }; //컨트롤러의 Yaw값만 중요하니깐

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) }; // X가 언리얼기준 정면
		AddMovementInput(Direction, Value);
	}
}

void AWraith::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Value);
	}
}

void AWraith::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AWraith::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AWraith::FireWeapon()
{
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this, FireSound);
	}

	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName(FName("BarrelSocket"));
	if (BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());
		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}

		// 화면 가운데 위치를 구하기 위해.
		FVector ViewportSize;
		

		//FHitResult HitResult;
		//const FVector Start = SocketTransform.GetLocation();
		//// end를 구하기위해서는 정면방향인 X축으로 방향벡터를 구해서 길이만큼 곱해줘야함.
		//const FQuat Rotation = SocketTransform.GetRotation();
		//FVector RotationXAxis = Rotation.GetAxisX(); //************ 몰랐던것
		//const FVector End = Start + (RotationXAxis * RangeFromBarrel);

		//GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);
		//
		//if (HitResult.bBlockingHit) // 맞았을경우 bBlockingHit true
		//{
		//	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);
		//	DrawDebugPoint(GetWorld(), HitResult.Location, 5.f, FColor::Red, false, 2.f);
		//	
		//	if(ImpactParicle)
		//	{
		//		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParicle, HitResult.Location);
		//	}
		//}
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (FireWeaponMontage && AnimInstance)
	{
		// 몽타주를 애니메이션 중간에 디폴트 슬롯으로 가로채도 
		// 몽타주의 블렌드타임을 0으로 설정하지않으면 느리게 바뀌어서 이상했음.
		AnimInstance->Montage_Play(FireWeaponMontage);
		AnimInstance->Montage_JumpToSection(FName("FireStart")); // 다시 돌아오게 하는거구나
	}
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
	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &AWraith::TurnAtRate);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AWraith::LookUpAtRate);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("FireButton"), IE_Pressed, this, &AWraith::FireWeapon);
}

