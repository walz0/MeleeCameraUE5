// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraBounds.h"
#include "Stage.h"
#include "Dummy.h"
#include "CamSubject.h"
#include "Camera/CameraComponent.h"
#include "MainCamera.generated.h"

UCLASS()
class MELEECAMERAUE5_API AMainCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainCamera();

	UFUNCTION(BlueprintCallable, Category = "Camera")
		void SetPos(FVector pos);

	UFUNCTION(BlueprintCallable, Category = "Camera")
		void LookAt(FVector pos);

	UFUNCTION(BlueprintCallable, Category = "Camera")
		void SetInterest(FVector pos);

	UFUNCTION(BlueprintCallable, Category = "Camera")
		void SetFOV(float degrees);

	UFUNCTION(BlueprintCallable, Category = "Camera")
		void FocusSubject(int slot);

	UFUNCTION(BlueprintCallable, Category = "Camera")
		void UpdateBounds();

	UFUNCTION(BlueprintCallable, Category = "Camera")
		void UpdateTarget();

	UFUNCTION(BlueprintCallable, Category = "Camera")
		void LerpInterest(float smoothness);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UCamSubject*> subjects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AStage* stage;

	float fov = 50.0f;
	float target_fov;

	FVector interest;
	FVector target_interest;

	FVector interest_lerp;

	FVector position;
	FVector target_position;

	FVector position_lerp;

	TArray<float> subjectFixednessMultipliers = TArray<float>{
		1.5f,
		1.32f,
		1.16f,
		1.0f
	};

	UCameraBounds* bounds;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Initializes camera subjects in the scene
	void InitCamSubjects(AStage* stageRef);
	// Initializes camera bounds
	void InitBounds();
	// Initializes stage reference
	AStage* InitStage();
	// Initializes camera reference
	void InitCamera();
	// Draws the camera bounds with a debug rectangle
	void Debug_DrawBounds();
	// Draws the camera interest with a debug circle
	void Debug_DrawInterest();
	// Draws the camera target interest with a debug circle
	void Debug_DrawTargetInterest();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

