// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralMusicGameMode.h"

#include "Components/AudioComponent.h"

AProceduralMusicGameMode::AProceduralMusicGameMode()
{
    // m_pDnBGenerator = NewObject<UDrumAndBassGenerator>(this, "DnB Generator");

    Generator = NewObject<UDrumAndBassGenerator>(this, "DnB Generator");

    Scales = ConstructorHelpers::FObjectFinder<UDataTable>(TEXT("/Script/Engine.DataTable'/Game/Blueprints/DT_MusicScales.DT_MusicScales'")).Object;

    if (!Scales)
    {
        UE_LOG(LogTemp, Warning, TEXT("Scales Not Found!"));
    }

    Generator->Initialise(Seed, m_MusicGenSpecs);

    m_pMusic = CreateDefaultSubobject<UAudioComponent>("Music");
    m_pMusic->SetSound(ConstructorHelpers::FObjectFinder<USoundBase>(*META_SOUND_PATH).Object);
    m_pMusic->SetAutoActivate(false);
    m_pMusic->SetupAttachment(RootComponent);
}

void AProceduralMusicGameMode::BeginPlay()
{
    // FDrumAndBassPatterns Patterns = m_pDnBGenerator->GenerateMusic(m_Seed, m_MusicGenSpecs);

    // SetDrumPatterns(Patterns.KickPattern, Patterns.SnarePattern, Patterns.HHPattern, Patterns.Perc1Pattern, Patterns.Perc2Pattern);
    // SetBassPatterns(Patterns.BassNotes);
    // SetChordPatterns(Patterns.Chords);
    // SetMelodyPatterns(Patterns.MelodyNotes);

    FDrumAndBassSegment Segment = Generator->GenerateMusic();

    SetDrumPatterns(Segment.Drums.KickPattern, Segment.Drums.SnarePattern, Segment.Drums.CymbalPattern, Segment.Drums.Perc1Pattern, Segment.Drums.Perc2Pattern);
    SetBassPatterns(Segment.Bass);
    SetChordPatterns(Segment.Chords);
    SetMelodyPatterns(Segment.Melody);
    
    PlayMusic();
}

void AProceduralMusicGameMode::SetDrumPatterns(TArray<float> a_KickPattern, TArray<float> a_SnarePattern, TArray<float> a_HHPattern, TArray<float> a_Perc1Pattern, TArray<float> a_Perc2Pattern)
{
    m_pMusic->SetFloatArrayParameter(KICK_PATTERN_NAME, a_KickPattern);
    m_pMusic->SetFloatArrayParameter(SNARE_PATTERN_NAME, a_SnarePattern);
    m_pMusic->SetFloatArrayParameter(HH_PATTERN_NAME, a_HHPattern);
    m_pMusic->SetFloatArrayParameter(PERC1_PATTERN_NAME, a_Perc1Pattern);
    m_pMusic->SetFloatArrayParameter(PERC2_PATTERN_NAME, a_Perc2Pattern);
}

void AProceduralMusicGameMode::SetBassPatterns(TArray<FMusicNote> a_BassNotes)
{
    TArray<TArray<float>> BassPatterns = SplitNoteArray(a_BassNotes);

    m_pMusic->SetFloatArrayParameter(BASS_NOTES_PATTERN_NAME, BassPatterns[0]);
    m_pMusic->SetFloatArrayParameter(BASS_VELOCITIES_PATTERN_NAME, BassPatterns[1]);
}

