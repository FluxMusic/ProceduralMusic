#pragma once

#include "CoreMinimal.h"

#include "../MusicGenerationCoreTypes.h"
#include "DrumAndBassSegmentGenerator.h"
#include "DnBDrumGenerator.h"
#include "DnBBassGenerator.h"
#include "DnBChordGenerator.h"
#include "DnBMelodyGenerator.h"

//Forward Declarations
struct FRandomStream;

class DnBVerseGenerator : public DrumAndBassSegmentGenerator
{
public:
    DnBVerseGenerator(FRandomStream& SeedIn, const FMusicGenerationSpecs& SpecsIn, UDataTable* ScalesIn);

    DnBVerseGenerator(DnBVerseGenerator& Other)
    : DrumAndBassSegmentGenerator(Other.Seed, Other.Scales)
    , DrumGenerator(Other.Seed, Other.Specs)
    , BassGenerator(Other.Seed, Other.Specs, Other.Scales)
    , ChordGenerator(Other.Seed, Other.Specs, Other.Scales)
    , MelodyGenerator(Other.Seed, Other.Specs, Other.Scales)
    {}

    FDrumAndBassSegment GenerateSegment();

private:
    void SnapMelodyTimingToBass();
    TArray<int32> GetNoteGroupStarts(TArray<FMusicNote> Notes);

    /**
     * Typically, a Segment is 3 repetitions of a melody and then a variation
     * This function copies the generated loop 3 times, then generates a new melody
     */
    void LengthenSegment();

private:
    DnBDrumGenerator   DrumGenerator;
    DnBBassGenerator   BassGenerator;
    DnBChordGenerator  ChordGenerator;
    DnBMelodyGenerator MelodyGenerator;

    FDrumAndBassSegment Segment;
};