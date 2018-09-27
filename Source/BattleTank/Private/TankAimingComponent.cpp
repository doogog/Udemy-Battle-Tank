// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	LastFireTime = FPlatformTime::Seconds();
	
}

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if ( RoundsLeft <= 0 )
	{
		FiringState = EFiringState::OutOfAmmo;
	}
	else if ( (FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds )
	{
		FiringState = EFiringState::Reloading;
	}
	else if ( IsBarrelMoving() )
	{
		FiringState = EFiringState::Aiming;
	}
	else
	{
		FiringState = EFiringState::Locked;
	}
}

void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if ( !ensureAlways( Barrel && Turret ) ) return;

	FString OurTankName = GetOwner()->GetName();
	FVector BarrelLocation = Barrel->GetComponentLocation();
	FVector OutLaunchVelocity(0);
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	bool bHaveAimingSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	if ( bHaveAimingSolution )
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelToward( AimDirection );
		float Time = GetWorld()->GetTimeSeconds();
		//UE_LOG(LogTemp, Warning, TEXT("%f: aim solution found!"), Time );
	}
}

void UTankAimingComponent::MoveBarrelToward(FVector AimDirection)
{
	if ( !ensure( Barrel && Turret ) ) return;

	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimAsRotator - BarrelRotator;
	//UE_LOG( LogTemp, Warning, TEXT("Elevate to Pitch: %f"), DeltaRotator.Pitch );

	Barrel->Elevate(DeltaRotator.Pitch);
	
	// Always yaw the shortest way
	if ( FMath::Abs(DeltaRotator.Yaw) < 180 )
	{
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else
	{
		Turret->Rotate(-DeltaRotator.Yaw);
	}
}

void UTankAimingComponent::Fire(TSubclassOf<AProjectile> ProjectileClass)
{
	//UE_LOG( LogTemp, Warning, TEXT("Fire!!") );

	if ( FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming )
	{
		if ( !ensure(Barrel && ProjectileClass) ) return;
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileClass,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		RoundsLeft--;

		//UE_LOG(LogTemp, Warning, TEXT("Rounds Left: %i"), RoundsLeft);
	}
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if ( !ensure(Barrel) ) return false;
	//bool bIsEqual = Barrel->GetForwardVector().Equals(();
	return !Barrel->GetForwardVector().Equals( AimDirection, 0.01 );
}

int UTankAimingComponent::GetRoundsLeft() const
{
	return RoundsLeft;
}