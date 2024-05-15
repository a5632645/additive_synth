#pragma once

#include <array>
#include "ui/WrapDropBox.h"
#include "ui/Knob.h"
#include "engine/forward_decalre.h"

namespace mana {
class FilterLayout {
public:
    FilterLayout(Synth& synth);

    void Paint();
    void SetBounds(int x, int y, int w, int h);
private:
    void OnFilterTypeChanged(int c);
    WrapDropBox filter_type_;
    std::array<Knob, 6> arg_knobs_;
};
}
