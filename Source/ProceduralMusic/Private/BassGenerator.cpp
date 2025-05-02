// Fill out your copyright notice in the Description page of Project Settings.


#include "BassGenerator.h"

#include "Math/RandomStream.h"
#include "MusicGenerationSpecs.h"

BassGenerator::BassGenerator(FRandomStream &SeedIn, const FMusicGenerationSpecs& SpecsIn, UDataTable* ScalesIn)
: Seed(SeedIn)
, Specs(SpecsIn)
, Scales(ScalesIn)
{}
