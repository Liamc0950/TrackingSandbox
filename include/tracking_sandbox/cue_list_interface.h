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
	void play(int num);

    void BuildCueList();
    void UpdateCueOnly(Cue cue_to_update, std::vector<MoveInstructions>)
    void UpdateTrack(Cue cue_to_update, std::vector<MoveInstructions>)
    void UpdateTrace(Cue cue_to_update, std::vector<MoveInstructions>)
    void UpdateTraceTrace(Cue cue_to_update, std::vector<MoveInstructions>)

};