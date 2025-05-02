// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MusicGenerationSpecs.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MusicGenerator.generated.h"

//Forward Declarations
class UDataTable;

struct FRandomStream;

/**
 * Wrapper Class for Music Generation of all supported Genres
 */
UCLASS()
class UMusicGenerator : public UObject
{
    GENERATED_BODY()
public:
    UMusicGenerator();

    void Initialise(const FRandomStream& SeedIn, const FMusicGenerationSpecs SpecsIn);

private:
    const FString ScaleDataTablePath = "/Script/Engine.DataTable'/Game/Blueprints/DT_MusicScales.DT_MusicScales'";

private:
    /**
     * Wrapper Method to inherit from actual Generation Classes
     */
    virtual void GenerateMusic();

private:
    UPROPERTY(VisibleAnywhere)
    UDataTable* Scales { nullptr };

    FRandomStream Seed { 0 };

    FMusicGenerationSpecs Specs;
};