// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerAnimInstance.h"

void URunnerAnimInstance::NativeInitializeAnimation() {

	if (Pawn == nullptr) {

		
	}

}

void URunnerAnimInstance::UpdateAnimationProperties() {

	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
	}

	if (Pawn) {
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.0f);
		MovementSpeed = LateralSpeed.Size();
	

	}
}