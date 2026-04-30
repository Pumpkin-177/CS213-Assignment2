# 🎵 JUCE Audio Player

> A feature-rich, interactive audio player built with the JUCE Framework in C++ — developed as part of **CS213, Assignment 2**.

---

## 📌 Overview

This project is a custom audio player that provides an interactive interface for playing, looping, marking, and segmenting audio files, with support for playlists and multi-track playback.

---

## ✨ Features

- ▶️ **Playback Control** — Play, pause, stop, and seek through audio files
- 🔁 **Loop & Repeat** — Loop entire tracks or defined regions seamlessly
- 📍 **Mark & Segment** — Set cue points and split audio into labeled regions
- 📋 **Playlist Manager** — Build and manage playlists with multi-track support
- 🎛️ **Multi-track Playback** — Load and control multiple audio files simultaneously
- 🖥️ **Custom GUI** — Fully interactive interface built from scratch using JUCE components

---

## 🗂️ Project Structure

```
├── Main.cpp                  # Application entry point
├── MainComponent.h / .cpp    # Top-level component & layout orchestration
├── PlayerAudio.h / .cpp      # Audio engine, DSP logic & transport control
└── PlayerGUI.h / .cpp        # UI controls, sliders, buttons & waveform view
```

---

## 🏗️ Architecture

```
Main.cpp
    └── MainComponent        (orchestrates all modules)
            ├── PlayerAudio  (AudioSource, Transport, DSP)
            └── PlayerGUI    (Sliders, Buttons, Waveform display)
```

---

## 🚀 Getting Started

### Prerequisites

- [JUCE Framework](https://juce.com/) installed
- A C++ IDE: Xcode, Visual Studio, or CLion

### Build Steps

1. **Open the project** — Load the `.jucer` file in Projucer, or use CMake
2. **Export to your IDE** — Use Projucer to generate the IDE project files
3. **Build & Run** — Compile and run the executable from your IDE
4. **Load audio files** — Drag & drop audio files or use the built-in file browser

---

## 🛠️ Built With

- **C++** — Core programming language
- **JUCE Framework** — Audio engine, GUI components, and cross-platform support

---

## 🎓 Course Info

Developed as part of **CS213 — Assignment 2**.  
Demonstrates real-time audio programming, event-driven GUI design, and object-oriented architecture using the JUCE C++ framework.
