#include "battle_system.h"

using namespace std;

#pragma region Battle System
BattleSystem::BattleSystem(BattleUI *battle_ui, PokemonReader *pokemon_reader) 
    :   m_is_battle_over(false), m_battle_ui(battle_ui), m_pokemon_reader(pokemon_reader), 
        m_player_trainer("PLAYER", pokemon_reader->MakePoke("Mimikyu"), pokemon_reader->MakePoke("Bibikyu")),
        m_enemy_trainer("ENEMY", pokemon_reader->MakePoke("Pikachu"), pokemon_reader->MakePoke("Bikabu"))
{
    // Initialize UI
    cout << m_player_trainer.GetLead()->GetName() << endl;
    m_battle_ui->SetPlayerPokemonHpLabelText(to_string(m_player_trainer.GetLeadHP()));
    m_battle_ui->SetEnemyPokemonHpLabelText(to_string(m_enemy_trainer.GetLeadHP()));
    m_battle_ui->SetPlayerPokemonSprite(m_player_trainer.GetLead()->GetImageName());
    m_battle_ui->SetEnemyPokemonSprite(m_enemy_trainer.GetLead()->GetImageName());
}

//Prompts the player to choose.
Command BattleSystem::GetPlayerInput()
{
    Command input_command;
    do 
    {
        // Read in input
        m_battle_ui->DisplayFSChoice();
        

        if (m_battle_ui->GetLastCommand().command == "FIGHT")
        {
            input_command.command = "FIGHT";
            input_command.selection = -1;
            return input_command;
        }

        if (m_battle_ui->GetLastCommand().command == "SWITCH")
        {
            Command switch_command = GetPlayerSwitchInput(true);

            input_command.command = switch_command.command;
            input_command.selection = (switch_command.command == "BACK") ? -1 : switch_command.selection;
        }
    } while (input_command.command == "BACK");
    
    return input_command;
}

// Helper function that validates the input command the player enters for switching.
bool BattleSystem::IsSwitchInputValid(Command c, bool allow_cancel)
{    
    // Allow an option to cancel the switch if bool allow_cancel is set to true
    if (allow_cancel && c.command=="BACK")
        return true;

    // Allow a switch to a valid-numbered, non-fainted, non-lead Pokemon only.
    return (c.command=="SWITCH" &&
            m_player_trainer.GetPokemon(c.selection)->GetHP() > 0 &&
            m_player_trainer.GetPokemon(c.selection) != m_player_trainer.GetLead());
}

// Prompts and accepts player input when switching Pokemon.
Command BattleSystem::GetPlayerSwitchInput(bool allow_cancel)
{
    Command switch_command;
    do
    {
        m_battle_ui->DisplayPChoice(&m_player_trainer, m_player_trainer.GetPokemonParty());
        switch_command = m_battle_ui->GetLastCommand();
    } while (!IsSwitchInputValid(switch_command, allow_cancel));

    return switch_command;
}

