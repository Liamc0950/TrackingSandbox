#ifndef CHANNEL_H
#define CHANNEL_H

#include <string>
#include "move_instruction.h"

class MoveInstruction;

class Channel {
public:
    Channel(int number, int home_value);

    int get_number() const;

    // Debug
    void print_move_instructions();

    // Move management
    void insert_move_in_timeline(MoveInstruction *new_move);

    void remove_move_from_timeline(MoveInstruction *move);

    // Value calculation
    int get_value_at_cue(int cue_number) const;

    int get_current_value() const;

    // Playback state
    void set_current_position_to_cue(int cue_number);

    void advance_to_next_move();

    void retreat_to_previous_move();

    // Timeline analysis
    MoveInstruction *get_next_change_after_cue(int cue_number) const;

    MoveInstruction *get_last_change_before_cue(int cue_number) const;

    bool has_move_at_cue(int cue_number) const;

    // Debug/info
    // void print_timeline() const;
    //
    // int get_move_count() const;

private:
    int m_number;
    int m_value;
    int m_home_value = 0;
    MoveInstruction* m_first_move;
    MoveInstruction* m_last_move;
    MoveInstruction* m_current_position;
};

#endif
