#include "battle_system.h"

using namespace std;

BattleSystem::BattleSystem(BattleUI *battle_ui, PokemonReader *pokemon_reader) 
    :   m_is_battle_over(false), m_battle_ui(battle_ui), m_pokemon_reader(pokemon_reader), 
        m_player_trainer("PLAYER", pokemon_reader->MakePoke("Mimikyu"), pokemon_reader->MakePoke("Bibikyu")),
        m_enemy_trainer("ENEMY", pokemon_reader->MakePoke("Pikachu"), pokemon_reader->MakePoke("Bikabu"))
{
    // Initialize UI
    m_battle_ui->SetPlayerPokemonHpLabelText(to_string(m_player_trainer.GetLeadHP()));
    m_battle_ui->SetEnemyPokemonHpLabelText(to_string(m_enemy_trainer.GetLeadHP()));
    m_battle_ui->SetPlayerPokemonSprite(m_player_trainer.GetLead()->GetImageName());
    m_battle_ui->SetEnemyPokemonSprite(m_enemy_trainer.GetLead()->GetImageName());
}

// Prompts the player to choose.
Command BattleSystem::GetPlayerInput()
{
    Command input_command;
    do 
    {
        // Read in input
        m_battle_ui->DisplayFSChoice();
        

        if (m_battle_ui->GetLastCommand().command == "FIGHT")
        {
            // Old implementation
            // input_command.command = "FIGHT";
            // input_command.selection = -1;

            Command move_command = GetPlayerMoveInput();

            input_command.command = move_command.command;
            input_command.selection = (move_command.command == "BACK") ? -1 : move_command.selection;
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

// Brings up the move selection menu and sets the player command accordingly.
Command BattleSystem::GetPlayerMoveInput()
{
    Command move_command;

    m_battle_ui->DisplayMChoice(&m_player_trainer);
    move_command = m_battle_ui->GetLastCommand();
    return move_command;
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

// Meant to be called in place of RunTurn. Used for debugging or prototyping new features.
void BattleSystem::RunTurnDebug()
{
    while (true)
    {
        Command debug_command = GetPlayerInput();
        cout << "COMMAND: " << debug_command.command << endl;
        cout << "SELECTION: " << debug_command.selection << endl;
    }
    
    return;
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
        Command recent_command = trainers_in_order[i]->GetRecentCommand();
        if (recent_command.command == "FIGHT")
        {
            // Decrement health appropriately using the move!
            int target_index = (i == 0) ? 1 : 0;
            Move used_move = trainers_in_order[i]->GetLead()->GetMove(recent_command.selection);
            int damage_taken = used_move.power;
            trainers_in_order[target_index]->GetLead()->TakeDamage(damage_taken);
            //trainers_in_order[i]->GetLead()->Attack(trainers_in_order[target_index]->GetLead());

            if (trainers_in_order[target_index] == &m_player_trainer)
            {
                m_battle_ui->SetPlayerPokemonHpLabelText(to_string(m_player_trainer.GetLeadHP()));
            }
            else 
            {
                m_battle_ui->SetEnemyPokemonHpLabelText(to_string(m_enemy_trainer.GetLeadHP()));
            }

            m_battle_ui->DisplayText(trainers_in_order[i]->GetLeadName() + " used " + used_move.name + "! " +
                                     trainers_in_order[target_index]->GetLeadName() + " took " + to_string(damage_taken) + " damage!");
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
                
                // Move on to the next turn
                break;
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
