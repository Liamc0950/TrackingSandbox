#ifndef CHANNEL_H
#define CHANNEL_H

#include <string>
#include "move_instruction.h"

class MoveInstruction;

class Channel
{
    public:
        Channel(unsigned short number, signed char value);

        void update_color(const MoveInstruction* previous_move, const MoveInstruction* current_move);

        unsigned short get_number() const;
        signed char get_value() const;
        std::string get_color();

        void set_value(signed char value);

    private:
        unsigned short m_number;
        signed char m_value;
        std::string m_color;

};

#endif