// Run repeatedly to create a battle. Gets the player's input for their turn then the enemy's.
// The battle ends when all the HP of a trainer's Pokemon runs out, upon which m_is_battle_over is set to true.
void BattleSystem::RunTurn()
{
    m_battle_ui->DisplayText("What do?");

    //Get inputs
    m_player_trainer.SetRecentCommand(GetPlayerInput());
    m_enemy_trainer.SetRecentCommand(m_enemy_trainer.DecideCommand("FIGHT"));

    // Sort trainers to move in order according to speed.
    Trainer *trainers_in_order[2];
    trainers_in_order[0] = &m_player_trainer;
    trainers_in_order[1] = &m_enemy_trainer;

    if (m_enemy_trainer.GetLeadSpd() > m_player_trainer.GetLeadSpd())
    {
        trainers_in_order[0] = &m_enemy_trainer;
        trainers_in_order[1] = &m_player_trainer;
    }
    
    // Before fighting, check to see if there are any switches.
    for (int i = 0; i < 2;i++)
    {
        if (trainers_in_order[i]->GetRecentCommand().command == "SWITCH")
        {
            Pokemon *switch_pokemon = trainers_in_order[i]->GetPokemon(trainers_in_order[i]->GetRecentCommand().selection);
            m_battle_ui->DisplayText(trainers_in_order[i]->GetName() + " withdrew " + trainers_in_order[i]->GetLeadName() + " and sent in " + switch_pokemon->GetName() + "!");
            trainers_in_order[i]->SwitchPokemon(trainers_in_order[i]->GetRecentCommand().selection);
            
            if (trainers_in_order[i] == &m_player_trainer)
            {
                m_battle_ui->SetPlayerPokemonHpLabelText(to_string(m_player_trainer.GetLeadHP()));
                m_battle_ui->SetPlayerPokemonSprite(m_player_trainer.GetLead()->GetImageName());
            }
            else 
            {
                m_battle_ui->SetEnemyPokemonHpLabelText(to_string(m_enemy_trainer.GetLeadHP()));
                m_battle_ui->SetEnemyPokemonSprite(m_enemy_trainer.GetLead()->GetImageName());
            }
        }
    }

    // Battle phase
    for (int i = 0; i < 2; i++)
    {
        // FIGHT selected
        if (trainers_in_order[i]->GetRecentCommand().command == "FIGHT")
        {
            // Decrement health appropriately
            int target_index = (i == 0) ? 1 : 0;
            int damage_taken = trainers_in_order[i]->GetLead()->Attack(trainers_in_order[target_index]->GetLead());

            if (trainers_in_order[target_index] == &m_player_trainer)
            {
                m_battle_ui->SetPlayerPokemonHpLabelText(to_string(m_player_trainer.GetLeadHP()));
            }
            else 
            {
                m_battle_ui->SetEnemyPokemonHpLabelText(to_string(m_enemy_trainer.GetLeadHP()));
            }

            m_battle_ui->DisplayText(trainers_in_order[i]->GetLeadName() + " attacked! " + trainers_in_order[target_index]->GetLeadName() + " took " + to_string(damage_taken) + " damage!");
            m_battle_ui->DisplayText(trainers_in_order[target_index]->GetLeadName() + " has " + to_string(trainers_in_order[target_index]->GetLeadHP()) + " HP left!");

            // If opposing Pokemon was knocked out by the attack
            if (trainers_in_order[target_index]->LeadHasFainted())
            {
                m_battle_ui->DisplayText(trainers_in_order[target_index]->GetLeadName() + " fainted!");
                trainers_in_order[target_index]->DecrementNumLivePokemon();

                // End game if the trainer has run out of Pokemon.
                if (trainers_in_order[target_index]->GetNumLivePokemon() <= 0)
                {        
                    m_battle_ui->DisplayText(trainers_in_order[target_index]->GetName() + " is out of Pokemon!");

                    if (trainers_in_order[target_index] == &m_player_trainer)
                        m_battle_ui->DisplayText("You lose!");
                    else
                        m_battle_ui->DisplayText("You win!");

                    m_is_battle_over = true;
                    return;
                }
                // Prompt a switch if the trainer still has Pokemon.
                else
                {
                    if (trainers_in_order[target_index] == &m_player_trainer)
                    {
                        int switch_pokemon_num = GetPlayerSwitchInput().selection;
                        Pokemon *switch_pokemon = m_player_trainer.GetPokemon(switch_pokemon_num);
                        m_battle_ui->DisplayText(m_player_trainer.GetName() + " sent in " + switch_pokemon->GetName() + "!");
                        m_player_trainer.SwitchPokemon(switch_pokemon_num);
                        m_battle_ui->SetPlayerPokemonSprite(m_player_trainer.GetLead()->GetImageName());
                        m_battle_ui->SetPlayerPokemonHpLabelText(to_string(m_player_trainer.GetLeadHP()));
                    }
                    else
                    {
                        int switch_pokemon_num = m_enemy_trainer.DecideSwitchedPokemon();
                        Pokemon *switch_pokemon = m_enemy_trainer.GetPokemon(switch_pokemon_num);
                        m_battle_ui->DisplayText(m_enemy_trainer.GetName() + " sent in " + switch_pokemon->GetName() + "!");
                        m_enemy_trainer.SwitchPokemon(switch_pokemon_num);
                        m_battle_ui->SetEnemyPokemonHpLabelText(to_string(m_enemy_trainer.GetLeadHP()));
                        m_battle_ui->SetEnemyPokemonSprite(m_enemy_trainer.GetLead()->GetImageName());
                    }
                }     
            }
        }
        // NOTHING selected
        else if (trainers_in_order[i]->GetRecentCommand().command == "NOTHING")
        {
            if (trainers_in_order[i] == &m_player_trainer)
                m_battle_ui->DisplayText(m_player_trainer.GetLeadName() + " did nothing! What a slacker!");
            else
                m_battle_ui->DisplayText(m_enemy_trainer.GetLeadName() + " did nothing! Let's go, self-care!");
        }
    }
}

