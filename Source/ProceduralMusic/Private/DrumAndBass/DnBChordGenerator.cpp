// Fill out your copyright notice in the Description page of Project Settings.


#include "DnBChordGenerator.h"

#include "Math/RandomStream.h"
#include "../MusicGenerationSpecs.h"

DnBChordGenerator::DnBChordGenerator(FRandomStream& SeedIn, const FMusicGenerationSpecs& SpecsIn, UDataTable* ScalesIn)
: ChordGenerator(SeedIn, SpecsIn, ScalesIn)
{}

FChordArrays DnBChordGenerator::GenerateChords(TArray<FMusicNote> BassNotes)
{
    TArray<TArray<FMusicNote>> ChordArray;
    ChordArray.SetNum(BassNotes.Num());

    TArray<int32> GroupStarts;
    GroupStarts.Add(0);

    for (size_t i = 1; i < BassNotes.Num(); i++)
    {
        if (BassNotes[i].MidiNote != BassNotes[i - 1].MidiNote)
        {
            GroupStarts.Add(i);
        }
    }

    TArray<int32> GroupLengths;

    for (int32 GroupStart : GroupStarts)
    {
        bool bFound = false;

        for (size_t i = ( GroupStart + 1); i < BassNotes.Num(); i++)
        {
            if (BassNotes[i].MidiNote != BassNotes[i - 1].MidiNote)
            {
                GroupLengths.Add(i - GroupStart);
                bFound = true;
                break;
            }
        }

        if (!bFound)
        {
            GroupLengths.Add(BassNotes.Num() - GroupStart);
        }
    }
    
    for (int32 i = 0; i < GroupStarts.Num(); i++)
    {
        TArray<FMusicNote> GeneratedChord = GenerateChordNotes(BassNotes[GroupStarts[i]].MidiNote, 60.f, "Major");

        for (size_t j = 0; j < GroupLengths[i]; j++)
        {
            ChordArray[GroupStarts[i] + j] = GeneratedChord;
        }
    }

    // for (size_t i = 0; i < m_Pattern.BassNotes.Num(); i++)
    // {
    //     ChordArray[i] = GenerateChordNotes(m_Pattern.BassNotes[i].MidiNote, 60.f, a_Seed, a_Specs, "Major");
    // }

    return TranscodeChords(ChordArray);
}

