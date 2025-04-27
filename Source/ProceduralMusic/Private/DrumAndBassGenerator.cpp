// Fill out your copyright notice in the Description page of Project Settings.


#include "DrumAndBassGenerator.h"

#include "Math/RandomStream.h"

UDrumAndBassGenerator::UDrumAndBassGenerator()
{
    m_pScaleDataTable = ConstructorHelpers::FObjectFinder<UDataTable>(*m_SCALE_DATA_TABLE_PATH).Object;
}

FDrumAndBassPatterns UDrumAndBassGenerator::GenerateMusic(int a_Seed, FMusicGenerationSpecs a_Specs)
{
    m_Seed = FRandomStream(a_Seed);

    GenerateDrums(m_Seed, a_Specs);

    for (size_t i = 0; i < m_Pattern.KickPattern.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("Kick: %f"), m_Pattern.KickPattern[i]);
    }
    for (size_t i = 0; i < m_Pattern.SnarePattern.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("Snare: %f"), m_Pattern.SnarePattern[i]);
    }
    for (size_t i = 0; i < m_Pattern.HHPattern.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("HH: %f"), m_Pattern.HHPattern[i]);
    }
    for (size_t i = 0; i < m_Pattern.Perc1Pattern.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("Perc1: %f"), m_Pattern.Perc1Pattern[i]);
    }
    for (size_t i = 0; i < m_Pattern.Perc2Pattern.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("Perc2: %f"), m_Pattern.Perc2Pattern[i]);
    }
    

    GenerateBass(m_Seed, a_Specs);

    for (size_t i = 0; i < m_Pattern.BassNotes.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("Bass Note: %f, Bass Velocity: %f"), m_Pattern.BassNotes[i].MidiNote, m_Pattern.BassNotes[i].Velocity);
    }

    GenerateChords(m_Seed, a_Specs);

    for (size_t i = 0; i < m_Pattern.Chords.RootNotes.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("Chord Root Note: %f, Chord Root Velocity: %f"), m_Pattern.Chords.RootNotes[i].MidiNote, m_Pattern.Chords.RootNotes[i].Velocity);
    }

    for (size_t i = 0; i < m_Pattern.Chords.ThirdNotes.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("Chord Third Note: %f, Chord Third Velocity: %f"), m_Pattern.Chords.ThirdNotes[i].MidiNote, m_Pattern.Chords.ThirdNotes[i].Velocity);
    }

    for (size_t i = 0; i < m_Pattern.Chords.FifthNotes.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("Chord Fifth Note: %f, Chord Fifth Velocity: %f"), m_Pattern.Chords.FifthNotes[i].MidiNote, m_Pattern.Chords.FifthNotes[i].Velocity);
    }

    for (size_t i = 0; i < m_Pattern.Chords.SeventhNotes.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("Chord Seventh Note: %f, Chord Seventh Velocity: %f"), m_Pattern.Chords.SeventhNotes[i].MidiNote, m_Pattern.Chords.SeventhNotes[i].Velocity);
    }

    for (size_t i = 0; i < m_Pattern.Chords.NinthNotes.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("Chord Ninth Note: %f, Chord Ninth Velocity: %f"), m_Pattern.Chords.NinthNotes[i].MidiNote, m_Pattern.Chords.NinthNotes[i].Velocity);
    }

    for (size_t i = 0; i < m_Pattern.Chords.EleventhNotes.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("Chord Eleventh Note: %f, Chord Eleventh Velocity: %f"), m_Pattern.Chords.EleventhNotes[i].MidiNote, m_Pattern.Chords.EleventhNotes[i].Velocity);
    }

    for (size_t i = 0; i < m_Pattern.Chords.ThirteenthNotes.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("Chord Thirteenth Note: %f, Chord Thirteenth Velocity: %f"), m_Pattern.Chords.ThirteenthNotes[i].MidiNote, m_Pattern.Chords.ThirteenthNotes[i].Velocity);
    }

    GenerateMelody(m_Seed, a_Specs);

    for (size_t i = 0; i < m_Pattern.MelodyNotes.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("Melody Note: %f, Melody Velocity: %f"), m_Pattern.MelodyNotes[i].MidiNote, m_Pattern.MelodyNotes[i].Velocity);
    }


    return m_Pattern;
}

