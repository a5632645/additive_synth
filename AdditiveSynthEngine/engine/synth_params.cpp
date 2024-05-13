#include "synth_params.h"

namespace mana {
SynthParams::SynthParams() {
    using enum ModulationType;

    param_bank_.AddOrCreateIfNull(kPoly, "pitch_bend");
    param_bank_.AddOrCreateIfNull(kMono, "output_gain");

    param_bank_.AddOrCreateIfNull<IntParameter>(kDisable, "phase.type");
    for (int arg_idx = 0; arg_idx < 2; ++arg_idx) {
        param_bank_.AddOrCreateIfNull(kDisable, "phase.arg{}", arg_idx);
    }

    for (int osc_idx = 0; osc_idx < 2; ++osc_idx) {
        param_bank_.AddOrCreateIfNull<IntParameter>(kDisable, "timber.osc{}.type", osc_idx);
        for (int arg_idx = 0; arg_idx < 4; ++arg_idx) {
            param_bank_.AddOrCreateIfNull(kPoly, "timber.osc{}.arg{}", osc_idx, arg_idx);
        }
    }
    param_bank_.AddOrCreateIfNull(kPoly, "timber.osc2_shift");
    param_bank_.AddOrCreateIfNull(kPoly, "timber.osc2_beating");
    param_bank_.AddOrCreateIfNull(kPoly, "timber.osc1_gain");
    param_bank_.AddOrCreateIfNull(kPoly, "timber.osc2_gain");

    param_bank_.AddOrCreateIfNull<IntParameter>(kDisable, "unison.type");
    param_bank_.AddOrCreateIfNull<IntParameter>(kDisable, "unison.num_voice");
    param_bank_.AddOrCreateIfNull(kPoly, "unison.pitch");
    param_bank_.AddOrCreateIfNull(kPoly, "unison.phase");
    param_bank_.AddOrCreateIfNull(kPoly, "unison.pan");

    param_bank_.AddOrCreateIfNull<BoolParameter>(kDisable, "dissonance.enable");
    param_bank_.AddOrCreateIfNull<IntParameter>(kDisable, "dissonance.type");
    for (int arg_idx = 0; arg_idx < 2; ++arg_idx) {
        param_bank_.AddOrCreateIfNull(kPoly, "dissonance.arg{}", arg_idx);
    }

    param_bank_.AddOrCreateIfNull<BoolParameter>(kDisable, "filter.enable");
    param_bank_.AddOrCreateIfNull<IntParameter>(kDisable, "filter.type");
    for (int arg_idx = 0; arg_idx < 6; ++arg_idx) {
        param_bank_.AddOrCreateIfNull(kPoly, "filter.arg{}", arg_idx);
    }

    param_bank_.AddOrCreateIfNull<BoolParameter>(kDisable, "resynthsis.enable");
    for (int arg_idx = 0; arg_idx < 7; ++arg_idx) {
        param_bank_.AddOrCreateIfNull(kPoly, "resynthsis.arg{}", arg_idx);
    }

    for (int effect_idx = 0; effect_idx < 5; ++effect_idx) {
        param_bank_.AddOrCreateIfNull<BoolParameter>(kDisable, "effect{}.enable", effect_idx);
        param_bank_.AddOrCreateIfNull<IntParameter>(kDisable, "effect{}.type", effect_idx);
        for (int arg_idx = 0; arg_idx < 6; ++arg_idx) {
            param_bank_.AddOrCreateIfNull(kPoly, "effect{}.arg{}", effect_idx, arg_idx);
        }
    }

    for (int lfo_idx = 0; lfo_idx < 8; ++lfo_idx) {
        param_bank_.AddOrCreateIfNull(kPoly, "lfo{}.rate", lfo_idx);
        param_bank_.AddOrCreateIfNull(kPoly, "lfo{}.start_phase", lfo_idx);
        param_bank_.AddOrCreateIfNull(kPoly, "lfo{}.level", lfo_idx);
        param_bank_.AddOrCreateIfNull<BoolParameter>(kDisable, "lfo{}.restart", lfo_idx);
        param_bank_.AddOrCreateIfNull<IntParameter>(kDisable, "lfo{}.wave_type", lfo_idx);
        param_bank_.AddOrCreateIfNull<IntParameter>(kDisable, "lfo{}.wave_curve_idx", lfo_idx);
    };
}
}