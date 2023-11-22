// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "Stage.h"
#include "CamSubject.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MELEECAMERAUE5_API UCamSubject : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCamSubject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float dir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBox2D default_bounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector focus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector pos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float y_offset = 70.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		AStage* stage;

	FBox2D bounds;

	UFUNCTION(BlueprintCallable, Category = "Cam Subject")
		uint8 GetBounds(FVector subjectPos);

	void SetPos(FVector pos);
	void SetFocus(FVector pos);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void InitCamSubject();
	FVector2D Bounds_GetCenter(FBox2D bounds);
	void DrawBounds(float deltaTime);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
