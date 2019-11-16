#include "trainer.h"

#pragma region Trainer

// Constructor
Trainer::Trainer(Name name, Pokemon p1, Pokemon p2, Pokemon p3, Pokemon p4, Pokemon p5, Pokemon p6)
    :   m_pokemon_party{{p1},{p2},{p3},{p4},{p5},{p6}}
{
    m_name = name;

    // Count the number of Pokemon in the party
    m_num_pokemon = m_num_live_pokemon = 0;
    for (int i = 0; i < 6; i++)
    {
        if (m_pokemon_party[i].GetName() == InvalidPokemonName)
        {
            break;
        } 
        else
        {
            m_num_pokemon++;
            m_num_live_pokemon++;
        }
    }
    
    m_lead_pokemon = &m_pokemon_party[0];
}

// Returns the Pokemon at the party slot specified by pokemon_num.
Pokemon *Trainer::GetPokemon(int pokemon_num)
{
    if (pokemon_num >= m_num_pokemon)
        return nullptr;
    else
        return &m_pokemon_party[pokemon_num];
}

// Changes the lead Pokemon to the Pokemon at the party slot specified by pokemon_num
void Trainer::SwitchPokemon(int pokemon_num)
{
    if (pokemon_num >= m_num_pokemon)
        return;
    else
        m_lead_pokemon = &m_pokemon_party[pokemon_num];
}

// Prints out a list of Pokemon in the trainer's party along with their condition.
void Trainer::ListPartyMembers()
{
    Name pokemon_name;
    Name suffix;
    for (int i = 0; i < m_num_pokemon; i++)
    {
        if (m_pokemon_party[i].HasFainted())
            suffix = " (fainted)";
        else if (&m_pokemon_party[i] == m_lead_pokemon)
            suffix = " (lead)";
        else
            suffix = "";

        pokemon_name = m_pokemon_party[i].GetName() + suffix;
        std::cout << i << ": " << pokemon_name << std::endl;
    }
}

#pragma endregion Trainer

#pragma region Trainer AI

// Makes decisions for the trainer. 
Command TrainerAI::DecideCommand(std::string decision)
{
    Command decided_command;
    if (decision == "SWITCH")
    {
        decided_command.command = "SWITCH";
        decided_command.selection = DecideSwitchedPokemon();
        
        // Do nothing if only one Pokemon
        if (decided_command.selection == -1)
        {
            decided_command.command = "NOTHING";
        }
    }
    else if (decision == "FIGHT")
    {
        decided_command.command = "FIGHT";
        decided_command.selection = -1;
    }
    else 
    {
        decided_command.command = "NOTHING";
        decided_command.selection = -1;
    }

    return decided_command;
}

int TrainerAI::DecideSwitchedPokemon()
{
    for (int i = 0; i < GetNumPokemon(); i++)
    {
        if ((!IsLead(GetPokemon(i))) && (!GetPokemon(i)->HasFainted()))
        {
            return i;
        }
    }
    return -1; //Should never reach here
}

#pragma endregion Trainer AI