TArray<FMusicNote> DnBChordGenerator::GenerateChordNotes(float BassNote, float RootNote, FName Scale)
{
    float RandomWeight = Seed.FRand();

    TArray<float> ChordNotes;
    ChordNotes.SetNum(7);

    uint8 ChordNoteAmount ( 0 );

    switch (Specs.Energy)
    {
        case MusicEnergy::Low:
            ChordNoteAmount = Seed.RandRange(3, 4);
            break;
        case MusicEnergy::Medium:
            ChordNoteAmount = Seed.RandRange(3, 5);
            break;
        case MusicEnergy::High:
            ChordNoteAmount = Seed.RandRange(3, 7);
            break;
        
        default:
            ChordNoteAmount = Seed.RandRange(3, 4);
            break;
    }

    for (size_t i = 0; i < ChordNotes.Num(); i++)
    {
        if (!(i < ChordNoteAmount))
        {
            ChordNotes[i] = -1.f;
            continue;
        }
            
        if (i == 0)
        {
            ChordNotes[i] = 0.f;
        }
        else if (i == 1)
        {
            float ThirdWeight ( 0.f );
            
            float ThirdProbability ( 1.f );
            float Sus2Probability  ( 0.00f );
            float Sus4Probability  ( 0.00f );

            ThirdWeight += ThirdProbability;

            if (RandomWeight <= ThirdWeight)
            {
                ChordNotes[i] = 4.f;

                UE_LOG(LogTemp, Display, TEXT("Generated Third!"));
            }
            else
            {
                ThirdWeight += Sus2Probability;
                
                if (RandomWeight <= ThirdWeight)
                {
                    ChordNotes[i] = 2.f;
    
                    UE_LOG(LogTemp, Display, TEXT("Generated Sus2!"));
                }
                else
                {
                    ThirdWeight += Sus4Probability;
                    
                    if (RandomWeight <= ThirdWeight)
                    {
                        ChordNotes[i] = 5.f;
        
                        UE_LOG(LogTemp, Display, TEXT("Generated Sus4!"));
                    }
                }
            }
        }
        else if (i == 2)
        {
            ChordNotes[i] = 7.f;
        }
        else if (i == 3)
        {
            ChordNotes[i] = 11.f;
        }
        else if (i == 4)
        {
            ChordNotes[i] = 14.f;
        }
        else if (i == 5)
        {
            ChordNotes[i] = 17.f;
        }
        else if (i == 6)
        {
            ChordNotes[i] = 21.f;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Error in DrumAndBassGenerator.cpp: GenerateChordNotes! Too many Notes in a chord specified!"));
        }
    }

    TArray<float> ScaleDegrees = Scales->FindRow<FMusicScale>(Scale, "GenerateChordNotes", false)->ScaleDegrees;
    TArray<float> ExtendedScale = ScaleDegrees;
    for (float Entry : ScaleDegrees)
    {
        ExtendedScale.Add(Entry + 12.f);
    }

    TArray<FMusicNote> ChordMidiNotes;
    ChordMidiNotes.SetNum(ChordNotes.Num());

    for (size_t i = 0; i < ChordNotes.Num(); i++)
    {
        if (ChordNotes[i] == -1.f || BassNote == 0.f)
        {
            ChordMidiNotes[i] = FMusicNote( 0.f, 0.f );
            continue;
        }
        
        float MidiNote = BassNote - RootNote + ChordNotes[i];
        
        // UE_LOG(LogTemp, Display, TEXT("MidiNote: %f, BassNote: %f, RootNote: %f, ChordNotes[i]: %f"), MidiNote, a_BassNote, a_RootNote, ChordNotes[i]);

        if (ExtendedScale.Find(MidiNote) != INDEX_NONE)
        {
            ChordMidiNotes[i] = FMusicNote( MidiNote + RootNote, 127.f );
        }
        else
        {
            MidiNote -= 1.f;
            if (ExtendedScale.Find(MidiNote) != INDEX_NONE)
            {
                ChordMidiNotes[i] = FMusicNote( MidiNote + RootNote, 127.f );
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Error in DrumAndBassGenerator.cpp: GenerateChordNotes! Failed to snap to scale!"));
            }
        }
    }
    
    return ChordMidiNotes;
}

FChordArrays DnBChordGenerator::TranscodeChords(TArray<TArray<FMusicNote>> ChordArray)
{
    TArray<FMusicNote> RootNotes;
    TArray<FMusicNote> ThirdNotes;
    TArray<FMusicNote> FifthNotes;
    TArray<FMusicNote> SeventhNotes;
    TArray<FMusicNote> NinthNotes;
    TArray<FMusicNote> EleventhNotes;
    TArray<FMusicNote> ThirteenthNotes;

    RootNotes.SetNum(ChordArray.Num());
    ThirdNotes.SetNum(ChordArray.Num());
    FifthNotes.SetNum(ChordArray.Num());
    SeventhNotes.SetNum(ChordArray.Num());
    NinthNotes.SetNum(ChordArray.Num());
    EleventhNotes.SetNum(ChordArray.Num());
    ThirteenthNotes.SetNum(ChordArray.Num());

    for (size_t i = 0; i < ChordArray.Num(); i++)
    {
        if (!RootNotes.IsValidIndex(i))
        {
            UE_LOG(LogTemp, Error, TEXT("Index Too High"));
            break;
        }
        
        RootNotes[i]       = ChordArray[i][0];
        ThirdNotes[i]      = ChordArray[i][1];
        FifthNotes[i]      = ChordArray[i][2];
        SeventhNotes[i]    = ChordArray[i][3];
        NinthNotes[i]      = ChordArray[i][4];
        EleventhNotes[i]   = ChordArray[i][5];
        ThirteenthNotes[i] = ChordArray[i][6];
    }
    return FChordArrays(RootNotes, ThirdNotes, FifthNotes, SeventhNotes, NinthNotes, EleventhNotes, ThirteenthNotes);
}