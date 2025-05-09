// Fill out your copyright notice in the Description page of Project Settings.


#include "DnBBassGenerator.h"

#include "Math/RandomStream.h"
#include "../MusicGenerationSpecs.h"

DnBBassGenerator::DnBBassGenerator(FRandomStream &SeedIn, const FMusicGenerationSpecs &SpecsIn, UDataTable* ScalesIn)
: BassGenerator(SeedIn, SpecsIn, ScalesIn)
{}

TArray<FMusicNote> DnBBassGenerator::GenerateBass()
{
    TArray<FMusicNote> BassNotes
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

    int32 BassNoteAmount = GenerateBassNoteAmount();

    UE_LOG(LogTemp, Display, TEXT("BassNoteAmount: %i"), BassNoteAmount);

    float PreviousNote ( 0.f );
    
    TArray<ENoteOption> BassRhythm;
    BassRhythm.SetNum(BassNotes.Num());

    TArray<float> Possibilities { 1.f, 0.f, 0.2f, 0.f, 0.8f, 0.f, 0.2f, 0.f, 0.8f, 0.f, 0.2f, 0.f, 0.8f, 0.f, 0.2f, 0.f };

    int32 NoteGenerationCount = 0;

    for (int32 i = 0; i < BassRhythm.Num(); i++)
    {
        float RandomChance = Seed.FRand();

        if (RandomChance <= Possibilities[i] || (BassNotes.Num() - i <= BassNoteAmount - NoteGenerationCount))
        {
            BassRhythm[i] = ENoteOption::NewNote;
        }
        else
        {
            BassRhythm[i] = ENoteOption::Sustain;
        }
    }

    for (size_t i = 0; i < BassRhythm.Num(); i++)
    {
        switch (BassRhythm[i])
        {
            case ENoteOption::NewNote:
            {
                FMusicNote NewNote = GenerateBassMidiNote(PreviousNote, 60.f, "Major");
                PreviousNote = NewNote.MidiNote;
                BassNotes[i] = NewNote;
                break;
            }
            case ENoteOption::Sustain:
            {
                if (i <= 0)
                {
                    BassNotes[i] = BassNotes[BassNotes.Num() - 1];
                }
                else
                {
                    BassNotes[i] = BassNotes[i - 1];
                }
                break;
            }
        }
    }
    
    for (size_t i = 0; i < BassNotes.Num(); i++)
    {
        UE_LOG(LogTemp, Display, TEXT("BassNoteB4: %f, BassNoteVelocityB4: %f"), BassNotes[i].MidiNote, BassNotes[i].Velocity);
    }
    
    return CheckGeneration(BassNoteAmount, BassNotes);
}

int32 DnBBassGenerator::GenerateBassNoteAmount()
{
    switch (Specs.Energy)
    {
    case MusicEnergy::Low:
        return Seed.RandRange(4, 5);
    case MusicEnergy::Medium:
        return Seed.RandRange(5, 6);
    case MusicEnergy::High:
        return Seed.RandRange(5, 7);

    default:
        return 4;
    }
}

