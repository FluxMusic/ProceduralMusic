// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MusicGenerationSpecs.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

//Forward Declarations
class UDataTable;

struct FRandomStream;

/**
 * Wrapper Class for Segment Generators of all supported Genres
 */
class SegmentGenerator
{
public:
    SegmentGenerator(FRandomStream& SeedIn, UDataTable* ScalesIn);

    void SetScales(UDataTable* ScalesIn) 
    { 
        if (!ScalesIn)
        {
            UE_LOG(LogTemp, Display, TEXT("SetScales is nullptr"));
        }

        Scales = ScalesIn; 
        UE_LOG(LogTemp, Display, TEXT("Scales: %i, ScalesIn: %i"), &Scales, &ScalesIn);
    }

protected:
    FRandomStream& Seed;

    FMusicGenerationSpecs Specs;

    UDataTable* Scales;
};
