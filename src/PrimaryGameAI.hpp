//---------------------------------------------------------------------------------------------------------------------
// PrimaryGameAI.hpp
//
// This is a header file, where all of the functions are initialised. Similar methods have been used prior this
// for PrimaryGame.hpp for previous assignments. This is upgraded version, which is suitable for fully
// functional AI.
//
// Group: Gruppe 15
// Authors: Faris Mujcinagic 12127517, Milan Mejic 12034456, Fedja Mulabegovic 12127976
//---------------------------------------------------------------------------------------------------------------------
//

#pragma once
#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <vector>
#include <algorithm>

#include <network/Client.hpp>
#include <sstream>
#include "network/Server.hpp"
#include "AImove.hpp"

class PrimaryGameAI
{
private:
  int argc_;
  const char **argv_;
  char current_side_;

public:
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This is a constructor for the "PrimaryGameAI" class which takes in two arguments, an integer "argc" and an array
  /// of strings "argv". The ": argc_(argc), argv_(argv)" is called member initializer list, it is used to
  /// initialize the member variables of the class with the input arguments in the constructor.
  ///
  /// @param argc int
  /// @param argv const char **
  //
  PrimaryGameAI(int argc, const char **argv) : argc_(argc), argv_(argv){};

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function given word (a char array) is valid. It iterates through each character in the word and check
  /// if it is a digit using the isdigit() function. If any character is not a digit, the function returns false.
  ///
  /// @param word char* Given word for check
  ///
  /// @return true or false
  //
  bool checkIfValid(char *word);

  /// This function uses a stringstream object to iterate through the input string and extract substrings
  /// separated by spaces. Each substring is then added to the vector using the push_back() method.
  /// The function then returns the vector of substrings
  ///
  /// @param turn char 1 or 0
  ///
  /// @return int 1 or 0
  //
  int playerTurn(char turn);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This is a handy function which converts given string to vector.
  /// It is usually used, when specific index is needed.
  ///
  /// @param string_for_conversion std::string To convert
  ///
  /// @return std::vector<std::string> Converted vector
  //
  std::vector<std::string> convertStringToVector(std::string string_for_conversion);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function takes in an integer "num" and a string "string_for_parse" as input. It converts the input string
  /// to a vector of strings with the string "GameState ". It then find the last element of the vector
  /// (by using the "num" input) and returns it as a string.
  ///
  /// @param num Desired index
  /// @param string_for_parse
  ///
  /// @return std::string tile_state_vec[size - num]
  ///
  std::string parseFunc(int num, std::string string_for_parse);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function sets up the state of a game for an AI player by creating instances(objects) of several classes
  /// and determines a move for the AI player using the "AImove" class.
  ///
  /// @param tile_state TileDeck::TileState object
  /// @param gameStateMessage std::string current gamestate message
  ///
  ///
  /// @return determine_move
  ///
  auto setObjectsForGame(TileDeck::TileState &tile_state, std::string gameStateMessage);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function client object is used to send our AI move to the server. It also calls our main function
  /// getMove() which gives final move. There is also check in case user exits, or disconnects and crash is avoided.
  ///
  /// @return 0
  ///
  int originalGame();
};