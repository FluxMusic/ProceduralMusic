// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MusicGenerationSpecs.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DrumAndBassGenerator.generated.h"

/**
 * Forward Declarations
 */

struct FRandomStream;

UENUM(BlueprintType)
enum class ENoteOption : uint8
{
	NewNote UMETA(DisplayName="New Note"),
	Sustain UMETA(DisplayName="Sustain")
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

/**
 * 
 */

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


/**
 * 
 */
UCLASS(BlueprintType)
class UDrumAndBassGenerator : public UObject
{
	GENERATED_BODY()
	
public:
	UDrumAndBassGenerator();

private:
	const FString m_SCALE_DATA_TABLE_PATH = "/Script/Engine.DataTable'/Game/Blueprints/DT_MusicScales.DT_MusicScales'";

public:
	UFUNCTION(BlueprintCallable)
	FDrumAndBassPatterns GenerateMusic(int a_Seed, FMusicGenerationSpecs a_Specs);

private:
	/**
	 * Functions for Drum Generation
	 */

	void GenerateDrums(const FRandomStream& a_Seed, const FMusicGenerationSpecs& a_Specs);
	TArray<float> CreateKickProbabilities(const FMusicGenerationSpecs& a_Specs);
	TArray<float> CreateSnareProbabilities(const FMusicGenerationSpecs& a_Specs);
	TArray<float> CreateHHProbabilities(const FMusicGenerationSpecs& a_Specs);
	TArray<float> CreatePercProbabilities(const FMusicGenerationSpecs& a_Specs);
	TArray<float> GenerateDrumPattern(const FRandomStream& a_Seed, TArray<float> a_Probabilities);

	/**
	 * Functions for Bass Generation
	 */

	void GenerateBass(const FRandomStream& a_Seed, const FMusicGenerationSpecs& a_Specs);
	int32 GenerateBassNoteAmount(const FRandomStream& a_Seed, const FMusicGenerationSpecs& a_Specs);
	ENoteOption GenerateBassNoteProbabilities(const FRandomStream& a_Seed, const FMusicGenerationSpecs& a_Specs);
	ENoteOption GenerateMarkovBassNoteProbabilities(ENoteOption a_PreviousOption, const FRandomStream& a_Seed, const FMusicGenerationSpecs& a_Specs);
	FMusicNote GenerateBassMidiNote(float a_PreviousNote, const FRandomStream& a_Seed, float a_RootNote, FName a_Scale);
	TArray<FMusicNote> CheckGeneration(int32 a_NoteAmount, TArray<FMusicNote> a_NotesToCheck, const FMusicGenerationSpecs& a_Specs);
	int32 CheckNoteAmount(TArray<FMusicNote> a_NoteArray);

	/**
	 * Functions for Chord Generation
	 */

	void GenerateChords(const FRandomStream& a_Seed, const FMusicGenerationSpecs& a_Specs);
	TArray<FMusicNote> GenerateChordNotes(float a_BassNote, float a_RootNote, const FRandomStream& a_Seed, const FMusicGenerationSpecs& a_Specs, FName a_Scale);
	FChordArrays TranscodeChords(TArray<TArray<FMusicNote>> a_ChordArray);

	/**
	 * Functions for Melody Generation
	 */

	 void GenerateMelody(const FRandomStream& a_Seed, const FMusicGenerationSpecs& a_Specs);
	 ENoteOption GenerateMelodyProbabilities(const FRandomStream& a_Seed, const FMusicGenerationSpecs& a_Specs);
	 ENoteOption GenerateMarkovMelodyProbabilities(ENoteOption a_PreviousOption, const FRandomStream& a_Seed, const FMusicGenerationSpecs& a_Specs);
	 FMusicNote GenerateMelodyMidiNote(float a_PreviousNote, const FRandomStream& a_Seed, float a_RootNote, FName a_Scale);

private:
	FRandomStream m_Seed;

	FDrumAndBassPatterns m_Pattern;

	TObjectPtr<UDataTable> m_pScaleDataTable;
};
