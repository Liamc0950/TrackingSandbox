#include <iostream>

#include "cue_list.h"
#include "console_interface.h"

int main() {
    std::cout << "Welcome to Tracking Sandbox" << "!\n";

    auto cue_list = new CueList(5);

    cue_list->update_channel(1, 100, CueList::UpdateMode::UPDATE_TRACK, 2);
    cue_list->update_channel(3, 50, CueList::UpdateMode::UPDATE_TRACK, 3);
    cue_list->update_channel(5, 50, CueList::UpdateMode::UPDATE_TRACK, 1);
    cue_list->update_channel(5, 50, CueList::UpdateMode::UPDATE_TRACK, 1);

    ConsoleInterface console(*cue_list);
    console.start();

    // cue_list->update_channel(3, 30, CueList::UpdateMode::UPDATE_TRACE, 5);

    cue_list->draw_cue_list();
    return 0;
}