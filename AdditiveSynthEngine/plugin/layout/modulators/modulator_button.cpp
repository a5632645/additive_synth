#include "modulator_button.h"

#include "layout/main_window.h"
#include "engine/synth.h"

class SimpleFlagComponent : public juce::Component {
public:
    SimpleFlagComponent(std::string_view id)
        : modulator_id_(id) {}

    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black);
        g.setColour(juce::Colours::white);
        g.drawRect(getLocalBounds());
    }

    void mouseDrag(const juce::MouseEvent& event) override {
        auto* drag_container = juce::DragAndDropContainer::findParentDragContainerFor(this);
        if (drag_container == nullptr) {
            DBG("no drag container");
            return;
        }

        if (drag_container->isDragAndDropActive()) {
            return;
        }

        drag_container->startDragging(juce::String{ modulator_id_.data(), modulator_id_.length() }, this);

        // highlight
        auto main_window = findParentComponentOfClass<mana::MainWindow>();
        if (main_window != nullptr) {
            main_window->BeginHighlightModulator(modulator_id_);
        }
    }

    void mouseUp(const juce::MouseEvent& event) override {
        auto main_window = findParentComponentOfClass<mana::MainWindow>();
        main_window->StopHighliteModulator();
    }
private:
    const std::string_view modulator_id_;
};

namespace mana {
ModulatorButton::ModulatorButton(std::string_view modulator_id, Synth& synth)
    : synth_(synth)
    , modulator_id_(modulator_id) {
    num_modulation_label_ = std::make_unique<juce::Label>("num_modulation_label", juce::String{ num_modulator_ });
    drag_component_ = std::make_unique<SimpleFlagComponent>(modulator_id);
    // todo: let label listen to modulator's change
    addAndMakeVisible(num_modulation_label_.get());
    addAndMakeVisible(drag_component_.get());

    setSize(20, 16);
    synth_.GetSynthParams().AddModulationListener(this);
}

ModulatorButton::~ModulatorButton() {
    synth_.GetSynthParams().RemoveModulationListener(this);
    num_modulation_label_ = nullptr;
    drag_component_ = nullptr;
}

void ModulatorButton::resized() {
    auto b = getLocalBounds();
    drag_component_->setBounds(b.removeFromRight(16));
    num_modulation_label_->setBounds(b);
}

void ModulatorButton::OnModulationAdded(std::shared_ptr<ModulationConfig> config) {
    if (config->modulator_id == modulator_id_) {
        num_modulator_++;
        num_modulation_label_->setText(juce::String{ num_modulator_ }, juce::NotificationType::sendNotification);
    }
}

void ModulatorButton::OnModulationRemoved(std::string_view modulator_id, std::string_view param_id) {
    if (modulator_id == modulator_id_) {
        num_modulator_--;
        num_modulation_label_->setText(juce::String{ num_modulator_ }, juce::NotificationType::sendNotification);
    }
}
}