void AProceduralMusicGameMode::SetChordPatterns(FChordArrays a_Chords)
{
    TArray<TArray<float>> RootNotes = SplitNoteArray(a_Chords.RootNotes);
    TArray<TArray<float>> ThirdNotes = SplitNoteArray(a_Chords.ThirdNotes);
    TArray<TArray<float>> FifthNotes = SplitNoteArray(a_Chords.FifthNotes);
    TArray<TArray<float>> SeventhNotes = SplitNoteArray(a_Chords.SeventhNotes);
    TArray<TArray<float>> NinthNotes = SplitNoteArray(a_Chords.NinthNotes);
    TArray<TArray<float>> EleventhNotes = SplitNoteArray(a_Chords.EleventhNotes);
    TArray<TArray<float>> ThirteenthNotes = SplitNoteArray(a_Chords.ThirteenthNotes);

    m_pMusic->SetFloatArrayParameter(CHORD_ROOT_NOTES_PATTERN_NAME, RootNotes[0]);
    m_pMusic->SetFloatArrayParameter(CHORD_ROOT_VELOCITIES_PATTERN_NAME, RootNotes[1]);
    m_pMusic->SetFloatArrayParameter(CHORD_THIRD_NOTES_PATTERN_NAME, ThirdNotes[0]);
    m_pMusic->SetFloatArrayParameter(CHORD_THIRD_VELOCITIES_PATTERN_NAME, ThirdNotes[1]);
    m_pMusic->SetFloatArrayParameter(CHORD_FIFTH_NOTES_PATTERN_NAME, FifthNotes[0]);
    m_pMusic->SetFloatArrayParameter(CHORD_FIFTH_VELOCITIES_PATTERN_NAME, FifthNotes[1]);
    m_pMusic->SetFloatArrayParameter(CHORD_SEVENTH_NOTES_PATTERN_NAME, SeventhNotes[0]);
    m_pMusic->SetFloatArrayParameter(CHORD_SEVENTH_VELOCITIES_PATTERN_NAME, SeventhNotes[1]);
    // m_pMusic->SetFloatArrayParameter(CHORD_NINTH_NOTES_PATTERN_NAME, NinthNotes[0]);
    // m_pMusic->SetFloatArrayParameter(CHORD_NINTH_VELOCITIES_PATTERN_NAME, NinthNotes[1]);
    // m_pMusic->SetFloatArrayParameter(CHORD_ELEVENTH_NOTES_PATTERN_NAME, EleventhNotes[0]);
    // m_pMusic->SetFloatArrayParameter(CHORD_ELEVENTH_VELOCITIES_PATTERN_NAME, EleventhNotes[1]);
    // m_pMusic->SetFloatArrayParameter(CHORD_THIRTEENTH_NOTES_PATTERN_NAME, ThirteenthNotes[0]);
    // m_pMusic->SetFloatArrayParameter(CHORD_THIRTEENTH_VELOCITIES_PATTERN_NAME, ThirteenthNotes[1]);
}

void AProceduralMusicGameMode::SetMelodyPatterns(TArray<FMusicNote> a_MelodyNotes)
{
    TArray<TArray<float>> MelodyNotes = SplitNoteArray(a_MelodyNotes);

    m_pMusic->SetFloatArrayParameter(MELODY_NOTES_PATTERN_NAME, MelodyNotes[0]);
    m_pMusic->SetFloatArrayParameter(MELODY_VELOCITIES_PATTERN_NAME, MelodyNotes[1]);
}

TArray<TArray<float>> AProceduralMusicGameMode::SplitNoteArray(TArray<FMusicNote> a_NotePattern)
{
    TArray<TArray<float>> Patterns;

    TArray<float> MidiNotes, Velocities;
    MidiNotes.SetNum(a_NotePattern.Num());
    Velocities.SetNum(a_NotePattern.Num());

    for (size_t i = 0; i < a_NotePattern.Num(); i++)
    {
        MidiNotes[i] = a_NotePattern[i].MidiNote;
        Velocities[i] = a_NotePattern[i].Velocity;
    }

    Patterns.Add(MidiNotes);
    Patterns.Add(Velocities);
    
    return Patterns;
}

void AProceduralMusicGameMode::PlayMusic()
{
    if (!m_pMusic)
    {
        UE_LOG(LogTemp, Error, TEXT("Audio Component for the music does not exist!"));
        return;
    }
    if (!m_pMusic->GetSound())
    {
        UE_LOG(LogTemp, Error, TEXT("No Sound loaded to the Component!"));
        return;
    }
    if (m_pMusic->IsActive()  == false) m_pMusic->SetActive(true);
    if (m_pMusic->IsPlaying() == false) m_pMusic->Play();

    m_pMusic->SetTriggerParameter(PLAY_TRIGGER_NAME);
}
