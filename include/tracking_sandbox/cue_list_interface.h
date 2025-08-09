#pragma once

#include <vector>

#include <cue.h>
#include <move_instruction.h>
#include <cue_list.h>

class CueListInterface
{
  private:
	  CueList *cue_list;

  public:
	  CueListInterface(int number_of_cues);

    void ProcessCueList();
    void UpdateCueOnly(Cue cue_to_update, std::vector<MoveInstruction>);
    void UpdateTrack(Cue cue_to_update, std::vector<MoveInstruction>);
    void UpdateTrace(Cue cue_to_update, std::vector<MoveInstruction>);
    void UpdateTraceTrace(Cue cue_to_update, std::vector<MoveInstruction>);

};