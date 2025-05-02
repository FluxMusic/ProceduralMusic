#pragma once

#include "CoreMinimal.h"

//Forward Declarations
struct FRandomStream;
struct FMusicGenerationSpecs;

class BassGenerator
{
public:
    BassGenerator(FRandomStream& SeedIn, const FMusicGenerationSpecs& SpecsIn, UDataTable* ScalesIn);

private:
    // virtual TArray<float> CreateKickProbabilities();
    // virtual TArray<float> CreateSnareProbabilities();
    // virtual TArray<float> CreateCymbalProbabilities();
    // virtual TArray<float> CreateTomProbabilities();
    // virtual TArray<float> CreatePercProbabilities();

protected:
    FRandomStream& Seed;

    const FMusicGenerationSpecs& Specs;

    UDataTable* Scales;
};