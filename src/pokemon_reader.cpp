#include "pokemon_reader.h"

using namespace std;

// Constructor
PokemonReader::PokemonReader(std::string file_path)
{
    boost::property_tree::read_json(file_path, root);
}

// Returns a Pokemon object with the stats of the pokemon with name pokemon_name.
Pokemon PokemonReader::MakePoke(std::string pokemon_name)
{
    if (!root.get_child_optional(pokemon_name).is_initialized())
    {
        cout << "Error with PokemonReader::MakePoke - POKEMON NOT FOUND." << endl;
        Pokemon error_poke;
        return error_poke;
    }

    Pokemon new_poke(   root.get<int>(pokemon_name+".dexnum"),
                        pokemon_name,
                        root.get<int>(pokemon_name+".hp"),
                        root.get<int>(pokemon_name+".atk"),
                        root.get<int>(pokemon_name+".spd"));

            return new_poke;
}

// Prints out the stats of the Pokemon with name pokemon_name.
void PokemonReader::MakeReport(std::string pokemon_name)
{
    if (!root.get_child_optional(pokemon_name).is_initialized())
    {
        cout << "Error with PokemonReader::MakeReport - POKEMON NOT FOUND."<< endl;
        return;
    }

    cout << "NAME: " << pokemon_name << endl;
    cout << "DEXNUM: " << root.get<int>(pokemon_name+".dexnum") << endl;
    cout << "HP: " << root.get<int>(pokemon_name+".hp") << endl;
    cout << "ATK: " << root.get<int>(pokemon_name+".atk") << endl;
    cout << "SPD: " << root.get<int>(pokemon_name+".spd") << endl;
}