void UDrumAndBassGenerator::GenerateDrums(const FRandomStream& a_Seed, const FMusicGenerationSpecs& a_Specs)
{
    m_Pattern.KickPattern = GenerateDrumPattern(a_Seed, CreateKickProbabilities(a_Specs));

    m_Pattern.SnarePattern = GenerateDrumPattern(a_Seed, CreateSnareProbabilities(a_Specs));

    m_Pattern.HHPattern = GenerateDrumPattern(a_Seed, CreateHHProbabilities(a_Specs));

    TArray<float> PercProbs = CreatePercProbabilities(a_Specs);
    m_Pattern.Perc1Pattern = GenerateDrumPattern(a_Seed, PercProbs);

    FRandomStream NewSeed = a_Seed;
    NewSeed.GenerateNewSeed();
    m_Pattern.Perc2Pattern = GenerateDrumPattern(NewSeed, PercProbs);
}

TArray<float> UDrumAndBassGenerator::CreateKickProbabilities(const FMusicGenerationSpecs& a_Specs)
{
    switch (a_Specs.Energy)
    {
    case MusicEnergy::Low:
        return { 1.f, 0.f, 0.2f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.2f, 0.f };
    
    case MusicEnergy::Medium:
        return { 1.f, 0.f, 0.2f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.2f, 0.f };

    case MusicEnergy::High:
        return { 1.f, 0.f, 0.2f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.2f, 0.f };
    
    default:
        return { 1.f, 0.f, 0.2f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.2f, 0.f };
    }
}

TArray<float> UDrumAndBassGenerator::CreateSnareProbabilities(const FMusicGenerationSpecs& a_Specs)
{
    switch (a_Specs.Energy)
    {
    case MusicEnergy::Low:
        return { 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f };
    
    case MusicEnergy::Medium:
        return { 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.5f, 0.f, 0.5f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f };

    case MusicEnergy::High:
        return { 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.7f, 0.f, 0.7f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.4f };
    
    default:
        return { 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f };
    }
}

TArray<float> UDrumAndBassGenerator::CreateHHProbabilities(const FMusicGenerationSpecs& a_Specs)
{
    switch (a_Specs.Energy)
    {
    case MusicEnergy::Low:
        return { 0.8f, 0.f, 0.8f, 0.f, 0.8f, 0.f, 0.8f, 0.f, 0.8f, 0.f, 0.8f, 0.f, 0.8f, 0.f, 0.8f, 0.f };
    
    case MusicEnergy::Medium:
        return { 0.9f, 0.3f, 0.9f, 0.3f, 0.9f, 0.3f, 0.9f, 0.3f, 0.9f, 0.3f, 0.9f, 0.3f, 0.9f, 0.3f, 0.9f, 0.3f };

    case MusicEnergy::High:
        return { 0.9f, 0.7f, 0.9f, 0.7f, 0.9f, 0.7f, 0.9f, 0.7f, 0.9f, 0.7f, 0.9f, 0.7f, 0.9f, 0.7f, 0.9f, 0.7f };
    
    default:
        return { 0.8f, 0.f, 0.8f, 0.f, 0.8f, 0.f, 0.8f, 0.f, 0.8f, 0.f, 0.8f, 0.f, 0.8f, 0.f, 0.8f, 0.f };
    }
}

