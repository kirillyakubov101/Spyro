// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpyroCharacter.generated.h"

UCLASS()
class SPYRO_API ASpyroCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpyroCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



private:

	//stats and components
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraMain;

	UPROPERTY(EditDefaultsOnly,Category="Stats")
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float SprintSpeedMult;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float jumpForce;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float TurnRate;

	//Gliding stats
	UPROPERTY(EditDefaultsOnly, Category = "Gliding")
	float GravityScaleGlide = 0.07f;

	UPROPERTY(EditDefaultsOnly, Category = "Gliding")
	float AirControlGlide = 0.7f;

	void MoveForward(float axis);
	void MoveHorizontal(float axis);

	//Test
	void TestForwardMove(float axis);
	void TestHorizontalMove(float axis);
	void RotateTowardsMoveDirection();
	FVector DirectionMove;

	void CallJumpEvent();
	void CallEndJumpEvent();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void JumpEvent();

	UFUNCTION(BlueprintImplementableEvent)
	void StopJumpEvent();


	
private:
	class UCharacterMovementComponent* CharacterMovementComponent;

	bool CanGlide = false;

	//Function for BlueprintUse
	UFUNCTION(BlueprintPure)
	bool IsAirbornAfterJump();

	UFUNCTION(BlueprintCallable)
	void StopGliding();

	UFUNCTION(BlueprintCallable)
	void Glide();
};
