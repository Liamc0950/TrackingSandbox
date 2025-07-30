#include <string>
#include <move_instruction.h>

class Channel
{
  private:
    unsigned short m_number;
    signed char m_value;
    std::string m_color;

  public:
    Channel(unsigned short number, signed char value);

    void update_color(MoveInstruction* previous_move, MoveInstruction* current_move);

    unsigned short get_number();
    signed char get_value();
    std::string get_color();


};