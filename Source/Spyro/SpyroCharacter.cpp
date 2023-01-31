// Fill out your copyright notice in the Description page of Project Settings.


#include "SpyroCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASpyroCharacter::ASpyroCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	BodyMesh = GetMesh();

}

// Called when the game starts or when spawned
void ASpyroCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("This is spyro"));
	
}

// Called every frame
void ASpyroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->DeltaTimeVariable = DeltaTime;

}

// Called to bind functionality to input
void ASpyroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASpyroCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveHorizontal", this, &ASpyroCharacter::MoveHorizontal);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed,this, &ASpyroCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ASpyroCharacter::StopJumping);

}


void ASpyroCharacter::MoveForward(float axis)
{
	if (axis == 0.f) { return; }
	FVector Direction = GetActorForwardVector() * axis;
	AddMovementInput(Direction, DeltaTimeVariable * MoveSpeed);
	
}

void ASpyroCharacter::MoveHorizontal(float axis)
{
	if (axis == 0.f) { return; }
	FRotator LookGoalDirection = (GetActorRightVector() * axis).Rotation();
	FRotator Rotation = FMath::RInterpTo(BodyMesh->GetComponentRotation(), LookGoalDirection, DeltaTimeVariable,2.f);
	BodyMesh->SetWorldRotation(Rotation);
	//AddMovementInput(Direction, DeltaTimeVariable * MoveSpeed);
}

void ASpyroCharacter::LookVertical(float axis)
{
}

void ASpyroCharacter::LookHorizontal(float axis)
{
}

void ASpyroCharacter::Jump()
{
	ACharacter::Jump();
}

