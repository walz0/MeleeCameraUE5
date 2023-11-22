// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage.h"
#include "DrawDebugHelpers.h"

// Sets default values
AStage::AStage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStage::BeginPlay()
{
	Super::BeginPlay();
	Debug_DrawCameraLimits();
}

// Called every frame
void AStage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStage::InitStage() {

}

float AStage::GetFixedness() {
	return fixedness;
}

float AStage::GetCameraLimitTop() {
	return cam_limit_top;
}

float AStage::GetCameraLimitRight() {
	return cam_limit_right;
}
float AStage::GetCameraLimitBottom() {
	return cam_limit_bottom;
}

float AStage::GetCameraLimitLeft() {
	return cam_limit_left;
}

FVector2D AStage::GetCameraOffset() {
	return cam_offset;
}

float AStage::GetVerticalRotation() {
	return vertical_rot;
}

float AStage::GetHorizontalRotation() {
	return horizontal_rot;
}

float AStage::GetTilt() {
	return tilt;
}

float AStage::GetCamDistMin() {
	return cam_dist_min;
}

float AStage::GetCamDistMax() {
	return cam_dist_max;
}

void AStage::Debug_DrawCameraLimits() {
	DrawDebugBox(
		GetWorld(), 
		FVector(0.0f, 0.0f, 0.0f), 
		FVector(0.0f, cam_limit_right, cam_limit_top), 
		FColor::Blue, 
		true, 
		-1, 
		0, 
		10
	);
}

