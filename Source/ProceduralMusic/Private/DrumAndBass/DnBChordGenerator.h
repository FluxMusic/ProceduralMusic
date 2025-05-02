#pragma once

#include "CoreMinimal.h"

#include "../ChordGenerator.h"
#include "../MusicGenerationCoreTypes.h"

//Forward Declarations
struct FRandomStream;
struct FMusicGenerationSpecs;

/**
 * Chord Generation Class for Drum And Bass Chords
 */
class DnBChordGenerator : public ChordGenerator
{
public:
    DnBChordGenerator(FRandomStream& SeedIn, const FMusicGenerationSpecs& SpecsIn, UDataTable* ScalesIn);

    FChordArrays GenerateChords(TArray<FMusicNote> BassNotes);

private:
    TArray<FMusicNote> GenerateChordNotes(float BassNote, float RootNote, FName Scale);
    FChordArrays TranscodeChords(TArray<TArray<FMusicNote>> ChordArray);
};