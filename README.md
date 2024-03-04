# Sostenuto Pedal
This program provides the [sostenuto pedal](https://en.wikipedia.org/wiki/Piano_pedals#Sostenuto_pedal) function.
- It keeps the status of the sostenuto pedal and keys depending on the MIDI CC66 and NOTE ON/OFF.
- It drops or sends NOTE OFF messages depending on status above.
- It drops the MIDI CC66  messages.

There are two versions, one is works on [ReaJS](https://www.reaper.fm/reaplugs/ "ReaPlugs") by Cockos, and another one is a VST3 plugin using the JUCE framework.

## How to install the ReaJS version
Place it as follows.
```
C:\Users\<someone>\AppData\Roaming\REAPER\Effects\0eq00\sostenuto_pedal
