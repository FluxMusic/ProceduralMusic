// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MusicGenerationSpecs.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DrumAndBassGenerator.generated.h"


/**
 * TEMPORARY
 * TODO: Get this out after prototype
 */

USTRUCT(BlueprintType)
struct FArrayWrapper
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<float> Array;
};


/**
 * 
 */
UCLASS()
class UDrumAndBassGenerator : public UObject
{
	GENERATED_BODY()
	
public:
	UDrumAndBassGenerator();

public:
	UFUNCTION(BlueprintCallable)
	TArray<FArrayWrapper> GenerateMusic(int seed, FMusicGenerationSpecs Specs);

private:
	/**
	 * Functions for Drum Generation
	 */

	void GenerateDrums();
	void CreateKickProbabilities();
	void CreateSnareProbabilities();
	void CreateHHProbabilities();
	void CreatePercProbabilities();
	void GenerateDrumPattern();
};
