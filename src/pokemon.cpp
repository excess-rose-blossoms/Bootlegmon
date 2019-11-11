#include "pokemon.h"

using namespace std;

Pokemon::Pokemon(int dex_number, Name name, Stat hp, Stat atk, Stat spd) : m_dex_number(dex_number), m_name(name), m_hp(hp), m_atk(atk), m_spd(spd) 
{
    m_image_name = name + ".jpg";
}

// Prints out a report on the Pokemon's stats. Used for debugging purposes.
void Pokemon::ReportStats()
{
    cout << "POKEMON: " << m_name << endl;
    cout << "DEX NUMBER: " << m_dex_number << endl;
    cout << "HP: " << m_hp << endl;
    cout << "ATK: " << m_atk << endl;
    cout << "SPD: " << m_spd << endl;
}