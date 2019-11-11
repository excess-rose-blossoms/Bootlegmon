#pragma once

#include <iostream>
#include <string>
#include "pokemon.h"
#include "types.h"

class Trainer
{
    public:
        Trainer() : m_name("???UninitializedTrainer???"){}

        Trainer(Name name, Pokemon p1, Pokemon p2 = Pokemon(), Pokemon p3 = Pokemon(), Pokemon p4 = Pokemon(), Pokemon p5 = Pokemon(), Pokemon p6 = Pokemon())
            : m_pokemon_party{{p1},{p2},{p3},{p4},{p5},{p6}}
        {
            m_name = name;
            // CHANGE
            m_num_pokemon = m_num_live_pokemon = 2;

            m_lead_pokemon = &m_pokemon_party[0];

            //REMOVE
            std::cout << "ADDRESS: " << &m_pokemon_party[0] << std::endl;
            std::cout << "BEFOREIMNAME: " << GetLead()->GetImageName() << std::endl;
            std::cout << "TRAINER INIT DONE" << std::endl;
            std::cout << "ADDRESS: " << &m_pokemon_party[0] << std::endl;
        }

        inline Pokemon *GetLead() { return m_lead_pokemon; }
        
        Pokemon *GetPokemon(int pokemon_num) 
        {
            if (pokemon_num >= m_num_pokemon)
                return nullptr;
            else
                return &m_pokemon_party[pokemon_num];
        }

        void SwitchPokemon(int pokemon_num) 
        { 
            if (pokemon_num >= m_num_pokemon)
                return;
            else
                m_lead_pokemon = &m_pokemon_party[pokemon_num];
        }

        void ListPartyMembers ()
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

        inline Name GetName() { return m_name; }
        // Helper function that returns the name of the lead Pokemon.
        inline Name GetLeadName() { return m_lead_pokemon->GetName(); }
        // Helper function that returns the hp of the lead Pokemon.
        inline Stat GetLeadHP() { return m_lead_pokemon->GetHP(); }
        // Helper function that returns whether the lead Pokemon has fainted.
        inline bool LeadHasFainted() { return m_lead_pokemon->HasFainted(); }
        // Helper funtion that returns the speed of the lead Pokemon.
        inline Stat GetLeadSpd() { return m_lead_pokemon->GetSpd(); }
        // Accessor for m_pokemon_party
        inline Pokemon *GetPokemonParty() { return &m_pokemon_party[0]; }

        // Helper function that takes in a Pokemon pointer and returns whether it is
        // the lead Pokemon.
        inline bool IsLead(Pokemon *pokemon) { return (pokemon == m_lead_pokemon); }

        inline int GetNumPokemon() { return m_num_pokemon; }
        inline int GetNumLivePokemon() { return m_num_live_pokemon; }

        void DecrementNumLivePokemon() { m_num_live_pokemon--; }

        inline Command GetRecentCommand() { return m_recent_command; }

        void SetRecentCommand(Command recent_command) { m_recent_command = recent_command; }

    private:
        Name m_name;
        // The Pokemon that has been sent out to battle at the moment.
        Pokemon *m_lead_pokemon;
        Pokemon m_pokemon_party[6];
        int m_num_pokemon;
        int m_num_live_pokemon;
        //Used to store the most recent command the trainer gave.
        Command m_recent_command;
};

class TrainerAI: public Trainer
{
    public:
        TrainerAI(): Trainer(){}
        TrainerAI(Name name, Pokemon p1, Pokemon p2) : Trainer(name, p1, p2){}

        // Makes decisions for the trainer.
        Command DecideCommand(std::string decision = "NOTHING");

        // Helper function that decides which Pokemon to send in and returns its corresponding number.
        int DecideSwitchedPokemon();
};
