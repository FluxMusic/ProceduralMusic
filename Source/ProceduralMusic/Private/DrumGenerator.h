#pragma once

#include "CoreMinimal.h"

//Forward Declarations
struct FRandomStream;
struct FMusicGenerationSpecs;

class DrumGenerator
{
public:
    DrumGenerator(FRandomStream& SeedIn, const FMusicGenerationSpecs& SpecsIn);

private:
    // virtual TArray<float> CreateKickProbabilities();
    // virtual TArray<float> CreateSnareProbabilities();
    // virtual TArray<float> CreateCymbalProbabilities();
    // virtual TArray<float> CreateTomProbabilities();
    // virtual TArray<float> CreatePercProbabilities();

protected:
    FRandomStream& Seed;

    const FMusicGenerationSpecs& Specs;
};