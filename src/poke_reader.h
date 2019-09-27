#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "pokemon.h"

class PokeReader
{
    public:
        // Returns a Pokemon by reading the appropriate line from the file "pokedex.txt".
        // dex_number: Corresponds to the dex number/line in the text file to read.
        Pokemon ReadPokemon(int dex_number)
        {
            std::ifstream file("pokedex.txt");
            std::string line;
            int counter = 0;

            while (std::getline(file, line) && counter <= dex_number)
            {
                if (counter == dex_number)
                {
                    std::vector<std::string> pokemon_data = split(line);
                    Pokemon return_pokemon(dex_number, pokemon_data[0], stoi(pokemon_data[1]), stoi(pokemon_data[2]), stoi(pokemon_data[3]));
                    return return_pokemon;
                }
                counter++;
            }
            
            // Should not reach here
            Pokemon error_pokemon;
            return error_pokemon;
        }

        // Helper function to split a line of Pokemon data based on commas, and return a vector with the split line.
        std::vector<std::string> split(const std::string & line)
        {
            std::vector<std::string> parts;
            std::string data = "";

            for (int i = 0; i < line.size(); i++)
            {
                if (line[i]==',')
                {
                    parts.push_back(data);
                    data = "";
                    continue;
                }
                data = data + line[i];
            }
            if (data!="")
                parts.push_back(data);

            return parts;
        }
};