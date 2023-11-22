// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCamera.h"
#include "Kismet/KismetMathLibrary.h"  // Include the math library
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "UObject/UObjectGlobals.h"

// Sets default values
AMainCamera::AMainCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainCamera::BeginPlay()
{
	Super::BeginPlay();

    // Init class members
    InitBounds();
    InitCamera();
    AStage* stageRef = InitStage();
    InitCamSubjects(stageRef);
}

// Called every frame
void AMainCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
  
    // Update CameraBounds
    UpdateBounds();

    // Update target interest / position
    UpdateTarget();

    // Set interest to center of camera bounds
    /*SetInterest(FVector(
        0.0f, 
        ((bounds->x_max - bounds->x_min) / 2.0f) - abs(bounds->x_min),
        ((bounds->y_max - bounds->y_min) / 2.0f) - abs(bounds->y_min)
    ));*/


	UE_LOG(LogTemp, Warning, TEXT("inter %f, %f, %f"), interest.X, interest.Y, interest.Z);
	UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), target_interest.X, target_interest.Y, target_interest.Z);

    interest = FMath::Lerp(interest, target_interest, 0.05f);
    // LerpInterest(0.8f);

    // Look at point of interest
    LookAt(interest);


    position = FMath::Lerp(position, target_position, 0.05f);
    // Set position of camera
    SetPos(position);

    // Draw bounds
    Debug_DrawBounds(DeltaTime);

    if (subjects.Num() > 0) {
        float speed = 10.0f;
        FVector prevPos = subjects[0]->GetOwner()->GetActorLocation();
        FVector newPos = FVector(
            prevPos.X, 
            prevPos.Y + InputComponent->GetAxisValue(TEXT("Horizontal")) * speed, 
            prevPos.Z + InputComponent->GetAxisValue(TEXT("Vertical")) * speed
        );

		// UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), newPos.X, newPos.Y, newPos.Z);
        subjects[0]->GetOwner()->SetActorLocation(newPos);
    }

	// UE_LOG(LogTemp, Warning, TEXT("x_max %f, x_min %f, y_max %f, y_min %f"), bounds->x_max, bounds->x_min, bounds->y_min, bounds->y_max);
}

// Called to bind functionality to input
void AMainCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis(TEXT("Horizontal"));
	InputComponent->BindAxis(TEXT("Vertical"));

	InputComponent->BindAxis(TEXT("Horizontal2"));
	InputComponent->BindAxis(TEXT("Vertical2"));
}

void AMainCamera::InitCamSubjects(AStage* stageRef) {
	TArray<AActor*> actorsToFind;
    if(UWorld* World = GetWorld()) {
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADummy::StaticClass(), actorsToFind);
	}
    for (AActor* dummyActor : actorsToFind) {
        ADummy* dummyCast = Cast<ADummy>(dummyActor);
        if (dummyCast)
        {
            UCamSubject* camSubject = dummyActor->FindComponentByClass<UCamSubject>();
            // Set reference to stage in each subject
            if (stageRef != NULL) {
				camSubject->stage = stageRef;
            }
            subjects.Add(camSubject);
        }   
    }
}

void AMainCamera::InitCamera() {
	// Get all components of type UCameraComponent attached to the actor and its children
    TArray<UCameraComponent*> cameraComponents;
    GetComponents<UCameraComponent>(cameraComponents);

    if (cameraComponents.Num() > 0) {
        // Set reference to camera
        camera = cameraComponents[0];
    }
}

void AMainCamera::InitBounds() {
    // Initialize CameraBounds object
	bounds = NewObject<UCameraBounds>();
    // Set default bounds values
    bounds->x_min = 0.0f;
    bounds->y_min = 0.0f;
    bounds->x_max = 0.0f;
    bounds->y_max = 0.0f;
}

AStage* AMainCamera::InitStage() {
 	TArray<AActor*> actorsToFind;
    if(UWorld* World = GetWorld()) {
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStage::StaticClass(), actorsToFind);
	}
    for (AActor* stageActor : actorsToFind) {
        AStage* stageCast = Cast<AStage>(stageActor);
        if (stageCast)
        {
            // Set stage reference
            stage = stageCast;
            return stage;
        }
    }
    return NULL;
}

