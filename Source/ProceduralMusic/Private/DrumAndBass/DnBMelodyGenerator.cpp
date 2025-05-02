// Fill out your copyright notice in the Description page of Project Settings.


#include "DnBMelodyGenerator.h"

#include "Math/RandomStream.h"
#include "../MusicGenerationSpecs.h"

DnBMelodyGenerator::DnBMelodyGenerator(FRandomStream& SeedIn, const FMusicGenerationSpecs& SpecsIn, UDataTable* ScalesIn)
: MelodyGenerator(SeedIn, SpecsIn, ScalesIn)
{}

TArray<FMusicNote> DnBMelodyGenerator::GenerateMelody()
{
    TArray<FMusicNote> MelodyNotes
    {
        {0.f, 0.f},
        {0.f, 0.f},
        {0.f, 0.f},
        {0.f, 0.f},
        {0.f, 0.f},
        {0.f, 0.f},
        {0.f, 0.f},
        {0.f, 0.f},
        {0.f, 0.f},
        {0.f, 0.f},
        {0.f, 0.f},
        {0.f, 0.f},
        {0.f, 0.f},
        {0.f, 0.f},
        {0.f, 0.f},
        {0.f, 0.f}
    };

    int32 MelodyNoteAmount = GenerateMelodyNoteAmount();

    UE_LOG(LogTemp, Display, TEXT("MelodyNoteAmount: %i"), MelodyNoteAmount);

    float PreviousNote ( 0.f );

    TArray<ENoteOption> GenericPattern;
    TArray<ENoteOption> MarkovPattern;

    GenericPattern.SetNum(MelodyNotes.Num());
    MarkovPattern.SetNum (MelodyNotes.Num());

    for (size_t i = 0; i < MelodyNotes.Num(); i++)
    {
        GenericPattern[i] = GenerateMelodyProbabilities();
    }

    for (size_t i = 0; i < MelodyNotes.Num(); i++)
    {
        if (MelodyNotes.IsValidIndex(i - 1))
        {
            MarkovPattern[i] = GenerateMarkovMelodyProbabilities(MarkovPattern[i - 1]);
        }
        else
        {
            MarkovPattern[i] = ENoteOption::NewNote;
        }
    }
    
    TArray<ENoteOption> FinalPattern;
    FinalPattern.SetNum(MarkovPattern.Num());

    for (size_t i = 0; i < MarkovPattern.Num(); i++)
    {
        if (GenericPattern[i] == MarkovPattern[i])
        {
            FinalPattern[i] = MarkovPattern[i];
        }
        else
        {
            if (FinalPattern.IsValidIndex(i - 1))
            {
                FinalPattern[i] = GenerateMarkovMelodyProbabilities(FinalPattern[i - 1]);
            }
            else
            {
                FinalPattern[i] = ENoteOption::NewNote;
            }
        }
    }

    for (size_t i = 0; i < FinalPattern.Num(); i++)
    {
        switch (FinalPattern[i])
        {
            case ENoteOption::NewNote:
            {
                FMusicNote NewNote = GenerateMelodyNote(PreviousNote, 60.f, "Major");
                PreviousNote = NewNote.MidiNote;
                MelodyNotes[i] = NewNote;
                break;
            }
            case ENoteOption::Sustain:
            {
                if (i <= 0)
                {
                    MelodyNotes[i] = MelodyNotes[MelodyNotes.Num() - 1];
                }
                else
                {
                    MelodyNotes[i] = MelodyNotes[i - 1];
                }
                break;
            }
        }
    }

    for (size_t i = 0; i < MelodyNotes.Num(); i++)
    {
        UE_LOG(LogTemp, Display, TEXT("MelodyNoteB4: %f, MelodyNoteVelocityB4: %f"), MelodyNotes[i].MidiNote, MelodyNotes[i].Velocity);
    }
    
    return CheckGeneration(MelodyNoteAmount, MelodyNotes);
}

