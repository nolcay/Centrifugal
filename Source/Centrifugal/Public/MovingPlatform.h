// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "MovingPlatform.generated.h"


UENUM(BlueprintType)
enum class EPlatformMovement : uint8
{
	// Reverse direction once the platform reaches the end of the spline track.
	PingPong,
	// Snap platform to beginning of spline track once it reaches the end of the spline track.
	LoopFromBeginning,
};


UENUM(BlueprintType)
enum class EPlatformDirection : uint8
{
	// Platform is moving from start-to-end.
	Forward,
	// Platform is moving from end-to-start.
	Backward
};


UCLASS()
class CENTRIFUGAL_API AMovingPlatform : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMovingPlatform();

	void SetPlatformTime(float val) { _PlatformTime = val; }
	void SetDeltaTime(float val) { _DeltaTime = val; }
	void SetCachedPlatformTime(float val) { _CachedPlatformTime = val; }
	void SetPlatformDirection(EPlatformDirection val) { _PlatformDirection = val; }

	UFUNCTION(BlueprintCallable, Category = "Platform")
	void UpdatePlatformTime();

	UFUNCTION(BlueprintCallable, Category = "Platform")
	void SetPlatformLocationAndRotation(bool bSweep = false);

	UFUNCTION(BlueprintCallable, Category = "Platform")
	bool WasEndpointReached();

	UFUNCTION(BlueprintCallable, Category = "Platform")
	void PingPong();

	UFUNCTION(BlueprintCallable, Category = "Platform")
	void LoopFromBeginning();

	/** Whether the platform loops when it reaches an end point. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform")
	bool bLoop;

	/** Whether the platform moves. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform")
	bool bMovePlatform;

	/** Whether the platform waits when it reaches the beginning or end of its track. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform")
	bool bWaitAtEndpoints;

	/** Stop the platform when it hits a solid object. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform")
	bool bStopOnHit;

	/** Whether we use the track's roll rotation.
	 *  *** Warning! Enabling Roll can have unexpected results ***
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform")
	bool bUseTrackRoll;

	/** Whether we use the track's pitch rotation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform")
	bool bUseTrackPitch;

	/** Whether we use the track's yaw rotation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform")
	bool bUseTrackYaw;

	/** How long the platform will wait at the beginning and end of its track before moving again. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform")
	float PlatformEndpointWaitTime;

	/** Where the platform starts on the track. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform")
	float PlatformStartTime;

	/** How long it takes to get from one end of the track to the other. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform")
	float TrackDuration;

	/** 1 = forward, -1 = backward. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform")
	EPlatformDirection PlatformStartDirection;

	/** How to handle reaching the beginning or end of the Track spline. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform")
	EPlatformMovement PlatformMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void PingPongDelayFinished();

	/** Time/distance traveled along the path. */
	UPROPERTY(BlueprintReadOnly, Category = "Platform")
	float _PlatformTime;

	UPROPERTY(BlueprintReadOnly, Category = "Platform")
	float _DeltaTime;

	UPROPERTY(BlueprintReadOnly, Category = "Platform")
	float _CachedPlatformTime;

	/** Is the platform moving forward or backward? */
	UPROPERTY(BlueprintReadOnly, Category = "Platform")
	EPlatformDirection _PlatformDirection;

	USplineComponent* _Track;
	UStaticMeshComponent* _MovingPlatformMesh;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
