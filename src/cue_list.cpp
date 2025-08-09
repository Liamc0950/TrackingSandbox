#include "cue_list.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

CueList::CueList(const unsigned short number_of_cues) {
    m_first_cue = nullptr;
    m_last_cue = nullptr;
    m_current_cue = nullptr;


    //By default, use incremental whole numbers and channels 1-5
    std::map<int, Channel> default_channels = {
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

void CueList::insert_cue(int after_cue_number, int new_cue_number, float fade_time) {
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
    // 1. Find the cue before deleting it
    Cue* cue_to_delete = find_cue(cue_number);
    if (!cue_to_delete) {
        return;  // Cue doesn't exist
    }

    // 2. Remove all move instructions from channel timelines - this assumes delete to track?
    for (const auto& move : cue_to_delete->get_moves()) {
        int channel_number = move->get_channel_number();
        m_channel_map.at(channel_number).remove_move_from_timeline(move.get());
    }

    // 3. Update linked list pointers
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

    // 4. Update current cue pointer if necessary
    if (m_current_cue == cue_to_delete) {
        m_current_cue = next_cue ? next_cue : prev_cue;
    }

    // 5. Remove from lookup map
    m_cue_lookup_map.erase(cue_number);

    // 6. Finally, remove from storage (this destroys the cue and its moves)
    m_cue_storage.erase(
        std::remove_if(m_cue_storage.begin(), m_cue_storage.end(),
            [cue_number](const Cue& cue) {
                return cue.get_cue_number() == cue_number;
            }),
        m_cue_storage.end()
    );
}

Cue* CueList::find_cue(int cue_number) {
    auto cue_entry = m_cue_lookup_map.find(cue_number);
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
        // Update
        MoveInstruction* move = cue->get_move_for_channel(channel_number);
        move->set_target_value(value);
    } else {
        // Add
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
            break;
        }

    }

    // Use your existing set_move_instruction
    if (target_cue) {
        set_move_instruction(target_cue->get_cue_number(), channel_number, new_value);
    }
}

// Playback control
void CueList::go_to_cue(int cue_number) {
    Cue* cue = find_cue(cue_number);
    if (cue) {
        m_current_cue = cue;
    }
}

void CueList::go_to_next_cue() {
    if (m_current_cue && m_current_cue->get_next_cue()) {
        m_current_cue = m_current_cue->get_next_cue();
    }
}

void CueList::go_to_previous_cue() {
    if (m_current_cue && m_current_cue->get_previous_cue()) {
        m_current_cue = m_current_cue->get_previous_cue();
    }
}

int CueList::get_current_cue_number() const {
    return m_current_cue->get_cue_number();
}

// Channel queries (uses timeline traversal)
int CueList::get_channel_value_at_cue(int channel_id, int cue_number) const {
    const Channel channel = m_channel_map.at(channel_id);

    return channel.get_value_at_cue(cue_number);

}

std::vector<int> CueList::get_active_channels_at_cue(int cue_number) {
    Cue* cue = find_cue(cue_number);

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
Channel& CueList::get_channel(int channel_id) {
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

void CueList::print_cue(Cue& cue) {
    int cue_num = cue.get_cue_number();

    std::cout << "|" << std::setw(8) << std::left << cue_num << "|"
              << std::setw(8) << std::left << get_channel_value_at_cue(1, cue_num) << "|"
              << std::setw(8) << std::left << get_channel_value_at_cue(2, cue_num) << "|"
              << std::setw(8) << std::left << get_channel_value_at_cue(3, cue_num) << "|"
              << std::setw(8) << std::left << get_channel_value_at_cue(4, cue_num) << "|"
              << std::setw(8) << std::left << get_channel_value_at_cue(5, cue_num) << "|" << std::endl;
}
