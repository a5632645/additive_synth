#include "resynthsis.h"

#include "engine/synth.h"
#include "param/resynthsis_param.h"
#include "utli/warp.h"
#include "param/param_helper.h"

namespace mana {
void Resynthesis::Init(float sample_rate, float update_rate) {
    sample_rate_ = sample_rate;
    inv_sample_rate_ = 1.0f / sample_rate;
    skip_ = sample_rate / update_rate;
}

void Resynthesis::Process(Partials& partials) {
    using namespace std::views;

    if (!IsWork()) {
        return;
    }

    std::array<float, kNumPartials> gains{};
    for (int i = 0; i < kNumPartials; ++i) {
        float final_frame_nor_pos = pos_offset_curve_->Get(i) + frame_pos_;
        {
            float temp{};
            final_frame_nor_pos = std::modf(final_frame_nor_pos, &temp);
        }
        const auto& resynthsis_frames = synth_.GetResynthsisFrames().frames;
        size_t frame_idx = static_cast<size_t>((resynthsis_frames.size() - 0.01f) * final_frame_nor_pos);
        const auto& frame = resynthsis_frames[frame_idx];

        gains[i] = frame.gains[i];
    }

    if (formant_mix_ != float{}) {
        auto formant_gains = GetFormantGains(partials);
        std::ranges::transform(gains, formant_gains, gains.begin(),
                               [this](float no_shift_gain, float shift_gain) {
            return std::lerp(no_shift_gain, shift_gain, formant_mix_);
        });
    }

    auto osc_mix = 1.0f - gain_mix_;
    auto res_mix = gain_mix_;
    std::ranges::transform(partials.gains, gains, partials.gains.begin(),
                           [osc_mix, res_mix](float osc_gain, float res_gain) {
        return osc_mix * osc_gain + res_mix * res_gain;
    });
}

void Resynthesis::OnUpdateTick() {
    is_enable_ = is_enable_arg_->GetBool();

    formant_mix_ = helper::GetAlterParamValue(args_, param::Resynthsis_FormantMix{});
    formant_shift_ = helper::GetAlterParamValue(args_, param::Resynthsis_FormantShift{});
    freq_scale_ = helper::GetAlterParamValue(args_, param::Resynthsis_FreqScale{});
    frame_offset_ = helper::GetAlterParamValue(args_, param::Resynthsis_FrameOffset{});
    frame_speed_ = helper::GetAlterParamValue(args_, param::Resynthsis_FrameSpeed{});
    gain_mix_ = helper::GetAlterParamValue(args_, param::Resynthsis_GainMix{});

    if (!IsWork()) return;

    auto pos_inc = frame_speed_ * (float)skip_ / synth_.GetResynthsisFrames().frame_interval_sample / synth_.GetResynthsisFrames().frames.size();
    frame_player_pos_ += pos_inc;
    while (frame_player_pos_ > 1.0f) {
        frame_player_pos_ -= 1.0f;
    }
    while (frame_player_pos_ < 0.0f) {
        frame_player_pos_ += 1.0f;
    }

    frame_pos_ = frame_player_pos_ + frame_offset_;
    while (frame_pos_ > 1.0f) {
        frame_pos_ -= 1.0f;
    }
}

void Resynthesis::PrepareParams(OscillorParams & params) {
    formant_remap_curve_ = params.GetParentSynthParams().GetCurveBank().GetCurvePtr("resynthsis.formant_remap");
    pos_offset_curve_ = params.GetParentSynthParams().GetCurveBank().GetCurvePtr("resynthsis.pos_offset");

    is_enable_arg_ = params.GetParam<BoolParameter>("resynthsis.enable");
    is_formant_remap_ = params.GetParam<BoolParameter>("resynthsis.formant_remap");
    for (int i = 0; auto & parg : args_) {
        parg = params.GetPolyFloatParam("resynthsis.arg{}", i++);
    }
}

void Resynthesis::PreGetFreqDiffsInRatio(Partials& partials) {
    if (!is_enable_ || !synth_.IsResynthsisAvailable() || freq_scale_ == float{}) {
        partials.ratios.fill(0.0f);
        return;
    }

    using namespace std::views;

    for (int i = 0; i < kNumPartials; ++i) {
        float final_frame_nor_pos = pos_offset_curve_->Get(i) + frame_pos_;
        {
            float temp{};
            final_frame_nor_pos = std::modf(final_frame_nor_pos, &temp);
        }
        const auto& resynthsis_frames = synth_.GetResynthsisFrames().frames;
        size_t frame_idx = static_cast<size_t>((resynthsis_frames.size() - 0.01f) * final_frame_nor_pos);
        const auto& frame = resynthsis_frames[frame_idx];
        partials.ratios[i] = frame.ratio_diffs[i] * freq_scale_;
    }

    /*auto frame_idx = static_cast<size_t>((synth_.GetResynthsisFrames().frames.size() - 1) * frame_pos_);
    const auto& frame = synth_.GetResynthsisFrames().frames.at(frame_idx);
    std::ranges::transform(frame.ratio_diffs | take(kNumPartials), partials.ratios.begin(),
                           [this](float ratio_diff) {return ratio_diff * freq_scale_; });*/
}

bool Resynthesis::IsWork() const {
    return is_running_ && is_enable_ && synth_.IsResynthsisAvailable();
}

std::array<float, kNumPartials> Resynthesis::GetFormantGains(Partials& partials) const {
    auto formant_ratio = std::exp2(-formant_shift_ / 12.0f);
    std::array<float, kNumPartials> output{};

    const auto& resynthsis_datas = synth_.GetResynthsisFrames();
    if (!is_formant_remap_->GetBool()) { // disable remap
        for (int i = 0; i < kNumPartials; ++i) {
            float final_frame_nor_pos = pos_offset_curve_->Get(i) + frame_pos_;
            {
                float temp{};
                final_frame_nor_pos = std::modf(final_frame_nor_pos, &temp);
            }
            const auto& resynthsis_frames = synth_.GetResynthsisFrames().frames;
            size_t frame_idx = static_cast<size_t>((resynthsis_frames.size() - 0.01f) * final_frame_nor_pos);
            const auto& frame = resynthsis_frames[frame_idx];

            //auto idx = partials.freqs[i] * (kNumPartials)*formant_ratio - 1.0f;
            //auto int_idx = static_cast<int>(std::round(idx));

            //if (int_idx < 0 || int_idx >= kNumPartials) {
            //    output[i] = 0.0f;
            //}
            //else {
            //    output[i] = frame.gains[int_idx];
            //}
            auto remap_freq = partials.freqs[i] * 0.5f * sample_rate_ * formant_ratio;
            auto formant_max_freq = (kNumPartials + 1.0f) * resynthsis_datas.base_freq;
            auto formant_data_max_freq = kNumPartials * resynthsis_datas.base_freq;
            if (remap_freq > formant_max_freq || remap_freq < 0.0f) {
                output[i] = 0.0f;
            }
            else if (remap_freq < resynthsis_datas.base_freq) {
                auto frac = remap_freq / resynthsis_datas.base_freq;
                output[i] = frac * frame.gains.front();
            }
            else if (remap_freq > formant_data_max_freq) {
                auto frac = 1.0f - (remap_freq - formant_data_max_freq) / resynthsis_datas.base_freq;
                output[i] = frac * frame.gains.back();
            }
            else {
                auto index_k = remap_freq / resynthsis_datas.base_freq - 1.0f;
                auto left_idx = static_cast<int>(std::floor(index_k));
                auto right_idx = static_cast<int>(std::ceil(index_k)) & (kNumPartials - 1); // if idx euqal to 256 that is mirror to bin 0
                auto frac = index_k - left_idx;
                output[i] = std::lerp(frame.gains[left_idx], frame.gains[right_idx], frac);
            }
        }
    }
    else { // enable formant remap
        for (int i = 0; i < kNumPartials; ++i) {
            float final_frame_nor_pos = pos_offset_curve_->Get(i) + frame_pos_;
            {
                float temp{};
                final_frame_nor_pos = std::modf(final_frame_nor_pos, &temp);
            }
            const auto& resynthsis_frames = synth_.GetResynthsisFrames().frames;
            size_t frame_idx = static_cast<size_t>((resynthsis_frames.size() - 0.01f) * final_frame_nor_pos);
            const auto& frame = resynthsis_frames[frame_idx];

            /*auto idx = partials.freqs[i] * (kNumPartials)*formant_ratio - 1.0f;
            auto norm_idx = idx / static_cast<float>(kNumPartials);
            if (norm_idx < 0.0f || norm_idx > 1.0f) {
                output[i] = 0.0f;
                continue;
            }

            auto remap_norm_idx = formant_remap_curve_->GetNormalize(norm_idx);
            auto remap_idx = static_cast<int>(std::round(remap_norm_idx * kNumPartials));

            if (remap_idx < 0 || remap_idx >= kNumPartials) {
                output[i] = 0.0f;
            }
            else {
                output[i] = frame.gains[remap_idx];
            }*/

            // remap frequency
            auto org_freq = 0.5f * sample_rate_ * partials.freqs[i];
            auto remap_freq = org_freq;
            constexpr auto blend_min = 20.0f;
            constexpr auto blend_max = 20000.0f;
            if (org_freq >= blend_min && org_freq <= blend_max) {
                auto map_input_x = (1.0f / 3.0f) * std::log10(org_freq / blend_min);
                auto map_x = formant_remap_curve_->GetNormalize(map_input_x);
                remap_freq = blend_min * std::exp(std::numbers::ln10_v<float> *map_x * 3.0f);
            }

            // do formant shift
            auto formant_max_freq = (kNumPartials + 1.0f) * resynthsis_datas.base_freq;
            auto formant_data_max_freq = kNumPartials * resynthsis_datas.base_freq;
            if (remap_freq > formant_max_freq || remap_freq < 0.0f) {
                output[i] = 0.0f;
            }
            else if (remap_freq < resynthsis_datas.base_freq) {
                auto frac = remap_freq / resynthsis_datas.base_freq;
                output[i] = frac * frame.gains.front();
            }
            else if (remap_freq > formant_data_max_freq) {
                auto frac = 1.0f - (remap_freq - formant_data_max_freq) / resynthsis_datas.base_freq;
                output[i] = frac * frame.gains.back();
            }
            else {
                auto index_k = remap_freq / resynthsis_datas.base_freq - 1.0f;
                auto left_idx = static_cast<int>(std::floor(index_k));
                auto right_idx = static_cast<int>(std::ceil(index_k)) & (kNumPartials - 1); // if idx euqal to 256 that is mirror to bin 0
                auto frac = index_k - left_idx;
                output[i] = std::lerp(frame.gains[left_idx], frame.gains[right_idx], frac);
            }
        }
    }

    return output;
}

void Resynthesis::OnNoteOn(int /*note*/) {
    frame_player_pos_ = float{};
    is_running_ = true;
}
}