# KAudioSwi - KDE Audio Output Switcher

A KRunner plugin for KDE Plasma that allows quick switching between audio output devices.

## Features

- Quickly change the default audio output device using KRunner
- Displays notifications on successful or failed audio device switching
- Seamlessly integrates with KDE Plasma desktop environment
- Simple and intuitive interface

## How It Works

KAudioSwi adds a new command to KRunner that allows you to switch between your audio output devices. Just:

1. Open KRunner (typically with Alt+Space or Alt+F2)
2. Type `to` followed by part of your audio device name
3. Select from the matching devices
4. The audio output will immediately switch to the selected device

## Usage Examples

- `to` - Lists all available audio output devices
- `to speakers` - Shows devices with "speakers" in their name
- `to bluetooth` - Shows devices with "bluetooth" in their name

## Requirements

- KDE Plasma 6
- Qt 6.0 or higher
- KDE Frameworks 6.0 or higher
- PulseAudio (the plugin uses the `pactl` command)

## Installation

### Building from Source

```bash
mkdir build
cd build
cmake ..
make
sudo make install
```

### Dependencies

Required packages:
- cmake
- Qt6 Multimedia
- KF6 Runner
- KF6 Notifications

## Technical Details

The plugin works by:
1. Discovering available audio output devices using Qt's QMediaDevices API
2. When a device is selected, using `pactl set-default-sink` to change the system's default audio output
3. Showing a notification about the success or failure of the operation

