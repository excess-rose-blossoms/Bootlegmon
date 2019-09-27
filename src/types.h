#pragma once
#include <string>

typedef std::string Name;
// Used only for Pokemon stats like HP, Atk, Def, etc...
typedef int Stat;
typedef int Power;
typedef int Accuracy;

// Returned by BattleSystem::GetPlayerInput().
// command: What the player chose from the top menus (eg. "FIGHT", "SWITCH", ...)
// selection: What the player chose from the submenus (eg. which pokemon to switch to). Set to -1 if not relevant.
struct Command
{
    Command(){};
    Command(std::string command, int selection):command(command), selection(selection){}
    std::string command;
    int selection;
};

const std::string InvalidPokemonName = "???Missingno???";