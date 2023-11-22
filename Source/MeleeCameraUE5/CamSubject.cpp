// Fill out your copyright notice in the Description page of Project Settings.


#include "CamSubject.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UCamSubject::UCamSubject()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UCamSubject::BeginPlay()
{
	Super::BeginPlay();

	InitCamSubject();
}


// Called every frame
void UCamSubject::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* parentActor = GetOwner();
	// Keep focus on the root of the player
	FVector actorPos = parentActor->GetActorLocation();
	SetFocus(actorPos);
	SetPos(actorPos);

	DrawBounds(DeltaTime);
}

void UCamSubject::InitCamSubject() {
	// Initialize CamSubject
	AActor* parentActor = GetOwner();
	if (parentActor != NULL) {
		FVector actorPos = parentActor->GetActorLocation();
		SetFocus(actorPos);
	}
}

void UCamSubject::SetFocus(FVector focusPos) {
	focus = focusPos;
}

void UCamSubject::SetPos(FVector newPos) {
	pos = newPos;
}

FVector2D UCamSubject::Bounds_GetCenter(FBox2D bounding_box) {
	float width = bounding_box.Max.X - bounding_box.Min.X;
	float height = bounding_box.Max.Y - bounding_box.Min.Y;

	FVector2D center = FVector2D(
		(width / 2.0f) - abs(bounding_box.Min.X),
		(height / 2.0f) - abs(bounding_box.Min.Y)
	);

	return center;
}

uint8 UCamSubject::GetBounds(FVector subjectPos) {
	// Get camera limits for stage
	float cameraLimitLeft = stage->GetCameraLimitLeft();
	float cameraLimitRight = stage->GetCameraLimitRight();
	float cameraLimitTop = stage->GetCameraLimitTop();
	float cameraLimitBottom = stage->GetCameraLimitBottom();

	/* Bitfield for bounds
		0b00000001 -> top
		0b00000010 -> bottom
		0b00000100 -> left
		0b00001000 -> right

		0b00000000 -> none
	*/
	uint8 boundField = 0;

	if (subjectPos.Y > cameraLimitTop) {
		UE_LOG(LogTemp, Warning, TEXT("TOP"));
		boundField = 1 << 0;
	}
	if (subjectPos.Y < cameraLimitBottom) {
		UE_LOG(LogTemp, Warning, TEXT("BOTTOM"));
		boundField = 1 << 1;
	}
	if (subjectPos.X < cameraLimitLeft) {
		UE_LOG(LogTemp, Warning, TEXT("LEFT"));
		boundField = 1 << 2;
	}
	if (subjectPos.X > cameraLimitRight) {
		UE_LOG(LogTemp, Warning, TEXT("RIGHT"));
		boundField = 1 << 3;
	}

	return boundField;
}


void UCamSubject::DrawBounds(float deltaTime) {
	float width = default_bounds.Max.X - default_bounds.Min.X;
	float height = default_bounds.Max.Y - default_bounds.Min.Y;

	FVector center = FVector(
		focus.X, 
		focus.Y + width / 2.0f - abs(default_bounds.Min.X),
		focus.Z + height / 2.0f - abs(default_bounds.Min.Y) + y_offset
	);

	DrawDebugBox(
		GetWorld(),
		center,
		FVector(0.0f, width / 2.0f, height/ 2.0f),
		FColor::Blue, 
		false, 
		deltaTime, 
		0, 
		10
	);
}

