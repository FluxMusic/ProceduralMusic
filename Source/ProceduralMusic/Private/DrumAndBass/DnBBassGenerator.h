#pragma once

#include "CoreMinimal.h"

#include "../BassGenerator.h"
#include "../MusicGenerationCoreTypes.h"

// #include "DnBBassGenerator.generated.h"

//Forward Declarations
struct FRandomStream;
struct FMusicGenerationSpecs;

/**
 * Bass Generation Class for Drum And Bass Drums
 */
class DnBBassGenerator : BassGenerator
{
public:
    DnBBassGenerator(FRandomStream& SeedIn, const FMusicGenerationSpecs& SpecsIn, UDataTable* ScalesIn);

    TArray<FMusicNote> GenerateBass();

private:
    int32 GenerateBassNoteAmount();
    FMusicNote GenerateBassMidiNote(float PreviousNote, float RootNote, FName Scale);
    TArray<FMusicNote> CheckGeneration(int32 NoteAmount, TArray<FMusicNote> NotesToCheck);
    int32 CheckNoteAmount(TArray<FMusicNote> NoteArray);
    
    //Deprecated
    ENoteOption GenerateBassNoteProbabilities();
    ENoteOption GenerateMarkovBassNoteProbabilities();
};