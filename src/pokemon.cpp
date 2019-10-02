#include "pokemon.h"

Pokemon::Pokemon(int dex_number, Name name, Stat hp, Stat atk, Stat spd) : m_dex_number(dex_number), m_name(name), m_hp(hp), m_atk(atk), m_spd(spd) 
{
    m_image_name = name + ".jpg";
}