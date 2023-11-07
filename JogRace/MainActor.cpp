// Fill out your copyright notice in the Description page of Project Settings.


#include "MainActor.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "JogRace/MyTriggerBox.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"


USpringArmComponent* spring;


// Sets default values
AMainActor::AMainActor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	/*Setting up the Spring Arm*/
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 2.0f;
	////////////////

	spring = SpringArm;

	/*Setting up the Camera*/
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	/*Connecting the camera with the springArm*/
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	////////////////

	/*Setting up the BoxTrigger Component*/
	
	//MyTrigger = Cast<AMyTriggerBox>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyTriggerBox::StaticClass()));
	

	///////////

	//Defining the Right and Left rotation rate of the Actor
	RotationRightRate = 36.0f;
	RotationLeftRate = 36.0f;
	//////////////
	

}

// Called when the game starts or when spawned
void AMainActor::BeginPlay()
{
	Super::BeginPlay();


		// Find and store references to all trigger boxes
		for (TActorIterator<AMyTriggerBox> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			MyTriggerBoxes.Add(*ActorItr);
			
		}


}

// Called every frame
void AMainActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*Defining the "MoveRight" and "MoveLeft" Control movement.*/
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction);
	//////////////
	
}

// Called to bind functionality to input
void AMainActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("TurnRight", IE_Pressed, this, &AMainActor::TurnRight);
	PlayerInputComponent->BindAction("TurnLeft", IE_Pressed, this, &AMainActor::TurnLeft);

}


void AMainActor::TurnRight() {

	SetupRotation(1.0f, RotationRightRate);

	FVector ActorCurrentLocation = GetActorLocation();

	  ActorCurrentLocation.Y += 800.f;

	  SetActorLocation(ActorCurrentLocation);

	UE_LOG(LogTemp, Warning, TEXT("%f"), ActorCurrentLocation.Y)
	
}

void AMainActor::TurnLeft() {

	SetupRotation(-1.0f, RotationLeftRate);

	FVector ActorCurrentLocation = GetActorLocation();

	ActorCurrentLocation.Y -= 800.f;

	SetActorLocation(ActorCurrentLocation);

	UE_LOG(LogTemp, Warning, TEXT("%f"), ActorCurrentLocation.Y)
}

/*This Function is responsible for setting up the left and right rotaion of the character. */
/*And also it makes sure the rotation only happens when the characters enters the trigger box...*/
void AMainActor::SetupRotation(float InputValue, float RotationRate) {

	for (AMyTriggerBox* MainTrigger : MyTriggerBoxes)
	{
		if (MainTrigger)
		{
			// Access properties and functions of the trigger box
			bool DemoActivateMoveLeftRight = MainTrigger->bActivateMoveLeftRight;
			if (DemoActivateMoveLeftRight)
			{
				AddControllerYawInput(InputValue * RotationRate);// This determines how the character rotate at the left and right hand side.
				UE_LOG(LogTemp, Warning, TEXT("Rotating..."))

			}
			else {

				UE_LOG(LogTemp, Warning, TEXT("Not Yet"))

			}

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("TriggerBox Not Active"))
		}
	}

}