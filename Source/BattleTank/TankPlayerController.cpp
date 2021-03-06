// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "CameraController.h"
#include "DrawDebugHelpers.h"
#include "TankAimingComponent.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	/*if ( ControlledTank )
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController controlled: %s"), *ControlledTank->GetName());
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController has no possessed Tank!"));
	}*/

	UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if ( ensure(AimingComponent) )
	{
		FoundAimingComponent(AimingComponent);
	}
	/*else
	{
		UE_LOG( LogTemp, Warning, TEXT("TankPlayerController couldn't find AimingComponent!") );
	}*/
}

// Called every frame
void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if ( !GetPawn() ) return;
	UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if ( !ensure(AimingComponent) ) return;

	FVector HitLocation; // Out Parameter
	bool bGetGotHitLocation = GetSightRayHitLocation(HitLocation);
	//UE_LOG(LogTemp, Warning, TEXT("bGetGotHitLocation: %i"), bGetGotHitLocation);
	if ( bGetGotHitLocation )
	{
		AimingComponent->AimAt( HitLocation );
		//DrawDebugLine(GetWorld(), PlayerCameraManager->GetCameraLocation(), HitLocation, FColor(255, 0, 0), false, 0.0f, 0.0f, 10.0f);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	// find the crosshair position
	int32 ViewportSizeX;
	int32 ViewportSizeY;
	GetViewportSize( ViewportSizeX, ViewportSizeY );

	FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation);
	FVector LookDirection;

	if ( GetLookDirection(ScreenLocation, LookDirection) )
	{
		return GetLookVectorHitLocation(LookDirection, OutHitLocation);
	}
	
	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& OutLookDirection) const
{
	FVector CameraWorldLocation; // to be discarded
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, OutLookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation) const
{
	FHitResult HitResult;
	//FVector StartLocation = GetControlledTank()->GetActorLocation();
	FVector StartLocation = PlayerCameraManager->GetCameraLocation();

	if ( GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			StartLocation + (LookDirection * LineTraceRange),
			ECollisionChannel::ECC_Visibility
	) )
	{
		OutHitLocation = HitResult.Location;
		return true;
	}

	OutHitLocation = FVector(0);
	return false;
}