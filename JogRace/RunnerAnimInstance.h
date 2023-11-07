// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RunnerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class JOGRACE_API URunnerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties();

	virtual	void NativeInitializeAnimation() override;


	UPROPERTY(BlueprintReadOnly, Category = Animation);
	float MovementSpeed;

	class APawn* Pawn;
	
};
