//---------------------------------------------------------------------------------------------------------------------
// main.cpp
//
// In this Assignment, it is checked for command line arguments,
// and according to possible errors / invalid inputs, corresponding exit codes
// are returned. Also, at the end of the assignment, NET-ENUM values are
// parsed from valid file and sent to funciton (void send(const std::string &content);).
//
// Group: Gruppe 15
// Authors: Faris Mujcinagic 12127517, Milan Mejic 12034456, Fedja Mulabegovic 12127976
//---------------------------------------------------------------------------------------------------------------------
//
#include "Parse.hpp"
#include "TileDeck.hpp"
#include "network/Server.hpp"
#include "network/Exceptions.hpp"
#include "LegalMoves.hpp"
#include "PlayerStats.hpp"
#include "HandleGame.hpp"
#include "PrimaryGame.hpp"

#include <bits/stdc++.h>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, const char **argv)
{
  primaryGame primary_game(argc, argv);

  if (primary_game.originalGame() == 0)
  {
    return 0;
  }

  return 0;
}
