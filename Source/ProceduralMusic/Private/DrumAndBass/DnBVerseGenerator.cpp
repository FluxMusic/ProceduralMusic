#include "DnBVerseGenerator.h"

DnBVerseGenerator::DnBVerseGenerator(FRandomStream &SeedIn, const FMusicGenerationSpecs& SpecsIn, UDataTable* ScalesIn)
: DrumAndBassSegmentGenerator(SeedIn, ScalesIn)
, DrumGenerator(SeedIn, SpecsIn)
, BassGenerator(SeedIn, SpecsIn, ScalesIn)
, ChordGenerator(SeedIn, SpecsIn, ScalesIn)
, MelodyGenerator(SeedIn, SpecsIn, ScalesIn)
{}

FDrumAndBassSegment DnBVerseGenerator::GenerateSegment()
{
    Segment.Drums = DrumGenerator.GenerateDrums();
    Segment.Bass  = BassGenerator.GenerateBass();
    Segment.Chords = ChordGenerator.GenerateChords(Segment.Bass);
    Segment.Melody = MelodyGenerator.GenerateMelody();

    // LengthenSegment();

    SnapMelodyTimingToBass();

    return Segment;
}

void DnBVerseGenerator::SnapMelodyTimingToBass()
{
    TArray<int32> BassGroupStarts   = GetNoteGroupStarts(Segment.Bass);
    TArray<int32> MelodyGroupStarts = GetNoteGroupStarts(Segment.Melody);
    
    for (int32 GroupStart : MelodyGroupStarts)
    {
        if (BassGroupStarts.Find(GroupStart) != INDEX_NONE)
        {
            continue;
        }
        else
        {
            if (BassGroupStarts.Find(GroupStart - 1) != INDEX_NONE)
            {
                Segment.Melody[GroupStart - 1] = Segment.Melody[GroupStart];
            }
            else if (BassGroupStarts.Find(GroupStart + 1) != INDEX_NONE)
            {
                Segment.Melody[GroupStart] = Segment.Melody[GroupStart - 1];
            }
            else
            {
                continue;
            }
        }
    }

    // for (size_t i = 0; i < m_Pattern.MelodyNotes.Num(); i++)
    // {
    //     UE_LOG(LogTemp, Display, TEXT("MelodyNoteSnapped: %f, MelodyNoteVelocitySnapped: %f"), m_Pattern.MelodyNotes[i].MidiNote, m_Pattern.MelodyNotes[i].Velocity);
    // }
}

TArray<int32> DnBVerseGenerator::GetNoteGroupStarts(TArray<FMusicNote> Notes)
{
    TArray<int32> NoteGroupStarts;

    NoteGroupStarts.Add(0);

    for (size_t i = 1; i < Notes.Num(); i++)
    {
        if (Notes[i].MidiNote != Notes[i - 1].MidiNote)
        {
            NoteGroupStarts.Add(i);
        }
    }

    return NoteGroupStarts;
}

void DnBVerseGenerator::LengthenSegment()
{
    //Probably very expensive
    FDrumAndBassSegment SegmentCopy = Segment;
    
    //Bass is easiest to access, doesn't matter which pattern to loop through though
    int32 SegmentLength = SegmentCopy.Bass.Num();

    //Copy the previous Notes
    for (size_t j = 0; j < 2; j++)
    {
        for (int32 i = 0; i < SegmentLength; i++)
        {
            Segment.Drums.KickPattern.Add  (SegmentCopy.Drums.KickPattern[i]);
            Segment.Drums.SnarePattern.Add (SegmentCopy.Drums.SnarePattern[i]);
            Segment.Drums.CymbalPattern.Add(SegmentCopy.Drums.CymbalPattern[i]);
            Segment.Drums.Perc1Pattern.Add (SegmentCopy.Drums.Perc1Pattern[i]);
            Segment.Drums.Perc2Pattern.Add (SegmentCopy.Drums.Perc2Pattern[i]);
    
            Segment.Bass.Add(SegmentCopy.Bass[i]);
    
            Segment.Chords.RootNotes.Add      (SegmentCopy.Chords.RootNotes[i]);
            Segment.Chords.ThirdNotes.Add     (SegmentCopy.Chords.ThirdNotes[i]);
            Segment.Chords.FifthNotes.Add     (SegmentCopy.Chords.FifthNotes[i]);
            Segment.Chords.SeventhNotes.Add   (SegmentCopy.Chords.SeventhNotes[i]);
            Segment.Chords.NinthNotes.Add     (SegmentCopy.Chords.NinthNotes[i]);
            Segment.Chords.EleventhNotes.Add  (SegmentCopy.Chords.EleventhNotes[i]);
            Segment.Chords.ThirteenthNotes.Add(SegmentCopy.Chords.ThirteenthNotes[i]);
    
            Segment.Melody.Add(SegmentCopy.Melody[i]);
        }
    }

    //Copy everything but the melody
    for (int32 i = 0; i < SegmentLength; i++)
    {
        Segment.Drums.KickPattern.Add  (SegmentCopy.Drums.KickPattern[i]);
        Segment.Drums.SnarePattern.Add (SegmentCopy.Drums.SnarePattern[i]);
        Segment.Drums.CymbalPattern.Add(SegmentCopy.Drums.CymbalPattern[i]);
        Segment.Drums.Perc1Pattern.Add (SegmentCopy.Drums.Perc1Pattern[i]);
        Segment.Drums.Perc2Pattern.Add (SegmentCopy.Drums.Perc2Pattern[i]);

        Segment.Bass.Add(SegmentCopy.Bass[i]);

        Segment.Chords.RootNotes.Add      (SegmentCopy.Chords.RootNotes[i]);
        Segment.Chords.ThirdNotes.Add     (SegmentCopy.Chords.ThirdNotes[i]);
        Segment.Chords.FifthNotes.Add     (SegmentCopy.Chords.FifthNotes[i]);
        Segment.Chords.SeventhNotes.Add   (SegmentCopy.Chords.SeventhNotes[i]);
        Segment.Chords.NinthNotes.Add     (SegmentCopy.Chords.NinthNotes[i]);
        Segment.Chords.EleventhNotes.Add  (SegmentCopy.Chords.EleventhNotes[i]);
        Segment.Chords.ThirteenthNotes.Add(SegmentCopy.Chords.ThirteenthNotes[i]);
    }
    
    TArray<FMusicNote> NewMelody = MelodyGenerator.GenerateMelody();

    for (int32 i = 0; i < NewMelody.Num(); i++)
    {
        Segment.Melody.Add(NewMelody[i]);
    }
}
