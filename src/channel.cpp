#include "channel.h"

#include <iostream>
#include <ostream>


Channel::Channel(const int number, const int value) : m_number(number),
                                                      m_value(value),
                                                      m_home_value(value),
                                                      m_first_move(nullptr),
                                                      m_last_move(nullptr),
                                                      m_current_position(nullptr) {
}

void Channel::print_move_instructions() {
    MoveInstruction* current_move = m_first_move;
    while (current_move != nullptr) {
        std::cout << "Move at Cue: " << current_move->get_cue_number() << " to Value: " << current_move->get_target_value() << std::endl;
        current_move = current_move->get_next_move();
    }

}

void Channel::insert_move_in_timeline(MoveInstruction *new_move) {
    int target_cue = new_move->get_cue_number();

    // Empty timeline
    if (m_first_move == nullptr) {
        m_first_move = m_last_move = new_move;
        return;
    }

    // Insert at beginning
    if (target_cue < m_first_move->get_cue_number()) {
        new_move->set_next_move(m_first_move);
        m_first_move->set_previous_move(new_move);
        m_first_move = new_move;
        return;
    }

    // Insert at end
    if (target_cue > m_last_move->get_cue_number()) {
        m_last_move->set_next_move(new_move);
        new_move->set_previous_move(m_last_move);
        m_last_move = new_move;
        return;
    }

    // Insert in middle - find position
    MoveInstruction *current = m_first_move;
    //Iterate through moves until we find the matching cue number
    while (current->get_next_move() && current->get_next_move()->get_cue_number() < target_cue) {
        current = current->get_next_move();
    }

    // Insert after current
    new_move->set_next_move(current->get_next_move());
    new_move->set_previous_move(current);

    //If there's a next move, update it's previous_move to the inserted move
    if (current->get_next_move()) {
        current->get_next_move()->set_previous_move(new_move);
    }
    //Update the next_move of the move we're inserting after
    current->set_next_move(new_move);
}

void Channel::remove_move_from_timeline(MoveInstruction *move) {
    // Update linked list pointers
    if (move->get_previous_move()) {
        move->get_previous_move()->set_next_move(move->get_next_move());
    } else {
        m_first_move = move->get_next_move(); // Was first move
    }

    if (move->get_next_move()) {
        move->get_next_move()->set_previous_move(move->get_previous_move());
    } else {
        m_last_move = move->get_previous_move(); // Was last move
    }
}


int Channel::get_number() const {
    return m_number;
}

int Channel::get_value_at_cue(int cue_number) const {
    MoveInstruction *current = m_first_move;
    MoveInstruction *last_valid = nullptr;

    // Walk timeline until we pass target cue
    while (current && current->get_cue_number() <= cue_number) {
        last_valid = current;
        current = current->get_next_move();
    }

    // Return last move value, or home if no moves
    return last_valid ? last_valid->get_target_value() : m_home_value;
}

int Channel::get_current_value() const {
    if (m_current_position != nullptr) {
        return m_current_position->get_target_value();
    }
    return MoveInstruction::NULL_MOVE_MARKER;
}

void Channel::set_current_position_to_cue(const int cue_number) {
    MoveInstruction* current = m_first_move;
    MoveInstruction* last_valid = nullptr;

    while (current && current->get_cue_number() <= cue_number) {
        last_valid = current;
        current = current->get_next_move();
    }

    m_current_position = last_valid;
}

void Channel::advance_to_next_move() {
    if (m_current_position && m_current_position->get_next_move()) {
        m_current_position = m_current_position->get_next_move();
    }
}

void Channel::retreat_to_previous_move() {
    if (m_current_position && m_current_position->get_previous_move()) {
        m_current_position = m_current_position->get_previous_move();
    }
}

MoveInstruction *Channel::get_next_change_after_cue(int cue_number) const {
    MoveInstruction *current = m_first_move;
    MoveInstruction *last_valid = nullptr;

    // Walk timeline until we pass target cue
    while (current && current->get_cue_number() <= cue_number) {
        last_valid = current;
        current = current->get_next_move();
    }

    // Return next move
    return (last_valid && last_valid->get_next_move()) ? last_valid->get_next_move() : nullptr;
}

MoveInstruction *Channel::get_last_change_before_cue(int cue_number) const {
    MoveInstruction *current = m_first_move;
    MoveInstruction *last_valid = nullptr;

    // Walk timeline until we pass target cue
    while (current && current->get_cue_number() < cue_number) {
        last_valid = current;
        current = current->get_next_move();
    }

    // Return next move
    return last_valid;
}

bool Channel::has_move_at_cue(int cue_number) const {
    MoveInstruction *current = m_first_move;

    // Walk timeline until we find a move at the cue_number
    while (current && current->get_cue_number() <= cue_number) {
        if (current->get_cue_number() == cue_number) {
            return true;
        }
        current = current->get_next_move();
    }

    return false;
}
