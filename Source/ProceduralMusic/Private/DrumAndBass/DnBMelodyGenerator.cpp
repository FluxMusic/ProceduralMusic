// Fill out your copyright notice in the Description page of Project Settings.


#include "DnBMelodyGenerator.h"

#include "Math/RandomStream.h"
#include "../MusicGenerationSpecs.h"

DnBMelodyGenerator::DnBMelodyGenerator(FRandomStream& SeedIn, const FMusicGenerationSpecs& SpecsIn, UDataTable* ScalesIn)
: MelodyGenerator(SeedIn, SpecsIn, ScalesIn)
{}

TArray<FMusicNote> DnBMelodyGenerator::GenerateMelody()
{
    TArray<FMusicNote> MelodyNotes;
    MelodyNotes.Empty();

    PreGenerate();

    UE_LOG(LogTemp, Display, TEXT("MelodyNoteAmount: %i"), MelodyNoteAmount);

    TArray<ENoteLength> Rhythm1 = GenerateRhythm();
    TArray<ENoteLength> Rhythm2 = Rhythm1;
    TArray<ENoteLength> Rhythm3 = Rhythm1;
    TArray<ENoteLength> Rhythm4 = Rhythm1;

    TArray<FMusicNote> Pattern1 = GenerateNotes(Rhythm1);
    TArray<FMusicNote> Pattern2 = Pattern1;
    TArray<FMusicNote> Pattern3 = Pattern1;
    TArray<FMusicNote> Pattern4 = Pattern1;

    switch (MelodyPattern)
    {
        case EMelodyPattern::AA2A3B:
        {
            Rhythm4 = GenerateRhythm();

            Pattern2 = GenerateNotes(Rhythm2);
            Pattern3 = GenerateNotes(Rhythm3);
            Pattern4 = GenerateNotes(Rhythm4);

            break;
        }
        case EMelodyPattern::AA2AA3:
        {
            Pattern2 = GenerateNotes(Rhythm2);
            Pattern4 = GenerateNotes(Rhythm4);

            break;
        }
        case EMelodyPattern::AAAB:
        {
            Rhythm4 = GenerateRhythm();

            Pattern4 = GenerateNotes(Rhythm4);

            break;
        }
        case EMelodyPattern::AAAA2:
        {
            Pattern4 = GenerateNotes(Rhythm4);

            break;
        }
        case EMelodyPattern::AABC:
        {
            Rhythm3 = GenerateRhythm();
            Rhythm4 = GenerateRhythm();

            break;
        }

    
        default:
        {
            break;
        }
    }

    MelodyNotes.Append(Pattern1);
    MelodyNotes.Append(Pattern2);
    MelodyNotes.Append(Pattern3);
    MelodyNotes.Append(Pattern4);

    for (size_t i = 0; i < MelodyNotes.Num(); i++)
    {
        UE_LOG(LogTemp, Display, TEXT("MelodyNoteB4: %f, MelodyNoteVelocityB4: %f"), MelodyNotes[i].MidiNote, MelodyNotes[i].Velocity);
    }
    
    return MelodyNotes;
}

void DnBMelodyGenerator::PreGenerate()
{
    MelodyNoteAmount = GenerateMelodyNoteAmount();
    MelodyPattern = GenerateMelodyPattern();
}

int32 DnBMelodyGenerator::GenerateMelodyNoteAmount()
{
    switch (Specs.Energy)
    {
    case MusicEnergy::Low:
        return Seed.RandRange(8, 14);
    case MusicEnergy::Medium:
        return Seed.RandRange(10, 18);
    case MusicEnergy::High:
        return Seed.RandRange(12, 20);

    default:
        return 5;
    }
}

EMelodyPattern DnBMelodyGenerator::GenerateMelodyPattern()
{
    float Weight = 0.f;
    const float RandomWeight = Seed.FRand();

    const TArray<TPair<EMelodyPattern, float>> MelodyPatternArray = MelodyPatternMap.Array();

    for (int32 i = 0; i < MelodyPatternArray.Num(); i++)
    {
        Weight += MelodyPatternArray[i].Value;

        if (RandomWeight <= Weight)
        {
            return MelodyPatternArray[i].Key;
        }
    }
    
    UE_LOG(LogTemp, Error, TEXT("DnBMelodyGenerator::GenerateMelodyPattern: Weights setup incorrectly!"));
    return EMelodyPattern::AAAA2;
}

