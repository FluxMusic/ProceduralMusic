// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MusicGenerationSpecs.h"

#include "CoreMinimal.h"
#include "../SegmentGenerator.h"

//Forward Declarations
struct FRandomStream;

class DrumAndBassSegmentGenerator : public SegmentGenerator
{
public:
    DrumAndBassSegmentGenerator(FRandomStream& SeedIn, UDataTable* ScalesIn);

private:
};
