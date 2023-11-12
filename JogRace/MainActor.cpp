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

	/*Setting up the Camera*/
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	/*Connecting the camera with the springArm*/
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	////////////////

	//Defining the Right and Left rotation rate of the Actor
	RotationRightRate = 36.0f;
	RotationLeftRate = 36.0f;
	//////////////
	
	SwitchLaneLimit = 2;
	DemoActivateMoveLeftRight = false;
	NewLocationVector = FVector(0.0f);
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

	DeltaRef = DeltaTime;

	//UE_LOG(LogTemp, Warning, TEXT("X:%f, Y:%f"), NewLocationVector.X, NewLocationVector.Y)

	//UE_LOG(LogTemp, Warning, TEXT("X:%f, Y:%f"), GetActorForwardVector().X, GetActorForwardVector().Y)
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

	/*This section deals with moving the character from one lane/track to the other */
	if (!DemoActivateMoveLeftRight && SwitchLaneLimit < 3) {
		MoveToNextLane(700.0f);
		SwitchLaneLimit++;
		

	}

}

void AMainActor::TurnLeft() {

	SetupRotation(-1.0f, RotationLeftRate);

	/*This section deals with moving the character from one lane/track to the other */
	if (!DemoActivateMoveLeftRight && SwitchLaneLimit > 1) {
		MoveToNextLane(-700.0f);
		SwitchLaneLimit--;
		//UE_LOG(LogTemp, Warning, TEXT("%i"), SwitchLaneLimit)

	}

}


/*This Function sets up the left and right rotaion of the character. */
/*And also it makes sure the rotation only happens when the characters enters the trigger box...*/
void AMainActor::SetupRotation(float InputValue, float RotationRate) {

	for (AMyTriggerBox* MainTrigger : MyTriggerBoxes)
	{
		if (MainTrigger)
		{
			/* Access properties and functions of the trigger box*/
			 DemoActivateMoveLeftRight = MainTrigger->bActivateMoveLeftRight;
			if (DemoActivateMoveLeftRight)
			{

				FVector ActorFowardVector = GetActorForwardVector();//Gets the actor foward vector/direction

				/* Gets the actor location when the actor rotates to another direction at this function call */
				FVector GetNewLocationVector = GetActorLocation() + ActorFowardVector * DeltaRef;

				/*Gets the actor location from 'TriggerBox class' when the character enters the box*/
				int TriggerBeginValueX = MainTrigger->TriggerBeginValueX;
				int TriggerBeginValueY = MainTrigger->TriggerBeginValueY;

				
				/*This functions are used to determine the location area of the characters before they rotate to another direction*/
				DetermineAreaOfActor_BeforeRotate(TriggerBeginValueX, GetNewLocationVector.X, ActorFowardVector.X, 'X');
				DetermineAreaOfActor_BeforeRotate(TriggerBeginValueY, GetNewLocationVector.Y, ActorFowardVector.Y, 'Y');
				

				AddControllerYawInput(InputValue * RotationRate);// This determines how the character rotate at the left and right hand side.
				UE_LOG(LogTemp, Warning, TEXT("Rotating..."))
			}

		}
		
	}

}


/*This function moves the character to the next lane or track of the road*/
void AMainActor::MoveToNextLane(float Distance) {
	FVector ActorRightVector = GetActorRightVector();//Getting the right relative actor vector/direction
	FVector ActorCurrentLocation = GetActorLocation();//Getting the Actor current location
	FVector NewRelativeLoation = ActorCurrentLocation + ActorRightVector * Distance;
	SetActorRelativeLocation(NewRelativeLoation);//Setting Actor location based on actor local axis Direction....

}



void AMainActor::DetermineAreaOfActor_BeforeRotate(int TriggerBeginValue_XY, float GetNewLocationVector_XY, float ActorFowardVector_XY, char XY) {


	int FowardVector_XY = FMath::RoundToInt(ActorFowardVector_XY);//Rounded the foward vector, from float to integer

	UE_LOG(LogTemp, Warning, TEXT("%c : %i"), XY, FowardVector_XY)

	if (FowardVector_XY) {

		int FirstLaneAreaLimit_BeforeTurn = TriggerBeginValue_XY + 830;//This is the Distance limit of the first lane area after the triggerBox
		int SecondLaneAreaLimit_BeforeTurn = FirstLaneAreaLimit_BeforeTurn + 830; //This is the Distance limit of the second lane area after the first lane area

		//int ThirdLaneAreaLimit_BeforeTurn = SecondLaneAreaLimit_BeforeTurn + 830;

		if (GetNewLocationVector_XY > SecondLaneAreaLimit_BeforeTurn) {//Checking if the actors current location is greater than the SecondLaneArea limit

			UE_LOG(LogTemp, Warning, TEXT("Switch to 1st Lane after rotating"))
		}
		else if ((GetNewLocationVector_XY < SecondLaneAreaLimit_BeforeTurn) && (GetNewLocationVector_XY > FirstLaneAreaLimit_BeforeTurn)) {

			UE_LOG(LogTemp, Warning, TEXT("Switch to middle Lane after rotating"))

		}
		else {

			UE_LOG(LogTemp, Warning, TEXT("Switch to 3rd Lane after rotating"))
		}

	}

}