void AMainCamera::FocusSubject(int slot) {
    interest = subjects[slot]->focus;
}

void AMainCamera::LookAt(FVector pos) {
    // Get look at rotation
    FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), pos);
    // Set rotation
    SetActorRotation(rot);
}

void AMainCamera::SetPos(FVector pos) {
	SetActorLocation(pos);
}

void AMainCamera::SetInterest(FVector pos) {
    interest = pos;
}

void AMainCamera::SetFOV(float degrees) {
    fov = 0.0f;
}

void AMainCamera::Debug_DrawBounds(float deltaTime) {
	float width = bounds->x_max - bounds->x_min;
	float height = bounds->y_max - bounds->y_min;

	FVector center = FVector(
		0.0f, 
		width / 2.0f - abs(bounds->x_min),
		height / 2.0f - abs(bounds->y_min)
	);

	DrawDebugBox(
		GetWorld(), 
		center, 
		FVector(0.0f, width / 2.0f, height / 2.0f), 
		FColor::Green, 
		false, 
		-1, 
		0, 
		10
	);
}

void AMainCamera::UpdateBounds() {
    int totalSubjects = subjects.Num();

	float baseFixedness = 1.0f;
	if (totalSubjects < 5) {
		baseFixedness = subjectFixednessMultipliers[totalSubjects - 1];
	}
	float stageFixedness = stage->GetFixedness();

    float fixedness = stageFixedness * baseFixedness;

    // Initialize bound values
    float y_min = TNumericLimits<float>::Max();
    float y_max = TNumericLimits<float>::Min();
    float x_min = TNumericLimits<float>::Max();
    float x_max = TNumericLimits<float>::Min();

	//UE_LOG(LogTemp, Warning, TEXT("%d"), subjectBounds);
    
    // Set bound bit fields
    uint8 top = 1 << 0;
    uint8 bottom = 1 << 1;
    uint8 left = 1 << 2;
    uint8 right = 1 << 3;

	float cameraLimit = 0;

    for (int i = 0; i < totalSubjects; i++) {
        UCamSubject* currentSubject = subjects[i];

		FVector unscaledBounds = FVector(
            currentSubject->focus.Y, 
            currentSubject->focus.Z, 
            currentSubject->focus.X
        );
		FVector scaledBounds = FVector(
            currentSubject->focus.Y, 
            currentSubject->focus.Z, 
            currentSubject->focus.X
        );

        uint8 subjectBounds = currentSubject->GetBounds(unscaledBounds);
		//UE_LOG(LogTemp, Warning, TEXT("%d"), subjectBounds);

        // Check camera limits
        if (subjectBounds != 0) {
            if (subjectBounds & left) {
				// UE_LOG(LogTemp, Warning, TEXT("LEFT"));
                cameraLimit = stage->GetCameraLimitLeft();
                unscaledBounds.X = cameraLimit;
            }
			if (subjectBounds & right) {
                cameraLimit = stage->GetCameraLimitRight();
                unscaledBounds.X = cameraLimit;
            }
			if (subjectBounds & top) {
                cameraLimit = stage->GetCameraLimitTop();
                unscaledBounds.Y = cameraLimit;
            }
			if (subjectBounds & bottom) {
                cameraLimit = stage->GetCameraLimitBottom();
                unscaledBounds.Y = cameraLimit;
            }
        }

        // Focus position + left cam subject bound * fixedness
        scaledBounds.X = currentSubject->default_bounds.Min.X * fixedness + unscaledBounds.X;
        // Get new bounds in case this extends beyond the camera limit
        subjectBounds = currentSubject->GetBounds(scaledBounds);
		// Check camera limits
        if (subjectBounds != 0) {
            if (subjectBounds & left) {
                cameraLimit = stage->GetCameraLimitLeft();
                scaledBounds.X = cameraLimit;
            }
			if (subjectBounds & right) {
                cameraLimit = stage->GetCameraLimitRight();
                scaledBounds.X = cameraLimit;
            }
			if (subjectBounds & top) {
                cameraLimit = stage->GetCameraLimitTop();
                scaledBounds.Y = cameraLimit;
            }
			if (subjectBounds & bottom) {
                cameraLimit = stage->GetCameraLimitBottom();
                scaledBounds.Y = cameraLimit;
            }
        }

        cameraLimit = scaledBounds.X;
        // Set X bounds
        if (x_min > cameraLimit) {
            x_min = scaledBounds.X;
        }
		if (x_max < cameraLimit) {
            x_max = scaledBounds.X;
        }

        // TODO :: comment
        scaledBounds.X = currentSubject->default_bounds.Max.X * fixedness + unscaledBounds.X;
        // Get new bounds in case this extends beyond the camera limit
        subjectBounds = currentSubject->GetBounds(scaledBounds);
		// Check camera limits
        if (subjectBounds != 0) {
            if (subjectBounds & left) {
                cameraLimit = stage->GetCameraLimitLeft();
                scaledBounds.X = cameraLimit;
            }
			if (subjectBounds & right) {
                cameraLimit = stage->GetCameraLimitRight();
                scaledBounds.X = cameraLimit;
            }
			if (subjectBounds & top) {
                cameraLimit = stage->GetCameraLimitTop();
                scaledBounds.Y = cameraLimit;
            }
			if (subjectBounds & bottom) {
                cameraLimit = stage->GetCameraLimitBottom();
                scaledBounds.Y = cameraLimit;
            }
        }

        cameraLimit = scaledBounds.X;
        // Update X bounds
        if (cameraLimit < x_min) {
            x_min = scaledBounds.X;
        }
		if (cameraLimit > x_max) {
            x_max = scaledBounds.X;
        }

        scaledBounds.Y = currentSubject->default_bounds.Min.Y * fixedness + unscaledBounds.Y;
		// Get new bounds in case this extends beyond the camera limit
        subjectBounds = currentSubject->GetBounds(scaledBounds);
		// Check camera limits
        if (subjectBounds != 0) {
            if (subjectBounds & left) {
                cameraLimit = stage->GetCameraLimitLeft();
                scaledBounds.X = cameraLimit;
            }
			if (subjectBounds & right) {
                cameraLimit = stage->GetCameraLimitRight();
                scaledBounds.X = cameraLimit;
            }
			if (subjectBounds & top) {
                cameraLimit = stage->GetCameraLimitTop();
                scaledBounds.Y = cameraLimit;
            }
			if (subjectBounds & bottom) {
                cameraLimit = stage->GetCameraLimitBottom();
                scaledBounds.Y = cameraLimit;
            }
        }

        cameraLimit = scaledBounds.Y;
		// Set Y bounds
        if (y_min > cameraLimit) {
            y_min = scaledBounds.Y;
        }
		if (y_max < cameraLimit) {
            y_max = scaledBounds.Y;
        }

        scaledBounds.Y = (currentSubject->default_bounds.Max.Y + currentSubject->y_offset) * fixedness + unscaledBounds.Y;
		// Get new bounds in case this extends beyond the camera limit
        subjectBounds = currentSubject->GetBounds(scaledBounds);
		// Check camera limits
        if (subjectBounds != 0) {
            if (subjectBounds & left) {
                cameraLimit = stage->GetCameraLimitLeft();
                scaledBounds.X = cameraLimit;
            }
			if (subjectBounds & right) {
                cameraLimit = stage->GetCameraLimitRight();
                scaledBounds.X = cameraLimit;
            }
			if (subjectBounds & top) {
                cameraLimit = stage->GetCameraLimitTop();
                scaledBounds.Y = cameraLimit;
            }
			if (subjectBounds & bottom) {
                cameraLimit = stage->GetCameraLimitBottom();
                scaledBounds.Y = cameraLimit;
            }
        }

        cameraLimit = scaledBounds.Y;
		// Update Y bounds
        if (cameraLimit < y_min) {
            y_min = scaledBounds.Y;
        }
		if (cameraLimit > y_max) {
            y_max = scaledBounds.Y;
        }
    }

    // Default bounds if level is empty
    if (totalSubjects == 0) {
        FVector2D cameraOffset = stage->GetCameraOffset();
        x_min = (cameraOffset.X - 40.0f * 10.0f);
        y_min = (cameraOffset.X - 40.0f * 10.0f);
        x_max = (cameraOffset.X + 40.0f * 10.0f);
        y_max = (cameraOffset.X + 40.0f * 10.0f);
    }

    // Get current depth position of camera
	float zPos = position.X;

    // Set zoom parameters
    float maxAdditionalZoom = 490.0f;
	float zoomScale;

	if (zPos < 0.0f) {
		zPos = -zPos;
	}
	if (zPos >= 0) {
		if (zPos <= stage->GetCamDistMax()) {
			zoomScale = zPos / stage->GetCamDistMax();
		}
		else {
			zoomScale = 1.0f;
		}
	}
	else {
		zoomScale = 0.0f;
	}

    bounds->x_min = x_min;
    bounds->y_min = y_min - (zoomScale * maxAdditionalZoom + 10.0f);
    bounds->x_max = x_max;
    bounds->y_max = y_max;
}

