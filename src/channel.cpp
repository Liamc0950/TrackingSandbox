#include "channel.h"


Channel::Channel(unsigned short number, signed char value) : m_number(number), m_value(value) {
    m_number = number;
    m_color = "magenta"; //By default, treat value as tracking
}

void Channel::update_color(const MoveInstruction* previous_move, const MoveInstruction* current_move){
    if(current_move->get_cue()->is_blocked()){
        m_color = "white";
    }
    else if(previous_move->get_value() < current_move->get_value()){
        m_color = "blue";
    }
    else if(previous_move->get_value() > current_move->get_value()){
        m_color = "green";
    }
}

unsigned short Channel::get_number() const{
    return m_number;
}
signed char Channel::get_value() const{
    return m_value;
}
std::string Channel::get_color(){
    return m_color;
}

void Channel::set_value(const signed char value){
    if (value >= -1 && value <= 100){
        m_value = value;
    }
    else{
        throw std::invalid_argument("Channel value must be between 0 and 100, or -1 to represent a null value.");
    }
}
