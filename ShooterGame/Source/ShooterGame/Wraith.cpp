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
	CameraBoom->TargetArmLength = 150.f; // �� ����.
	CameraBoom->bUsePawnControlRotation = true; // ��Ʈ�ѷ����� �ԷµǴ� �����ӿ� ���� ȸ����
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // attach camera to end of boom
	Camera->bUsePawnControlRotation = false; // camera does not rotate relative to arm


	// ��Ʈ�ѷ��� ���� ���� ����// ��� false�� ��Ʈ�ѷ��� ī�޶󿡸� ��������
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // ��ǲ �־��ִ� �������� ĳ���Ͱ� ������
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f); // �� ���Ⱚ��ŭ �ӵ��� ����
	GetCharacterMovement()->JumpZVelocity = 500.f; // ���� ��
	GetCharacterMovement()->AirControl = 0.2f; // ���߿��� �����ϼ��ִ� ��
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
		const FRotator Rotation{ Controller->GetControlRotation() }; // ��Ʈ�ѷ��� �����̼� �������� �����ΰ�����
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 }; //��Ʈ�ѷ��� Yaw���� �߿��ϴϱ�

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) }; // X�� �𸮾���� ����
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

		// ȭ�� ��� ��ġ�� ���ϱ� ����.
		FVector ViewportSize;
		

		//FHitResult HitResult;
		//const FVector Start = SocketTransform.GetLocation();
		//// end�� ���ϱ����ؼ��� ��������� X������ ���⺤�͸� ���ؼ� ���̸�ŭ ���������.
		//const FQuat Rotation = SocketTransform.GetRotation();
		//FVector RotationXAxis = Rotation.GetAxisX(); //************ ��������
		//const FVector End = Start + (RotationXAxis * RangeFromBarrel);

		//GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);
		//
		//if (HitResult.bBlockingHit) // �¾������ bBlockingHit true
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
		// ��Ÿ�ָ� �ִϸ��̼� �߰��� ����Ʈ �������� ����ä�� 
		// ��Ÿ���� ����Ÿ���� 0���� �������������� ������ �ٲ� �̻�����.
		AnimInstance->Montage_Play(FireWeaponMontage);
		AnimInstance->Montage_JumpToSection(FName("FireStart")); // �ٽ� ���ƿ��� �ϴ°ű���
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

