#pragma once

#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "pokemon.h"

class PokemonReader
{
    public:
        // Constructor
        PokemonReader(std::string file_path);

        // Returns a Pokemon object with the stats of the pokemon with name pokemon_name.
        Pokemon MakePoke(std::string pokemon_name);

        // Prints out the stats of the Pokemon with name pokemon_name. Used for debugging.
        void MakeReport(std::string pokemon_name);

    private:
        boost::property_tree::ptree root;
};