void BattleSystem::PrintReport()
{
    cout << "Battle between " << m_player_trainer.GetName() << " (player) using " << m_player_trainer.GetLead()->GetName() 
    << " and " << m_enemy_trainer.GetName() << " (enemy) using " << m_enemy_trainer.GetLead()->GetName() << "." << endl;
}
#pragma endregion Battle System

#pragma region Battle UI

BattleUI::BattleUI(tgui::Gui &gui)
{
    InitBattleTextGroup(gui);
    InitFightSwitchChoiceGroup(gui);
    InitPokemonSwitchGroup(gui);
    InitPokemonUI(gui);

    m_is_user_ready = false;
}

void BattleUI::InitBattleTextGroup(tgui::Gui &gui)
{
    m_btext_group = tgui::Group::create();
    gui.add(m_btext_group, "Battle Text Group");
    m_text_groups.push_back(m_btext_group);

    m_btext_label = tgui::Label::create();
    m_btext_label->setPosition("0%", "70%");
    m_btext_label->setSize("100%", "30%");
    m_btext_label->setText("TEXT HERE");   
    m_btext_label->setTextSize(text_size);
    m_btext_label->getRenderer()->setBackgroundColor(bg_color);
    m_btext_label->getRenderer()->setTextColor(text_color);
    m_btext_label->getRenderer()->setPadding(padding);

    // Set up signal
    void (BattleUI::*func)(bool);
    func = &BattleUI::SetIsUserReady;
    m_btext_label->connect("Clicked", func, this, true); 

    m_btext_group->add(m_btext_label);
}

void BattleUI::InitFightSwitchChoiceGroup(tgui::Gui &gui)
{
    // FIGHT-SWITCH CHOICE GROUP
    m_fs_group = tgui::Group::create();
    gui.add(m_fs_group, "Fight Switch Choice Group");
    m_text_groups.push_back(m_fs_group);
    m_fstext_label = tgui::Label::create();
    m_fstext_label->setPosition("0%", "70%");
    m_fstext_label->setSize("60%", "30%");
    m_fstext_label->setText("What will you do?");
    m_fstext_label->setTextSize(text_size);
    m_fstext_label->getRenderer()->setTextColor(text_color);
    m_fstext_label->getRenderer()->setBackgroundColor(bg_color);
    m_fstext_label->getRenderer()->setPadding(padding);
    m_fs_group->add(m_fstext_label);

    m_fsfight_label = tgui::Label::create();
    m_fsfight_label->setPosition("60%", "70%");
    m_fsfight_label->setSize("40%", "15%");
    m_fsfight_label->setText("FIGHT");
    m_fsfight_label->setTextSize(text_size);
    m_fsfight_label->getRenderer()->setTextColor(text_color);
    m_fsfight_label->getRenderer()->setBackgroundColor(bg_color);
    m_fsfight_label->getRenderer()->setPadding(padding);

    // Set up signal
    void (BattleUI::*f_func)(Command);
    f_func = &BattleUI::SetLastCommand;
    Command f_command("FIGHT", -1);
    m_fsfight_label->connect("Clicked", f_func, this, f_command);

    void (BattleUI::*a_func)(bool);
    a_func = &BattleUI::SetIsUserReady;
    m_fsfight_label->connect("Clicked", a_func, this, true);

    m_fs_group->add(m_fsfight_label);

    m_fsswitch_label = tgui::Label::create();
    m_fsswitch_label->setPosition("60%", "85%");
    m_fsswitch_label->setSize("40%", "15%");
    m_fsswitch_label->setText("SWITCH");
    m_fsswitch_label->setTextSize(text_size);
    m_fsswitch_label->getRenderer()->setTextColor(text_color);
    m_fsswitch_label->getRenderer()->setBackgroundColor(bg_color);
    m_fsswitch_label->getRenderer()->setPadding(padding);

    // Set up signal
    void (BattleUI::*s_func)(Command);
    s_func = &BattleUI::SetLastCommand;
    Command s_command("SWITCH", -1);
    m_fsswitch_label->connect("Clicked", s_func, this, s_command);

    void (BattleUI::*b_func)(bool);
    b_func = &BattleUI::SetIsUserReady;
    m_fsswitch_label->connect("Clicked", b_func, this, true);   

    m_fs_group->add(m_fsswitch_label);
}

