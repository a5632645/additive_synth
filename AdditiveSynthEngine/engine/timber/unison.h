#pragma once

#include <random>
#include "engine/modulation/Parameter.h"
#include "engine/poly_param.h"
#include "engine/oscillor_param.h"
#include "engine/partials.h"

namespace mana {
class Unison {
public:
    void Init(float sample_rate, float update_rate);
    void PrepareParams(OscillorParams& params);
    void Process(Partials& partials);
    void OnUpdateTick();
    void OnNoteOn();
    void OnNoteOff();
private:
    std::default_random_engine random_;
    std::uniform_real_distribution<float> urd_{ -1.0f,1.0f };
    float update_skip_{};

    std::array<float, 9> voice_ratios_;
    std::array<float, 9> voice_phases_;

    IntChoiceParameter* unison_type_{};
    IntParameter* num_voice_{};
    int old_num_voice_{};
    PolyModuFloatParameter* pitch_{};
    PolyModuFloatParameter* phase_{};
    PolyModuFloatParameter* pan_{};
};
}