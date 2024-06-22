#include "master_layout.h"

#include "gui_param_pack.h"
#include "param/standard_param.h"
#include <algorithm>
#include "engine/synth.h"

namespace mana {
MasterLayout::MasterLayout(Synth& synth)
    : synth_(synth)
    //, wave_scope_(synth)
    , spectrum_(synth) {
    const auto& bank = synth.GetParamBank();
    pitch_bend_ = std::make_unique<WrapSlider>(bank.GetParamPtr("pitch_bend"));
    output_gain_ = std::make_unique<WrapSlider>(bank.GetParamPtr("output_gain"));

    addAndMakeVisible(pitch_bend_.get());
    addAndMakeVisible(output_gain_.get());
    //addAndMakeVisible(wave_scope_.GetWidget());
    addAndMakeVisible(spectrum_);

    info_label_ = std::make_unique<juce::Label>();
    addAndMakeVisible(info_label_.get());
}

void MasterLayout::resized() {
    auto b = getLocalBounds();
    pitch_bend_->setBounds(b.removeFromLeft(50));
    output_gain_->setBounds(b.removeFromLeft(50));
    //wave_scope_.SetBounds(x + 50, y, w, h / 2);
    info_label_->setBounds(b.removeFromRight(200));
    spectrum_.setBounds(b);
}
}