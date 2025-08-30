#include "cue_list.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

CueList::CueList(const unsigned short number_of_cues) {
    m_first_cue = nullptr;
    m_last_cue = nullptr;
    m_current_cue = nullptr;


    //By default, use incremental whole numbers and channels 1-5
    const std::map<int, Channel> default_channels = {
        {1, Channel(1, -1)},
        {2, Channel(2, -1)},
        {3, Channel(3, -1)},
        {4, Channel(4, -1)},
        {5, Channel(5, -1)}
    };
    m_channel_map = default_channels;

    for (int i = 1; i <= number_of_cues; i++) {
        this->append_cue(i);
    }
}
// Cue management
void CueList::append_cue(int cue_number, float fade_time) {
    // Create cue directly in storage
    m_cue_storage.emplace_back(cue_number);

    // Get stable pointer to the object in storage
    Cue* cue_ptr = &m_cue_storage.back();

    // Add to lookup map
    m_cue_lookup_map[cue_number] = cue_ptr;

    // Update linked list pointers
    if (m_last_cue) {
        m_last_cue->set_next_cue(cue_ptr);
        cue_ptr->set_previous_cue(m_last_cue);
    } else {
        m_first_cue = cue_ptr;  // First cue
    }
    m_last_cue = cue_ptr;
}

void CueList::insert_cue(const int after_cue_number, int new_cue_number, float fade_time) {
    Cue *after_cue = find_cue(after_cue_number);

    if (!after_cue) {
        std::cerr << "Cue " << after_cue_number << " not found" << std::endl;
        return;
    }

    // Create new cue (storage order irrelevant)
    m_cue_storage.emplace_back(new_cue_number);
    Cue *new_cue = &m_cue_storage.back();
    m_cue_lookup_map[new_cue_number] = new_cue;

    // Fix logical ordering via linked list
    Cue *next_cue = after_cue->get_next_cue();

    after_cue->set_next_cue(new_cue);
    new_cue->set_previous_cue(after_cue);
    new_cue->set_next_cue(next_cue);

    if (next_cue) {
        next_cue->set_previous_cue(new_cue);
    } else {
        m_last_cue = new_cue;
    }
}

void CueList::delete_cue(int cue_number) {
    // Find the cue before deleting it
    const Cue* cue_to_delete = find_cue(cue_number);
    if (!cue_to_delete) {
        return;  // Cue doesn't exist
    }

    // Remove all move instructions from channel timelines - this assumes delete to track?
    for (const auto& move : cue_to_delete->get_moves()) {
        int channel_number = move->get_channel_number();
        m_channel_map.at(channel_number).remove_move_from_timeline(move.get());
    }

    // Update linked list pointers
    Cue* prev_cue = cue_to_delete->get_previous_cue();
    Cue* next_cue = cue_to_delete->get_next_cue();

    if (prev_cue) {
        prev_cue->set_next_cue(next_cue);
    } else {
        m_first_cue = next_cue;  // Was first cue
    }

    if (next_cue) {
        next_cue->set_previous_cue(prev_cue);
    } else {
        m_last_cue = prev_cue;  // Was last cue
    }

    // Update current cue pointer if necessary
    if (m_current_cue == cue_to_delete) {
        m_current_cue = next_cue ? next_cue : prev_cue;
    }

    // Remove from lookup map
    m_cue_lookup_map.erase(cue_number);

    // remove from storage
    m_cue_storage.erase(
        std::ranges::remove_if(m_cue_storage,
                               [cue_number](const Cue& cue) {
                                   return cue.get_cue_number() == cue_number;
                               }).begin(),
        m_cue_storage.end()
    );
}

Cue* CueList::find_cue(const int cue_number) {
    const auto cue_entry = m_cue_lookup_map.find(cue_number);
    return (cue_entry != m_cue_lookup_map.end()) ? cue_entry->second : nullptr;
}

