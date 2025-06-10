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
    void PreGenerate();

    int32 GenerateBassNoteAmount();
    TArray<ENoteLength> GenerateRhythm();
    TArray<FMusicNote> GenerateNotes(TArray<ENoteLength> Rhythm);
    FMusicNote GenerateBassMidiNote(float PreviousNote, float RootNote, FName Scale);
    TArray<FMusicNote> CheckGeneration(int32 NoteAmount, TArray<FMusicNote> NotesToCheck);
    int32 CheckNoteAmount(TArray<FMusicNote> NoteArray);

    int32 CheckNoteRhythm(TArray<ENoteLength> NotesToCheck);

private:
    int32 BassNoteAmount { 0 };

    const TMap<ENoteLength, float> NoteLengthMap
    {
        { ENoteLength::Quarter, 0.05f },
        { ENoteLength::Half,    0.2f  },
        { ENoteLength::HalfDot, 0.15f },
        { ENoteLength::Full,    0.5f  },
        { ENoteLength::FullDot, 0.1f  },
    };
};