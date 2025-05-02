// Fill out your copyright notice in the Description page of Project Settings.


#include "MelodyGenerator.h"

#include "Math/RandomStream.h"
#include "MusicGenerationSpecs.h"

MelodyGenerator::MelodyGenerator(FRandomStream &SeedIn, const FMusicGenerationSpecs& SpecsIn, UDataTable* ScalesIn)
: Seed(SeedIn)
, Specs(SpecsIn)
, Scales(ScalesIn)
{}
