#include "resynthsis_layout.h"

#include <format>
#include <AudioFile.h>
#include "param/resynthsis_param.h"
#include "layout/gui_param_pack.h"
#include <future>

namespace mana {
ResynthsisLayout::ResynthsisLayout(Synth& synth)
    : synth_(synth) {
    for (int i = 0; auto & k : arg_knobs_) {
        k.set_parameter(synth.GetParamBank().GetParamPtr(std::format("resynthsis.arg{}", i++)));
    }

    SetGuiKnobs(arg_knobs_,
                param::Resynthsis_FormantMix{},
                param::Resynthsis_FormantShift{},
                param::Resynthsis_FrameOffset{},
                param::Resynthsis_FrameSpeed{},
                param::Resynthsis_FreqScale{},
                param::Resynthsis_GainMix{});

    is_enable_.SetText("resynthsis");
    is_enable_.SetChecked(synth.GetSynthParam().resynthsis.is_enable);
}

void ResynthsisLayout::Paint() {
    synth_.GetSynthParam().resynthsis.is_enable = is_enable_.Show();
    std::ranges::for_each(arg_knobs_, &Knob::display);

    DrawRectangle(bound_.GetX(), bound_.GetY(), bound_.GetWidth(), bound_.GetHeight(), RED);

    if (IsFileDropped()) {
        FilePathList files = ::LoadDroppedFiles();
        std::vector<std::string> output(files.paths, files.paths + files.count);
        ::UnloadDroppedFiles(files);

        auto audio_file = std::ranges::find_if(output, [](const std::string& file_name) {
            return file_name.find(".wav") != std::string::npos;
        });
        if (audio_file == output.end()) {
            return;
        }

        auto task = [file_path = *audio_file, work_id = ++resynthsis_work_counter_, this] {
            AudioFile<float> audio_file_;
            if (!audio_file_.load(file_path) || audio_file_.samples.empty()) {
                std::cerr << std::format(R"([resynthsis]: load file "{}" *failed*)", file_path) << std::endl;
                return;
            }

            auto frame = synth_.CreateResynthsisFrames(audio_file_.samples.at(0),
                                                       static_cast<float>(audio_file_.getSampleRate()));
            std::clog << std::format(R"([resynthsis]: load file "{}" *success*)", file_path) << std::endl;

            if (resynthsis_work_counter_.load() != work_id) {
                std::clog << std::format(R"([resynthsis]: file "{}" load *canceled*)", file_path) << std::endl;
                return;
            }

            std::scoped_lock lock{ synth_.GetSynthLock() };
            synth_.SetResynthsisFrames(std::move(frame));
        };
        std::thread{ std::move(task) }.detach();
    }
}

void ResynthsisLayout::SetBounds(int x, int y, int w, int h) {
    auto x_f = static_cast<float>(x);
    auto y_f = static_cast<float>(y);
    auto w_f = static_cast<float>(w);
    is_enable_.SetBounds(rgc::Bounds(x_f, y_f, 12.0f, 12.0f));
    bound_ = rgc::Bounds{ x_f,y_f + 50.0f + 12.0f,w_f,h - 50.0f - 12.0f };

    for (int i = 0; auto & k : arg_knobs_) {
        k.set_bound(x + 50 * i, y + 12, 50, 50);
        ++i;
    }
}
}