//
// Created by Liam Corley on 8/27/25.
//
#include "cue_list.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(cue_list_module) {
    enum_<CueList::UpdateMode>("UpdateMode")
        .value("UPDATE_TRACE", CueList::UpdateMode::UPDATE_TRACE)
        .value("UPDATE_TRACE_TRACE", CueList::UpdateMode::UPDATE_TRACE_TRACE)
        .value("UPDATE_TRACK", CueList::UpdateMode::UPDATE_TRACK)
        .value("UPDATE_CUE_ONLY", CueList::UpdateMode::UPDATE_CUE_ONLY);

    class_<CueList>("CueList")
        .constructor<unsigned short>()
        .function("updateChannel", &CueList::update_channel)
        .function("getChannelValueAtCue", &CueList::get_channel_value_at_cue)
        .function("getChannelColorAtCue", &CueList::get_channel_color_at_cue);
}
#endif
