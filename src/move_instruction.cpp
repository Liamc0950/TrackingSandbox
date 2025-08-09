#include "move_instruction.h"

MoveInstruction::MoveInstruction(Channel channel, signed char value, Cue* cue) : m_channel(&channel), m_value(value), m_cue(cue) {
    m_channel = &channel;
    m_value = value;
    m_cue = cue;
}

void MoveInstruction::insert_move_next(MoveInstruction* move) {
    m_next_move = move;
}

void MoveInstruction::insert_move_previous(MoveInstruction* move) {
    m_previous_move = move;
}

MoveInstruction* MoveInstruction::get_previous_move() const {
    if (this->m_previous_move == nullptr) {
        auto* null_move = new MoveInstruction(*this->get_channel(), NULL_MOVE_MARKER, this->get_cue());
        return null_move;
    }
    return m_previous_move;
}

MoveInstruction* MoveInstruction::get_next_move() {
    // if (m_next_move == nullptr) {
    //     MoveInstruction null_move = MoveInstruction(channel_to_move, NULL_MOVE_MARKER);
    //
    //     return &null_move;
    // } else {
    //     return m_next_move;
    // }
}

unsigned char MoveInstruction::get_value() const {
    return m_value;
}

Channel* MoveInstruction::get_channel() const {
    return m_channel;
}
Cue* MoveInstruction::get_cue() const {
    return m_cue;
}
void MoveInstruction::set_cue(Cue* cue) {
    m_cue = cue;
}
