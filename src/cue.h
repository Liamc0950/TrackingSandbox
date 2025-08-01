#include <vector>
#include <move_instruction.h>

class Cue
{
  private:
    unsigned short m_number;
	  std::vector<MoveInstruction*>* m_move_instructions;

  public:
	  Cue(int number);
    
    void process_channel_values();

    void UpdateCueOnly(std::vector<MoveInstruction>);
    void UpdateTrack(std::vector<MoveInstruction>);
    void UpdateTrace(std::vector<MoveInstruction>);
    void UpdateTraceTrace(std::vector<MoveInstruction>);

    unsigned short get_number();

    std::vector<MoveInstruction> get_move_instructions();



};