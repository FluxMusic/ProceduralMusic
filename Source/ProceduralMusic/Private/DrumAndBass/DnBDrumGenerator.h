#pragma once

#include "CoreMinimal.h"

#include "../DrumGenerator.h"
#include "../MusicGenerationCoreTypes.h"

//Forward Declarations
struct FRandomStream;
struct FMusicGenerationSpecs;

/**
 * Drum Generation Class for Drum And Bass Drums
 */
class DnBDrumGenerator : DrumGenerator
{
public:
    DnBDrumGenerator(FRandomStream& SeedIn, const FMusicGenerationSpecs& SpecsIn);

    /**
     * Generates Patterns for each Instrument in the Drums
     * And returns the pattern
     */
    FDrumAndBassDrums GenerateDrums();

private:
    /**
     * These functions contain typical patterns for Drum and Bass
     * And have been transcoded to return probabilities for each hit
     */
    TArray<float> CreateKickProbabilities();
    TArray<float> CreateSnareProbabilities();
    TArray<float> CreateCymbalProbabilities();
    TArray<float> CreatePercProbabilities();

    /**
     * Takes the probabilities created from the respective functions
     * And generates a Note based on the probability
     */
    TArray<float> GenerateDrumPattern(const TArray<float>& Probabilities);
};