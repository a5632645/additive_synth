#include "effect.h"

#include "octaver.h"
#include "reverb.h"
#include "chrous.h"

namespace mana {
void Effect::Init(float sample_rate) {
    processers_[param::EffectType::EffectTypeEnum::kOctaver] = std::make_unique<Octaver>();
    processers_[param::EffectType::EffectTypeEnum::kReverb] = std::make_unique<Reverb>();
    processers_[param::EffectType::EffectTypeEnum::kChorus] = std::make_unique<Chorus>();

    for (auto&&[k, v] : processers_) {
        v->Init(sample_rate);
    }
    p_processor_ = processers_.begin()->second.get();
}

void Effect::Process(Partials& partials) {
    if (!is_enable_) return;
    p_processor_->Process(partials);
}

void Effect::OnUpdateTick(const SynthParam& params, int skip, int /*module_idx*/) {
    is_enable_ = params.effects[effect_idx_].is_enable;
    effect_type_ = param::IntChoiceParam<param::EffectType, param::EffectType::EffectTypeEnum>::GetEnum(
        params.effects[effect_idx_].effect_type);
    p_processor_ = processers_.at(effect_type_).get();

    p_processor_->OnUpdateTick(params, skip, effect_idx_);
}

void Effect::OnNoteOn(int note) {
    for (auto&&[k, v] : processers_) {
        v->OnNoteOn(note);
    }
}

void Effect::OnNoteOff() {
    for (auto&&[k, v] : processers_) {
        v->OnNoteOff();
    }
}
}