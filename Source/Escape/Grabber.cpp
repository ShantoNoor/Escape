// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

#define OUT 

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(!PhysicsHandle) UE_LOG(LogTemp, Warning, TEXT("No Physics Handle Component found on %s"), *GetOwner()->GetName());

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Input Component found on %s"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabbed!"));
	FHitResult Hit = GetFirstPhysicsBodyInReach();

	if(PhysicsHandle && Hit.GetActor())
	{
		// UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();
		PhysicsHandle->GrabComponentAtLocation(
			Hit.GetComponent(),
			NAME_None,
			GetLineTraceEnd()
		);
	}

}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Released!"));
	if(PhysicsHandle) PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(
			FName( TEXT("") ),
			false,
			GetOwner()	
		)
	);

	// UE_LOG(LogTemp, Warning, TEXT("%s"), *Hit.ToString());
	if(Hit.GetActor())
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Hit.GetActor()->GetName());

	return Hit;
}

FVector UGrabber::GetLineTraceEnd()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	return LineTraceEnd;
}
