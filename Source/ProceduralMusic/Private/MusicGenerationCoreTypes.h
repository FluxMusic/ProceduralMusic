// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MusicGenerationCoreTypes.generated.h"

UENUM(BlueprintType)
enum class ENoteOption : uint8
{
	NewNote UMETA(DisplayName="New Note"),
	Sustain UMETA(DisplayName="Sustain")
};

UENUM(BlueprintType)
enum class ENoteLength : uint8
{
	None       = 0  UMETA(DisplayName="None"),
	Sixteenth  = 1  UMETA(DisplayName="Sixteenth"),
	Eighth     = 2  UMETA(DisplayName="Eighth"),
	EighthDot  = 3  UMETA(DisplayName="DottedEighth"),
	Quarter    = 4  UMETA(DisplayName="Quarter"),
	QuarterDot = 6  UMETA(DisplayName="DottedQuarter"),
	Half       = 8  UMETA(DisplayName="Half"),
	HalfDot    = 8  UMETA(DisplayName="DottedHalf"),
	Full       = 16 UMETA(DisplayName="Full"),      
	FullDot    = 24 UMETA(DisplayName="DottedFull"),      
	TwoFull    = 32 UMETA(DisplayName="TwoFull")      
};

USTRUCT(BlueprintType)
struct FMusicScale : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> ScaleDegrees;
};

USTRUCT(BlueprintType)
struct FMusicNote
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MidiNote;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Velocity;
};

USTRUCT(BlueprintType)
struct FMarkovChain
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScaleDegree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Probability;
};

USTRUCT(BlueprintType)
struct FMarkovRhythmChain
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ENoteOption NoteOption;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Probability;
};

/**
 * Simple Container for all Drum Patterns
 */
USTRUCT(BlueprintType)
struct FDrumAndBassDrums
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<float> KickPattern;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<float> SnarePattern;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<float> CymbalPattern;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<float> Perc1Pattern;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<float> Perc2Pattern;
};

USTRUCT(BlueprintType)
struct FChordArrays
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMusicNote> RootNotes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMusicNote> ThirdNotes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMusicNote> FifthNotes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMusicNote> SeventhNotes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMusicNote> NinthNotes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMusicNote> EleventhNotes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMusicNote> ThirteenthNotes;
};

USTRUCT(BlueprintType)
struct FDrumAndBassPatterns
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> KickPattern;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> SnarePattern;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> HHPattern;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Perc1Pattern;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Perc2Pattern;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMusicNote> BassNotes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FChordArrays Chords;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMusicNote> MelodyNotes;
};

USTRUCT(BlueprintType)
struct FDrumAndBassSegment
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDrumAndBassDrums Drums;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMusicNote> Bass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FChordArrays Chords;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMusicNote> Melody;
};