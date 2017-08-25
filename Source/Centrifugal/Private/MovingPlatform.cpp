// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"


// Sets default values
AMovingPlatform::AMovingPlatform()
	: bLoop(true)
	, bMovePlatform(true)
	, bWaitAtEndpoints(true)
	, bStopOnHit(true)
	, bUseTrackRoll(false)
	, bUseTrackPitch(false)
	, bUseTrackYaw(false)
	, PlatformEndpointWaitTime(1.0f)
	, PlatformStartTime(0.0f)
	, TrackDuration(10.0f)
	, _PlatformTime(0.0f)
	, _DeltaTime(0.0f)
	, _CachedPlatformTime(0.0f)
	, PlatformMovement(EPlatformMovement::PingPong)
	, PlatformStartDirection(EPlatformDirection::Forward)
	, _PlatformDirection(EPlatformDirection::Forward)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMovingPlatform::UpdatePlatformTime()
{
	_CachedPlatformTime = _PlatformTime;

	float adjustedPlatformTime = 0.0f;
	if (_PlatformDirection == EPlatformDirection::Forward)
	{
		adjustedPlatformTime = _DeltaTime + _PlatformTime;
	}
	else
	{
		adjustedPlatformTime = -_DeltaTime + _PlatformTime;
	}
	float splineDuration = 0.0f;
	if (_Track != nullptr)
	{
		splineDuration = _Track->Duration;
	}

	_PlatformTime = FMath::Clamp<float>(adjustedPlatformTime, 0.0f, splineDuration);
}

void AMovingPlatform::SetPlatformLocationAndRotation(bool bSweep)
{
	if (_Track == nullptr || _MovingPlatformMesh == nullptr)
	{
		return;
	}
	FRotator rotAtTime = _Track->GetRotationAtTime(_PlatformTime, ESplineCoordinateSpace::World, true);

	FRotator rotMovingPlatform = _MovingPlatformMesh->GetComponentRotation();

	float roll = bUseTrackRoll ? rotAtTime.Roll : rotMovingPlatform.Roll;
	float pitch = bUseTrackPitch ? rotAtTime.Pitch : rotMovingPlatform.Pitch;
	float yaw = bUseTrackYaw ? rotAtTime.Yaw : rotMovingPlatform.Yaw;

	FRotator newRotation = FRotator::MakeFromEuler(FVector(roll, pitch, yaw));
	FVector newLocation = _Track->GetLocationAtTime(_PlatformTime, ESplineCoordinateSpace::World, true);

	FHitResult HitResult;

	_MovingPlatformMesh->SetWorldLocationAndRotation(newLocation, newRotation, bSweep, &HitResult, ETeleportType::None);

	if (HitResult.bBlockingHit || HitResult.bStartPenetrating)
	{
		_PlatformTime = _CachedPlatformTime;
	}
}

bool AMovingPlatform::WasEndpointReached()
{
	if (_PlatformDirection == EPlatformDirection::Forward)
	{
		if (_Track != nullptr)
		{
			return FMath::IsNearlyEqual(_PlatformTime, _Track->Duration);
		}
	}
	else if (_PlatformDirection == EPlatformDirection::Backward)
	{
		return FMath::IsNearlyEqual(_PlatformTime, 0.0f);
	}
	return false;
}

void AMovingPlatform::PingPong()
{
	FTimerHandle ThrowawayTimerHandle;

	if (_PlatformDirection == EPlatformDirection::Backward)
	{
		_PlatformDirection = EPlatformDirection::Forward;
	}
	else
	{
		_PlatformDirection = EPlatformDirection::Backward;
	}

	if (!bWaitAtEndpoints)
	{
		return;
	}

	bMovePlatform = false;
	GetWorldTimerManager().SetTimer(ThrowawayTimerHandle, this, &AMovingPlatform::PingPongDelayFinished, PlatformEndpointWaitTime, false);
}

void AMovingPlatform::PingPongDelayFinished()
{
	UpdatePlatformTime();
	SetPlatformLocationAndRotation(bStopOnHit);
	bMovePlatform = true;
}

void AMovingPlatform::LoopFromBeginning()
{
	if (_PlatformDirection == EPlatformDirection::Forward)
	{
		_PlatformTime = 0.0f;
	}
	else
	{
		if (_Track != nullptr)
		{
			_PlatformTime = _Track->Duration;
		}
	}
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
}

void AMovingPlatform::OnConstruction(const FTransform& Transform)
{
	TArray<USplineComponent*> Splines;
	GetComponents(Splines);
	if (Splines.Num() > 0)
	{
		_Track = Splines[0];
		_Track->Duration = TrackDuration;
	}

	TArray<UStaticMeshComponent*> Platforms;
	GetComponents(Platforms);
	if (Platforms.Num() > 0)
	{
		_MovingPlatformMesh = Platforms[0];
	}

	_PlatformDirection = PlatformStartDirection;
	_PlatformTime = PlatformStartTime;
	SetPlatformLocationAndRotation(false);
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_DeltaTime = DeltaTime;
	if (!bMovePlatform)
	{
		return;
	}

	UpdatePlatformTime();
	SetPlatformLocationAndRotation(bStopOnHit);

	if (!WasEndpointReached())
	{
		return;
	}

	if (bLoop)
	{
		switch (PlatformMovement)
		{
		case EPlatformMovement::PingPong:
			PingPong();
			break;
		case EPlatformMovement::LoopFromBeginning:
			LoopFromBeginning();
			break;
		default:
			break;
		}
	}
	else
	{
		bMovePlatform = false;
	}
}
