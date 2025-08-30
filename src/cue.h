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

class Cue {
public:
    explicit Cue(int number, float fade_time=3.0f);

    [[nodiscard]] int get_cue_number() const;

    // Move instruction management
    MoveInstruction* add_move_instruction(int channel_number, int value);
    void remove_move_instruction(int channel_number);
    [[nodiscard]] MoveInstruction* get_move_for_channel(int channel_number) const;
    [[nodiscard]] bool has_move_for_channel(int channel_id) const;

    // Properties
    [[nodiscard]] int get_number() const { return m_number; }
    [[nodiscard]] float get_fade_time() const { return m_fade_time; }
    void set_fade_time(const float time) { m_fade_time = time; }

    // Navigation
    [[nodiscard]] Cue* get_next_cue() const { return m_next_cue; }
    [[nodiscard]] Cue* get_previous_cue() const { return m_previous_cue; }
    void set_next_cue(Cue* next) { m_next_cue = next;}
    void set_previous_cue(Cue* prev) { m_previous_cue = prev; }

    // Access moves
    [[nodiscard]] const std::vector<std::unique_ptr<MoveInstruction>>& get_moves() const;

private:
    int m_number;

    std::vector<std::unique_ptr<MoveInstruction> > m_move_instructions;

    Cue *m_previous_cue;
    Cue *m_next_cue;

    float m_fade_time;

    bool m_is_block;
};

#endif