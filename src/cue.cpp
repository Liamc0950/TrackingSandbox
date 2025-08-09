
#include "cue.h"

#include "cue_list.h"
#include <algorithm>

Cue::Cue(int number, float fade_time)
    : m_number(number)
    , m_fade_time(fade_time)
    , m_previous_cue(nullptr)
    , m_next_cue(nullptr)
    , m_is_block(false) {
}

int Cue::get_cue_number() const {
    return m_number;
}

// Move instruction management
MoveInstruction* Cue::add_move_instruction(int channel_number, int value) {
    // Check if this channel already has a move in this cue
    for (const auto& existing_move : m_move_instructions) {
        if (existing_move->get_channel_number() == channel_number) {
            // Update existing move instead of creating duplicate
            existing_move->set_target_value(value);
            return existing_move.get();
        }
    }

    // No existing move found - create new one
    auto new_move = std::make_unique<MoveInstruction>(channel_number, value, this);
    MoveInstruction* raw_ptr = new_move.get();  // Get raw pointer before moving

    // Add to cue's moves
    m_move_instructions.push_back(std::move(new_move));

    return raw_ptr;  // Return raw pointer for channel

}
void Cue::remove_move_instruction(int channel_number) {
    m_move_instructions.erase(
        std::remove_if(m_move_instructions.begin(), m_move_instructions.end(),
            [channel_number](const std::unique_ptr<MoveInstruction>& move) {
                return move->get_channel_number() == channel_number;
            }),
        m_move_instructions.end()
    );
}
MoveInstruction* Cue::get_move_for_channel(int channel_number) const {
    for (const auto& move : m_move_instructions) {
        if (move->get_channel_number() == channel_number) {
            return move.get();
        }
    }
    return nullptr;
}
bool Cue::has_move_for_channel(int channel_number) const {
    for (const auto& move : m_move_instructions) {
        if (move->get_channel_number() == channel_number) {
            return true;
        }
    }
    return false;
}

// Access moves
const std::vector<std::unique_ptr<MoveInstruction>>& Cue::get_moves() const {
    return m_move_instructions;
}
