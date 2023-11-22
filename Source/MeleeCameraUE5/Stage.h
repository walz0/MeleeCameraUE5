// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Stage.generated.h"

UCLASS()
class MELEECAMERAUE5_API AStage : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Stage")
		float GetFixedness();

	UFUNCTION(BlueprintCallable, Category = "Stage")
		float GetCameraLimitTop();

	UFUNCTION(BlueprintCallable, Category = "Stage")
		float GetCameraLimitRight();

	UFUNCTION(BlueprintCallable, Category = "Stage")
		float GetCameraLimitBottom();

	UFUNCTION(BlueprintCallable, Category = "Stage")
		float GetCameraLimitLeft();

	UFUNCTION(BlueprintCallable, Category = "Stage")
		FVector2D GetCameraOffset();

	UFUNCTION(BlueprintCallable, Category = "Stage")
		float GetVerticalRotation();

	UFUNCTION(BlueprintCallable, Category = "Stage")
		float GetHorizontalRotation();

	UFUNCTION(BlueprintCallable, Category = "Stage")
		float GetTilt();

	UFUNCTION(BlueprintCallable, Category = "Stage")
		float GetCamDistMin();

	UFUNCTION(BlueprintCallable, Category = "Stage")
		float GetCamDistMax();

	UFUNCTION(BlueprintCallable, Category = "Stage")
		void Debug_DrawCameraLimits();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fixedness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float cam_limit_top;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float cam_limit_right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float cam_limit_bottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float cam_limit_left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float cam_dist_max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float cam_dist_min;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float vertical_rot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float horizontal_rot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float tilt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D cam_offset;
	
	// Sets default values for this actor's properties
	AStage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void InitStage();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
