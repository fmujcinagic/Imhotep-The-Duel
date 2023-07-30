//---------------------------------------------------------------------------------------------------------------------
// mainAI.cpp
//
// Here is the implemention of AI. In this file we have the main function, that starts the whole program. Within
// this function is called object of a class PrimaryGame with which we acces to the originalgame function that runs the game.
// The basic idea of the program is to make an AI that can beat others AI's and a human. It calculates the best possible
// moves through the game and acts accordignly.
//
// Group: Gruppe 15
// Authors: Faris Mujcinagic 12127517, Milan Mejic 12034456, Fedja Mulabegovic 12127976
//---------------------------------------------------------------------------------------------------------------------
//
#include <iostream>
#include "PrimaryGameAI.hpp"

//---------------------------------------------------------------------------------------------------------------------
///
/// This is the main function, here we call object of a class and starts the game. It checks for 0 (the return value of
/// original game function when the program ends) in order to stop the program.
///
/// @param argc number of arguments from the command line
/// @param argv arguments from command line
///
//
int main(int argc, const char **argv)
{
  PrimaryGameAI primary_game(argc, argv);
  if (primary_game.originalGame() == 0)
  {
    return 0;
  }
  return 0;
}