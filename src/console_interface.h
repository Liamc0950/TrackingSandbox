//
// Created by Liam Corley on 8/9/25.
//

#ifndef CONSOLE_INTERFACE_H
#define CONSOLE_INTERFACE_H
#include "cue_list.h"
#include <string>
#include <vector>

class ConsoleInterface {
private:
    CueList& m_cue_list;
    std::vector<std::string> m_command_history;
    bool m_running;

public:
    explicit ConsoleInterface(CueList& cue_list);

    void start();
    void stop();
    void process_command(const std::string& input);

    static void show_help();
    void show_status();

private:
    void handle_update_command(std::istringstream& iss) const;

    void handle_goto_command(std::istringstream& iss) const;
    void handle_show_command(std::istringstream& iss) const;
    // void handle_add_command(std::istringstream& iss);
    // void handle_delete_command(std::istringstream& iss);

    static bool confirm_action(const std::string& message);

    static void print_prompt();
    void add_to_history(const std::string& command);
};

#endif //CONSOLE_INTERFACE_H