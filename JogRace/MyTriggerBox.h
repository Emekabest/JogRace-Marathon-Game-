// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Components/BoxComponent.h"
#include "MyTriggerBox.generated.h"

/**
 * 
 */

UCLASS()
class JOGRACE_API AMyTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
public:

	AMyTriggerBox();

	
	bool bActivateMoveLeftRight;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "TriggerBox")
	UBoxComponent* TriggerBox;

	/*Functions................*/
	
	/*Only activate when the actor enters into the trigger object.*/
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};