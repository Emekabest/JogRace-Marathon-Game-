// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTriggerBox.h"
#include "MainActor.h"
#include "Components/BoxComponent.h"

AMyTriggerBox::AMyTriggerBox(){

	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	bActivateMoveLeftRight = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(GetRootComponent());
	//RootComponent = TriggerBox;

	TriggerBox->SetCollisionProfileName(TEXT("TriggerBox"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMyTriggerBox::OnOverlapBegin);
	// Bind the OnOverlapBegin function to the OnComponentBeginOverlap event

	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AMyTriggerBox::OnOverlapEnd);
	// Bind the OnOverlapEnd function to the OnComponentEndOverlap event
	
}


/* Defining the OnEnterTriggerBox function..............................*/

void AMyTriggerBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


	// Check if the overlapping actor is your character
	if (OtherActor && OtherActor != this && OtherActor->IsA(AMainActor::StaticClass()))
	{
		AMainActor* YourCharacter = Cast<AMainActor>(OtherActor);
		if (YourCharacter)
		{

			bActivateMoveLeftRight = true;
			
			UE_LOG(LogTemp, Warning, TEXT("Begin trigger"));

		}
		else {
			
			UE_LOG(LogTemp, Warning, TEXT("no trigger"));

		}
			
	}
}

void AMyTriggerBox::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	if (OtherActor && OtherActor->IsA(AMainActor::StaticClass()))
	{
		AMainActor* YourCharacter = Cast<AMainActor>(OtherActor);
		if (YourCharacter)
		{
			bActivateMoveLeftRight = false;
			UE_LOG(LogTemp, Warning, TEXT("Is out!!!"));

		}
		

	}
	

		
}