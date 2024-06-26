#pragma once

#include <array>
#include <future>
#include "engine/forward_decalre.h"
#include "ui/Knob.h"
#include "raygui-cpp.h"
#include "utli/spin_lock.h"
#include "raylib-cpp.hpp"
#include "ui/wrap_check_box.h"
#include "ui/WrapDropBox.h"

namespace mana {
class ResynthsisLayout {
public:
    ResynthsisLayout(Synth& synth);

    void Paint();
    void SetBounds(int x, int y, int w, int h);
private:
    void CheckAndDoResynthsis();
    void DrawSpectrum();

    Synth& synth_;
    WrapCheckBox is_enable_;
    WrapCheckBox is_formant_remap_;
    std::array<Knob, 7> arg_knobs_;
    std::atomic<int> resynthsis_work_counter_;
    rgc::Bounds bound_;
    utli::SpinLock ui_lock_;
    std::vector<std::vector<Color>> render_img_;
};
}