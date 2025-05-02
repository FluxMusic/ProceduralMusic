// Fill out your copyright notice in the Description page of Project Settings.


#include "DrumGenerator.h"

#include "Math/RandomStream.h"
#include "MusicGenerationSpecs.h"

DrumGenerator::DrumGenerator(FRandomStream &SeedIn, const FMusicGenerationSpecs& SpecsIn)
: Seed(SeedIn)
, Specs(SpecsIn)
{}
