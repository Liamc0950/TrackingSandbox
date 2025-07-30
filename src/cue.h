#include <vector>
#include <move_instruction.h>

class Cue
{
  private:
	std::vector<MoveInstruction*>* m_move_instructions;

  public:
	Cue(int cue_number);

    void process_channel_values();

    void UpdateCueOnly(std::vector<MoveInstruction>);
    void UpdateTrack(std::vector<MoveInstruction>);
    void UpdateTrace(std::vector<MoveInstruction>);
    void UpdateTraceTrace(std::vector<MoveInstruction>);

    std::vector<MoveInstruction> get_move_instructions();

};