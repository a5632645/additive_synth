﻿add_executable (AdditiveSynthEngine
"AdditiveSynthEngine.cpp"
"engine/Oscillor.h"
"engine/Oscillor.cpp"
"engine/synth.cpp"




"engine/standard/freq.h"
"engine/standard/freq.cpp"

"engine/modulation/Parameter.h"

"engine/modulation/ParamBank.h"

"engine/modulation/Modulator.h"

"engine/modulation/Modulation.h"


"param/timber_param.h"
"engine/timber/timber.cpp"
"engine/timber/dual_saw.h"
"engine/timber/dual_saw.cpp"
"engine/timber/sync.h"
"engine/timber/sync.cpp"

"param/param.h"
"engine/oscillor_param.h"

"param/standard_param.h"
"param/dissonance_param.h"



"ui/WrapDropBox.h"
"ui/WrapDropBox.cpp"
#"ui/WrapGroupBox.h"
#"ui/WrapGroupBox.cpp"
"ui/Knob.cpp"



#"layout/SynthLayout.h"
#"layout/SynthLayout.cpp"
"layout/synth/dissonance_layout.h"
"layout/synth/dissonance_layout.cpp"
"layout/synth/timber_layout.h"
"layout/synth/timber_layout.cpp"

"ui/oscilloscope.h"
"ui/oscilloscope.cpp"
"engine/filter/filter.h"
"engine/filter/filter.cpp"
"engine/dissonance/dissonance.h"
"engine/dissonance/dissonance.cpp"


 "layout/synth/filter_layout.h"
 "layout/synth/filter_layout.cpp"
 "engine/effect/effect.h"
 "engine/effect/effect.cpp"
 "param/effect_param.h"
 "engine/effect/octaver.h"
 "layout/effect/sp_effect_layout.h"
 "layout/effect/sp_effect_layout.cpp"
 "engine/effect/reverb.h"
 "engine/effect/chrous.h"
 "engine/resynthsis/resynthsis.h"
 "engine/resynthsis/resynthsis.cpp"
 "engine/resynthsis/resynthsis_data.h"
 "engine/resynthsis/window.h"
 "layout/gui_param_pack.h"
 "engine/effect/phaser.h"
 "engine/effect/scramble.h"
 "param/resynthsis_param.h"
 "layout/resynthsis/resynthsis_layout.h"
 "layout/resynthsis/resynthsis_layout.cpp"
 "utli/NoiseGenerator.h"
 "layout/standard_layout.h"
 "layout/standard_layout.cpp"
  "utli/spin_lock.h"
  "utli/convert.h"
  "engine/standard/phase.h"
  "engine/standard/phase.cpp"
  "utli/warp.h"
  "engine/timber/timber_frame.h"
  "engine/timber/timber_gen.h"
  "engine/timber/timber_gen.cpp"
  "layout/synth/osc_layout.h"
  "layout/synth/osc_layout.cpp"
  
  "engine/effect/decay.h"
  "engine/effect/harmonic_delay.h"
  "engine/modulation/curve.h"
  "ui/curve_editor.h"
  "ui/curve_editor.cpp"
  "layout/curve_layout.h"
  "layout/curve_layout.cpp"
  "layout/main_window.h"
  "layout/main_window.cpp"
  "ui/warp_text_input.h"
  "ui/warp_text_input.cpp"
 "ui/wrap_check_box.h"
 "ui/wrap_check_box.cpp"
 "engine/modulation/lfo.h"
 "engine/modulation/lfo.cpp"
 "param/lfo_param.h"
 "engine/modulation/modulator_bank.h"
 "engine/modulation/modulator_bank.cpp"
 #"layout/modulator_layout.h"
 #"layout/modulator_layout.cpp"
 "layout/modulators/lfo_layout.h"
 "layout/modulators/lfo_layout.cpp"
 "layout/matrix/modulation_matrix_layout.h"
 "layout/matrix/modulation_matrix_layout.cpp"
 "engine/oscillor_param.cpp"
 "engine/synth_params.h"
 "engine/synth_params.cpp"
 "engine/poly_param.h"
 "engine/effect/effect_base.h"
 "engine/timber/osc_param.h"
 "engine/timber/unison.h"
 "engine/timber/unison.cpp"
 "layout/synth/unison_layout.h"
 "layout/synth/unison_layout.cpp"
 "ui/raygui.impl.cpp"
 "engine/effect/spectral_gate.h"
 "engine/forward_decalre.h"
 "engine/timber/noise.h"
 "engine/timber/noise.cpp"
 "engine/timber/my_pwm.h"
 "engine/sine_bank.cpp"
 "engine/timber/impluse.h"
 "engine/filter/formant_filter.h"
 "engine/filter/formant_filter.cpp"
 "engine/modulation/envelop.h"
 "engine/modulation/envelop.cpp"
 "param/envelop_param.h"
 "layout/modulators/envelop_layout.h"
 "layout/modulators/envelop_layout.cpp"
 "engine/modulation/param_range.h"
 "param/unison_param.h"
 "layout/synth/synth_layout.h"
 "layout/synth/synth_layout.cpp"
 "layout/synth/phase_layout.h"
 "layout/synth/phase_layout.cpp"
 "layout/synth/filter_route_layout.h"
 "layout/synth/filter_route_layout.cpp"
 "layout/synth/final_filter_layout.h"
 "layout/synth/final_filter_layout.cpp"
 "layout/modulators/modulators_layout.h"
 "layout/modulators/modulators_layout.cpp"
 "layout/modulators/layout_base.h"
 "layout/effect/final_fx_layout.h"
 "layout/effect/final_fx_layout.cpp"
 "layout/about/about_layout.h"
 "layout/about/about_layout.cpp" "layout/master_layout.h" "layout/master_layout.cpp" "ui/spectrum.h" "ui/spectrum.cpp" "engine/filter/filter_wrap.h" "engine/filter/filter_wrap.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AdditiveSynthEngine PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(AdditiveSynthEngine PUBLIC raylib)
target_link_libraries(AdditiveSynthEngine PUBLIC raylib_cpp)
target_link_libraries(AdditiveSynthEngine PUBLIC raygui_cpp)
target_link_libraries(AdditiveSynthEngine PUBLIC AudioFFT)
target_link_libraries(AdditiveSynthEngine PUBLIC AudioFile)
target_link_libraries(AdditiveSynthEngine PUBLIC gcem)
target_link_libraries(AdditiveSynthEngine PUBLIC xsimd)
target_link_libraries(AdditiveSynthEngine PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(AdditiveSynthEngine PRIVATE rtmidi)
#target_link_libraries(AdditiveSynthEngine PRIVATE raygui)
target_include_directories(AdditiveSynthEngine PRIVATE .)