TArray<ENoteLength> DnBMelodyGenerator::GenerateRhythm()
{
    TArray<ENoteLength> Array;
    //Just to be extra safe.
    Array.Empty();

    int32 NotesGenerated = 0;

    TArray<TPair<ENoteLength, float>> NoteLengthArray = NoteLengthMap.Array();

    while (NotesGenerated < MelodyNoteAmount)
    {
        float Weight = 0.f;
        float RandomWeight = Seed.FRand();
        

        for (int32 i = 0; i < NoteLengthMap.Num(); i++)
        {
            Weight += NoteLengthArray[i].Value;

            if (RandomWeight <= Weight)
            {
                UE_LOG(LogTemp, Warning, TEXT("Space: %i"), CheckNoteRhythm(Array));
                UE_LOG(LogTemp, Warning, TEXT("Trying to fit: %i"), NoteLengthArray[i].Key);
                UE_LOG(LogTemp, Warning, TEXT("Remainder: %i"), MelodyNoteAmount - NotesGenerated);

                //Check if note fits
                if (static_cast<uint8>(NoteLengthArray[i].Key) > (64 - CheckNoteRhythm(Array)))
                {
                    UE_LOG(LogTemp, Error, TEXT("Note does not fit"));
                    break;
                }
                
                //Check if remainder of notes can be generated
                if (static_cast<uint8>(NoteLengthArray[i].Key) < (64 - CheckNoteRhythm(Array) - (MelodyNoteAmount - NotesGenerated)))
                {
                    Array.Add(NoteLengthArray[i].Key);
                    NotesGenerated++;

                    UE_LOG(LogTemp, Warning, TEXT("Note: %i"), NoteLengthArray[i].Key);

                    break;
                }
            }
        }
    }

    return Array;
}

