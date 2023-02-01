// Fill out your copyright notice in the Description page of Project Settings.


#include "SpyroCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ASpyroCharacter::ASpyroCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;


	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	BodyMesh->SetupAttachment(RootComponent);

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

}

// Called every frame
void ASpyroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


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

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed,this, &ASpyroCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ASpyroCharacter::StopJumping);

}


void ASpyroCharacter::MoveForward(float axis)
{
	if (Controller != nullptr && axis != 0)
	{
		//const FRotator Rotation = Controller->GetControlRotation(); //same as telling the camera rotation
		const FRotator Rotation = CameraMain->GetComponentRotation(); 
		const FRotator Yaw = FRotator(0, Rotation.Yaw, 0);
		//const FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X); //direction in which we want the plyaer to move

		//MySolution
		FRotator newRot = UKismetMathLibrary::FindLookAtRotation(CameraMain->GetComponentLocation(),GetActorLocation());
		newRot.Pitch = 0.f;
		newRot.Roll = 0.f;

		AddMovementInput(CameraMain->GetForwardVector(), axis); //move object
		FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), newRot, GetWorld()->GetDeltaSeconds(), 10.0f); //rotate body
		SetActorRotation(CurrentRotation);
		//End

		/*FRotator NewRotation = FMath::RInterpTo(BodyMesh->GetComponentRotation(), Direction.Rotation(), GetWorld()->GetDeltaSeconds(), 10.f);
		BodyMesh->SetWorldRotation(NewRotation);

		AddMovementInput(Direction, axis);*/
		
		//DrawDebugLine(GetWorld(), Yaw.Vector(), Yaw.Vector() * 10000.f, FColor::Blue, false,3.f);
		
		

		
	}
	
}

void ASpyroCharacter::MoveHorizontal(float axis)
{
	if (Controller != nullptr && axis != 0)
	{
		//const FRotator Rotation = Controller->GetControlRotation();
		const FRotator Rotation = CameraMain->GetComponentRotation();
		const FRotator Yaw = FRotator(0, Rotation.Yaw, 0);

		AddMovementInput(CameraMain->GetRightVector(), axis);

		//FRotator newRot = UKismetMathLibrary::FindLookAtRotation(CameraMain->GetComponentLocation(), GetActorLocation());
		/*newRot.Pitch = 0.f;
		newRot.Roll = 0.f;*/
		FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), (CameraMain->GetRightVector()*axis).Rotation(), GetWorld()->GetDeltaSeconds(), 10.0f); //rotate body
		SetActorRotation(CurrentRotation);
		//const FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y); //direction in which we want the plyaer to move
		//AddMovementInput(Direction, axis);
		
		/*FRotator NewRotation = FMath::RInterpTo(BodyMesh->GetComponentRotation(), Direction.Rotation(), GetWorld()->GetDeltaSeconds(), 2);
		BodyMesh->SetWorldRotation(NewRotation);*/
	}
}



void ASpyroCharacter::Jump() 
{
	ACharacter::Jump();
}

