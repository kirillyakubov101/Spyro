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

	float DeltaTimeVariable = 0.f;

	UPROPERTY(EditDefaultsOnly,Category="Stats")
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float SprintSpeedMult;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float jumpForce;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	USkeletalMeshComponent* BodyMesh;

	void MoveForward(float axis);
	void MoveHorizontal(float axis);
	void LookVertical(float axis);
	void LookHorizontal(float axis);
	void Jump();
	


};
