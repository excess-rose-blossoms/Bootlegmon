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

class BattleUI
{
public:
    BattleUI(tgui::Gui &gui);
    void InitBattleTextGroup(tgui::Gui &gui);
    void InitFightSwitchChoiceGroup(tgui::Gui &gui);
    void InitPokemonSwitchGroup(tgui::Gui &gui);
    void InitMoveChoiceGroup(tgui::Gui &gui);
    void InitPokemonUI(tgui::Gui &gui);

    // Used to display messages to the user during battle.
    void SetBattleText(std::string text) { m_btext_label->setText(text); }

    // HIDE DISPLAY

    // Hides all bottom text bar groups
    void HideBottomBar();
    
    // Displays the battle text widgets
    // message: What the label text is set to
    void DisplayText(std::string message);

    // Displays the fight switch choice widgets
    void DisplayFSChoice();

    // Displays the Pokemon Choice widgets
    void DisplayPChoice(Trainer *trainer, Pokemon pokemon_party[], int size = 6);

    // Displays the Move Choice widgets
    void DisplayMChoice();

    // Waits until m_is_user_ready has been set to true before setting it to false and proceeding.
    void WaitForUser();

    // Accessor and mutator for m_is_ready
    inline bool GetIsUserReady() { return m_is_user_ready; }
    inline void SetIsUserReady(bool is_user_ready) { m_is_user_ready = is_user_ready; }

    // Accessor and mutator for m_last_command
    inline Command GetLastCommand() { return m_last_command; }
    inline void SetLastCommand(Command last_command) { m_last_command = last_command; }

    inline void SetPlayerPokemonHpLabelText(std::string text) { m_player_pokemon_hp_label->setText(text); }
    inline void SetEnemyPokemonHpLabelText(std::string text) { m_enemy_pokemon_hp_label->setText(text); }

    void SetPlayerPokemonSprite(std::string image_name);
    void SetEnemyPokemonSprite(std::string image_name);

    // Draw sprites
    void DrawAllSprites(sf::RenderWindow &window);

private:
    // UI colors and stuff
    sf::Color bg_color = sf::Color::White;
    sf::Color inverse_color = sf::Color::Black;
    sf::Color text_color = sf::Color::Black;
    int text_size = 40;
    int padding = 25;

    //Vector containing all the groups that make up the bottom text bar
    std::vector<tgui::Group::Ptr> m_text_groups;

    // Battle text group widgets.
    tgui::Group::Ptr m_btext_group;
    tgui::Label::Ptr m_btext_label;

    // Fight Switch choice group widgets.
    tgui::Group::Ptr m_fs_group;
    tgui::Label::Ptr m_fstext_label;
    tgui::Label::Ptr m_fsfight_label;
    tgui::Label::Ptr m_fsswitch_label;

    // Pokemon switch choice group widgets.
    tgui::Group::Ptr m_switch_group;
    tgui::Label::Ptr m_switch_pokemon_labels[6];
    tgui::Label::Ptr m_switch_text_label;
    tgui::Label::Ptr m_switch_back_label;

    // Move choice group widgets.
    tgui::Group::Ptr m_move_group;
    tgui::Label::Ptr m_move_labels[4];
    tgui::Label::Ptr m_move_text_label;
    tgui::Label::Ptr m_move_back_label;

    // Set to true upon user input. Must be manually set to false after it is validated.
    bool m_is_user_ready;
    // Saves the last command the user clicked.
    Command m_last_command;

    // Vector containing pointers to all the sprites that need to be drawn
    std::vector<sf::Sprite *> m_sprite_pointers;
    sf::Sprite m_player_pokemon_sprite;
    sf::Texture m_player_pokemon_texture;

    sf::Sprite m_enemy_pokemon_sprite;
    sf::Texture m_enemy_pokemon_texture;

    // HP widgets
    tgui::Label::Ptr m_player_pokemon_hp_label;
    tgui::Label::Ptr m_enemy_pokemon_hp_label;
};