TArray<float> UDrumAndBassGenerator::CreatePercProbabilities(const FMusicGenerationSpecs& a_Specs)
{
    switch (a_Specs.Energy)
    {
    case MusicEnergy::Low:
        return { 0.2f, 0.2f, 0.f, 0.1f, 0.f, 0.1f, 0.1f, 0.f, 0.2f, 0.1f, 0.f, 0.1f, 0.f, 0.2f, 0.f, 0.1f };
    
    case MusicEnergy::Medium:
        return { 0.2f, 0.3f, 0.1f, 0.1f, 0.1f, 0.3f, 0.1f, 0.1f, 0.2f, 0.1f, 0.3f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f };

    case MusicEnergy::High:
        return { 0.2f, 0.3f, 0.1f, 0.1f, 0.1f, 0.4f, 0.1f, 0.1f, 0.2f, 0.1f, 0.4f, 0.1f, 0.4f, 0.2f, 0.1f, 0.1f };
    
    default:
        return { 0.2f, 0.2f, 0.f, 0.1f, 0.f, 0.1f, 0.1f, 0.f, 0.2f, 0.1f, 0.f, 0.1f, 0.f, 0.2f, 0.f, 0.1f };
    }
}

TArray<float> UDrumAndBassGenerator::GenerateDrumPattern(const FRandomStream& a_Seed, TArray<float> a_Probabilities)
{
    TArray<float> Pattern{ 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

    for (size_t i = 0; i < Pattern.Num(); i++)
    {
        if (a_Probabilities[i] == 0.f)
        {
            continue;
        }

        if (a_Probabilities[i] >= a_Seed.FRand())
        {
            Pattern[i] = 127.f;
        }
    }

    return Pattern;
}

void UDrumAndBassGenerator::GenerateBass(const FRandomStream &a_Seed, const FMusicGenerationSpecs &a_Specs)
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

    int32 BassNoteAmount = GenerateBassNoteAmount(a_Seed, a_Specs);

    UE_LOG(LogTemp, Display, TEXT("BassNoteAmount: %i"), BassNoteAmount);

    float PreviousNote ( 0.f );

    TArray<ENoteOption> GenericPattern;
    TArray<ENoteOption> MarkovPattern;

    GenericPattern.SetNum(BassNotes.Num());
    MarkovPattern.SetNum (BassNotes.Num());

    for (size_t i = 0; i < BassNotes.Num(); i++)
    {
        GenericPattern[i] = GenerateBassNoteProbabilities(a_Seed, a_Specs);
    }

    for (size_t i = 0; i < BassNotes.Num(); i++)
    {
        if (BassNotes.IsValidIndex(i - 1))
        {
            MarkovPattern[i] = GenerateMarkovBassNoteProbabilities(MarkovPattern[i - 1], a_Seed, a_Specs);
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
                FinalPattern[i] = GenerateMarkovBassNoteProbabilities(FinalPattern[i - 1], a_Seed, a_Specs);
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
                FMusicNote NewNote = GenerateBassMidiNote(PreviousNote, a_Seed, 60.f, "Major");
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
    
    m_Pattern.BassNotes = CheckGeneration(BassNoteAmount, BassNotes, a_Specs);
}

int32 UDrumAndBassGenerator::GenerateBassNoteAmount(const FRandomStream &a_Seed, const FMusicGenerationSpecs& a_Specs)
{
    switch (a_Specs.Energy)
    {
    case MusicEnergy::Low:
        return a_Seed.RandRange(4, 5);
    case MusicEnergy::Medium:
        return a_Seed.RandRange(5, 6);
    case MusicEnergy::High:
        return a_Seed.RandRange(5, 7);

    default:
        return 4;
    }
}

ENoteOption UDrumAndBassGenerator::GenerateBassNoteProbabilities(const FRandomStream& a_Seed, const FMusicGenerationSpecs& a_Specs)
{
    float SustainProbability ( 0.f );
    float NewNoteProbability ( 0.f );

    switch (a_Specs.Energy)
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

    float RandomChance = a_Seed.FRand();

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
    
    UE_LOG(LogTemp, Error, TEXT("Error in DrumAndBassGenerator.cpp: GenerateBassNoteProbabilities! Weights set up wrong!"));

    return ENoteOption::NewNote;
}

ENoteOption UDrumAndBassGenerator::GenerateMarkovBassNoteProbabilities(ENoteOption a_PreviousOption, const FRandomStream &a_Seed, const FMusicGenerationSpecs &a_Specs)
{
    TMap<ENoteOption, TArray<FMarkovRhythmChain>> MarkovRhythm
    {
        { ENoteOption::NewNote, { { ENoteOption::NewNote, 0.2f }, { ENoteOption::Sustain, 0.8f } } },
        { ENoteOption::Sustain, { { ENoteOption::NewNote, 0.6f }, { ENoteOption::Sustain, 0.4f } } }
    };

    float RandomWeight = a_Seed.FRand();
    float Weight ( 0.f );

    TArray<FMarkovRhythmChain>* Probabilities = MarkovRhythm.Find(a_PreviousOption);

    if (Probabilities == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Error in DrumAndBassGenerator.cpp: GenerateMarkovBassNoteProbabilities! Could not find previous option in the Markov Chain! PreviousOption: %i"), a_PreviousOption);

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

FMusicNote UDrumAndBassGenerator::GenerateBassMidiNote(float a_PreviousNote, const FRandomStream &a_Seed, float a_RootNote, FName a_Scale)
{
    if (a_PreviousNote <= 0.f)
    {
        float RandomMidiNote = static_cast<float>(a_Seed.RandRange(60, 71));

        TArray<float> ScaleDegrees = m_pScaleDataTable->FindRow<FMusicScale>(a_Scale, "GenerateBassMidiNote", false)->ScaleDegrees;

        TArray<float> ScaleNotes = ScaleDegrees;

        for (size_t i = 0; i < ScaleDegrees.Num(); i++)
        {
            ScaleNotes[i] = ScaleDegrees[i] + a_RootNote;
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

        float RandomWeight = a_Seed.FRand();
        float Weight ( 0.f );

        TArray<FMarkovChain>* Probabilities = MarkovProbabilities.Find(a_PreviousNote - a_RootNote);

        if (Probabilities == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("Error in DrumAndBassGenerator.cpp: GenerateBassMidiNote! Could not find previous note in the Markov Chain! PreviousNote: %f"), a_PreviousNote - a_RootNote);

            return FMusicNote( 0.f, 0.f );
        }
        
        for (FMarkovChain& Probability : *Probabilities)
        {
            Weight += Probability.Probability;

            if (RandomWeight <= Weight)
            {
                TArray<float> ScaleDegrees = m_pScaleDataTable->FindRow<FMusicScale>(a_Scale, "GenerateBassMidiNote", false)->ScaleDegrees;

                TArray<float> ScaleNotes = ScaleDegrees;

                float BassNote = a_PreviousNote + Probability.ScaleDegree;

                for (size_t i = 0; i < ScaleDegrees.Num(); i++)
                {
                    ScaleNotes[i] = ScaleDegrees[i] + a_RootNote;
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

                        if (BassNote > a_RootNote && BassNote <= ScaleNotes[ScaleNotes.Num() - 1])
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

TArray<FMusicNote> UDrumAndBassGenerator::CheckGeneration(int32 a_NoteAmount, TArray<FMusicNote> a_NotesToCheck, const FMusicGenerationSpecs& a_Specs)
{
    int32 NoteAmount = CheckNoteAmount(a_NotesToCheck);
    
    UE_LOG(LogTemp, Display, TEXT("NoteAmount: %i"), NoteAmount);

    if (NoteAmount == a_NoteAmount)
    {
        return a_NotesToCheck;
    }
    
    TArray<FMusicNote> FinalNotes = a_NotesToCheck;

    int32 Failsafe = 0;
    
    while (CheckNoteAmount(FinalNotes) > a_NoteAmount)
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

int32 UDrumAndBassGenerator::CheckNoteAmount(TArray<FMusicNote> a_NoteArray)
{
    if (a_NoteArray.Num() == 0)
    {
        return 0;
    }
    
    int32 NoteAmount = 1;

    for (size_t i = 1; i < a_NoteArray.Num(); i++)
    {
        if (a_NoteArray[i].MidiNote != a_NoteArray[i - 1].MidiNote)
        {
            NoteAmount++;
        }
    }

    return NoteAmount;
}

void UDrumAndBassGenerator::GenerateChords(const FRandomStream &a_Seed, const FMusicGenerationSpecs &a_Specs)
{
    TArray<TArray<FMusicNote>> ChordArray;
    ChordArray.SetNum(m_Pattern.BassNotes.Num());

    TArray<int32> GroupStarts;
    GroupStarts.Add(0);

    for (size_t i = 1; i < m_Pattern.BassNotes.Num(); i++)
    {
        if (m_Pattern.BassNotes[i].MidiNote != m_Pattern.BassNotes[i - 1].MidiNote)
        {
            GroupStarts.Add(i);
        }
    }

    TArray<int32> GroupLengths;

    for (int32 GroupStart : GroupStarts)
    {
        for (size_t i = ( GroupStart + 1); i < m_Pattern.BassNotes.Num(); i++)
        {
            if (m_Pattern.BassNotes[i].MidiNote != m_Pattern.BassNotes[i - 1].MidiNote)
            {
                GroupLengths.Add(i - GroupStart);
                break;
            }
            else if (i == m_Pattern.BassNotes.Num() - 1)
            {
                GroupLengths.Add(m_Pattern.BassNotes.Num() - GroupStart);
                break;
            }
            
        }
    }
    
    for (int32 i = 0; i < GroupStarts.Num(); i++)
    {
        TArray<FMusicNote> GeneratedChord = GenerateChordNotes(m_Pattern.BassNotes[GroupStarts[i]].MidiNote, 60.f, a_Seed, a_Specs, "Major");

        for (size_t j = 0; j < GroupLengths[i]; j++)
        {
            ChordArray[GroupStarts[i] + j] = GeneratedChord;
        }
    }
    

    // for (size_t i = 0; i < m_Pattern.BassNotes.Num(); i++)
    // {
    //     ChordArray[i] = GenerateChordNotes(m_Pattern.BassNotes[i].MidiNote, 60.f, a_Seed, a_Specs, "Major");
    // }
    m_Pattern.Chords = TranscodeChords(ChordArray);
}

TArray<FMusicNote> UDrumAndBassGenerator::GenerateChordNotes(float a_BassNote, float a_RootNote, const FRandomStream &a_Seed, const FMusicGenerationSpecs &a_Specs, FName a_Scale)
{
    float RandomWeight = a_Seed.FRand();

    TArray<float> ChordNotes;
    ChordNotes.SetNum(7);

    uint8 ChordNoteAmount ( 0 );

    switch (a_Specs.Energy)
    {
        case MusicEnergy::Low:
            ChordNoteAmount = a_Seed.RandRange(3, 4);
            break;
        case MusicEnergy::Medium:
            ChordNoteAmount = a_Seed.RandRange(3, 5);
            break;
        case MusicEnergy::High:
            ChordNoteAmount = a_Seed.RandRange(3, 7);
            break;
        
        default:
            ChordNoteAmount = a_Seed.RandRange(3, 4);
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
            
            float ThirdProbability ( 0.9f );
            float Sus2Probability  ( 0.05f );
            float Sus4Probability  ( 0.05f );

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

    TArray<float> ScaleDegrees = m_pScaleDataTable->FindRow<FMusicScale>(a_Scale, "GenerateChordNotes", false)->ScaleDegrees;
    TArray<float> ExtendedScale = ScaleDegrees;
    for (float Entry : ScaleDegrees)
    {
        ExtendedScale.Add(Entry + 12.f);
    }

    TArray<FMusicNote> ChordMidiNotes;
    ChordMidiNotes.SetNum(ChordNotes.Num());

    for (size_t i = 0; i < ChordNotes.Num(); i++)
    {
        if (ChordNotes[i] == -1.f || a_BassNote == 0.f)
        {
            ChordMidiNotes[i] = FMusicNote( 0.f, 0.f );
            continue;
        }
        
        float MidiNote = a_BassNote - a_RootNote + ChordNotes[i];
        
        // UE_LOG(LogTemp, Display, TEXT("MidiNote: %f, BassNote: %f, RootNote: %f, ChordNotes[i]: %f"), MidiNote, a_BassNote, a_RootNote, ChordNotes[i]);

        if (ExtendedScale.Find(MidiNote) != INDEX_NONE)
        {
            ChordMidiNotes[i] = FMusicNote( MidiNote + a_RootNote, 127.f );
        }
        else
        {
            MidiNote -= 1.f;
            if (ExtendedScale.Find(MidiNote) != INDEX_NONE)
            {
                ChordMidiNotes[i] = FMusicNote( MidiNote +  a_RootNote, 127.f );
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Error in DrumAndBassGenerator.cpp: GenerateChordNotes! Failed to snap to scale!"));
            }
        }
    }
    
    return ChordMidiNotes;
}

FChordArrays UDrumAndBassGenerator::TranscodeChords(TArray<TArray<FMusicNote>> a_ChordArray)
{
    TArray<FMusicNote> RootNotes;
    TArray<FMusicNote> ThirdNotes;
    TArray<FMusicNote> FifthNotes;
    TArray<FMusicNote> SeventhNotes;
    TArray<FMusicNote> NinthNotes;
    TArray<FMusicNote> EleventhNotes;
    TArray<FMusicNote> ThirteenthNotes;

    RootNotes.SetNum(a_ChordArray.Num());
    ThirdNotes.SetNum(a_ChordArray.Num());
    FifthNotes.SetNum(a_ChordArray.Num());
    SeventhNotes.SetNum(a_ChordArray.Num());
    NinthNotes.SetNum(a_ChordArray.Num());
    EleventhNotes.SetNum(a_ChordArray.Num());
    ThirteenthNotes.SetNum(a_ChordArray.Num());

    for (size_t i = 0; i < a_ChordArray.Num(); i++)
    {
        if (!RootNotes.IsValidIndex(i))
        {
            UE_LOG(LogTemp, Error, TEXT("Index Too High"));
            break;
        }
        
        RootNotes[i]       = a_ChordArray[i][0];
        ThirdNotes[i]      = a_ChordArray[i][1];
        FifthNotes[i]      = a_ChordArray[i][2];
        SeventhNotes[i]    = a_ChordArray[i][3];
        NinthNotes[i]      = a_ChordArray[i][4];
        EleventhNotes[i]   = a_ChordArray[i][5];
        ThirteenthNotes[i] = a_ChordArray[i][6];
    }
    return FChordArrays(RootNotes, ThirdNotes, FifthNotes, SeventhNotes, NinthNotes, EleventhNotes, ThirteenthNotes);
}

void UDrumAndBassGenerator::GenerateMelody(const FRandomStream &a_Seed, const FMusicGenerationSpecs &a_Specs)
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

    float PreviousNote ( 0.f );

    TArray<ENoteOption> GenericPattern;
    TArray<ENoteOption> MarkovPattern;

    GenericPattern.SetNum(MelodyNotes.Num());
    MarkovPattern.SetNum (MelodyNotes.Num());

    for (size_t i = 0; i < MelodyNotes.Num(); i++)
    {
        GenericPattern[i] = GenerateMelodyProbabilities(a_Seed, a_Specs);
    }

    for (size_t i = 0; i < MelodyNotes.Num(); i++)
    {
        if (MelodyNotes.IsValidIndex(i - 1))
        {
            MarkovPattern[i] = GenerateMarkovMelodyProbabilities(MarkovPattern[i - 1], a_Seed, a_Specs);
        }
        else
        {
            MarkovPattern[i] = GenerateMarkovMelodyProbabilities(ENoteOption::NewNote, a_Seed, a_Specs);
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
                FinalPattern[i] = GenerateMarkovBassNoteProbabilities(FinalPattern[i - 1], a_Seed, a_Specs);
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
                FMusicNote NewNote = GenerateMelodyMidiNote(PreviousNote, a_Seed, 60.f, "Major");
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
    
    m_Pattern.MelodyNotes = MelodyNotes;
}

ENoteOption UDrumAndBassGenerator::GenerateMelodyProbabilities(const FRandomStream &a_Seed, const FMusicGenerationSpecs &a_Specs)
{
    float SustainProbability ( 0.f );
    float NewNoteProbability ( 0.f );

    switch (a_Specs.Energy)
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

    float RandomChance = a_Seed.FRand();

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

ENoteOption UDrumAndBassGenerator::GenerateMarkovMelodyProbabilities(ENoteOption a_PreviousOption, const FRandomStream &a_Seed, const FMusicGenerationSpecs &a_Specs)
{
    TMap<ENoteOption, TArray<FMarkovRhythmChain>> MarkovRhythm
    {
        { ENoteOption::NewNote, { { ENoteOption::NewNote, 0.1f }, { ENoteOption::Sustain, 0.8f } } },
        { ENoteOption::Sustain, { { ENoteOption::NewNote, 0.5f }, { ENoteOption::Sustain, 0.4f } } }
    };

    float RandomWeight = a_Seed.FRand();
    float Weight ( 0.f );

    TArray<FMarkovRhythmChain>* Probabilities = MarkovRhythm.Find(a_PreviousOption);

    if (Probabilities == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Error in DrumAndBassGenerator.cpp: GenerateMarkovMelodyProbabilities! Could not find previous option in the Markov Chain! PreviousOption: %i"), a_PreviousOption);

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

FMusicNote UDrumAndBassGenerator::GenerateMelodyMidiNote(float a_PreviousNote, const FRandomStream &a_Seed, float a_RootNote, FName a_Scale)
{
    if (a_PreviousNote <= 0.f)
    {
        float RandomMidiNote = static_cast<float>(a_Seed.RandRange(60, 71));

        TArray<float> ScaleDegrees = m_pScaleDataTable->FindRow<FMusicScale>(a_Scale, "GenerateMelodyMidiNote", false)->ScaleDegrees;

        TArray<float> ScaleNotes = ScaleDegrees;

        for (size_t i = 0; i < ScaleDegrees.Num(); i++)
        {
            ScaleNotes[i] = ScaleDegrees[i] + a_RootNote;
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

        float RandomWeight = a_Seed.FRand();
        float Weight ( 0.f );

        TArray<FMarkovChain>* Probabilities = MarkovProbabilities.Find(a_PreviousNote - a_RootNote);

        if (Probabilities == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("Error in DrumAndBassGenerator.cpp: GenerateMelodyMidiNote! Could not find previous note in the Markov Chain! PreviousNote: %f"), a_PreviousNote - a_RootNote);

            return FMusicNote( 0.f, 0.f );
        }
        
        for (FMarkovChain& Probability : *Probabilities)
        {
            Weight += Probability.Probability;

            if (RandomWeight <= Weight)
            {
                TArray<float> ScaleDegrees = m_pScaleDataTable->FindRow<FMusicScale>(a_Scale, "GenerateMelodyMidiNote", false)->ScaleDegrees;

                TArray<float> ScaleNotes = ScaleDegrees;

                float MelodyNote = a_PreviousNote + Probability.ScaleDegree;

                for (size_t i = 0; i < ScaleDegrees.Num(); i++)
                {
                    ScaleNotes[i] = ScaleDegrees[i] + a_RootNote;
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
                        // MelodyNote -= 1.f;

                        if (MelodyNote > a_RootNote && MelodyNote <= ScaleNotes[ScaleNotes.Num() - 1])
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
