#include "move_instruction.h"

MoveInstruction::MoveInstruction(const int channel_number, const int value, Cue* cue) : m_channel_number(
        channel_number), m_target_value(value), m_cue(cue), m_previous_move(nullptr), m_next_move(nullptr) {
}

// Core data access

int MoveInstruction::get_target_value() const {
        return m_target_value;
}

int MoveInstruction::get_channel_number() const {
        return m_channel_number;
}

Cue* MoveInstruction::get_cue() const {
        return m_cue;
}

void MoveInstruction::set_target_value(const int value) {
        if (value < -1 || value > 100) {
                throw std::invalid_argument("Value must be between 0 and 100, or -1 to represent a null move.");
        }
        m_target_value = value;
}


int MoveInstruction::get_cue_number() const {
        return m_cue ? m_cue->get_cue_number() : -1;
}

// Timeline navigation
MoveInstruction* MoveInstruction::get_next_move() const {
        return m_next_move;
}

MoveInstruction* MoveInstruction::get_previous_move() const {
        return m_previous_move;
}

void MoveInstruction::set_next_move(MoveInstruction *next) {
        m_next_move = next;
}

void MoveInstruction::set_previous_move(MoveInstruction *prev) {
        m_previous_move = prev;
}


