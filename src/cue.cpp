
#include "cue.h"

#include "cue_list.h"


Cue::Cue(int number)
    : m_number(number)
    , m_move_instructions(std::list<MoveInstruction*>())
    , m_previous_cue(nullptr)
    , m_next_cue(nullptr)
    , m_is_block(false) {
}


void Cue::process_channel_values(const std::map<int, Channel>& channels) {
    for (MoveInstruction* move: m_move_instructions) {
        Channel moving_channel = *move->get_channel();
        //If channel to move is in the active channels map
        if (channels.find(moving_channel.get_number()) != channels.end()) {
            //Move the channel
            moving_channel.set_value(move->get_value());
            //Update channel color
            moving_channel.update_color(move->get_previous_move(), move);
        }
    }
}

Channel Cue::get_channel_in_cue(Channel channel, Cue cue) {
    Channel result_channel = channel;
    if (cue.get_previous_cue() != nullptr) {
        Channel result_channel = get_channel_in_cue(channel, *cue.get_previous_cue());
    }
    for (MoveInstruction* move: m_move_instructions) {
        Channel moving_channel = *move->get_channel();
        //If channel to move is in the active channels map
        if (result_channel.get_number() == moving_channel.get_number()) {
            //Move the channel
            result_channel.set_value(move->get_value());
            //Update channel color
            result_channel.update_color(move->get_previous_move(), move);
            return result_channel;
        }
    }
    return result_channel;

}


// void Cue::UpdateCueOnly(std::vector<MoveInstruction*) {
//     // for (MoveInstruction &move: m_move_instructions) {
//     //     // save current value as a move instruction
//     //     Channel channel_to_move = move.get_channel();
//     //     Cue next_cue = this.get_next_cue()
//     //     MoveInstruction current_state_move = new MoveInstruction(&channel_to_move, channel_to_move.get_value(),
//     //                                                              &next_cue);
//     //     // make a new move instruction for the new value, add to this cue
//     //     MoveInstruction updated_state_move = move;
//     //     move.insert_move_next(&current_state_move);
//     //     next_cue.add_move_instruction(&current_state_move);
//     //     this.add_move_instruction(&move);
//     //     // the old value move instruction we created should become that channel's next move instruction in the next cue
//     // }
// }

void Cue::UpdateTrack(CueList* cue_list, unsigned short channel_number, signed char new_value) {
    Channel channel_to_update = cue_list->get_channels().at(channel_number);

    MoveInstruction new_move = MoveInstruction(channel_to_update, new_value, this);

    m_move_instructions.push_back(&new_move);
}

void Cue::UpdateTrace(std::vector<MoveInstruction*>){}

void Cue::UpdateTraceTrace(std::vector<MoveInstruction*>){}

unsigned short Cue::get_number() {
    return m_number;
}

std::list<MoveInstruction*> Cue::get_move_instructions() {
    return m_move_instructions;
}

Cue* Cue::get_next_cue() const {
    return m_next_cue;
}

Cue* Cue::get_previous_cue() const {
    return m_previous_cue;
}

bool Cue::is_blocked() const {
    return m_is_block;
}

void Cue::set_next_cue(Cue* cue) {
    m_next_cue = cue;
}

void Cue::set_previous_cue(Cue* cue) {
    m_previous_cue = cue;
}
