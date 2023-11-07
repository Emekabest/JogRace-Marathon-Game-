// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainActor.generated.h"

class AMyTriggerBox;

UCLASS()
class JOGRACE_API AMainActor : public ACharacter
{
	GENERATED_BODY()

private: 

	/*Used in rotating the character to the right and left*/
	float RotationRightRate;
	float RotationLeftRate;


public:
	// Sets default values for this character's properties
	AMainActor();
	/*Varirable Declaration @ public..........................*/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;

	bool ActivateMoveLeftRight;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*Declaring an array containing all instance of 'AMyTriggerBoxes' in the level...*/
	TArray<AMyTriggerBox*> MyTriggerBoxes; //This Variable now contains all trigger boxs in the level.

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/////

	/*Responsible for moving the character to the left and right*/
	void TurnRight();
	void TurnLeft();
	//////


	void SetupRotation(float InputValue, float RotationRate);

	void MoveNextLaneRight();


};
