// Fill out your copyright notice in the Description page of Project Settings.


#include "SpyroCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASpyroCharacter::ASpyroCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	CameraMain = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Main"));
	CameraMain->SetupAttachment(SpringArm);
	CameraMain->bUsePawnControlRotation = false;
	
}

// Called when the game starts or when spawned
void ASpyroCharacter::BeginPlay()
{
	Super::BeginPlay();

	CharacterMovementComponent = GetCharacterMovement(); //get the characterMovement
	DirectionMove = FVector(0,0,0);
}

// Called every frame
void ASpyroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CharacterMovementComponent == nullptr) { return; }

	if (CharacterMovementComponent->IsMovingOnGround())
	{
		StopGliding();
		CanGlide = true;
	}
	

}

// Called to bind functionality to input
void ASpyroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASpyroCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveHorizontal", this, &ASpyroCharacter::MoveHorizontal);

	PlayerInputComponent->BindAxis("LookVertical", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookHorizontal", this, &APawn::AddControllerYawInput);


	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ASpyroCharacter::CallJumpEvent);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ASpyroCharacter::CallEndJumpEvent);
}




void ASpyroCharacter::MoveForward(float axis)
{
	if (Controller != nullptr && axis != 0)
	{
		//const FRotator Rotation = Controller->GetControlRotation(); //same as telling the camera rotation
		const FRotator Rotation = CameraMain->GetComponentRotation(); 
		const FRotator Yaw = FRotator(0, Rotation.Yaw, 0);
		FRotator newRot;
		if (axis > 0.f) {

			newRot = UKismetMathLibrary::FindLookAtRotation(CameraMain->GetComponentLocation(), GetActorLocation());
			newRot.Pitch = 0.f;
			newRot.Roll = 0.f;
		}
		else if(axis < 0.f)
		{
			newRot = UKismetMathLibrary::FindLookAtRotation(CameraMain->GetComponentLocation() * (-1), GetActorLocation() *  ( - 1));
			newRot.Pitch = 0.f;
			newRot.Roll = 0.f;
			
		}
	

		AddMovementInput(CameraMain->GetForwardVector(), axis); //move object
		FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), newRot, GetWorld()->GetDeltaSeconds(), TurnRate); //rotate body
		SetActorRotation(CurrentRotation);
		
	}
	
}

void ASpyroCharacter::MoveHorizontal(float axis)
{
	if (Controller != nullptr && axis != 0)
	{
		
		const FRotator Rotation = CameraMain->GetComponentRotation();
		const FRotator Yaw = FRotator(0, Rotation.Yaw, 0);

		AddMovementInput(CameraMain->GetRightVector(), axis);

		
		FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), (CameraMain->GetRightVector()*axis).Rotation(), GetWorld()->GetDeltaSeconds(), TurnRate); //rotate body
		SetActorRotation(CurrentRotation);
		
	}
}

void ASpyroCharacter::CallJumpEvent()
{
	//JumpEvent();
	Jump();
	if (IsAirbornAfterJump() && CanGlide)
	{
		Glide();
		CanGlide = false;
	}
	else 
	{
		StopGliding();
	}
}

void ASpyroCharacter::CallEndJumpEvent()
{
	//StopJumpEvent();
	StopJumping();
}



void ASpyroCharacter::Glide()
{
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->Velocity.Z = 0.f;
		CharacterMovementComponent->AirControl = AirControlGlide;
		CharacterMovementComponent->GravityScale = GravityScaleGlide;
	}
}

bool ASpyroCharacter::IsAirbornAfterJump()
{
	bool state = false;
	if(CharacterMovementComponent)
	{
		state = CharacterMovementComponent->IsFalling();
	}

	return state;
}

void ASpyroCharacter::StopGliding()
{
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->AirControl = 0.2f; //default value
		CharacterMovementComponent->GravityScale = 1.f;
	}
}

//Testing
void ASpyroCharacter::TestForwardMove(float axis)
{
	DirectionMove.Z = axis;
}

//Testing
void ASpyroCharacter::RotateTowardsMoveDirection()
{
	DirectionMove.Normalize();

	if (DirectionMove.Length() >= 0.1f)
	{
		float targetAngle = FMath::Atan2(DirectionMove.X, DirectionMove.Z);
		targetAngle = FMath::RadiansToDegrees(targetAngle);
		targetAngle += CameraMain->GetComponentRotation().Yaw;
		FRotator GoalRotation = FRotator(0, targetAngle, 0);
		FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), GoalRotation, GetWorld()->GetDeltaSeconds(), TurnRate);
		SetActorRotation(CurrentRotation);
	}
}

//Testing
void ASpyroCharacter::TestHorizontalMove(float axis)
{
	DirectionMove.X = axis;
}