void AMainCamera::UpdateTarget() {
    FVector2D cameraOffset = stage->GetCameraOffset();

    float horizontalFOV = fov;
    float verticalFOV = FMath::RadiansToDegrees(2 * atan(tan(FMath::DegreesToRadians(horizontalFOV) / 2.0f) / camera->AspectRatio));

    float boundsWidth = bounds->x_max - bounds->x_min;
    float boundsHeight = bounds->y_max - bounds->y_min;

    float boundSize = boundsHeight;
    if (boundsHeight < boundsWidth) {
        boundSize = boundsWidth;
    }

    float upperThreshold = 1000.0f;
    float lowerThreshold = 600.0f;


	UE_LOG(LogTemp, Warning, TEXT("boundSize, %f"), boundSize);

    float scale = 0.0682f;
    if (boundSize <= upperThreshold && boundSize >= lowerThreshold) {
        scale = 0.0682f * ((boundSize - lowerThreshold) / (upperThreshold - lowerThreshold));
    }


	UE_LOG(LogTemp, Error, TEXT("scale, %f"), scale);

    float heightMultiplier = ((bounds->y_min - cameraOffset.Y) + (bounds->y_max - cameraOffset.Y)) * (0.5f - scale) + cameraOffset.Y;
    float stageVerticalRot = stage->GetVerticalRotation();

	float verticalRotScaled = -FMath::DegreesToRadians((heightMultiplier + -10.0f * 10.0f) * stageVerticalRot);
    if (verticalRotScaled > (FMath::DegreesToRadians(5.0f))) {
        verticalRotScaled = FMath::DegreesToRadians(5.0f);
    }
    if (verticalRotScaled < (FMath::DegreesToRadians(-8.0f))) {
        verticalRotScaled = FMath::DegreesToRadians(-8.0f);
    }

    float stageTilt = FMath::DegreesToRadians(stage->GetTilt());
    float totalTilt = (stageTilt + verticalRotScaled);

    float fovUp = FMath::DegreesToRadians(verticalFOV) * 0.5f + totalTilt;
    if (fovUp >= PI / 2.0f) {
		UE_LOG(LogTemp, Error, TEXT("FOV exceeds maximum value, "), fovUp);
    }

    float fovDown = FMath::DegreesToRadians(verticalFOV) * 0.5f - totalTilt;
    if (fovUp >= PI / 2.0f) {
		UE_LOG(LogTemp, Error, TEXT("FOV exceeds maximum value, "), fovDown);
    }

    float targetPosY = (bounds->y_max - bounds->y_min) / (tan(fovUp) + tan(fovDown));
    float tiltedTargetPosY = (targetPosY * tan(totalTilt));

    target_interest.Z = (tiltedTargetPosY + -(targetPosY * tan(fovUp) - bounds->y_max));



    float boundsMidpoint = (bounds->x_min + bounds->x_max) * 0.5f;
    float stageHorizontalRot = stage->GetHorizontalRotation();

	float horizontalRotScaled = -(FMath::DegreesToRadians((boundsMidpoint - cameraOffset.X) * stageHorizontalRot));

	if (horizontalRotScaled > (FMath::DegreesToRadians(17.5f))) {
        horizontalRotScaled = FMath::DegreesToRadians(17.5f);
    }
    if (horizontalRotScaled < FMath::DegreesToRadians(-17.5f)) {
        horizontalRotScaled = FMath::DegreesToRadians(-17.5f);
    }

    float fovRight = FMath::DegreesToRadians(verticalFOV) * 0.5 - horizontalRotScaled;
	if (fovRight>= PI / 2.0f) {
		UE_LOG(LogTemp, Error, TEXT("FOV exceeds maximum value, "), fovRight);
    }

    float fovLeft = FMath::DegreesToRadians(verticalFOV) * 0.5 + horizontalRotScaled;
    if (fovLeft >= PI / 2.0f) {
		UE_LOG(LogTemp, Error, TEXT("FOV exceeds maximum value, "), fovLeft);
    }

    float targetPosX = (bounds->x_max - bounds->x_min) / (camera->AspectRatio * tan(fovRight) + camera->AspectRatio * tan(fovLeft));
    float tiltedTargetPosX = camera->AspectRatio * (targetPosX * tan(horizontalRotScaled));

	target_interest.Y = (-(targetPosX * camera->AspectRatio * tan(fovRight) - bounds->x_max) - tiltedTargetPosX);

    target_interest.X = 0.0f;

    float depth = targetPosX;
    if (targetPosX < targetPosY) {
        depth = targetPosY;
    }

    float camDistMin = stage->GetCamDistMin();

	// UE_LOG(LogTemp, Warning, TEXT("%f, %f"), depth, camDistMin);
    if (depth < camDistMin) {
        depth = camDistMin;
    }
	
    float camDistMax = stage->GetCamDistMax();
    if (depth > camDistMax) {
        depth = camDistMax;
    }

    target_position.Y = (target_interest.Y) + tiltedTargetPosX;
    target_position.Z = (target_interest.Z) - tiltedTargetPosY;
    target_position.X = -((target_interest.X) + depth * 1.0f);

    // target_position.X = -2500.0f;
	// UE_LOG(LogTemp, Warning, TEXT("%f"), target_position.X);

	// UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), target_interest.X, target_interest.Y, target_interest.Z);
}

