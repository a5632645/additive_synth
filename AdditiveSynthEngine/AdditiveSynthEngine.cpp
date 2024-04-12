﻿#include <chrono>
#include "raylib.h"
#include <cstdint>
#include <limits>
#include "engine/synth.h"
#include "utli/Keyboard.hpp"
#include "layout/SynthLayout.h"

static mana::Synth synth_;
static mana::KeyBoard keyboard_;
static mana::SynthLayout synth_layout_{ synth_ };

static void ThisAudioCallback(void* buffer, unsigned int frames) {
    synth_.update_state(frames);
    synth_.Render(frames);
    std::ranges::copy(synth_.getBuffer(), static_cast<float*>(buffer));
}

int main(void) {
    InitWindow(800, 600, "additive synth");
    InitAudioDevice();
    SetAudioStreamBufferSizeDefault(480);
    AudioStream stream = LoadAudioStream(48000, 32, 1);
    SetAudioStreamCallback(stream, ThisAudioCallback);

    PlayAudioStream(stream);        // Start processing stream buffer (no data loaded currently)
    SetTargetFPS(30);               // Set our game to run at 30 frames-per-second

    // init synth
    synth_.Init(480, 48000.0f);
    synth_layout_.resized(0, 0, 800, 600);

    // link keyboard and synth
    keyboard_.onNoteOn = [](int n) {synth_.NoteOn(n, 1.0f); };
    keyboard_.onNoteOff = [](int n) {synth_.NoteOff(n, 1.0f); };

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        keyboard_.checkInFrame();

        // drawing
        BeginDrawing();
        ClearBackground(BLACK);

        const auto& buffer = synth_.getBuffer();
        for (int i = 0; i < 800; ++i) {
            size_t idx = static_cast<size_t>(i / 800.0f * buffer.size());
            int y = static_cast<int>(300 * (1.0f - buffer[idx]));
            DrawPixel(i, y, WHITE);
        }

        const auto& partials = synth_.GetDisplayOscillor().GetPartials();
        for (size_t i = 0; i < mana::kNumPartials; ++i) {
            int x = static_cast<int>(800 * partials.freqs[i]);
            int y = static_cast<int>(600 * (1.0f - partials.gains[i]));
            DrawLine(x, y, x, 600, RED);
        }

        synth_layout_.paint();


        EndDrawing();
    }

    UnloadAudioStream(stream);   // Close raw audio stream and delete buffers from RAM
    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)

    CloseWindow();              // Close window and OpenGL context

    return 0;
}