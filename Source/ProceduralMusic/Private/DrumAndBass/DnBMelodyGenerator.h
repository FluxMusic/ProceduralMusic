#pragma once

#include "CoreMinimal.h"

#include "../MelodyGenerator.h"
#include "../MusicGenerationCoreTypes.h"

//Forward Declarations
struct FRandomStream;
struct FMusicGenerationSpecs;

/**
 * Melody Generation Class for Drum And Bass Melodies
 */
class DnBMelodyGenerator : public MelodyGenerator
{
public:
    DnBMelodyGenerator(FRandomStream& SeedIn, const FMusicGenerationSpecs& SpecsIn, UDataTable* ScalesIn);

    TArray<FMusicNote> GenerateMelody();

private:
    int32 GenerateMelodyNoteAmount();
    ENoteOption GenerateMelodyProbabilities();
	ENoteOption GenerateMarkovMelodyProbabilities(ENoteOption a_PreviousOption);
    FMusicNote GenerateMelodyNote(float PreviousNote, float RootNote, FName Scale);
    TArray<FMusicNote> CheckGeneration(int32 FinalNoteAmount, TArray<FMusicNote> NotesToCheck);
    int32 CheckNoteAmount(TArray<FMusicNote> NoteArray);
};