#include <vector>

#include <channel.h>

class MoveInstruction
{
  public:
    MoveInstruction(Channel channel, signed char value){
        m_channel = channel;
        m_value = value
    }

    MoveInstruction* get_previous_move(){

        if this.previous_move = nullptr{
            MoveInstruction null_move = MoveInstruction(this.channel, NULL_MOVE_MARKER);

            return &null_move;

        }
        else{
            return m_previous_move;
        }
    }

    MoveInstruction* get_next_move(){

        if m_next_move = nullptr{
            MoveInstruction null_move = MoveInstruction(channel_to_move, NULL_MOVE_MARKER);

            return &null_move;

        }
        else{
            return m_next_move;
        }
    }

    unsigned char get_value(){
        return m_value;
    }
    Channel get_channel(){
        return m_channel;
    }

};