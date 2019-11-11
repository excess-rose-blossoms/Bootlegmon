#include "trainer.h"

#pragma region Trainer

#pragma endregion Trainer

#pragma region Trainer AI

// Makes decisions for the trainer. 
Command TrainerAI::DecideCommand(std::string decision)
{
    Command decided_command;
    if (decision == "SWITCH")
    {
        decided_command.command = "SWITCH";
        decided_command.selection = DecideSwitchedPokemon();
        
        // Do nothing if only one Pokemon
        if (decided_command.selection == -1)
        {
            decided_command.command = "NOTHING";
        }
    }
    else if (decision == "FIGHT")
    {
        decided_command.command = "FIGHT";
        decided_command.selection = -1;
    }
    else 
    {
        decided_command.command = "NOTHING";
        decided_command.selection = -1;
    }

    return decided_command;
}

int TrainerAI::DecideSwitchedPokemon()
{
    for (int i = 0; i < GetNumPokemon(); i++)
    {
        if ((!IsLead(GetPokemon(i))) && (!GetPokemon(i)->HasFainted()))
        {
            return i;
        }
    }
    return -1; //Should never reach here
}

#pragma endregion Trainer AI