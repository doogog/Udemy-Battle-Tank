// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"


void UTankMovementComponent::Initialize(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	//UE_LOG(LogTemp, Warning, TEXT("IntendMoveForward throw: %f"), Throw);
	if ( !LeftTrack || !RightTrack ) return;
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	if ( !LeftTrack || !RightTrack ) return;
	//UE_LOG(LogTemp, Warning, TEXT("IntendMoveForward throw: %f"), Throw);
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{					
	FVector TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	FVector AIForwardIntention = MoveVelocity.GetSafeNormal();

	float ForwardThrow = FVector::DotProduct( TankForward, AIForwardIntention );
	IntendMoveForward(ForwardThrow);

	float RightThrow = FVector::CrossProduct( TankForward, AIForwardIntention ).Z;
	IntendTurnRight( RightThrow );

	/*FString TankName = GetOwner()->GetName();
	UE_LOG( LogTemp, Warning, TEXT("Tank %s requesting move: %s"), *TankName, *AIForwardIntention.ToString() );*/
}