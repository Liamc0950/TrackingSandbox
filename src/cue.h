#ifndef CUE_H
#define CUE_H

#include <map>
#include <vector>
#include <list>
#include "move_instruction.h"
#include "channel.h"
#include "cue_list.h"


class Channel;
class MoveInstruction;
class CueList;

class Cue
{

  public:
    explicit Cue(int number);
    
    void process_channel_values(const std::map<int, Channel>& channels);

    Channel get_channel_in_cue(Channel channel, Cue cue);

    void UpdateCueOnly(std::vector<MoveInstruction*>);
    void UpdateTrack(CueList* cue_list, unsigned short channel_number, signed char new_value);
    void UpdateTrace(std::vector<MoveInstruction*>);
    void UpdateTraceTrace(std::vector<MoveInstruction*>);

    unsigned short get_number();

    Cue* get_next_cue() const;
    Cue* get_previous_cue() const;

    bool is_blocked() const;

    void set_next_cue(Cue* cue);
    void set_previous_cue(Cue* cue);


    std::list<MoveInstruction*> get_move_instructions();

    private:
        unsigned short m_number;
        std::list<MoveInstruction*> m_move_instructions;

        Cue *m_previous_cue;
        Cue *m_next_cue;

        bool m_is_block;
};

#endif