int32 DnBMelodyGenerator::GenerateMelodyNoteAmount()
{
    switch (Specs.Energy)
    {
    case MusicEnergy::Low:
        return Seed.RandRange(4, 7);
    case MusicEnergy::Medium:
        return Seed.RandRange(5, 8);
    case MusicEnergy::High:
        return Seed.RandRange(6, 10);

    default:
        return 5;
    }
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
            { 0.f,  { { 0.f, 0.05f }, { 1.f, 0.03f }, { 2.f, 0.2f  }, { 3.f, 0.08f }, { 4.f, 0.05f }, { 5.f, 0.15f }, { 6.f, 0.02f }, { 7.f, 0.2f  }, { 8.f, 0.03f }, { 9.f, 0.03f }, { 10.f, 0.1f  }, { 11.f, 0.06f } } },
            { 1.f,  { { 0.f, 0.15f }, { 1.f, 0.05f }, { 2.f, 0.1f  }, { 3.f, 0.05f }, { 4.f, 0.05f }, { 5.f, 0.1f  }, { 6.f, 0.05f }, { 7.f, 0.1f  }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.15f }, { 11.f, 0.05f } } },
            { 2.f,  { { 0.f, 0.1f  }, { 1.f, 0.05f }, { 2.f, 0.1f  }, { 3.f, 0.05f }, { 4.f, 0.05f }, { 5.f, 0.15f }, { 6.f, 0.05f }, { 7.f, 0.1f  }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.15f }, { 11.f, 0.05f } } },
            { 3.f,  { { 0.f, 0.1f  }, { 1.f, 0.05f }, { 2.f, 0.1f  }, { 3.f, 0.05f }, { 4.f, 0.05f }, { 5.f, 0.15f }, { 6.f, 0.05f }, { 7.f, 0.1f  }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.15f }, { 11.f, 0.05f } } },
            { 4.f,  { { 0.f, 0.05f }, { 1.f, 0.03f }, { 2.f, 0.1f  }, { 3.f, 0.05f }, { 4.f, 0.05f }, { 5.f, 0.2f  }, { 6.f, 0.03f }, { 7.f, 0.15f }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.15f }, { 11.f, 0.04f } } },
            { 5.f,  { { 0.f, 0.1f  }, { 1.f, 0.05f }, { 2.f, 0.1f  }, { 3.f, 0.05f }, { 4.f, 0.05f }, { 5.f, 0.05f }, { 6.f, 0.05f }, { 7.f, 0.15f }, { 8.f, 0.05f }, { 9.f, 0.1f  }, { 10.f, 0.15f }, { 11.f, 0.1f  } } },
            { 6.f,  { { 0.f, 0.05f }, { 1.f, 0.05f }, { 2.f, 0.05f }, { 3.f, 0.1f  }, { 4.f, 0.05f }, { 5.f, 0.15f }, { 6.f, 0.05f }, { 7.f, 0.15f }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.15f }, { 11.f, 0.05f } } },
            { 7.f,  { { 0.f, 0.1f  }, { 1.f, 0.05f }, { 2.f, 0.1f  }, { 3.f, 0.05f }, { 4.f, 0.05f }, { 5.f, 0.05f }, { 6.f, 0.05f }, { 7.f, 0.15f }, { 8.f, 0.05f }, { 9.f, 0.1f  }, { 10.f, 0.15f }, { 11.f, 0.1f  } } },
            { 8.f,  { { 0.f, 0.1f  }, { 1.f, 0.05f }, { 2.f, 0.1f  }, { 3.f, 0.05f }, { 4.f, 0.05f }, { 5.f, 0.15f }, { 6.f, 0.05f }, { 7.f, 0.1f  }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.15f }, { 11.f, 0.05f } } },
            { 9.f,  { { 0.f, 0.1f  }, { 1.f, 0.05f }, { 2.f, 0.1f  }, { 3.f, 0.05f }, { 4.f, 0.05f }, { 5.f, 0.15f }, { 6.f, 0.05f }, { 7.f, 0.1f  }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.15f }, { 11.f, 0.05f } } },
            { 10.f, { { 0.f, 0.05f }, { 1.f, 0.05f }, { 2.f, 0.05f }, { 3.f, 0.1f  }, { 4.f, 0.05f }, { 5.f, 0.15f }, { 6.f, 0.05f }, { 7.f, 0.15f }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.15f }, { 11.f, 0.05f } } },
            { 11.f, { { 0.f, 0.05f }, { 1.f, 0.03f }, { 2.f, 0.1f  }, { 3.f, 0.05f }, { 4.f, 0.05f }, { 5.f, 0.2f  }, { 6.f, 0.03f }, { 7.f, 0.15f }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.15f }, { 11.f, 0.04f } } },
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

            if (RandomWeight <= Weight)
            {
                TArray<float> ScaleDegrees = Scales->FindRow<FMusicScale>(Scale, "GenerateMelodyMidiNote", false)->ScaleDegrees;

                TArray<float> ScaleNotes = ScaleDegrees;

                float MelodyNote = PreviousNote + Probability.ScaleDegree;

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