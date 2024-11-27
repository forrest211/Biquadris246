#include "commandInterpreter.h"
#include "game.h"

#include <iostream>
#include <memory>
#include <regex>
#include <sstream>

using namespace std;

const int MAX_LEVEL = 4;

void CommandInterpreter::renameCommand(const string& commandName, const string& newName) {
    auto it = commands.find(commandName);
    if (it == commands.end()) {
        throw std::runtime_error("Error: Command \"" + commandName + "\" not found.");
    }
    if (commands.find(newName) != commands.end()) {
        throw std::runtime_error("Error: Command \"" + newName + "\" already exists.");
    }

    commands[newName] = it->second;
    commands.erase(it);
}

CommandInterpreter::CommandInterpreter(Game* game) : game(game) {
    commands["left"] = [this]() { this->game->getBoard()->moveBlock("l"); };
    commands["right"] = [this]() { this->game->getBoard()->moveBlock("r"); };
    commands["down"] = [this]() { this->game->getBoard()->moveBlock("d"); };
    commands["clockwise"] = [this]() { this->game->getBoard()->rotateBlock("CW"); };
    commands["counterclockwise"] = [this]() { this->game->getBoard()->moveBlock("CCW"); };
    commands["drop"] = [this]() { this->game->getBoard()->dropBlock(); };

    commands["levelup"] = [this]() {
        Player* p = this->game->getCurrentPlayer();
        p->setLevel(p->getLevel() + 1);
    };
    commands["leveldown"] = [this]() {
        Player* p = this->game->getCurrentPlayer();
        p->setLevel(p->getLevel() - 1);
    };
    commands["norandom"] = [this]() {
        string fileName;
        cout << "Enter file name: ";
        cin >> fileName;
        // ... set block sequence
        this->game->getCurrentPlayer()->setNoRand(fileName);
    };

    commands["I"] = [this]() { this->game->getBoard()->removeBlock(true); };
    commands["J"] = [this]() { this->game->getBoard()->removeBlock(true); };
    commands["L"] = [this]() { this->game->getBoard()->removeBlock(true); };
    commands["O"] = [this]() { this->game->getBoard()->removeBlock(true); };
    commands["S"] = [this]() { this->game->getBoard()->removeBlock(true); };
    commands["Z"] = [this]() { this->game->getBoard()->removeBlock(true); };
    commands["T"] = [this]() { this->game->getBoard()->removeBlock(true); };

    commands["restart"] = [this]() { this->game->restart(); };
    commands["quit"] = []() {
        cout << "Game Over. Exiting..." << endl;
        exit(0);
    };
    commands["help"] = [this]() {
        std::cout << "Available commands:\n";
        for (const auto& [key, _] : commands) {
            std::cout << "- " << key << "\n";
        }
        std::cout << "You can also prefix commands with a number (e.g., '3left' to move left three times).\n";
    };
    commands["rename"] = [this] {
        std::string commandName, newName;
        std::cout << "Enter command to rename: ";
        std::cin >> commandName;
        std::cout << "Enter new name: ";
        std::cin >> newName;

        try {
            renameCommand(commandName, newName);
            std::cout << "Command renamed from \"" << commandName << "\" to \"" << newName << "\"\n";
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    };
}

string CommandInterpreter::parseCommand(int& multiplier) const {
    string input;
    cout << "Enter command: ";
    if (!(cin >> input)) {
        std::cout << "End of input detected. Exiting..." << std::endl;
        exit(0);
    }

    regex commandRegex(R"((\d*)([a-zA-Z]+))");
    smatch matches;
    if (regex_match(input, matches, commandRegex)) {
        multiplier = 1;
        string commandName;

        // check if a multiplier is provided
        if (matches[1].matched && !matches[1].str().empty()) {
            multiplier = stoi(matches[1].str());
        }

        commandName = matches[2].str();
        // try to match the command name to a registered command
        string match;
        for (const auto& [key, value] : commands) {
            if (key.find(commandName) == 0) {  // check if input is a prefix of a command
                if (!match.empty()) {
                    // more than one command matches
                    cout << "Ambiguous command : \"" << commandName << "\". Type 'help' for a list of commands." << endl;
                    return "";
                }
                match = key;
            }
        }

        // execute the matched command
        if (!match.empty() && commands.find(match) != commands.end()) {
            // if the given multiplier is 0, but the matched command is one of
            // 'restart', 'norandom' or 'random', we must still execute said
            // command, though it matters not that the multiplier is -1 upon
            // exiting the function, as Game handles this case appropriately
            if (multiplier == 0 && (match == "restart" || match == "norandom" || match == "random")) {
                commands.at(match)();
            // otherwise, if the given multiplier is 0, we simply return an
            // empty string to ensure that Game would not do anything with the
            // given command, seeing that a zero multiplier is applied
            } else if (multiplier == 0) return "";

            // otherwise, we apply the command as many times as specified, while
            // also mutated the multiplier to tell Game that we performed a
            // command at least once, though the multiplier is used more
            // specifically by Game to handle when a multiplier is applied on the
            // 'drop' command
            for (int i = 0; i < multiplier; ++i) {
                commands.at(match)();
            }

            --multiplier;

            return match;
        } else {
            cout << "Invalid command: \"" << input << "\". Type 'help' for a list of commands." << endl;
            return "";
        }
    } else {
        std::cout << "Invalid input format: \"" << input << "\". Type 'help' for a list of commands." << std::endl;
        return "";
    }
}
