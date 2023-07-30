//---------------------------------------------------------------------------------------------------------------------
// PrimaryGame.hpp
//
// This is the initial file for starting work with the program, where we take the initial gamestate, parse it into a vector,
// check if the command line arguments have the correct values. After we call the game class objects, players, legal moves, etc.,
// we receive commands from the players and send them again to the object responsible for handle input,
// where the gamestate is updated and we get it back.
//
// Gruppe 15
// Authors: Faris Mujcinagic 12127517, Milan Mejic 12034456, Fedja Mulabegovic 12127976
//---------------------------------------------------------------------------------------------------------------------
//
#pragma once

#include <string>
#include <vector>
#include <iostream>

class primaryGame
{
private:
  int argc_;
  const char **argv_;
  char current_side_;

public:
  primaryGame(int argc, const char **argv) : argc_(argc), argv_(argv){};

  //---------------------------------------------------------------------------------------------------------------------
  ///
  // This function appears to be checking if a given word is composed entirely of digits (0-9).
  // It does this by iterating through each character in the word and using the isdigit function to check if the character is a digit.
  // If any character is not a digit, the function returns false. If all characters are digits, the function returns true.
  ///
  /// @param word
  ///
  /// @return true or false
  ///

  bool checkIfValid(char *word);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function is used in order to parse the valid text file.
  /// Idea between this function is to read the file, then using pointer(iterator)
  /// convert content of file to string which saved as 'file_content'.
  /// After that, using by using 'std::stringstream' and simple loop,
  /// string is parsed and string vector (tile_state_vec) is filled with words from file.
  /// Function ends with simple index choosing from the new vector.
  ///
  ///
  /// @param num Passed num, that dictates the index in vector.
  /// According to number we differentiate between remaining cards and seed.
  ///
  /// @return tile_state_vec[size - num], returns specific index of a string vector.
  /// Index is chosen according to our needs, E.g. remaining cards, seed...
  ///
  std::string parseFunc(int num);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This is a validation function, where all of the possible
  /// errors are checked and corresponding exit code is returned to the main func.
  ///
  ///
  /// @return exit code (number), according to the assignment specification
  //
  int checkArgv(char &current_side);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we read from a const char* textfile passed as parameter set it in a string. AFter that we convert
  /// string to vector of strings.
  ///
  ///
  /// @param *textfile passed parameter (text from a file (data type const cchar *))
  ///
  /// @return vector of strings (gamestate message)
  //
  std::vector<std::string> parseTheWords(const char *textfile);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we convert passed vector of strings to string using function accumulate().
  ///
  ///
  /// @param gamestate_vec passed parameter gamestate message (vector of strings)
  ///
  /// @return string of gamestate message
  //
  std::string convertVectorToString(std::vector<std::string> gamestate_vec);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This is originalgame() function where all other functions are called, what means that this function represent the loop
  /// and iteration of the game. It takes gamestate from the command line, with other function get it in the passed vector of string,
  /// prints the inital version of gamestate message, takes input commands from the players, send them to other objects of classes
  /// and get the updated version of gamestate message.
  ///
  /// @return exit code (number), according to the assignment specification
  //
  int originalGame();
};