void BattleUI::InitPokemonSwitchGroup(tgui::Gui &gui)
{
    m_switch_group = tgui::Group::create();
    gui.add(m_switch_group, "Pokemon Switch Group");
    m_text_groups.push_back(m_switch_group);

    m_switch_text_label = tgui::Label::create();
    m_switch_text_label->setPosition("0%", "70%");
    m_switch_text_label->setSize("30%", "30%");
    m_switch_text_label->setText("Switch to?");   
    m_switch_text_label->setTextSize(text_size);
    m_switch_text_label->getRenderer()->setBackgroundColor(bg_color);
    m_switch_text_label->getRenderer()->setTextColor(text_color);
    m_switch_text_label->getRenderer()->setPadding(padding);
    m_switch_group->add(m_switch_text_label);

    m_switch_back_label = tgui::Label::create();
    m_switch_back_label->setPosition("90%", "70%");
    m_switch_back_label->setSize("10%", "30%");
    m_switch_back_label->setText("Back");
    m_switch_back_label->setTextSize(text_size);
    m_switch_back_label->getRenderer()->setBackgroundColor(bg_color);
    m_switch_back_label->getRenderer()->setTextColor(text_color);
    m_switch_back_label->getRenderer()->setPadding(padding);

    // Set up signal
    void (BattleUI::*b_func)(Command);
    b_func = &BattleUI::SetLastCommand;
    Command b_command("BACK", -1);
    m_switch_back_label->connect("Clicked", b_func, this, b_command);

    void (BattleUI::*rb_func)(bool);
    rb_func = &BattleUI::SetIsUserReady;
    m_switch_back_label->connect("Clicked", rb_func, this, true);  

    m_switch_group->add(m_switch_back_label);

    for (int i = 0; i < 6; i++)
    {
        m_switch_pokemon_labels[i] = tgui::Label::create();
        m_switch_pokemon_labels[i]->setTextSize(text_size);
        m_switch_pokemon_labels[i]->setSize("30%", "10%");
        m_switch_pokemon_labels[i]->setText(to_string(i));
        m_switch_pokemon_labels[i]->getRenderer()->setBackgroundColor(bg_color);
        m_switch_pokemon_labels[i]->getRenderer()->setTextColor(text_color);
        m_switch_pokemon_labels[i]->getRenderer()->setPadding(padding);
        m_switch_group->add(m_switch_pokemon_labels[i], "Pokemon " + to_string(i + 1));

        // Set up signal
        void (BattleUI::*s_func)(Command);
        s_func = &BattleUI::SetLastCommand;
        Command s_command("SWITCH", i);
        m_switch_pokemon_labels[i]->connect("Clicked", s_func, this, s_command);

        void (BattleUI::*sb_func)(bool);
        sb_func = &BattleUI::SetIsUserReady;
        m_switch_pokemon_labels[i]->connect("Clicked", sb_func, this, true);  
    }

    m_switch_pokemon_labels[0]->setPosition("30%", "70%");
    m_switch_pokemon_labels[1]->setPosition("60%", "70%");
    m_switch_pokemon_labels[2]->setPosition("30%", "80%");
    m_switch_pokemon_labels[3]->setPosition("60%", "80%");
    m_switch_pokemon_labels[4]->setPosition("30%", "90%");
    m_switch_pokemon_labels[5]->setPosition("60%", "90%");
}

