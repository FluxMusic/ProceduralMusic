# ProceduralMusic

This is a test for a possible way to structure Procedural Music using MetaSounds in Unreal Engine 5.

## Controls

### Play

Plays or pauses the playback of the music element.

### Pan

Pans the element in the stereo field. Does not support vertical or horizontal dragging -> Used like a regular slider.

### Volume

The Volume Fader. Controls Volume of the element.

### Randomizers

Randomizes the melody of the selected element after 4 bars of playback.

### Scale

Selects a musical Scale for the melodies to snap to. Responsive immediately.

### Root Note

Selects the Root Note for the Scale. Responsive Immediately.

## Known Issues

### Rhythmic Randomization

Due to The Random Get Node in MetaSounds not resetting the way the no-repeat works, the randomization of the rhythms does not work. The Rhythm will, however, get randomized on construction of the MetaSound object.



