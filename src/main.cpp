#include <iostream>

#include "cue_list.h"

int main() {
    std::cout << "Welcome to Tracking Sandbox" << "!\n";

    auto cue_list = new CueList(5);

    Cue cue_to_update = *cue_list->get_cue_lookup_map().at(2);
    cue_to_update.UpdateTrack(cue_list, 2, 100);

    std::cout << "There are " << cue_list->get_cue_count() << " cues." << "\n";
    cue_list->draw_cue_list();
    return 0;
}