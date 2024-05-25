#pragma once

#include "timber_frame.h"
#include "osc_param.h"
#include "param/timber_param.h"
#include "param/param_helper.h"
#include <numbers>

namespace mana {
class PWM {
public:
    void Init(float sample_rate, float update_rate) {}
    void Process(TimberFrame& frame) {
        for (int i = 0; i < kNumPartials; ++i) {
            auto down = (i + 1.0f) * std::numbers::pi_v<float> *2.0f;
            frame.gains[i] = (1.0f - pwm_tilt_ * std::cos(std::numbers::pi_v<float> *(i + 1.0f) * pwm_width_)) / down;
        }
    }

    void OnUpdateTick(OscParam& params) {
        pwm_width_ = helper::GetAlterParamValue(params.args, param::Pwm_Width{});
        pwm_tilt_ = helper::GetAlterParamValue(params.args, param::Pwm_Tilt{});
    }

    void OnNoteOn(int note) {}
    void OnNoteOff() {}
private:
    float pwm_width_{};
    float pwm_tilt_{};
};
}