// move instruction operations
void CueList::set_move_instruction(int cue_number, int channel_number, int value) {
    // Validation
    Cue* cue = find_cue(cue_number);
    if (!cue) {
        throw std::invalid_argument("Cue " + std::to_string(cue_number) + " not found");
    }

    if (!m_channel_map.contains(channel_number)) {
        throw std::invalid_argument("Channel " + std::to_string(channel_number) + " not found");
    }

    // Check if move already exists
    if (cue->has_move_for_channel(channel_number)) {
        // Update existing move
        MoveInstruction* move = cue->get_move_for_channel(channel_number);

        // Remove from timeline first
        m_channel_map.at(channel_number).remove_move_from_timeline(move);

        // Update the value
        move->set_target_value(value);

        // Re-insert into timeline with new value
        m_channel_map.at(channel_number).insert_move_in_timeline(move);

    } else {
        // Add new move
        MoveInstruction* move = cue->add_move_instruction(channel_number, value);
        m_channel_map.at(channel_number).insert_move_in_timeline(move);
    }
}
void CueList::remove_move_instruction(int cue_number, int channel_number) {
    // Validation
    Cue* cue = find_cue(cue_number);
    if (!cue) {
        throw std::invalid_argument("Cue " + std::to_string(cue_number) + " not found");
    }

    if (!m_channel_map.contains(channel_number)) {
        throw std::invalid_argument("Channel " + std::to_string(channel_number) + " not found");
    }

    // Check if move exists
    if (cue->has_move_for_channel(channel_number)) {
        // Delete
        m_channel_map.find(channel_number)->second.remove_move_from_timeline(cue->get_move_for_channel(channel_number));
        cue->remove_move_instruction(channel_number);
    }
}
void CueList::update_channel(const int channel_number, const int new_value, const UpdateMode mode, const int target_cue_number) {

    // Validate channel exists
    if (!m_channel_map.contains(channel_number)) {
        throw std::invalid_argument("Channel " + std::to_string(channel_number) + " not found");
    }

    Cue* target_cue = nullptr;

    switch (mode) {
        case UpdateMode::UPDATE_TRACE: {
            // Find where this channel was last modified
            target_cue = m_channel_map.at(channel_number).get_last_change_before_cue(target_cue_number)->get_cue();
            if (!target_cue) {
                throw std::invalid_argument("Channel " + std::to_string(channel_number) +
                                          " has no moves to update");
            }
            break;
        }
        case UpdateMode::UPDATE_TRACE_TRACE: {

            //Get last move
            //If value in cue before is 0 or null:
                //get move where it moves to 0 or null
                //If we are going back to a previous move, we need to delete the first move we found
            //update
            // Find where this channel was last modified

            MoveInstruction* last_move = m_channel_map.at(channel_number).get_last_change_before_cue(target_cue_number);

            //If last move exists, target cue is that move's cue, otherwise it's the first cue in the list
            if (last_move) {
                target_cue = last_move->get_cue();
                Cue* previous_cue = target_cue->get_previous_cue();
                if (previous_cue) {
                    int value_previous_cue = get_channel_value_at_cue(channel_number, previous_cue->get_number());

                    //If the value of the channel in the previous cue is 0 or null
                    if (value_previous_cue == 0 || value_previous_cue == -1) {

                        MoveInstruction* previous_move = last_move->get_previous_move();

                        //As we are "moving" this move back to earlier in the cue list, we want to remove the existing move
                        last_move->get_cue()->remove_move_instruction(channel_number);
                        m_channel_map.at(channel_number).remove_move_from_timeline(last_move);

                        if (previous_move) {
                            target_cue = previous_move->get_cue();
                        }
                        else {
                            target_cue = m_first_cue;
                        }
                    }

                }
            }
            else {
                target_cue = m_first_cue;
            }

            break;
        }

        case UpdateMode::UPDATE_TRACK: {
            if (target_cue_number == -1) {
                throw std::invalid_argument("Must specify target cue number");
            }
            target_cue = find_cue(target_cue_number);
            if (!target_cue) {
                throw std::invalid_argument("Target cue " + std::to_string(target_cue_number) +
                                          " not found");
            }
            break;
        }
        case UpdateMode::UPDATE_CUE_ONLY: {
            if (target_cue_number == -1) {
                throw std::invalid_argument("Must specify target cue number");
            }
            target_cue = find_cue(target_cue_number);
            if (!target_cue) {
                throw std::invalid_argument("Target cue " + std::to_string(target_cue_number) +
                                          " not found");
            }

            // Get the old value before any changes
            int old_channel_value = get_channel_value_at_cue(channel_number, target_cue_number);

            // Set the new value in the target cue
            set_move_instruction(target_cue->get_cue_number(), channel_number, new_value);

            // If there's a next cue, directly set the old value there
            Cue* next_target_cue = target_cue->get_next_cue();

            if (next_target_cue) {
                set_move_instruction(next_target_cue->get_cue_number(), channel_number, old_channel_value);
            }
            return;
        }
    }

    //Perform update
    if (target_cue) {
        set_move_instruction(target_cue->get_cue_number(), channel_number, new_value);
    }
    else {
        throw std::out_of_range("No cue found");
    }

}


