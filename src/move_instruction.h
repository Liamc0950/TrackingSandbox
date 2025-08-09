#ifndef MOVE_INSTRUCTION_H // Include guard to prevent multiple inclusions
#define MOVE_INSTRUCTION_H

#include "channel.h"
#include "cue.h"

class Cue; //Forward Declaration

class MoveInstruction
{
    public:
        MoveInstruction(Channel channel, signed char value, Cue* cue);

        void insert_move_next(MoveInstruction* move);
        void insert_move_previous(MoveInstruction* move);

        [[nodiscard]] MoveInstruction* get_previous_move() const;
        MoveInstruction* get_next_move();
        [[nodiscard]] unsigned char get_value() const;
        [[nodiscard]] Channel* get_channel() const;
        [[nodiscard]] Cue* get_cue() const;
        void set_cue(Cue* cue);
    private:
        const signed char NULL_MOVE_MARKER = -1;

        Channel* m_channel;
        signed char m_value;
        MoveInstruction* m_previous_move;
        MoveInstruction* m_next_move;
        Cue* m_cue;
        bool m_is_block;

};
#endif
