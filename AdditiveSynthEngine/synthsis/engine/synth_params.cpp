#include "synth_params.h"

#include "param/standard_param.h"
#include "param/timber_param.h"
#include "param/dissonance_param.h"
#include "param/effect_param.h"
#include "param/envelop_param.h"
#include "param/lfo_param.h"
#include "param/filter_param.h"
#include "param/resynthsis_param.h"
#include "param/unison_param.h"

namespace mana {
static constexpr ParamRange kUnitRange{ 0.0f,1.0f };
}

namespace mana {
SynthParams::SynthParams(std::shared_ptr<ParamCreator> creator) {
    using enum ModulationType;

    //param_bank_.AddParameter(param::PitchBend::CreateParam(kPoly, "pitch_bend"));
    param_bank_.AddParameter(creator->CreateFloatParameter({
        .type = kPoly,
        .id = "pitch_bend",
        .name = "pitch_bend",
        .vmin = param::PitchBend::kMin,
        .vmax = param::PitchBend::kMax,
        .vdefault = param::PitchBend::kDefault }));

    //param_bank_.AddParameter(param::OutputGain::CreateParam(kDisable, "output_gain"));
    param_bank_.AddParameter(creator->CreateFloatParameter({
        .type = kDisable,
        .id = "output_gain",
        .name = "output_gain",
        .vmin = param::OutputGain::kMin,
        .vmax = param::OutputGain::kMax,
        .vdefault = param::OutputGain::kDefault }));

    //param_bank_.AddParameter(param::PhaseType::CreateParam(kDisable, "phase.type"));
    param_bank_.AddParameter(creator->CreateIntChoiceParameter({
        .id = "phase.type",
        .name = "phase.type",
        .choices = {param::PhaseType::kNames.begin(), param::PhaseType::kNames.end()},
        .vdefault = 0 }));
    for (int arg_idx = 0; arg_idx < 2; ++arg_idx) {
        //param_bank_.AddOrCreateIfNull(kPoly, kUnitRange, std::format("arg{}", arg_idx), "phase.arg{}", arg_idx);
        param_bank_.AddParameter(creator->CreateFloatParameter({
            .type = kPoly,
            .id = std::format("phase.arg{}", arg_idx),
            .name = std::format("phase.arg{}", arg_idx),
            .vmin = 0.0f,
            .vmax = 1.0f,
            .vdefault = 0.0f }));
    }

    for (int osc_idx = 0; osc_idx < 2; ++osc_idx) {
        //param_bank_.AddParameter(param::TimberType::CreateParam(kDisable, "timber.osc{}.type", osc_idx));
        param_bank_.AddParameter(creator->CreateIntChoiceParameter({
            .id = std::format("timber.osc{}.type", osc_idx),
            .name = std::format("timber.osc{}.type", osc_idx),
            .choices = {param::TimberType::kNames.begin(), param::TimberType::kNames.end()},
            .vdefault = 0 }));
        for (int arg_idx = 0; arg_idx < 4; ++arg_idx) {
            //param_bank_.AddParameter(CreateAlterParam(kPoly, "arg{1}", "timber.osc{}.arg{}", osc_idx, arg_idx));
            param_bank_.AddParameter(creator->CreateFloatParameter({
                .type = kPoly,
                .id = std::format("timber.osc{}.arg{}", osc_idx, arg_idx),
                .name = std::format("osc{}.arg{}", osc_idx, arg_idx),
                .vmin = 0.0f,
                .vmax = 1.0f,
                .vdefault = 0.0f }));
        }
    }
    //param_bank_.AddParameter(param::Timber_Osc2Shift::CreateParam(kPoly, "timber.osc2_shift"));
    param_bank_.AddParameter(creator->CreateFloatParameter({
        .type = kPoly,
        .id = "timber.osc2_shift",
        .name = "osc2_shift",
        .vmin = param::Timber_Osc2Shift::kMin,
        .vmax = param::Timber_Osc2Shift::kMax,
        .vdefault = param::Timber_Osc2Shift::kDefault }));
    //param_bank_.AddParameter(param::Timber_Osc2Beating::CreateParam(kPoly, "timber.osc2_beating"));
    param_bank_.AddParameter(creator->CreateFloatParameter({
        .type = kPoly,
        .id = "timber.osc2_beating",
        .name = "osc2_beating",
        .vmin = param::Timber_Osc2Beating::kMin,
        .vmax = param::Timber_Osc2Beating::kMax,
        .vdefault = param::Timber_Osc2Beating::kDefault }));
    //param_bank_.AddParameter(param::Timber_OscGain::CreateParam(kPoly, "timber.osc1_gain"));
    param_bank_.AddParameter(creator->CreateFloatParameter({
        .type = kPoly,
        .id = "timber.osc1_gain",
        .name = "osc1_gain",
        .vmin = param::Timber_OscGain::kMin,
        .vmax = param::Timber_OscGain::kMax,
        .vdefault = param::Timber_OscGain::kDefault }));
    //param_bank_.AddParameter(param::Timber_OscGain::CreateParam(kPoly, "timber.osc2_gain"));
    param_bank_.AddParameter(creator->CreateFloatParameter({
        .type = kPoly,
        .id = "timber.osc2_gain",
        .name = "osc2_gain",
        .vmin = param::Timber_OscGain::kMin,
        .vmax = param::Timber_OscGain::kMax,
        .vdefault = param::Timber_OscGain::kDefault }));
    //param_bank_.AddParameter(param::Timber_OscGain::CreateParam(kPoly, "timber.osc2_gain"));

    //param_bank_.AddParameter(param::Unison_Type::CreateParam(kDisable, "unison.type"));
    param_bank_.AddParameter(creator->CreateIntChoiceParameter({
        .id = "unison.type",
        .name = "unison.type",
        .choices = {param::Unison_Type::kNames.begin(), param::Unison_Type::kNames.end()},
        .vdefault = 0 }));
    //param_bank_.AddParameter(param::Unison_NumVoice::CreateParam(kDisable, "unison.num_voice"));
    param_bank_.AddParameter(creator->CreateIntParameter({
        .id = "unison.num_voice",
        .name = "unison.num_voice",
        .vmin = param::Unison_NumVoice::kMin,
        .vmax = param::Unison_NumVoice::kMax,
        .vdefault = param::Unison_NumVoice::kDefault }));
    //param_bank_.AddParameter(param::Unison_Pitch::CreateParam(kPoly, "unison.pitch"));
    param_bank_.AddParameter(creator->CreateFloatParameter({
        .type = kPoly,
        .id = "unison.pitch",
        .name = "unison.pitch",
        .vmin = param::Unison_Pitch::kMin,
        .vmax = param::Unison_Pitch::kMax,
        .vdefault = param::Unison_Pitch::kDefault }));
    //param_bank_.AddParameter(param::Unison_Phase::CreateParam(kPoly, "unison.phase"));
    param_bank_.AddParameter(creator->CreateFloatParameter({
        .type = kPoly,
        .id = "unison.phase",
        .name = "unison.phase",
        .vmin = param::Unison_Phase::kMin,
        .vmax = param::Unison_Phase::kMax,
        .vdefault = param::Unison_Phase::kDefault }));
    //param_bank_.AddParameter(param::Unison_Pan::CreateParam(kPoly, "unison.pan"));
    param_bank_.AddParameter(creator->CreateFloatParameter({
        .type = kPoly,
        .id = "unison.pan",
        .name = "unison.pan",
        .vmin = param::Unison_Pan::kMin,
        .vmax = param::Unison_Pan::kMax,
        .vdefault = param::Unison_Pan::kDefault }));

    //param_bank_.AddOrCreateIfNull<BoolParameter>(kDisable, kUnitRange, "enable", "dissonance.enable");
    param_bank_.AddParameter(creator->CreateBoolParameter({
        .id = "dissonance.enable",
        .name = "dissonance.enable",
        .vdefault = false }));
    //param_bank_.AddParameter(param::DissonanceType::CreateParam(kDisable, "dissonance.type"));
    param_bank_.AddParameter(creator->CreateIntChoiceParameter({
        .id = "dissonance.type",
        .name = "dissonance.type",
        .choices = {param::DissonanceType::kNames.begin(), param::DissonanceType::kNames.end()},
        .vdefault = 0 }));
    for (int arg_idx = 0; arg_idx < 2; ++arg_idx) {
        //param_bank_.AddParameter(CreateAlterParam(kPoly, "arg{}", "dissonance.arg{}", arg_idx));
        param_bank_.AddParameter(creator->CreateFloatParameter({
            .type = kPoly,
            .id = std::format("dissonance.arg{}", arg_idx),
            .name = std::format("dissonance.arg{}", arg_idx),
            .vmin = 0.0f,
            .vmax = 1.0f,
            .vdefault = 0.0f }));
    }

    //param_bank_.AddOrCreateIfNull<BoolParameter>(kDisable, kUnitRange, "filter_stream", "filter.stream_type");
    param_bank_.AddParameter(creator->CreateBoolParameter({
        .id = "filter.stream_type",
        .name = "filter.stream_type",
        .vdefault = false }));
    for (int filter_idx = 0; filter_idx < 2; ++filter_idx) {
        //param_bank_.AddOrCreateIfNull<BoolParameter>(kDisable, kUnitRange, std::format("filter{} enable", filter_idx), "filter{}.enable", filter_idx);
        param_bank_.AddParameter(creator->CreateBoolParameter({
            .id = std::format("filter{}.enable", filter_idx),
            .name = std::format("filter{}.enable", filter_idx),
            .vdefault = false }));
        //param_bank_.AddParameter(param::Filter_Type::CreateParam(kDisable, "filter{}.type", filter_idx));
        param_bank_.AddParameter(creator->CreateIntChoiceParameter({
            .id = std::format("filter{}.type", filter_idx),
            .name = std::format("filter{}.type", filter_idx),
            .choices = {param::Filter_Type::kNames.begin(), param::Filter_Type::kNames.end()},
            .vdefault = 0 }));
        for (int arg_idx = 0; arg_idx < 8; ++arg_idx) {
            //param_bank_.AddParameter(CreateAlterParam(kPoly, "arg{1}", "filter{}.arg{}", filter_idx, arg_idx));
            param_bank_.AddParameter(creator->CreateFloatParameter({
                .type = kPoly,
                .id = std::format("filter{}.arg{}", filter_idx, arg_idx),
                .name = std::format("filter{}.arg{}", filter_idx, arg_idx),
                .vmin = 0.0f,
                .vmax = 1.0f,
                .vdefault = 0.0f }));
        }
    }

    // resynthsis
    //param_bank_.AddOrCreateIfNull<BoolParameter>(kDisable, kUnitRange, "", "resynthsis.enable");
    param_bank_.AddParameter(creator->CreateBoolParameter({
        .id = "resynthsis.enable",
        .name = "resynthsis.enable",
        .vdefault = false }));
    //param_bank_.AddOrCreateIfNull<BoolParameter>(kDisable, kUnitRange, "", "resynthsis.formant_remap");
    param_bank_.AddParameter(creator->CreateBoolParameter({
        .id = "resynthsis.formant_remap",
        .name = "resynthsis.formant_remap",
        .vdefault = false }));
    for (int arg_idx = 0; arg_idx < 7; ++arg_idx) {
        //param_bank_.AddOrCreateIfNull(kPoly, kUnitRange, "", "resynthsis.arg{}", arg_idx);
        param_bank_.AddParameter(creator->CreateFloatParameter({
            .type = kPoly,
            .id = std::format("resynthsis.arg{}", arg_idx),
            .name = std::format("resynthsis.arg{}", arg_idx),
            .vmin = 0.0f,
            .vmax = 1.0f,
            .vdefault = 0.0f }));
    }

    // effect
    for (int effect_idx = 0; effect_idx < 5; ++effect_idx) {
        //param_bank_.AddOrCreateIfNull<BoolParameter>(kDisable, kUnitRange, "", "effect{}.enable", effect_idx);
        param_bank_.AddParameter(creator->CreateBoolParameter({
            .id = std::format("effect{}.enable", effect_idx),
            .name = std::format("effect{}.enable", effect_idx),
            .vdefault = false }));
        //param_bank_.AddParameter(param::EffectType::CreateParam(kDisable, "effect{}.type", effect_idx));
        param_bank_.AddParameter(creator->CreateIntChoiceParameter({
            .id = std::format("effect{}.type", effect_idx),
            .name = std::format("effect{}.type", effect_idx),
            .choices = {param::EffectType::kNames.begin(), param::EffectType::kNames.end()},
            .vdefault = 0 }));
        for (int arg_idx = 0; arg_idx < 6; ++arg_idx) {
            //param_bank_.AddOrCreateIfNull(kPoly, kUnitRange, std::format("arg{}", arg_idx), "effect{}.arg{}", effect_idx, arg_idx);
            param_bank_.AddParameter(creator->CreateFloatParameter({
                .type = kPoly,
                .id = std::format("effect{}.arg{}", effect_idx, arg_idx),
                .name = std::format("effect{}.arg{}", effect_idx, arg_idx),
                .vmin = 0.0f,
                .vmax = 1.0f,
                .vdefault = 0.0f }));
        }
    }

    // lfo
    for (int lfo_idx = 0; lfo_idx < 8; ++lfo_idx) {
        //param_bank_.AddOrCreateIfNull<BoolParameter>(kDisable, kUnitRange, "", "lfo{}.restart", lfo_idx);
        param_bank_.AddParameter(creator->CreateBoolParameter({
            .id = std::format("lfo{}.restart", lfo_idx),
            .name = std::format("lfo{}.restart", lfo_idx),
            .vdefault = false }));
        //param_bank_.AddParameter(param::LFO_Rate::CreateParam(kPoly, "lfo{}.rate", lfo_idx));
        param_bank_.AddParameter(creator->CreateFloatParameter({
            .type = kPoly,
            .id = std::format("lfo{}.rate", lfo_idx),
            .name = std::format("lfo{}.rate", lfo_idx),
            .vmin = 0.0f,
            .vmax = 1.0f,
            .vdefault = 0.0f }));
        //param_bank_.AddParameter(param::LFO_Phase::CreateParam(kPoly, "lfo{}.start_phase", lfo_idx));
        param_bank_.AddParameter(creator->CreateFloatParameter({
            .type = kPoly,
            .id = std::format("lfo{}.start_phase", lfo_idx),
            .name = std::format("lfo{}.start_phase", lfo_idx),
            .vmin = 0.0f,
            .vmax = 1.0f,
            .vdefault = 0.0f }));
        //param_bank_.AddParameter(param::LFO_Level::CreateParam(kPoly, "lfo{}.level", lfo_idx));
        param_bank_.AddParameter(creator->CreateFloatParameter({
            .type = kPoly,
            .id = std::format("lfo{}.level", lfo_idx),
            .name = std::format("lfo{}.level", lfo_idx),
            .vmin = 0.0f,
            .vmax = 1.0f,
            .vdefault = 0.0f }));
        //param_bank_.AddParameter(param::LFO_WaveType::CreateParam(kPoly, "lfo{}.wave_type", lfo_idx));
        param_bank_.AddParameter(creator->CreateIntChoiceParameter({
            .id = std::format("lfo{}.wave_type", lfo_idx),
            .name = std::format("lfo{}.wave_type", lfo_idx),
            .choices = {param::LFO_WaveType::kNames.begin(), param::LFO_WaveType::kNames.end()},
            .vdefault = 0 }));
    };

    // envelop
    for (int env_idx = 0; env_idx < 8; ++env_idx) {
        //param_bank_.AddParameter(param::Env_Attack::CreateParam(kPoly, "envelop{}.attack", env_idx));
        param_bank_.AddParameter(creator->CreateFloatParameter({
            .type = kPoly,
            .id = std::format("envelop{}.attack", env_idx),
            .name = std::format("envelop{}.attack", env_idx),
            .vmin = 0.0f,
            .vmax = 1.0f,
            .vdefault = 0.0f }));
        //param_bank_.AddParameter(param::Env_Decay::CreateParam(kPoly, "envelop{}.decay", env_idx));
        param_bank_.AddParameter(creator->CreateFloatParameter({
            .type = kPoly,
            .id = std::format("envelop{}.decay", env_idx),
            .name = std::format("envelop{}.decay", env_idx),
            .vmin = 0.0f,
            .vmax = 1.0f,
            .vdefault = 0.0f }));
        //param_bank_.AddParameter(param::Env_Sustain::CreateParam(kPoly, "envelop{}.sustain", env_idx));
        param_bank_.AddParameter(creator->CreateFloatParameter({
            .type = kPoly,
            .id = std::format("envelop{}.sustain", env_idx),
            .name = std::format("envelop{}.sustain", env_idx),
            .vmin = 0.0f,
            .vmax = 1.0f,
            .vdefault = 0.0f }));
        //param_bank_.AddParameter(param::Env_Release::CreateParam(kPoly, "envelop{}.release", env_idx));
        param_bank_.AddParameter(creator->CreateFloatParameter({
            .type = kPoly,
            .id = std::format("envelop{}.release", env_idx),
            .name = std::format("envelop{}.release", env_idx),
            .vmin = 0.0f,
            .vmax = 1.0f,
            .vdefault = 0.0f }));
    };

    // custom curves
    curve_manager_.AddCurve(kNumPartials, "resynthsis.formant_remap")
        .AddCurve(kNumPartials, "resynthsis.pos_offset")
        .AddCurve(kNumPartials, "effect.harmonic_delay.time")
        .AddCurve(kNumPartials, "effect.harmonic_delay.feedback")
        .AddCurve({ 0.0f,1.0f / 11.0f,2.0f / 11.0f,3.0f / 11.0f,4.0f / 11.0f,5.0f / 11.0f,6.0f / 11.0f,7.0f / 11.0f,8.0f / 11.0f,9.0f / 11.0f,10.0f / 11.0f,11.0f / 11.0f }, "dissonance.pitch_quantize");
    for (int i = 0; i < 8; ++i) {
        curve_manager_.AddCurve(kNumPartials, "lfo{}.wave", i);
    }
}
}