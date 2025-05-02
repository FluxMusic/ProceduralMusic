// Fill out your copyright notice in the Description page of Project Settings.


#include "MusicGenerator.h"

#include "Math/RandomStream.h"

UMusicGenerator::UMusicGenerator()
{
    Scales = ConstructorHelpers::FObjectFinder<UDataTable>(*ScaleDataTablePath).Object;
}

// void UMusicGenerator::Initialise(const FRandomStream &SeedIn, const FMusicGenerationSpecs SpecsIn)
// {
//     Seed = SeedIn;
//     Specs = SpecsIn;
// }

void UMusicGenerator::GenerateMusic()
{}