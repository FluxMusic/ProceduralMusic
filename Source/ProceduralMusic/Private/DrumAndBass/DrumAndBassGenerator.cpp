// Fill out your copyright notice in the Description page of Project Settings.


#include "DrumAndBassGenerator.h"

#include "Math/RandomStream.h"
#include "DnBVerseGenerator.h"

UDrumAndBassGenerator::UDrumAndBassGenerator()
{
    Scales = ConstructorHelpers::FObjectFinder<UDataTable>(*ScaleDataTablePath).Object;
}

void UDrumAndBassGenerator::Initialise(const FRandomStream& SeedIn, const FMusicGenerationSpecs& SpecsIn)
{
    Seed = SeedIn;
    Specs = SpecsIn;
}

FDrumAndBassSegment UDrumAndBassGenerator::GenerateMusic()
{
    DnBVerseGenerator Generator(Seed, Specs, Scales);

    return Generator.GenerateSegment();
}