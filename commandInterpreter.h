#ifndef COMMAND_INTERPRETER_H
#define COMMAND_INTERPRETER_H

#include <functional>
#include <string>
#include <unordered_map>

using namespace std;

class Game;

class CommandInterpreter {
    Game* game;
    unordered_map<string, function<void()>> commands;

    void renameCommand(const string& commandName, const string& newName);

   public:
    CommandInterpreter(Game* game);
    ~CommandInterpreter() = default;

    string parseCommand(int& multilpier) const;
};

#endif