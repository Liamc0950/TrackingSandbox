#ifndef CUE_LIST_H
#define CUE_LIST_H

#include <map>

#include "cue.h"
#include "channel.h"
#include <list>

class Cue;
class Channel;

class CueList
{
    public:
        explicit CueList(unsigned short number_of_cues);

        [[nodiscard]] int get_cue_count() const;

        [[nodiscard]] std::map<int, Channel> get_channels();

        void insert_cue(Cue cue, Cue *previous_cue);

        void append_cue(const Cue &cue);

        Cue get_cue_by_number(int cue_number) const;

        void process_cues();

        void draw_cue_list();


        std::map<int, Cue*> get_cue_lookup_map();

        std::list<Cue>& get_cues();

    private:

        std::list<Cue> m_cue_storage;
        std::map<int, Cue*> m_cue_lookup_map;

        Cue* m_first_cue;
        Cue* m_current_cue;
        Cue* m_last_cue;

        //Channels stored in vector where index is channel number
        std::map<int, Channel> m_channel_map;

        [[nodiscard]] int count_cues() const;

        void print_header();

        void print_cue(Cue &cue);


};

#endif
