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
    void PreGenerate();
    int32 GenerateMelodyNoteAmount();
    EMelodyPattern GenerateMelodyPattern();
    TArray<ENoteLength> GenerateRhythm();
    TArray<FMusicNote> GenerateNotes(TArray<ENoteLength> Rhythm);
    FMusicNote GenerateMelodyNote(float PreviousNote, float RootNote, FName Scale);

    int32 CheckNoteRhythm(TArray<ENoteLength> NotesToCheck);

    ENoteOption GenerateMelodyProbabilities();
	ENoteOption GenerateMarkovMelodyProbabilities(ENoteOption a_PreviousOption);
    TArray<FMusicNote> CheckGeneration(int32 FinalNoteAmount, TArray<FMusicNote> NotesToCheck);
    int32 CheckNoteAmount(TArray<FMusicNote> NoteArray);

private:
    int32 MelodyNoteAmount { 0 };

    EMelodyPattern MelodyPattern { EMelodyPattern::AA2A3B };

    const TMap<EMelodyPattern, float> MelodyPatternMap
    {
        { EMelodyPattern::AA2A3B, 0.2f },
        { EMelodyPattern::AA2AA3, 0.2f },
        { EMelodyPattern::AAAB,   0.2f },
        { EMelodyPattern::AAAA2,  0.2f },
        { EMelodyPattern::AABC,   0.2f },
    };

    const TMap<ENoteLength, float> NoteLengthMap
    {
        { ENoteLength::Sixteenth,  0.05f },
        { ENoteLength::Eighth,     0.2f  },
        { ENoteLength::EighthDot,  0.15f },
        { ENoteLength::Quarter,    0.3f  },
        { ENoteLength::QuarterDot, 0.15f },
        { ENoteLength::Half,       0.1f  },
        { ENoteLength::HalfDot,    0.05f },
    };
};