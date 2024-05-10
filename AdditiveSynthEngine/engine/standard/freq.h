#pragma once

#include "engine/IProcessor.h"

namespace mana {
class FreqProcessor : public IProcessor {
public:
    void Init(float sample_rate, float update_rate) override;
    void OnUpdateTick(const OscillorParams& param, int skip, int module_idx) override;
    void OnNoteOn(int note) override;
    void OnNoteOff() override;
    void Process(Partials& partials) override;
private:
    float reciprocal_nyquist_rate_{};
    float note_pitch_{};
    float base_frequency_{};
    float base_pitch_{};
};
}