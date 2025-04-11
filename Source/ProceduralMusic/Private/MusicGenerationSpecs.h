#pragma once

#include "CoreMinimal.h"
#include "MusicGenerationSpecs.generated.h"  

UENUM(BlueprintType)
enum class Genre : uint8
{
    None         UMETA(DisplayName = "None"),
    DrumAndBass  UMETA(DisplayName = "Drum and Bass"),
    Metal        UMETA(DisplayName = "Metal")
};

UENUM(BlueprintType)
enum class MusicSpeed : uint8
{
    None     UMETA(DisplayName = "None"),
    Slow     UMETA(DisplayName = "Slow"),
    Medium   UMETA(DisplayName = "Medium"),
    Fast     UMETA(DisplayName = "Fast"),
    Extreme  UMETA(DisplayName = "Extreme")
};

UENUM(BlueprintType)
enum class MusicEnergy : uint8
{
    None   UMETA(DisplayName = "None"),
    Low    UMETA(DisplayName = "Low"),
    Medium UMETA(DisplayName = "Medium"),
    High   UMETA(DisplayName = "High")
};

USTRUCT(BlueprintType)
struct FMusicGenerationSpecs
{
    GENERATED_USTRUCT_BODY()
    
public:
    UPROPERTY()
    bool bGenerateIntro  { true };
    UPROPERTY()
    bool bGenerateOutro  { false };
    UPROPERTY()
    bool bGenerateBridge { false };

    UPROPERTY()
    Genre Genre { Genre::DrumAndBass };
    UPROPERTY()
    MusicSpeed Speed { MusicSpeed::Medium };
    UPROPERTY()
    MusicEnergy Energy { MusicEnergy::Low };
};