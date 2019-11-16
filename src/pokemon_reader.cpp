#include "pokemon_reader.h"

using namespace std;

// Constructor
PokemonReader::PokemonReader(std::string pokemon_file_path, std::string move_file_path)
{
    boost::property_tree::read_json(pokemon_file_path, poke_root);
    boost::property_tree::read_json(move_file_path, move_root);
}

// Returns a Pokemon object with the stats of the pokemon with name pokemon_name.
Pokemon PokemonReader::MakePoke(std::string pokemon_name)
{
    if (!poke_root.get_child_optional(pokemon_name).is_initialized())
    {
        cout << "Error with PokemonReader::MakePoke - POKEMON NOT FOUND." << endl;
        Pokemon error_poke;
        return error_poke;
    }

    Pokemon new_poke(   poke_root.get<int>(pokemon_name+".dexnum"),
                        pokemon_name,
                        poke_root.get<int>(pokemon_name+".hp"),
                        poke_root.get<int>(pokemon_name+".atk"),
                        poke_root.get<int>(pokemon_name+".spd"));

            return new_poke;
}

// Prints out the stats of the Pokemon with name pokemon_name.
void PokemonReader::MakePokemonReport(std::string pokemon_name)
{
    if (!poke_root.get_child_optional(pokemon_name).is_initialized())
    {
        cout << "Error with PokemonReader::MakeReport - POKEMON NOT FOUND."<< endl;
        return;
    }

    cout << "NAME: " << pokemon_name << endl;
    cout << "DEXNUM: " << poke_root.get<int>(pokemon_name+".dexnum") << endl;
    cout << "HP: " << poke_root.get<int>(pokemon_name+".hp") << endl;
    cout << "ATK: " << poke_root.get<int>(pokemon_name+".atk") << endl;
    cout << "SPD: " << poke_root.get<int>(pokemon_name+".spd") << endl;
}