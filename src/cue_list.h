#ifndef CUE_LIST_H
#define CUE_LIST_H

#include <map>

#include "cue.h"
#include "channel.h"
#include <list>

class Cue;
class Channel;

class CueList {
public:
    explicit CueList(unsigned short number_of_cues);

    enum class UpdateMode {
        UPDATE_TRACE,      // Update original cue where channel was last set
        UPDATE_TRACK,      // Update in current cue (override)
        UPDATE_CUE_ONLY,   // Update cue only
        UPDATE_TRACE_TRACE,// Update as far back as possible
    };

    void append_cue(int cue_number, float fade_time = 3.0f);

    void insert_cue(int after_cue_number, int new_cue_number, float fade_time = 3.0f);

    void delete_cue(int cue_number);

    Cue *find_cue(int cue_number);

    void set_move_instruction(int cue_number, int channel_id, int value);

    void remove_move_instruction(int cue_number, int channel_id);

    void update_channel(int channel_number, int new_value, UpdateMode mode, int target_cue_number);

    int get_channel_value_at_cue(int channel_id, int cue_number) const;
    [[nodiscard]] std::string get_channel_color_at_cue(int channel_id, int cue_number) const;

    std::vector<int> get_active_channels_at_cue(int cue_number);

    Cue *find_last_cue_with_channel(int channel_number) const;

    Channel &get_channel(int channel_id);

    [[nodiscard]] const std::map<int, Channel> &get_all_channels() const;

    void draw_cue_list();

    [[nodiscard]] int count_cues() const;

private:
    std::list<Cue> m_cue_storage;
    std::map<int, Cue *> m_cue_lookup_map;
    std::map<int, Channel> m_channel_map;

    //Navigation
    Cue *m_first_cue;
    Cue *m_current_cue;
    Cue *m_last_cue;

    static void print_header();

    void print_cue(const Cue &cue) const;
};

#endif
