#ifndef MOVE_INSTRUCTION_H // Include guard to prevent multiple inclusions
#define MOVE_INSTRUCTION_H

#include "channel.h"
#include "cue.h"

class Cue; //Forward Declaration

class MoveInstruction {
public:

    MoveInstruction(int channel_number, int target_value, Cue* cue);

    // Core data access
    [[nodiscard]] int get_target_value() const;

    [[nodiscard]] int get_channel_number() const;

    [[nodiscard]] Cue *get_cue() const;

    void set_target_value(int value);

    [[nodiscard]] int get_cue_number() const; // Helper: parent_cue->get_number()

    // Timeline navigation
    [[nodiscard]] MoveInstruction *get_next_move() const;

    [[nodiscard]] MoveInstruction *get_previous_move() const;

    void set_next_move(MoveInstruction *next);

    void set_previous_move(MoveInstruction *prev);

    //Need to identify a "Null" move - home state of a channel. Should not just be 0.
    static constexpr signed char NULL_MOVE_MARKER = -1;

private:

    int m_channel_number;
    int m_target_value;
    Cue *m_cue;

    //Doubly linked list of moves for given channel
    MoveInstruction *m_previous_move;
    MoveInstruction *m_next_move;
};
#endif