// Channel queries (uses timeline traversal)
int CueList::get_channel_value_at_cue(int channel_id, int cue_number) const {
    const Channel channel = m_channel_map.at(channel_id);

    return channel.get_value_at_cue(cue_number);

}

std::string CueList::get_channel_color_at_cue(const int channel_id, const int cue_number) const {
    const Channel channel = m_channel_map.at(channel_id);
    const Cue* last_cue = m_cue_lookup_map.at(cue_number)->get_previous_cue();

    int last_cue_value = -1;

    if (last_cue) {
        last_cue_value = channel.get_value_at_cue(last_cue->get_cue_number());
    }
    else {
        last_cue_value = -1;
    }

    const int new_cue_value = channel.get_value_at_cue(cue_number);

    if (last_cue_value < new_cue_value) {
        return "blue";
    }else if (last_cue_value > new_cue_value) {
        return "green";
    }else if (last_cue_value == new_cue_value) {
        return "magenta";
    }
    return "white";

}

std::vector<int> CueList::get_active_channels_at_cue(const int cue_number) {
    const Cue* cue = find_cue(cue_number);

    std::vector<int> channel_numbers;

    for (const auto& move : cue->get_moves()) {
        channel_numbers.push_back(move->get_channel_number());
    }

    return channel_numbers;
}
Cue* CueList::find_last_cue_with_channel(const int channel_number) const {
    // Walk backwards from current cue to find last move for this channel
    if (!m_current_cue) return nullptr;

    Cue* current = m_current_cue;
    while (current) {
        if (current->has_move_for_channel(channel_number)) {
            return current;  // Found the source cue
        }
        current = current->get_previous_cue();
    }
    return nullptr;  // Channel never moves
}


// Channel access
Channel& CueList::get_channel(const int channel_id) {
    return m_channel_map.at(channel_id);
}

const std::map<int, Channel>& CueList::get_all_channels() const {
    return m_channel_map;
}

int CueList::count_cues() const {
    int count = 0;
    const Cue* current_cue = m_first_cue;

    while (current_cue != nullptr) {
        count++;
        current_cue = current_cue->get_next_cue();
    }

    return count;
}



void CueList::draw_cue_list(){
    std::cout << std::string(100, '=') << "\n";
    print_header();
    Cue* current = m_first_cue;
    while (current != nullptr) {
        print_cue(*current);
        current = current->get_next_cue();
    }
}

void CueList::print_header() {
    std::cout << "|" << std::setw(8) << std::left << "CUE #" << "|"
              << std::setw(8) << std::left << "CHAN 1" << "|"
              << std::setw(8) << std::left << "CHAN 2" << "|"
              << std::setw(8) << std::left << "CHAN 3" << "|"
              << std::setw(8) << std::left << "CHAN 4" << "|"
              << std::setw(8) << std::left << "CHAN 5" << "|" << std::endl;

    // Add separator line
    std::cout << std::string(61, '-') << std::endl;
}

void CueList::print_cue(const Cue& cue) const {
    const int cue_num = cue.get_cue_number();

    std::cout << "|" << std::setw(8) << std::left << cue_num << "|";
    print_channel_value(cue_num, 1);
    print_channel_value(cue_num, 2);
    print_channel_value(cue_num, 3);
    print_channel_value(cue_num, 4);
    print_channel_value(cue_num, 5);
    std::cout << std::endl;

}

void CueList::print_channel_value(const int cue_number, const int channel_number) const {
    const int channel_value = get_channel_value_at_cue(channel_number, cue_number);
    const std::string channel_color_name = get_channel_color_at_cue(channel_number, cue_number);

    std::string color_ansi_value;

    if (channel_color_name.empty()) {
        color_ansi_value = "\033[0m";
    }
    else if (channel_color_name == "green") {
        color_ansi_value = "\033[32m";
    }
    else if (channel_color_name == "blue") {
        color_ansi_value = "\033[34m";
    }
    else if (channel_color_name == "magenta") {
        color_ansi_value = "\033[35m";
    }
    else if (channel_color_name == "white") {
        color_ansi_value = "\033[0m";
    }

    std::cout << std::setw(8) << std::left << color_ansi_value << channel_value << "\033[0m" << "|";
}