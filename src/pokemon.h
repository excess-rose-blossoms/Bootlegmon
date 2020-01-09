#pragma once

#include <iostream>
#include <string>
#include "types.h"
#include "moves.h"

class Pokemon
{
    public:
        Pokemon(int dex_number, Name name, Stat hp, Stat atk, Stat spd, Move m1 = Move(), Move m2 = Move(), Move m3 = Move(), Move m4 = Move());
        Pokemon() : m_name(InvalidPokemonName) {}

        void TakeDamage(int damage) { m_hp = ((m_hp - damage) >= 0) ? (m_hp - damage) : 0; }

        bool HasFainted() { return (m_hp <= 0); }
        bool IsValid() { return m_name != InvalidPokemonName; }

        #pragma region Accessors/Mutators

        inline int GetDexNumber() { return m_dex_number; }
        inline Name GetName() { return m_name; }
        inline Stat GetHP() { return m_hp; }
        inline Stat GetAtk() { return m_atk; }
        inline Stat GetSpd() { return m_spd; }
        inline std::string GetImageName() { return m_image_name; }

        void SetDexNumber(int dex_number) { m_dex_number = dex_number; }
        void SetName(Name name) { m_name = name; }
        void SetHP(Stat hp) { m_hp = hp; }
        void SetAtk(Stat atk) { m_atk = atk; }
        void SetSpd(Stat spd) { m_spd = spd; }

        Move GetMove(int index) { return m_moves[index]; }
        void SetMove(int index, Move move) { m_moves[index] = move; }

#pragma endregion Accessors / Mutators

        // Prints out a report on the Pokemon's stats. Used for debugging purposes.
        void MakeReport();

    private:
        int m_dex_number;
        Name m_name;
        Stat m_hp;
        Stat m_atk;
        Stat m_spd;

        Move m_moves[4];

        std::string m_image_name;
};