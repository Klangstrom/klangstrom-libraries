/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
* Copyright (c) 2024 Dennis P Paul.
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "KlangstromEnvironment.h"
#ifdef KLST_ARCH_IS_EMU

#include "AudioDevice_ASP_EMU.h"
#include "Console.h"
#include "KlangstromEmulator.h"

class DrawableAudioCodec : public Drawable {
public:
    explicit DrawableAudioCodec(AudioDevice* audiodevice) : fAudioDevice(audiodevice) {}

    void draw(PGraphics* g_ptr) override {
        PGraphics& g = *g_ptr;

        g.stroke(1);
        g.noFill();
        g.pushMatrix();
        g.translate(20, 200);
        const int   mStrafe = 16;
        const float mHeight = 150;
        const float mWidth  = 512 - 20;

        if (fAudioDevice->peripherals->is_paused) {
            g.fill(1);
            g.textSize(KlangstromEmulator::DEFAULT_FONT_SIZE * 0.5f);
            g.text("PAUSED", 3, -mHeight / 2 + KlangstromEmulator::DEFAULT_FONT_SIZE * 0.5f + 2);
            g.noFill();
        }

        float** mBuffers = KlangstromEmulator::instance()->get_audio_output_buffers(); // TODO get buffer from each device
        for (int i = 0; i < audio_output_channels; i++) {
            const auto ii = static_cast<float>(i);
            g.stroke(1, 0.5f);
            g.rect(0, ii * mHeight - mHeight * 0.5f, mWidth, mHeight);
            g.line(0, ii * mHeight, mWidth, ii * mHeight);
            g.stroke(1);
            for (int j = mStrafe; j < DEFAULT_FRAMES_PER_BUFFER; j += mStrafe) {
                float       mSample0 = mBuffers[i][j - mStrafe] * 0.5f;
                float       mSample1 = mBuffers[i][j] * 0.5f;
                const float x0       = mWidth * (float) (j - mStrafe) / DEFAULT_FRAMES_PER_BUFFER;
                const float y0       = ii * mHeight + mSample0 * mHeight;
                const float x1       = mWidth * (float) j / DEFAULT_FRAMES_PER_BUFFER;
                const float y1       = ii * mHeight + mSample1 * mHeight;
                g.line(x0, y0, x1, y1);
                g.line(x0, y0, x0, ii * mHeight);
            }
        }
        g.translate(0, mHeight * 2 + 10);
        for (int i = 0; i < audio_input_channels; i++) {
            const auto ii = static_cast<float>(i);
            g.stroke(1, 0.5f);
            g.rect(0, ii * mHeight - mHeight * 0.5f, mWidth, mHeight);
            g.line(0, ii * mHeight, mWidth, ii * mHeight);
            g.stroke(1);
            mBuffers = KlangstromEmulator::instance()->get_audio_input_buffers();
            for (int j = mStrafe; j < DEFAULT_FRAMES_PER_BUFFER; j += mStrafe) {
                float       mSample0 = mBuffers[i][j - mStrafe] * 0.5f;
                float       mSample1 = mBuffers[i][j] * 0.5f;
                const float x0       = mWidth * (float) (j - mStrafe) / DEFAULT_FRAMES_PER_BUFFER;
                const float y0       = ii * mHeight + mSample0 * mHeight;
                const float x1       = mWidth * (float) j / DEFAULT_FRAMES_PER_BUFFER;
                const float y1       = ii * mHeight + mSample1 * mHeight;
                g.line(x0, y0, x1, y1);
                g.line(x0, y0, x0, ii * mHeight);
            }
        }
        g.popMatrix();
    }

private:
    AudioDevice* fAudioDevice;
};

#ifdef __cplusplus
extern "C" {
#endif

void audiodevice_resume(AudioDevice* audiodevice) {
    audiodevice->peripherals->is_paused = false;
}

void audiodevice_pause(AudioDevice* audiodevice) {
    audiodevice->peripherals->is_paused = true;
}

void audiodevice_deinit_BSP(AudioDevice* audiodevice) {
    (void) audiodevice;
}

void audiodevice_init_peripherals_BSP(AudioDevice* audiodevice) {
    audiodevice->peripherals = new AudioDevicePeripherals();
}

void audiodevice_deinit_peripherals_BSP(AudioDevice* audiodevice) {
    delete audiodevice->peripherals;
}

static void rx_input_callback(AudioDevice* audiodevice, uint8_t callback_type) {
    if (callback_type == CALLBACK_FULL_COMPLETE) {
    } else if (callback_type == CALLBACK_HALF_COMPLETE) {
    }
    (void) audiodevice; // TODO maybe copy input data to audioblock?
}

static void tx_output_callback(AudioDevice* audiodevice, uint8_t callback_type) {
    if (callback_type == CALLBACK_FULL_COMPLETE) {
    } else if (callback_type == CALLBACK_HALF_COMPLETE) {
    }
    if (audiodevice != nullptr && audiodevice->callback_audioblock != nullptr) {
        audiodevice->callback_audioblock(audiodevice->audioblock);
    }
}

static void error_callback(AudioDevice* audiodevice, uint8_t callback_type) {
    (void) audiodevice;
    (void) callback_type;
    console_error("error in audio device");
}

void audiodevice_init_device_BSP(AudioDevice* audiodevice) {
    audiodevice->peripherals->callback_rx    = rx_input_callback;
    audiodevice->peripherals->callback_tx    = tx_output_callback;
    audiodevice->peripherals->callback_error = error_callback;
    KlangstromEmulator::instance()->register_audio_device(audiodevice);
    KlangstromEmulator::instance()->register_drawable(new DrawableAudioCodec(audiodevice));
    audiodevice_pause(audiodevice);
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_EMU