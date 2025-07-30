#include <string>
#include <move_instruction.h>

class Channel
{
  public:
    Channel(unsigned short number, signed char value){
        m_number = number;
        m_value = value;
        m_color = "magenta"; //By default, treat value as tracking
    }

    void update_color(MoveInstruction* previous_move, MoveInstruction* current_move){
        if(*current_move.get_cue().is_blocked()){
            m_color = "white"
        }
        else if(*previous_move.get_value() < *current_move.get_value()){
            m_color = "blue";
        }
        else if(*previous_move.get_value() > *current_move.get_value()){
            m_color = "green";
        }
        return
    }

    unsigned short get_number(){
        return m_number;
    }
    signed char get_value(){
        return m_value;
    }
    std::string get_color(){
        return m_color;
    }


};