void BattleUI::InitPokemonUI(tgui::Gui &gui)
{
    // if (!m_player_pokemon_texture.loadFromFile("images/mimikyu.jpg")
    // {
    //     cout << "Cannot load image!" << endl;
    //     return;
    // }
    // m_player_pokemon_sprite.setTexture(m_player_pokemon_texture);
    m_player_pokemon_sprite.setPosition(50.f, 70.f);
    m_sprite_pointers.push_back(&m_player_pokemon_sprite);

    // if (!m_enemy_pokemon_texture.loadFromFile("images/bikabu.jpg"))
    // {
    //     cout << "Cannot load image!" << endl;
    // }
    // m_enemy_pokemon_sprite.setTexture(m_enemy_pokemon_texture);
    m_enemy_pokemon_sprite.setPosition(1000.f, 70.f);
    m_sprite_pointers.push_back(&m_enemy_pokemon_sprite);


    m_player_pokemon_hp_label = tgui::Label::create();
    m_player_pokemon_hp_label->setTextSize(text_size);
    m_player_pokemon_hp_label->setSize("30%", "10%");
    m_player_pokemon_hp_label->setPosition("10%", "0%");
    m_player_pokemon_hp_label->getRenderer()->setBackgroundColor(bg_color);
    m_player_pokemon_hp_label->getRenderer()->setTextColor(text_color);
    m_player_pokemon_hp_label->getRenderer()->setPadding(padding);

    m_enemy_pokemon_hp_label = tgui::Label::create();
    m_enemy_pokemon_hp_label->setTextSize(text_size);
    m_enemy_pokemon_hp_label->setSize("30%", "10%");
    m_enemy_pokemon_hp_label->setPosition("60%", "0%");
    m_enemy_pokemon_hp_label->getRenderer()->setBackgroundColor(bg_color);
    m_enemy_pokemon_hp_label->getRenderer()->setTextColor(text_color);
    m_enemy_pokemon_hp_label->getRenderer()->setPadding(padding);

    gui.add(m_player_pokemon_hp_label);
    gui.add(m_enemy_pokemon_hp_label);
}

// Hides all bottom text bar groups
void BattleUI::HideBottomBar()
{
    for (int i = 0; i < m_text_groups.size(); i++)
    {
        m_text_groups[i]->setVisible(false);
    }
}

// Displays the battle text widgets and waits for user input.
// message: What the label text is set to
void BattleUI::DisplayText(string message)
{
    HideBottomBar();
    m_btext_group->setVisible(true);
    m_btext_label->setText(message);

    WaitForUser();
}

// Displays the fight switch choice widgets and waits for user input.
void BattleUI::DisplayFSChoice()
{
    HideBottomBar();
    m_fs_group->setVisible(true);

    WaitForUser();
}

// Displays the Pokemon Choice widgets
// trainer: Pointer to the trainer who owns the pokemon party.
// pokemon_party: Array containing the pokemon the trainer owns.
// size: The size of the pokemon array.
void BattleUI::DisplayPChoice(Trainer *trainer, Pokemon pokemon_party[], int size)
{
    HideBottomBar();
    m_switch_group->setVisible(true);

    for (int i = 0; i < size; i++)
    {
        if (pokemon_party[i].IsValid())
        {
            sf::String name = pokemon_party[i].GetName();
            m_switch_pokemon_labels[i]->setVisible(true);
            if (trainer->IsLead(&pokemon_party[i]))
            {
                name = name + " (LEAD)";
            }
            if (pokemon_party[i].HasFainted())
            {
                name = name + " (FAINTED)";
            }
            tgui::Label::Ptr sss = m_switch_pokemon_labels[i];
            m_switch_pokemon_labels[i]->setText(name);
            m_switch_pokemon_labels[i]->getRenderer()->setBackgroundColor(bg_color);
        }
        else
        {
            m_switch_pokemon_labels[i]->setText("");
            m_switch_pokemon_labels[i]->getRenderer()->setBackgroundColor(inverse_color);
            m_switch_pokemon_labels[i]->setVisible(false);
        }
    }

    WaitForUser();
}

// Waits until m_is_user_ready has been set to true before setting it to false and proceeding.
void BattleUI::WaitForUser()
{
    while (!GetIsUserReady())
    {
        continue;
    }
    SetIsUserReady(false);
    return;
}

void BattleUI::DrawAllSprites(sf::RenderWindow &window)
{
    for (int i = 0; i < m_sprite_pointers.size(); i++)
    {
        window.draw(*m_sprite_pointers[i]);
    }
}

void BattleUI::SetPlayerPokemonSprite(std::string image_name)
{
    if (!m_player_pokemon_texture.loadFromFile("images/" + image_name))
    {
        cout << "Cannot load image!" << endl;
        return;
    }
    m_player_pokemon_sprite.setTexture(m_player_pokemon_texture);
}

void BattleUI::SetEnemyPokemonSprite(std::string image_name)
{
    if (!m_enemy_pokemon_texture.loadFromFile("images/" + image_name))
    {
        cout << "Cannot load image!" << endl;
        return;
    }
    m_enemy_pokemon_sprite.setTexture(m_enemy_pokemon_texture);
}

#pragma endregion Battle UI