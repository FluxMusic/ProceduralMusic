// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "MusicGenerationSpecs.h"
#include "DrumAndBass/DrumAndBassGenerator.h"

#include "ProceduralMusicGameMode.generated.h"

class UDrumAndBassGenerator;
class UAudioComponent;

struct FMusicNote;
struct FChordArrays;

/**
 * 
 */
UCLASS(BlueprintType)
class AProceduralMusicGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AProceduralMusicGameMode();

private:
	void BeginPlay() override;

	void SetDrumPatterns(TArray<float> a_KickPattern, TArray<float> a_SnarePattern, TArray<float> a_HHPattern, TArray<float> a_Perc1Pattern, TArray<float> a_Perc2Pattern);
	void SetBassPatterns(TArray<FMusicNote> a_BassNotes);
	void SetChordPatterns(FChordArrays a_Chords);
	void SetMelodyPatterns(TArray<FMusicNote> a_MelodyNotes);

	TArray<TArray<float>> SplitNoteArray(TArray<FMusicNote> a_NotePattern);

	void PlayMusic();

public:
	UPROPERTY(EditAnywhere)
	int32 m_Seed { 62879 };
	
	UPROPERTY(EditAnywhere)
	FMusicGenerationSpecs m_MusicGenSpecs
	{
		false,
		false,
		false,
		4,
		Genre::DrumAndBass,
		MusicSpeed::Slow,
		MusicEnergy::Low
	};
private:

	// UPROPERTY(VisibleAnywhere, DisplayName = "Drum And Bass Generator", Category = "Music Generation")
	// TObjectPtr<UDrumAndBassGenerator> m_pDnBGenerator = nullptr;

	FRandomStream Seed { m_Seed };

	TObjectPtr<UDrumAndBassGenerator> Generator = nullptr;

	UPROPERTY(EditAnywhere, DisplayName = "Music", Category = "Playback")
	TObjectPtr<UAudioComponent> m_pMusic = nullptr;

	UPROPERTY(VisibleAnywhere)
    UDataTable* Scales { nullptr };

private:
	const FString META_SOUND_PATH = "/Script/MetasoundEngine.MetaSoundSource'/Game/Audio/MetaSounds/META_DrumAndBass.META_DrumAndBass'";

	const FName PLAY_TRIGGER_NAME = "PlayMusic";

	const FName KICK_PATTERN_NAME = "KickPattern";
	const FName SNARE_PATTERN_NAME = "SnarePattern";
	const FName HH_PATTERN_NAME = "HHPattern";
	const FName PERC1_PATTERN_NAME = "Perc1Pattern";
	const FName PERC2_PATTERN_NAME = "Perc2Pattern";

	const FName BASS_NOTES_PATTERN_NAME = "BassNotes";
	const FName BASS_VELOCITIES_PATTERN_NAME = "BassVelocities";

	const FName CHORD_ROOT_NOTES_PATTERN_NAME = "ChordRoots"; 
	const FName CHORD_ROOT_VELOCITIES_PATTERN_NAME = "ChordRootVelocities"; 
	const FName CHORD_THIRD_NOTES_PATTERN_NAME = "Chord3rds"; 
	const FName CHORD_THIRD_VELOCITIES_PATTERN_NAME = "Chord3rdVelocities"; 
	const FName CHORD_FIFTH_NOTES_PATTERN_NAME = "Chord5ths"; 
	const FName CHORD_FIFTH_VELOCITIES_PATTERN_NAME = "Chord5thVelocities"; 
	const FName CHORD_SEVENTH_NOTES_PATTERN_NAME = "Chord7ths"; 
	const FName CHORD_SEVENTH_VELOCITIES_PATTERN_NAME = "Chord7thVelocities"; 

	const FName MELODY_NOTES_PATTERN_NAME = "MelodyNotes";
	const FName MELODY_VELOCITIES_PATTERN_NAME = "MelodyVelocities";
};
