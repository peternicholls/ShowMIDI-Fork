# ShowMIDI - Development Fork

<p><img align="right" src="https://github.com/user-attachments/assets/ad3e7647-c768-4817-a8bf-6fa65707389f" width="160px" /><br /></p>

ShowMIDI is a multi-platform GUI application to effortlessly visualize MIDI activity.

All the heavy lifting is done by the wonderful JUCE library.

The project website is https://github.com/gbevin/ShowMIDI

<p style="clear: both;"><img src="https://raw.githubusercontent.com/gbevin/ShowMIDI/main/Design/showmidi-1.0.0-line.png" style="width: 640px" /></p>
<p><img src="https://raw.githubusercontent.com/gbevin/ShowMIDI/main/Design/showmidi-1.0.0-graph.png" style="width: 640px" /></p>

## Community

Join the Forums: https://forum.uwyn.com

Chat on Discord: https://discord.gg/TgsCnwqWTf

## Purpose

This application fills a void in the available MIDI monitoring solutions.

Instead of wading through logs of MIDI messages to correlate relevant ones and identify what is happening, ShowMIDI visualizes the current activity and hides what you don't care about anymore. It provides you with a real-time glanceable view of all MIDI activity on your computer.

This animation shows the difference between a traditional MIDI monitor on the left and ShowMIDI on the right:

<p><img src="https://uwyn.com/images/showmidi-animation-small.gif" width="640" /></p>

ShowMIDI displays:

* MIDI Devices
* MIDI Channels (CH)
* Note On with Velocity (ON)
* Note Off with Velocity (OFF)
* Control Change (CC)
* Hi-Res 14-bit Control Change (HRCC)
* Pitch Bend Change (PB)
* Program Change (PRGM)
* Channel Pressure (CP)
* Polyphonic Key Pressure (PP)
* Registered Parameter Number (RPN)
* Non-Registered Parameter Number (NRPN)
* MPE Configuration (MPE MGR, MPE LZ, MPE UZ)
* Timing Clock as BPM (CLOCK BPM)
* Clock Start (START)
* Clock Continue (CONT)
* Clock Stop (STOP)
* System Exclusive (SYSEX)

## Video introduction

Here's a tutorial video that shows you a quick overview of why I created ShowMIDI:

<a href="https://uwyn.com/showmidi-tutorial" target="_blank"><img src="https://uwyn.com/showmidi-tutorial.jpg" alt="Tutorial Video" width="640" border="0" /></a>

## Download

You can download pre-built binaries from the release section: 
https://github.com/gbevin/ShowMIDI/releases

Since ShowMIDI is free and open-source, you can also easily build it yourself. Just take a look into the Builds directory when you download the sources.

If you're using the macOS Homebrew package manager, you can install ShowMIDI with:
```
brew install gbevin/tools/showmidi
```

## Usage

To use it, simply double-click the application to launch it.

All connected MIDI devices will automatically be shown in columns. By expanding the left side panel, individual devices can be shown or hidden. Holding the `alt` or `option` key while clicking on a device to change its visibility, will change the visibility of all the devices. 

When an incoming message first appears on a MIDI channel, that channel will appear at the top of the MIDI device column. For as long as a channel is displayed, it will keep its order in the device column. When a channel is auto-hidden through inactivity, the next time activity happens, the channel will be displayed at the top of the MIDI device column again.

To pause the MIDI data stream, press the `spacebar`, to un-pause, press it again.

The data can either be visualized as horizontal lines or as scrolling graphs. Toggling the visualization icon allows you to switch this is realtime, pressing the `v` key allows you to do that from the keyboard.

To quickly clear the displayed data, press the reset button or the `delete`/`backspace` key on your keyboard. 

The plugin versions can be used in any DAW that supports MIDI effect plugins. Some plugin formats, like VST3, might filter out certain MIDI messages and not present a fully accurate view.

## Themes

ShowMIDI ships with a handful of themes, more will be added in the future.

You can edit, create and save them yourself in the application. Feel free to submit your themes to me.

You can find the included themes [in the repository](https://github.com/gbevin/ShowMIDI/tree/main/Themes), please make sure to download the raw SVG files, otherwise the download might not contain the right information. Here are direct links to some of the included themes:

![](https://raw.githubusercontent.com/gbevin/ShowMIDI/main/Themes/bstation.svg)
![](https://raw.githubusercontent.com/gbevin/ShowMIDI/main/Themes/classic%20light.svg)
![](https://raw.githubusercontent.com/gbevin/ShowMIDI/main/Themes/darcula.svg)
![](https://raw.githubusercontent.com/gbevin/ShowMIDI/main/Themes/dark.svg)
![](https://raw.githubusercontent.com/gbevin/ShowMIDI/main/Themes/disco.svg)
![](https://raw.githubusercontent.com/gbevin/ShowMIDI/main/Themes/light.svg)
![](https://raw.githubusercontent.com/gbevin/ShowMIDI/main/Themes/mouha.svg)

## Credits

ShowMIDI was created by Geert Bevin: https://uwyn.com

The UI design was done by Stephen Petoniak: https://spetoniak.com

## Building from Source on Linux

ShowMIDI relies on Git submodules for compilation of external dependencies.

After cloning the repository, make sure to update and initialize all the Git submodules:

```
git submodule update --init --recursive
```

To build ShowMIDI on Linux you need a minimal set of packages installed beforehand, on Ubuntu this can be done with:

```
sudo apt install build-essential pkg-config libasound2-dev \
  libfreetype6-dev libx11-dev libxrandr-dev \
  libxinerama-dev libxcursor-dev
```

You'll also need to find a version of the VST2 SDK and put it in a location that the compiler can find.
Searching online for `vst2.x/aeffect.h` should give you some options for download.
Sadly it's not allowed to distribute this SDK, so it has not been included.
The folder of the VST2 SDK should be placed at `libs/vst2` in the source repository.

After that, go to the `LinuxMakefile` directory

```
cd Builds/LinuxMakefile
```

and build the binary by typing `make`

```
make CONFIG=Release
```

The resulting binary will be in the `Build/LinuxMakefile/build` directory and can be moved anywhere appropriate on your system, for instance into `/usr/local/bin`:

```
sudo mv build/ShowMIDI /usr/local/bin
```
