#include "cue_list.h"

#include <iomanip>
#include <iostream>

CueList::CueList(const unsigned short number_of_cues) {
    m_first_cue = nullptr;
    m_last_cue = nullptr;
    m_current_cue = nullptr;


    //By default, use incremental whole numbers and channels 1-5
    std::map<int, Channel> default_channels = {
        {1, Channel(1, -1)},
        {2, Channel(2, -1)},
        {3, Channel(3, -1)},
        {4, Channel(4, -1)},
        {5, Channel(5, -1)}
    };
    m_channel_map = default_channels;

    for (int i = 1; i <= number_of_cues; i++) {
        this->append_cue(Cue(i));
    }
}

int CueList::count_cues() const {
    int count = 0;
    const Cue* current_cue = m_first_cue;

    while (current_cue != nullptr) {
        count++;
        current_cue = current_cue->get_next_cue();
    }

    return count;
}

int CueList::get_cue_count() const {
    return count_cues();
}
std::map<int, Cue*> CueList::get_cue_lookup_map() {
    return m_cue_lookup_map;
}

std::map<int, Channel> CueList::get_channels() {
    return m_channel_map;
}
void CueList::insert_cue(Cue cue, Cue *previous_cue){}

void CueList::append_cue(const Cue& cue) {
    m_cue_storage.push_back(cue);
    Cue* new_cue_ptr = &m_cue_storage.back();
    m_cue_lookup_map[new_cue_ptr->get_number()] = new_cue_ptr;

    if (m_first_cue == nullptr) {
        m_first_cue = new_cue_ptr;
        m_last_cue = new_cue_ptr;
    } else {
        m_last_cue->set_next_cue(new_cue_ptr);
        new_cue_ptr->set_previous_cue(m_last_cue);
        m_last_cue = new_cue_ptr;
    }
}
Cue CueList::get_cue_by_number(const int cue_number) const {
    return *m_cue_lookup_map.at(cue_number);
}

void CueList::process_cues(){}


void CueList::draw_cue_list(){
    std::cout << std::string(100, '=') << "\n";
    print_header();
    Cue* current = m_first_cue;
    while (current != nullptr) {
        print_cue(*current);
        current = current->get_next_cue();
    }
}

void CueList::print_header() {
    std::cout << std::setw(5) << "|" << "CUE #" << "|"
              << std::setw(5) << "|" << "CHAN 1" << "|"
              << std::setw(5) << "|" << "CHAN 2" << "|"
              << std::setw(5) << "|" << "CHAN 3" << "|"
              << std::setw(5) << "|" << "CHAN 4" << "|"
              << std::setw(5) << "|" << "CHAN 5" << "|" << std::endl;
}

void CueList::print_cue(Cue& cue) {
    std::cout << std::setw(5) << "|" << cue.get_number() << "|"
              << std::setw(5) << "|" << m_channel_map.at(1).get_value()<< "|"
              << std::setw(5) << "|" << m_channel_map.at(2).get_value() << "|"
              << std::setw(5) << "|" << m_channel_map.at(3).get_value() << "|"
              << std::setw(5) << "|" << m_channel_map.at(4).get_value() << "|"
              << std::setw(5) << "|" << m_channel_map.at(5).get_value() << "|" << std::endl;
}

std::list<Cue>& CueList::get_cues() {
    return m_cue_storage;
}