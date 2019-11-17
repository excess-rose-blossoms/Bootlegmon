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
    // Validity check
    if (!poke_root.get_child_optional(pokemon_name).is_initialized())
    {
        cout << "Error with PokemonReader::MakePoke - POKEMON NOT FOUND." << endl;
        Pokemon error_poke;
        return error_poke;
    }

    // Init Pokemon
    Pokemon new_poke(   poke_root.get<int>(pokemon_name+".dexnum"),
                        pokemon_name,
                        poke_root.get<int>(pokemon_name+".hp"),
                        poke_root.get<int>(pokemon_name+".atk"),
                        poke_root.get<int>(pokemon_name+".spd"));

    // Get moves from learnset
    int counter = 0;
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, poke_root.get_child(pokemon_name+".movelearnset"))
    {
        Move new_move = MakeMove(v.second.get<int>(""));
        new_poke.SetMove(counter, new_move);
        counter++;
    }

    return new_poke;
}

// Returns a Pokemon object with the stats of the pokemon with name pokemon_name.
Move PokemonReader::MakeMove(int move_index)
{
    string move_index_string = to_string(move_index);

    if (!move_root.get_child_optional(move_index_string).is_initialized())
    {
        cout << "Error with PokemonReader::MakeMove - MOVE NOT FOUND."<< endl;
        Move error_move;
        return error_move;
    }

    Move new_move(  move_index,
                    move_root.get<string>(move_index_string + ".name"),
                    move_root.get<int>(move_index_string + ".power"),
                    move_root.get<int>(move_index_string + ".accuracy"));

    return new_move;
}

// Prints out the stats of the Pokemon with name pokemon_name.
void PokemonReader::MakePokemonReport(std::string pokemon_name)
{
    if (!poke_root.get_child_optional(pokemon_name).is_initialized())
    {
        cout << "Error with PokemonReader::MakePokemonReport - POKEMON NOT FOUND."<< endl;
        return;
    }

    cout << "NAME: " << pokemon_name << endl;
    cout << "DEXNUM: " << poke_root.get<int>(pokemon_name+".dexnum") << endl;
    cout << "HP: " << poke_root.get<int>(pokemon_name+".hp") << endl;
    cout << "ATK: " << poke_root.get<int>(pokemon_name+".atk") << endl;
    cout << "SPD: " << poke_root.get<int>(pokemon_name+".spd") << endl;
}

// Prints out the stats of the Pokemon with name pokemon_name.
void PokemonReader::MakeMoveReport(int move_index)
{
    string move_index_string = to_string(move_index);

    if (!move_root.get_child_optional(move_index_string).is_initialized())
    {
        cout << "Error with PokemonReader::MakeMoveReport - MOVE NOT FOUND."<< endl;
        return;
    }

    cout << "INDEX: " << move_index_string << endl;
    cout << "NAME: " << move_root.get<string>(move_index_string+".name") << endl;
    cout << "POWER: " << move_root.get<int>(move_index_string+".power") << endl;
    cout << "ACCURACY: " << move_root.get<int>(move_index_string+".accuracy") << endl;
    cout << endl;
}

// Blank function used for prototyping.
void PokemonReader::PrototypeGet()
{
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, poke_root.get_child("Mimikyu.movelearnset"))
    {
        Move new_move = MakeMove(v.second.get<int>(""));
    }
}