TArray<FMusicNote> DnBMelodyGenerator::GenerateNotes(TArray<ENoteLength> Rhythm)
{
    TArray<FMusicNote> Array;
    //Just to be safe
    Array.Empty();

    float PreviousNote = 0.f;

    for (int32 i = 0; i < Rhythm.Num(); i++)
    {
        FMusicNote GeneratedNote = GenerateMelodyNote(PreviousNote, 60.f, "Major");
        UE_LOG(LogTemp, Error, TEXT("GeneratedNote: %f"), GeneratedNote.MidiNote);
        PreviousNote = GeneratedNote.MidiNote;
        if (i == Rhythm.Num() - 1)
        {
            int32 Remainder = 64 - Array.Num();
            for (int32 j = 0; j < Remainder; j++)
            {
                Array.Add(GeneratedNote);
            }
        }
        else
        {
            for (uint8 j = 0; j < static_cast<uint8>(Rhythm[i]); j++)
            {
                Array.Add(GeneratedNote);
            }
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("ArraySize: %i"), Array.Num());

    return Array;
}

ENoteOption DnBMelodyGenerator::GenerateMelodyProbabilities()
{   
    float SustainProbability ( 0.f );
    float NewNoteProbability ( 0.f );

    switch (Specs.Energy)
    {
        case MusicEnergy::Low:
        {
            SustainProbability = 0.6f;
            NewNoteProbability = 0.4f;
            break;
        }
        case MusicEnergy::Medium:
        {
            SustainProbability = 0.5f;
            NewNoteProbability = 0.5f;
            break;
        }
        case MusicEnergy::High:
        {
            SustainProbability = 0.4f;
            NewNoteProbability = 0.6f;
            break;
        }
        default:
        {
            SustainProbability = 0.6f;
            NewNoteProbability = 0.4f;
            break;
        }
    }

    float RandomChance = Seed.FRand();

    float Weight ( 0.f );

    Weight += SustainProbability;

    if (RandomChance <= Weight)
    {
        return ENoteOption::Sustain;
    }
    
    Weight += NewNoteProbability;

    if (RandomChance <= Weight)
    {
        return ENoteOption::NewNote;
    }
    
    UE_LOG(LogTemp, Error, TEXT("Error in DrumAndBassGenerator.cpp: GenerateMelodyProbabilities! Weights set up wrong!"));

    return ENoteOption::NewNote;
}

ENoteOption DnBMelodyGenerator::GenerateMarkovMelodyProbabilities(ENoteOption PreviousOption)
{
    TMap<ENoteOption, TArray<FMarkovRhythmChain>> MarkovRhythm
    {
        { ENoteOption::NewNote, { { ENoteOption::NewNote, 0.1f }, { ENoteOption::Sustain, 0.8f } } },
        { ENoteOption::Sustain, { { ENoteOption::NewNote, 0.5f }, { ENoteOption::Sustain, 0.4f } } }
    };

    float RandomWeight = Seed.FRand();
    float Weight ( 0.f );

    TArray<FMarkovRhythmChain>* Probabilities = MarkovRhythm.Find(PreviousOption);

    if (Probabilities == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Error in DrumAndBassGenerator.cpp: GenerateMarkovMelodyProbabilities! Could not find previous option in the Markov Chain! PreviousOption: %i"), PreviousOption);

        return ENoteOption::NewNote;
    }
    
    for (FMarkovRhythmChain& Probability : *Probabilities)
    {
        Weight += Probability.Probability;

        if (RandomWeight <= Weight)
        {
            return Probability.NoteOption;
        }
    }

    return ENoteOption::NewNote;
}

FMusicNote DnBMelodyGenerator::GenerateMelodyNote(float PreviousNote, float RootNote, FName Scale)
{
    if (PreviousNote <= 0.f)
    {
        float RandomMidiNote = static_cast<float>(Seed.RandRange(60, 71));

        TArray<float> ScaleDegrees = Scales->FindRow<FMusicScale>(Scale, "GenerateMelodyMidiNote", false)->ScaleDegrees;

        TArray<float> ScaleNotes = ScaleDegrees;

        for (size_t i = 0; i < ScaleDegrees.Num(); i++)
        {
            ScaleNotes[i] = ScaleDegrees[i] + RootNote;
        }

        if (ScaleNotes.Find(RandomMidiNote) != INDEX_NONE)
        {
            return FMusicNote( RandomMidiNote, 127.f );
        }
        else
        {
            return FMusicNote( RandomMidiNote + 1.f, 127.f );
        }
    }
    else
    {
        TMap<float, TArray<FMarkovChain>> MarkovProbabilities
        {
            { 0.f,  { { 0.f, 0.01f }, { 1.f, 0.03f }, { 2.f, 0.5f  }, { 3.f, 0.03f }, { 4.f, 0.2f  }, { 5.f, 0.1f  }, { 6.f, 0.02f }, { 7.f, 0.07f }, { 8.f, 0.01f }, { 9.f, 0.01f }, { 10.f, 0.01f }, { 11.f, 0.01f } } },

            { 1.f,  { { 0.f, 0.4f  }, { 1.f, 0.01f }, { 2.f, 0.01f }, { 3.f, 0.01f }, { 4.f, 0.4f  }, { 5.f, 0.02f }, { 6.f, 0.1f  }, { 7.f, 0.01f }, { 8.f, 0.01f }, { 9.f, 0.01f }, { 10.f, 0.01f }, { 11.f, 0.01f } } },
            { 2.f,  { { 0.f, 0.4f  }, { 1.f, 0.01f }, { 2.f, 0.01f }, { 3.f, 0.01f }, { 4.f, 0.4f  }, { 5.f, 0.02f }, { 6.f, 0.1f  }, { 7.f, 0.01f }, { 8.f, 0.01f }, { 9.f, 0.01f }, { 10.f, 0.01f }, { 11.f, 0.01f } } },

            { 3.f,  { { 0.f, 0.25f }, { 1.f, 0.01f }, { 2.f, 0.4f  }, { 3.f, 0.02f }, { 4.f, 0.01f }, { 5.f, 0.25f }, { 6.f, 0.01f }, { 7.f, 0.01f }, { 8.f, 0.01f }, { 9.f, 0.01f }, { 10.f, 0.01f }, { 11.f, 0.01f } } },
            { 4.f,  { { 0.f, 0.25f }, { 1.f, 0.01f }, { 2.f, 0.4f  }, { 3.f, 0.02f }, { 4.f, 0.01f }, { 5.f, 0.25f }, { 6.f, 0.01f }, { 7.f, 0.01f }, { 8.f, 0.01f }, { 9.f, 0.01f }, { 10.f, 0.01f }, { 11.f, 0.01f } } },

            { 5.f,  { { 0.f, 0.01f }, { 1.f, 0.01f }, { 2.f, 0.1f  }, { 3.f, 0.02f }, { 4.f, 0.35f }, { 5.f, 0.01f }, { 6.f, 0.01f }, { 7.f, 0.35f }, { 8.f, 0.02f }, { 9.f, 0.1f  }, { 10.f, 0.02f }, { 11.f, 0.01f } } },
            { 6.f,  { { 0.f, 0.01f }, { 1.f, 0.01f }, { 2.f, 0.1f  }, { 3.f, 0.02f }, { 4.f, 0.35f }, { 5.f, 0.01f }, { 6.f, 0.01f }, { 7.f, 0.35f }, { 8.f, 0.02f }, { 9.f, 0.1f  }, { 10.f, 0.02f }, { 11.f, 0.01f } } },

            { 7.f,  { { 0.f, 0.01f }, { 1.f, 0.01f }, { 2.f, 0.01f }, { 3.f, 0.01f }, { 4.f, 0.1f  }, { 5.f, 0.02f }, { 6.f, 0.35f }, { 7.f, 0.01f }, { 8.f, 0.02f }, { 9.f, 0.35f }, { 10.f, 0.1f  }, { 11.f, 0.01f } } },
            { 8.f,  { { 0.f, 0.01f }, { 1.f, 0.01f }, { 2.f, 0.01f }, { 3.f, 0.01f }, { 4.f, 0.1f  }, { 5.f, 0.02f }, { 6.f, 0.35f }, { 7.f, 0.01f }, { 8.f, 0.02f }, { 9.f, 0.35f }, { 10.f, 0.1f  }, { 11.f, 0.01f } } },

            { 9.f,  { { 0.f, 0.01f }, { 1.f, 0.01f }, { 2.f, 0.01f }, { 3.f, 0.01f }, { 4.f, 0.01f }, { 5.f, 0.2f  }, { 6.f, 0.02f }, { 7.f, 0.35f }, { 8.f, 0.01f }, { 9.f, 0.01f }, { 10.f, 0.01f }, { 11.f, 0.35f } } },
            { 10.f, { { 0.f, 0.01f }, { 1.f, 0.01f }, { 2.f, 0.01f }, { 3.f, 0.01f }, { 4.f, 0.01f }, { 5.f, 0.2f  }, { 6.f, 0.02f }, { 7.f, 0.35f }, { 8.f, 0.01f }, { 9.f, 0.01f }, { 10.f, 0.01f }, { 11.f, 0.35f } } },

            { 11.f, { { 0.f, 0.01f }, { 1.f, 0.01f }, { 2.f, 0.01f }, { 3.f, 0.01f }, { 4.f, 0.01f }, { 5.f, 0.01f }, { 6.f, 0.1f  }, { 7.f, 0.01f }, { 8.f, 0.3f  }, { 9.f, 0.02f }, { 10.f, 0.5f  }, { 11.f, 0.01f } } },
        };

        float RandomWeight = Seed.FRand();
        float Weight ( 0.f );

        TArray<FMarkovChain>* Probabilities = MarkovProbabilities.Find(PreviousNote - RootNote);

        if (Probabilities == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("Error in DrumAndBassGenerator.cpp: GenerateMelodyMidiNote! Could not find previous note in the Markov Chain! PreviousNote: %f"), PreviousNote - RootNote);

            return FMusicNote( 0.f, 0.f );
        }
        
        for (FMarkovChain& Probability : *Probabilities)
        {
            Weight += Probability.Probability;

            UE_LOG(LogTemp, Warning, TEXT("Probability: %f"), Probability.Probability);

            if (RandomWeight <= Weight)
            {
                TArray<float> ScaleDegrees = Scales->FindRow<FMusicScale>(Scale, "GenerateMelodyMidiNote", false)->ScaleDegrees;

                TArray<float> ScaleNotes = ScaleDegrees;

                float MelodyNote = RootNote + Probability.ScaleDegree;

                for (size_t i = 0; i < ScaleDegrees.Num(); i++)
                {
                    ScaleNotes[i] = ScaleDegrees[i] + RootNote;
                }
                
                if (ScaleNotes.Find(MelodyNote) != INDEX_NONE)
                {
                    return FMusicNote( MelodyNote, 127.f );
                }
                else
                {
                    //TODO: This clamps the note to the highest note in the scale,
                    //      it should be tested if lowering by an octave is better
                    while (ScaleNotes.Find(MelodyNote) == INDEX_NONE)
                    {
                        if (MelodyNote > RootNote && MelodyNote <= ScaleNotes[ScaleNotes.Num() - 1])
                        {
                            MelodyNote -= 1.f;
                        }
                        else
                        {
                            MelodyNote -= 12.f;
                        }
                    }
                    return FMusicNote( MelodyNote, 127.f );
                }
            }
        }
    }

    //Shouldn't execute
    UE_LOG(LogTemp, Error, TEXT("Error in DrumAndBassGenerator.cpp: GenerateMelodyMidiNote! Reached end of function!"));
    return FMusicNote( 0.f, 0.f );
}

