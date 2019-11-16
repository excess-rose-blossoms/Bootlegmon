#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <TGUI/TGUI.hpp>
#include "pokemon.h"
#include "trainer.h"
#include "types.h"
#include "pokemon_reader.h"
#include "battle_ui.h"

class BattleSystem
{
public:
    BattleSystem(BattleUI *battle_ui, PokemonReader *pokemon_reader);

    // Prompts the player to choose
    Command GetPlayerInput();

    // Run repeatedly to create a battle. Gets the player's input for their turn then the enemy's.
    // The battle ends when one Pokemon's HP runs out, upon which m_is_battle_over is set to true.
    void RunTurn();

    // Meant to be called in place of RunTurn. Used for debugging or prototyping new features.
    void RunTurnDebug();

    // Accessor for m_is_battle_over
    inline bool IsBattleOver() { return m_is_battle_over; };

    // Prints the participant names.
    void PrintReport();

    Command GetPlayerSwitchInput(bool allow_cancel = false);

    Command GetPlayerMoveInput();

    // Checks whether the switch input is a number.
    // allow_cancel: Allows the player to input a number equal to m_player_trainer.GetNumPokemon()
    // if set to true, allowing the player to cancel the switch.
    bool IsSwitchInputValid(Command c, bool allow_cancel = false);

private:
    Trainer m_player_trainer;
    TrainerAI m_enemy_trainer;
    bool m_is_battle_over;
    BattleUI *m_battle_ui;
    PokemonReader *m_pokemon_reader;
};