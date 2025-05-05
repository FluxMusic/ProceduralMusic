// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MusicGenerationSpecs.h"
#include "MusicGenerationCoreTypes.h"
#include "DrumAndBassSegmentGenerator.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DrumAndBassGenerator.generated.h"

/**
 * Forward Declarations
 */
class DrumAndBassSegmentGenerator;

struct FRandomStream;
struct FDrumAndBassSegment;

/**
 * 
 */
UCLASS(BlueprintType)
class UDrumAndBassGenerator : public UObject //, public UMusicGenerator
{
	GENERATED_BODY()
	
public:
	UDrumAndBassGenerator();

	void Initialise(const FRandomStream& SeedIn, const FMusicGenerationSpecs& SpecsIn);

private:
	const FString ScaleDataTablePath = "/Script/Engine.DataTable'/Game/Blueprints/DT_MusicScales.DT_MusicScales'";

public:
	UFUNCTION(BlueprintCallable)
	FDrumAndBassSegment GenerateMusic();

private:
	void PreGenerate();

	FDrumAndBassSegment TranscodeSegments(TArray<FDrumAndBassSegment> SegmentsIn);

private:
	TArray<DrumAndBassSegmentGenerator> Segments;

	UPROPERTY(VisibleAnywhere)
    UDataTable* Scales { nullptr };

    FRandomStream Seed { 0 };

    FMusicGenerationSpecs Specs;
};