void AMainCamera::LerpInterest(float smoothness) {
    int totalSubjects = subjects.Num();

	UE_LOG(LogTemp, Warning, TEXT("%d"), totalSubjects);
    float boundSize;

	if (totalSubjects == 0) {
		boundSize = 99999.0;
	}
	else {
		float boundsWidth = bounds->x_max - bounds->x_min;
		float boundsHeight = bounds->y_max - bounds->y_min;

        boundSize = boundsHeight;
		if (boundsHeight < boundsWidth) {
			boundSize = boundsWidth;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("boundSize %f"), boundSize);

    float scale;
	if (boundSize <= 900.0f * 10.0f) {
		if (120.0f * 10.0f <= boundSize) {
			scale = ((0.1f - 0.05f) * ((boundSize - 120.0f * 10.0f) / (900.0f * 10.0f - 120.0f * 10.0f)) + 0.05f);
		}
		else {
			scale = 0.05f;
		}
	}
	else {
		scale = 0.1f;
	}


	UE_LOG(LogTemp, Warning, TEXT("scale %f"), scale);

	float lerpValue = 1000.0f * (scale * smoothness);
	if ((lerpValue <= 1.0) && (lerpValue< 0.0001)) {
		lerpValue= 0.0001;
	}

	UE_LOG(LogTemp, Warning, TEXT("lerpValue %f"), lerpValue);


	interest.Y = (target_interest.Y - interest.Y) * lerpValue + interest.Y;
	UE_LOG(LogTemp, Warning, TEXT("inter %f"), interest.Y);
	interest.Z = (target_interest.Z - interest.Z) * lerpValue + interest.Z;
}
