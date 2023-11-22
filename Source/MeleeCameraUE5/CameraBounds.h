// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CameraBounds.generated.h"

/**
 * 
 */
UCLASS()
class MELEECAMERAUE5_API UCameraBounds : public UObject
{
	GENERATED_BODY()

	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			float x_min;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			float y_min;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			float x_max;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			float y_max;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TArray<AActor*> subjects;

		//UFUNCTION(BlueprintCallable, Category="CameraBounds")
		//	void MyCustomFunction();
};
