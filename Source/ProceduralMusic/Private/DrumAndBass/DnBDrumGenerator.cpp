// Fill out your copyright notice in the Description page of Project Settings.


#include "DnBDrumGenerator.h"

#include "Math/RandomStream.h"
#include "../MusicGenerationSpecs.h"

DnBDrumGenerator::DnBDrumGenerator(FRandomStream& SeedIn, const FMusicGenerationSpecs& SpecsIn)
: DrumGenerator(SeedIn, SpecsIn)
{}

FDrumAndBassDrums DnBDrumGenerator::GenerateDrums()
{
    FDrumAndBassDrums Drums;

    Drums.KickPattern   = GenerateDrumPattern(CreateKickProbabilities());
    Drums.SnarePattern  = GenerateDrumPattern(CreateSnareProbabilities());
    Drums.CymbalPattern = GenerateDrumPattern(CreateCymbalProbabilities());

    TArray<float> PercProbs = CreatePercProbabilities();
    Drums.Perc1Pattern  = GenerateDrumPattern(PercProbs);

    //Could call anything here, just need to reseed
    Seed.GetUnsignedInt();
    Drums.Perc2Pattern  = GenerateDrumPattern(PercProbs);

    return Drums;
}

TArray<float> DnBDrumGenerator::CreateKickProbabilities()
{
    switch (Specs.Energy)
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

TArray<float> DnBDrumGenerator::CreateSnareProbabilities()
{
    switch (Specs.Energy)
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

TArray<float> DnBDrumGenerator::CreateCymbalProbabilities()
{
    switch (Specs.Energy)
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

TArray<float> DnBDrumGenerator::CreatePercProbabilities()
{
    switch (Specs.Energy)
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

TArray<float> DnBDrumGenerator::GenerateDrumPattern(const TArray<float>& Probabilities)
{
    TArray<float> Pattern{ 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

    for (int32 i = 0; i < Pattern.Num(); i++)
    {
        if (Probabilities[i] == 0.f)
        {
            continue;
        }
        
        if (Probabilities[i] >= Seed.FRand())
        {
            Pattern[i] = 127.f;
        }
    }

    return Pattern;
}
