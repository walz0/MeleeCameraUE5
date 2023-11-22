// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "CamSubject.h"
#include "Dummy.generated.h"

UCLASS()
class MELEECAMERAUE5_API ADummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADummy();

	FVector pos = FVector(0.0f,0.0f,0.0f);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	UCamSubject* cam_subject;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
