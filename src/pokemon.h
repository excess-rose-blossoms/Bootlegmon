#pragma once

#include <iostream>
#include <string>
#include "types.h"

class Pokemon
{
    public:
        Pokemon(int dex_number, Name name, Stat hp, Stat atk, Stat spd) : m_dex_number(dex_number), m_name(name), m_hp(hp), m_atk(atk), m_spd(spd) {}
        Pokemon() : m_name(InvalidPokemonName){}

        int Attack(Pokemon *enemy) 
        { 
            enemy->TakeDamage(m_atk);
            return m_atk;
        }

        void TakeDamage(int damage) { m_hp = ((m_hp - damage) >= 0) ? (m_hp - damage) : 0; }

        bool HasFainted() { return (m_hp <= 0); }
        bool IsValid() { return m_name != InvalidPokemonName; }

        #pragma region Accessors/Mutators

        inline int GetDexNumber() { return m_dex_number; }
        inline Name GetName() { return m_name; }
        inline Stat GetHP() { return m_hp; }
        inline Stat GetAtk() { return m_atk; }
        inline Stat GetSpd() { return m_spd; }

        void SetDexNumber(int dex_number) { m_dex_number = dex_number; }
        void SetName(Name name) { m_name = name; }
        void SetHP(Stat hp) { m_hp = hp; }
        void SetAtk(Stat atk) { m_atk = atk; }
        void SetSpd(Stat spd) { m_spd = spd; }

        #pragma endregion Accessors / Mutators

        // Prints out a report on the Pokemon's stats. Used for debugging purposes.
        void ReportStats()
        {
            std::cout << "DEX NUMBER: " << m_dex_number << std::endl;
            std::cout << "POKEMON: " << m_name << std::endl;
            std::cout << "HP: " << m_hp << std::endl;
            std::cout << "ATK: " << m_atk << std::endl;
            std::cout << "SPD: " << m_spd << std::endl;
        }

    private:
        int m_dex_number;
        Name m_name;
        Stat m_hp;
        Stat m_atk;
        Stat m_spd;
};