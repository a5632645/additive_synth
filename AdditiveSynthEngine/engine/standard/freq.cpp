#include "freq.h"

#include <cmath>
#include "param/standard_param.h"
#include "engine/oscillor_param.h"

namespace mana {
void FreqProcessor::Init(float sample_rate, float update_rate) {
    reciprocal_nyquist_rate_ = 2.0f / sample_rate;
}

void FreqProcessor::OnUpdateTick() {
    //base_pitch_ = note_pitch_ + param::PitchBend::GetNumber(pitch_bend_->GetValue());
    base_pitch_ = note_pitch_ + pitch_bend_->GetValue();
    base_frequency_ = std::exp2(base_pitch_ / 12.0f) * 8.1758f * reciprocal_nyquist_rate_;
}

void FreqProcessor::OnNoteOn(int note) {
    note_pitch_ = static_cast<float>(note);
}

void FreqProcessor::OnNoteOff() {
    //note_pitch_ = -1;
}

void FreqProcessor::PrepareParams(OscillorParams & params) {
    pitch_bend_ = params.GetPolyFloatParam("pitch_bend");
}

void FreqProcessor::Process(Partials& partials) {
    partials.base_frequency = base_frequency_;
    partials.base_pitch = base_pitch_;
}
}