//
// Created by Liam Corley on 8/9/25.
//
#include "console_interface.h"
#include <iostream>
#include <sstream>

ConsoleInterface::ConsoleInterface(CueList& cue_list)
    : m_cue_list(cue_list), m_running(false) {}

void ConsoleInterface::start() {
    m_running = true;
    std::string input;

    std::cout << "Lighting Console - Interactive Mode" << std::endl;
    std::cout << "Type 'help' for commands, 'quit' to exit" << std::endl;
    // show_status();

    while (m_running) {
        print_prompt();
        std::getline(std::cin, input);

        if (!input.empty()) {
            add_to_history(input);

            try {
                process_command(input);
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
    }
}

void ConsoleInterface::stop() {
    m_running = false;
    std::cout << "Console interface stopped." << std::endl;
}

void ConsoleInterface::process_command(const std::string& input) {
    std::istringstream iss(input);
    std::string command;
    iss >> command;

    if (command == "help") {
        show_help();
    } else if (command == "quit" || command == "exit") {
        stop();
    } else if (command == "show") {
        handle_show_command(iss);
    } else if (command == "goto") {
        handle_goto_command(iss);
    } else if (command == "update") {
        handle_update_command(iss);
    // } else if (command == "add") {
    //     handle_add_command(iss);
    // } else if (command == "delete") {
    //     handle_delete_command(iss);
    } else if (command == "status") {
        show_status();
    } else {
        std::cout << "Unknown command: '" << command << "'. Type 'help' for available commands." << std::endl;
    }
}

void ConsoleInterface::show_help() {
    std::cout << "\n=== Console Commands ===" << std::endl;
    std::cout << "  help                                    - Show this help" << std::endl;
    std::cout << "  status                                  - Show system status" << std::endl;
    std::cout << "  show                                    - Display cue list" << std::endl;
    std::cout << "  goto <cue>                              - Go to cue number" << std::endl;
    std::cout << "  set <cue> <channel> <value>             - Set channel value in specific cue" << std::endl;
    std::cout << "  update trace <channel> <value> <cue>    - Update channel at source cue" << std::endl;
    std::cout << "  update track <channel> <value> <cue>    - Update channel in cue to track" << std::endl;
    std::cout << "  update cue_only <channel> <value> <cue> - Update channel in cue only" << std::endl;
    std::cout << "  add cue <number>                        - Add new cue" << std::endl;
    std::cout << "  delete cue <number>                     - Delete cue" << std::endl;
    std::cout << "  quit                                    - Exit console" << std::endl;
    std::cout << "========================\n" << std::endl;
}

void ConsoleInterface::show_status() {
    std::cout << "\n=== System Status ===" << std::endl;
    std::cout << "Total cues: " << m_cue_list.count_cues() << std::endl;

    try {
        std::cout << "Current cue: " << m_cue_list.get_current_cue_number() << std::endl;
    } catch (const std::exception&) {
        std::cout << "Current cue: None set" << std::endl;
    }

    std::cout << "Active channels: " << m_cue_list.get_all_channels().size() << std::endl;
    std::cout << "Command history: " << m_command_history.size() << " commands" << std::endl;
    std::cout << "=====================\n" << std::endl;
}

void ConsoleInterface::print_prompt() {
    std::cout << "User>>";
}

void ConsoleInterface::add_to_history(const std::string& command) {
    m_command_history.push_back(command);

    // Keep history size manageable
    if (m_command_history.size() > 100) {
        m_command_history.erase(m_command_history.begin());
    }
}

bool ConsoleInterface::confirm_action(const std::string& message) {
    std::cout << message << " (y/n): ";
    std::string response;
    std::getline(std::cin, response);

    return (response == "y" || response == "yes" || response == "Y" || response == "YES");
}

// Command handlers would go here...
void ConsoleInterface::handle_show_command(std::istringstream& iss) const {
    m_cue_list.draw_cue_list();
}

void ConsoleInterface::handle_goto_command(std::istringstream& iss) const {
    int cue_num;
    if (!(iss >> cue_num)) {
        std::cout << "Usage: goto <cue>" << std::endl;
        return;
    }

    m_cue_list.go_to_cue(cue_num);
    std::cout << "Current cue: " << cue_num << std::endl;
}
void ConsoleInterface::handle_update_command(std::istringstream& iss) const {
    std::string mode_str;
    int channel_number, value, cue_num;

    // Parse basic parameters
    if (!(iss >> mode_str >> channel_number >> value >> cue_num)) {
        std::cout << "Usage: update <source|here|at> <channel> <value> <cue>" << std::endl;
        return;
    }

    try {
        if (mode_str == "track") {
            m_cue_list.update_channel(channel_number, value, CueList::UpdateMode::UPDATE_TRACK, cue_num);
        } else if (mode_str == "trace") {
            m_cue_list.update_channel(channel_number, value, CueList::UpdateMode::UPDATE_TRACE, cue_num);
        } else if (mode_str == "cue_only") {
            m_cue_list.update_channel(channel_number, value, CueList::UpdateMode::UPDATE_CUE_ONLY, cue_num);
        }
    } catch (const std::exception& e) {
        std::cout << "Update failed: " << e.what() << std::endl;
    }
}
