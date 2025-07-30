#include <vector>

#include <channel.h>

class MoveInstruction
{
  private:
    const signed char NULL_MOVE_MARKER = -1;

	Channel* m_channel;
    signed char m_value;
    MoveInstruction* m_previous_move;
    MoveInstruction* m_next_move;
    bool m_is_block;

  public:
    MoveInstruction(Channel channel, signed char value);

    MoveInstruction* get_previous_move();
    MoveInstruction* get_next_move();
    unsigned char get_value();
    Channel get_channel();

};