FMusicNote DnBBassGenerator::GenerateBassMidiNote(float PreviousNote, float RootNote, FName Scale)
{
    if (PreviousNote <= 0.f)
    {
        float RandomMidiNote = static_cast<float>(Seed.RandRange(60, 71));

        if (!Scales) 
        {
            UE_LOG(LogTemp, Warning, TEXT("Scales is nullptr!"));
        }

        TArray<float> ScaleDegrees = Scales->FindRow<FMusicScale>(Scale, "GenerateBassMidiNote", false)->ScaleDegrees;

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
            { 0.f,  { { 0.f, 0.15f }, { 1.f, 0.05f }, { 2.f, 0.1f }, { 3.f, 0.1f }, { 4.f, 0.05f }, { 5.f, 0.15f }, { 6.f, 0.05f }, { 7.f, 0.2f }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.05f }, { 11.f, 0.f } } },
            { 1.f,  { { 0.f, 0.15f }, { 1.f, 0.05f }, { 2.f, 0.1f }, { 3.f, 0.1f }, { 4.f, 0.05f }, { 5.f, 0.1f  }, { 6.f, 0.05f }, { 7.f, 0.2f }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.05f }, { 11.f, 0.f } } },
            { 2.f,  { { 0.f, 0.15f }, { 1.f, 0.05f }, { 2.f, 0.1f }, { 3.f, 0.1f }, { 4.f, 0.05f }, { 5.f, 0.1f  }, { 6.f, 0.05f }, { 7.f, 0.2f }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.05f }, { 11.f, 0.f } } },
            { 3.f,  { { 0.f, 0.2f  }, { 1.f, 0.05f }, { 2.f, 0.1f }, { 3.f, 0.1f }, { 4.f, 0.05f }, { 5.f, 0.1f  }, { 6.f, 0.05f }, { 7.f, 0.2f }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.05f }, { 11.f, 0.f } } },
            { 4.f,  { { 0.f, 0.15f }, { 1.f, 0.05f }, { 2.f, 0.1f }, { 3.f, 0.1f }, { 4.f, 0.05f }, { 5.f, 0.1f  }, { 6.f, 0.05f }, { 7.f, 0.2f }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.05f }, { 11.f, 0.f } } },
            { 5.f,  { { 0.f, 0.15f }, { 1.f, 0.05f }, { 2.f, 0.1f }, { 3.f, 0.1f }, { 4.f, 0.05f }, { 5.f, 0.1f  }, { 6.f, 0.05f }, { 7.f, 0.2f }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.05f }, { 11.f, 0.f } } },
            { 6.f,  { { 0.f, 0.15f }, { 1.f, 0.05f }, { 2.f, 0.1f }, { 3.f, 0.1f }, { 4.f, 0.05f }, { 5.f, 0.1f  }, { 6.f, 0.05f }, { 7.f, 0.2f }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.05f }, { 11.f, 0.f } } },
            { 7.f,  { { 0.f, 0.25f }, { 1.f, 0.05f }, { 2.f, 0.1f }, { 3.f, 0.1f }, { 4.f, 0.05f }, { 5.f, 0.1f  }, { 6.f, 0.05f }, { 7.f, 0.1f }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.05f }, { 11.f, 0.f } } },
            { 8.f,  { { 0.f, 0.15f }, { 1.f, 0.05f }, { 2.f, 0.1f }, { 3.f, 0.1f }, { 4.f, 0.05f }, { 5.f, 0.1f  }, { 6.f, 0.05f }, { 7.f, 0.2f }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.05f }, { 11.f, 0.f } } },
            { 9.f,  { { 0.f, 0.15f }, { 1.f, 0.05f }, { 2.f, 0.1f }, { 3.f, 0.1f }, { 4.f, 0.05f }, { 5.f, 0.1f  }, { 6.f, 0.05f }, { 7.f, 0.2f }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.05f }, { 11.f, 0.f } } },
            { 10.f, { { 0.f, 0.15f }, { 1.f, 0.05f }, { 2.f, 0.1f }, { 3.f, 0.1f }, { 4.f, 0.05f }, { 5.f, 0.1f  }, { 6.f, 0.05f }, { 7.f, 0.2f }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.05f }, { 11.f, 0.f } } },
            { 11.f, { { 0.f, 0.15f }, { 1.f, 0.05f }, { 2.f, 0.1f }, { 3.f, 0.1f }, { 4.f, 0.05f }, { 5.f, 0.1f  }, { 6.f, 0.05f }, { 7.f, 0.2f }, { 8.f, 0.05f }, { 9.f, 0.05f }, { 10.f, 0.05f }, { 11.f, 0.f } } }
        };

        float RandomWeight = Seed.FRand();
        float Weight ( 0.f );

        TArray<FMarkovChain>* Probabilities = MarkovProbabilities.Find(PreviousNote - RootNote);

        if (Probabilities == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("Error in DrumAndBassGenerator.cpp: GenerateBassMidiNote! Could not find previous note in the Markov Chain! PreviousNote: %f"), PreviousNote - RootNote);

            return FMusicNote( 0.f, 0.f );
        }
        
        for (FMarkovChain& Probability : *Probabilities)
        {
            Weight += Probability.Probability;

            if (RandomWeight <= Weight)
            {
                TArray<float> ScaleDegrees = Scales->FindRow<FMusicScale>(Scale, "GenerateBassMidiNote", false)->ScaleDegrees;

                TArray<float> ScaleNotes = ScaleDegrees;

                float BassNote = PreviousNote + Probability.ScaleDegree;

                for (size_t i = 0; i < ScaleDegrees.Num(); i++)
                {
                    ScaleNotes[i] = ScaleDegrees[i] + RootNote;
                }
                
                if (ScaleNotes.Find(BassNote) != INDEX_NONE)
                {
                    return FMusicNote( BassNote, 127.f );
                }
                else
                {
                    //TODO: This clamps the note to the highest note in the scale,
                    //      it should be tested if lowering by an octave is better
                    while (ScaleNotes.Find(BassNote) == INDEX_NONE)
                    {
                        // BassNote -= 1.f;

                        if (BassNote > RootNote && BassNote <= ScaleNotes[ScaleNotes.Num() - 1])
                        {
                            BassNote -= 1.f;
                        }
                        else
                        {
                            BassNote -= 12.f;
                        }
                    }
                    return FMusicNote( BassNote, 127.f );
                }
            }
        }
    }

    //Shouldn't execute
    UE_LOG(LogTemp, Error, TEXT("Error in DrumAndBassGenerator.cpp: GenerateBassMidiNote! Reached end of function!"));
    return FMusicNote( 0.f, 0.f );
}

TArray<FMusicNote> DnBBassGenerator::CheckGeneration(int32 FinalNoteAmount, TArray<FMusicNote> NotesToCheck)
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

int32 DnBBassGenerator::CheckNoteAmount(TArray<FMusicNote> NoteArray)
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