int32 DnBMelodyGenerator::CheckNoteRhythm(TArray<ENoteLength> NotesToCheck)
{
    if (NotesToCheck.Num() == 0)
    {
        return 0;
    }
    
    int32 Result = 0;

    for (int32 i = 0; i < NotesToCheck.Num(); i++)
    {
        Result += static_cast<int32>(NotesToCheck[i]);
    }
    
    return Result;
}

TArray<FMusicNote> DnBMelodyGenerator::CheckGeneration(int32 FinalNoteAmount, TArray<FMusicNote> NotesToCheck)
{
    int32 NoteAmount = CheckNoteAmount(NotesToCheck);
    
    UE_LOG(LogTemp, Display, TEXT("NoteAmount: %i"), NoteAmount);

    if (NoteAmount == FinalNoteAmount)
    {
        return NotesToCheck;
    }
    
    TArray<FMusicNote> FinalNotes = NotesToCheck;

    int32 Failsafe = 0;
    
    while (CheckNoteAmount(FinalNotes) > FinalNoteAmount)
    {
        Failsafe++;

        if (Failsafe > 20)
        {
            UE_LOG(LogTemp, Error, TEXT("While Loop exceeded 20 revisions!"));
            break;
        }

        TArray<int32> GroupStarts;
        GroupStarts.Add(0);

        for (size_t i = 1; i < FinalNotes.Num(); i++)
        {
            if (FinalNotes[i].MidiNote != FinalNotes[i - 1].MidiNote)
            {
                GroupStarts.Add(i);
            }
        }
        
        int32 ShortestGroupStart = GroupStarts[0];
        int32 ShortestGroupLength = FinalNotes.Num();

        for (size_t i = 0; i < GroupStarts.Num(); i++)
        {
            int32 Start = GroupStarts[i];
            int32 End = (i + 1 < GroupStarts.Num()) ? GroupStarts[i + 1] : FinalNotes.Num();
            int32 Length = End - Start;

            if (Length < ShortestGroupLength)
            {
                ShortestGroupStart = Start;
                ShortestGroupLength = Length;
            }
        }
        
        int32 GroupStart = ShortestGroupStart;
        int32 GroupEnd = GroupStart + ShortestGroupLength;

        if (GroupStart > 0)
        {
            FMusicNote PreviousNote = FinalNotes[GroupStart - 1];
            for (size_t i = GroupStart; i < GroupEnd; i++)
            {
                FinalNotes[i] = PreviousNote;
            }
        }
        else if (GroupEnd < FinalNotes.Num())
        {
            FMusicNote NextNote = FinalNotes[GroupEnd];
            for (size_t i = GroupStart; i < GroupEnd; i++)
            {
                FinalNotes[i] = NextNote;
            }
        }
    }
    return FinalNotes;
}

int32 DnBMelodyGenerator::CheckNoteAmount(TArray<FMusicNote> NoteArray)
{
    if (NoteArray.Num() == 0)
    {
        return 0;
    }
    
    int32 NoteAmount = 1;

    for (size_t i = 1; i < NoteArray.Num(); i++)
    {
        if (NoteArray[i].MidiNote != NoteArray[i - 1].MidiNote)
        {
            NoteAmount++;
        }
    }

    return NoteAmount;
}