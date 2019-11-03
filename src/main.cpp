#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <TGUI/TGUI.hpp>
#include "battle_system.h"
#include "trainer.h"
#include "pokemon.h"
#include "types.h"
#include "poke_reader.h"
#include "moves.h"

using namespace std;

void RunBattleLoop(BattleSystem &battle_system)
{
    while (!battle_system.IsBattleOver())
    {
        battle_system.RunTurn();
    }
}

int main()
{
    // // Create the window, GUI, and UI.
    // sf::RenderWindow window{{1600, 1000}, "Bootlegmon"};
    // tgui::Gui gui{window};

    // BattleUI battle_ui(gui);

    // // Create a second thread to run the battle loop.
    // BattleSystem battle_system(&battle_ui);
    // thread battle_thread(RunBattleLoop, ref(battle_system));

    // // Poll and draw widgets.
    // while (window.isOpen())
    // {
    //     sf::Event event;
    //     while (window.pollEvent(event))
    //     {
    //         if (event.type == sf::Event::Closed)
    //         {
    //             window.close();
    //         }

    //         gui.handleEvent(event); // Pass the event to the widgets
    //     }

    //     window.clear();
    //     gui.draw(); // Draw all widgets
    //     battle_ui.DrawAllSprites(window); // Draw all sprites
    //     window.display();
    // }

}

// #include <TGUI/TGUI.hpp>
// #include "poke_reader.h"

// using namespace std;

// // The class that contains all the UI elements for the Pokemon choose screen.
// class PokePickUI
// {
//     public:
//         PokePickUI(tgui::Gui &gui)
//         {
//             // Set up Pokemon edit-box
//             m_pokemon_edit_box = tgui::EditBox::create();
//             m_pokemon_edit_box->setPosition("0%", "10%");
//             m_pokemon_edit_box->setSize("50%", "50%");
//             m_pokemon_edit_box->setReadOnly(true);

//             void (PokePickUI::*poke_function_ptr)(void);
//             poke_function_ptr = &PokePickUI::LoadPokemonListBox;
//             m_pokemon_edit_box->connect("Clicked", poke_function_ptr, this);

//             gui.add(m_pokemon_edit_box, "PokemonEditBox");

//             // Set up stats display box
//             m_hp_label = tgui::Label::create();
//             m_hp_label->setPosition("50%", "10%");
//             m_hp_label->setSize("50%", "10%");
//             m_hp_label->getRenderer()->setBackgroundColor(sf::Color::White);
//             m_hp_label->getRenderer()->setTextColor(sf::Color::Black);
//             m_hp_label->setText("HP: ");
//             gui.add(m_hp_label, "PokemonHPLabel");

//             m_atk_label = tgui::Label::create();
//             m_atk_label->setPosition("50%", "20%");
//             m_atk_label->setSize("50%", "10%");
//             m_atk_label->getRenderer()->setBackgroundColor(sf::Color::White);
//             m_atk_label->getRenderer()->setTextColor(sf::Color::Black);
//             m_atk_label->setText("ATK: ");
//             gui.add(m_atk_label, "PokemonAtkLabel");

//             m_spd_label = tgui::Label::create();
//             m_spd_label->setPosition("50%", "30%");
//             m_spd_label->setSize("50%", "10%");
//             m_spd_label->getRenderer()->setBackgroundColor(sf::Color::White);
//             m_spd_label->getRenderer()->setTextColor(sf::Color::Black);
//             m_spd_label->setText("SPD: ");
//             gui.add(m_spd_label, "PokemonSpdLabel");

//             // Set up multipurpose list-box
//             m_list_box = tgui::ListBox::create();
//             m_list_box->setPosition("0%", "60%");
//             m_list_box->setSize("100%", "50%");
//             gui.add(m_list_box, "ListBox");
//         }

//         // Called when m_pokemon_edit_box is clicked. Repopulates the multipurpose list box with info about Pokemon
//         // and lets it be used to select the contents of several Pokemon related widgets.
//         void LoadPokemonListBox()
//         {
//             m_list_box->removeAllItems();

//             // Populate the list box with all the names of the Pokemon that m_poke_reader finds.
//             int pokemon_num = 0;
//             Pokemon current_pokemon = m_poke_reader.ReadPokemon(pokemon_num);
//             while (current_pokemon.GetName() != "Missingno")
//             {
//                 string hp = to_string(current_pokemon.GetHP());
//                 string atk = to_string(current_pokemon.GetAtk());
//                 string spd = to_string(current_pokemon.GetSpd());
//                 m_list_box->addItem(current_pokemon.GetName() + ", HP:"+hp +", ATK:"+ atk +", SPD:"+ spd);
//                 pokemon_num++;
//                 current_pokemon = m_poke_reader.ReadPokemon(pokemon_num);
//             }

//             // On an item selection, load relevant Pokemon data into display widgets.
//             m_list_box->connect("ItemSelected",
//                                 [&](const sf::String &list_selection) {
//                                     m_list_box->setSelectedItem(list_selection);
//                                     Pokemon relevant_pokemon = m_poke_reader.ReadPokemon(m_list_box->getSelectedItemIndex());
//                                     m_pokemon_edit_box->setText(relevant_pokemon.GetName());
//                                     m_hp_label->setText("HP: " + to_string(relevant_pokemon.GetHP()));
//                                     m_atk_label->setText("ATK: " + to_string(relevant_pokemon.GetAtk()));
//                                     m_spd_label->setText("SPD: " + to_string(relevant_pokemon.GetSpd()));
//                                 });
//         }

//     private:
//         //Contains an edit box in which you can enter your desired Pokemon's name.
//         tgui::EditBox::Ptr m_pokemon_edit_box;

//         // Contains a scrolling list of options depending on which field is clicked.
//         tgui::ListBox::Ptr m_list_box;

//         // Stats display text.
//         tgui::Label::Ptr m_hp_label;
//         tgui::Label::Ptr m_atk_label;
//         tgui::Label::Ptr m_spd_label;

//         // Top bar. Allows you to add up to 6 Pokemon.
//         tgui::Label::Ptr m_top_bar[];

//         //Reads "pokedex.txt" to obtain data.
//         PokeReader m_poke_reader;
// };

// int main()
// {
//     sf::RenderWindow window{{1600, 1000}, "Bootlegmon"};
//     tgui::Gui gui{window}; // Create the gui and attach it to the window

//     PokePickUI poke_pick_ui(gui);

//     while (window.isOpen())
//     {
//         sf::Event event;
//         while (window.pollEvent(event))
//         {
//             if (event.type == sf::Event::Closed)
//                 window.close();

//             gui.handleEvent(event); // Pass the event to the widgets
//         }

//         window.clear();
//         gui.draw(); // Draw all widgets
//         window.display();
//     }
// }