// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"
#include "..\Public\MovingPlatform.h"


// Sets default values
AMovingPlatform::AMovingPlatform()
	: bLoop(true)
	, bMovePlatform(true)
	, bWaitAtEndpoints(true)
	, bStopOnHit(true)
	, bUseTrackRoll(false)
	, buseTrackPitch(false)
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

void AMovingPlatform::UpdatePlatformTime(USplineComponent* TrackSpline)
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
	if (TrackSpline != nullptr)
	{
		splineDuration = TrackSpline->Duration;
	}

	_PlatformTime = FMath::Clamp<float>(adjustedPlatformTime, 0.0f, splineDuration);
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_DeltaTime = DeltaTime;
	if (bMovePlatform)
	{

	}
}
