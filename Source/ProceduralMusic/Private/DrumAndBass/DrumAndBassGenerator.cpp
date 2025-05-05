// Fill out your copyright notice in the Description page of Project Settings.


#include "DrumAndBassGenerator.h"

#include "Math/RandomStream.h"
#include "DnBVerseGenerator.h"

UDrumAndBassGenerator::UDrumAndBassGenerator()
{
    Scales = ConstructorHelpers::FObjectFinder<UDataTable>(*ScaleDataTablePath).Object;
}

void UDrumAndBassGenerator::Initialise(const FRandomStream& SeedIn, const FMusicGenerationSpecs& SpecsIn)
{
    Seed = SeedIn;
    Specs = SpecsIn;
}

FDrumAndBassSegment UDrumAndBassGenerator::GenerateMusic()
{
    // PreGenerate();

    TArray<FDrumAndBassSegment> GeneratedSegments;

    for (int32 i = 0; i < Specs.MusicSegmentAmount; i++)
    {
        if (i == 0 && Specs.bGenerateIntro)
        {
            //Add Intro Segment and continue loop
        }
        
        if (i == (Specs.MusicSegmentAmount - 1) && Specs.bGenerateOutro)
        {
            //Add Outro Segment and continue loop
        }

        DnBVerseGenerator Generator(Seed, Specs, Scales);

        GeneratedSegments.Add(Generator.GenerateSegment());
    }

    // DnBVerseGenerator Generator(Seed, Specs, Scales);

    // return Generator.GenerateSegment();

    return TranscodeSegments(GeneratedSegments);
}

void UDrumAndBassGenerator::PreGenerate()
{
    for (int32 i = 0; i < Specs.MusicSegmentAmount; i++)
    {
        if (i == 0 && Specs.bGenerateIntro)
        {
            // Add Intro Generator to Segment Array

            // Segments.Add(DnBIntroGenerator(Seed, Specs, Scales));
            // continue;
        }
        
        if (i == (Specs.MusicSegmentAmount - 1) && Specs.bGenerateOutro)
        {
            // Add Outro Generator to Segment Array

            // Segments.Add(DnBOutroGenerator(Seed, Specs, Scales));
            // continue;
        }
        
        Segments.Add(DnBVerseGenerator(Seed, Specs, Scales));
    }
}

FDrumAndBassSegment UDrumAndBassGenerator::TranscodeSegments(TArray<FDrumAndBassSegment> SegmentsIn)
{
    FDrumAndBassSegment Song;

    for (int32 i = 0; i < SegmentsIn.Num(); i++)
    {
        Song.Drums.KickPattern.Append  (SegmentsIn[i].Drums.KickPattern);
        Song.Drums.SnarePattern.Append (SegmentsIn[i].Drums.SnarePattern);
        Song.Drums.CymbalPattern.Append(SegmentsIn[i].Drums.CymbalPattern);
        Song.Drums.Perc1Pattern.Append (SegmentsIn[i].Drums.Perc1Pattern);
        Song.Drums.Perc2Pattern.Append (SegmentsIn[i].Drums.Perc2Pattern);

        Song.Bass.Append(SegmentsIn[i].Bass);

        Song.Chords.RootNotes.Append      (SegmentsIn[i].Chords.RootNotes);
        Song.Chords.ThirdNotes.Append     (SegmentsIn[i].Chords.ThirdNotes);
        Song.Chords.FifthNotes.Append     (SegmentsIn[i].Chords.FifthNotes);
        Song.Chords.SeventhNotes.Append   (SegmentsIn[i].Chords.SeventhNotes);
        Song.Chords.NinthNotes.Append     (SegmentsIn[i].Chords.NinthNotes);
        Song.Chords.EleventhNotes.Append  (SegmentsIn[i].Chords.EleventhNotes);
        Song.Chords.ThirteenthNotes.Append(SegmentsIn[i].Chords.ThirteenthNotes);

        Song.Melody.Append(SegmentsIn[i].Melody);
    }
    
    return Song;
}