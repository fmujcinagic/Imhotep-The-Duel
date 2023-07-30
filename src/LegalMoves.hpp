
//---------------------------------------------------------------------------------------------------------------------
// LegalMoves.hpp
//
// In this file, we are proving if the message from the server(player move) is legal or not.
// Couple of the functions are called which are responsible for checking.
// Group: Gruppe 15
// Authors: Faris Mujcinagic 12127517, Milan Mejic 12034456, Fedja Mulabegovic 12127976
//---------------------------------------------------------------------------------------------------------------------
//

#pragma once
#include "Parse.hpp"
#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <vector>

class legalMoves
{

private:
  bool term_for_placed_meeple_by_action_token_ = false;
  std::string meeple_place_by_action_token_ = "";

public:
  std::string message_content_;
  parseInput parse_object;
  std::string action_message_;
  std::string action_num_;
  std::string take_tile_action_num_ = "";
  std::string second_swap_action_num_;
  std::string third_swap_action_num_;
  std::vector<std::string> action_message_vec_;
  std::vector<std::string> action_tokens_vec_;

  int current_player_turn_;

  std::vector<std::string> board_;
  std::vector<std::vector<std::string>> boats_;
  std::string first_row_;
  std::string second_row_;
  std::string third_row_;
  std::string first_col_;
  std::string second_col_;
  std::string third_col_;

  std::map<char, std::string> meeple_map;

  legalMoves(parseInput obj) : parse_object(obj){};

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function appears to be checking whether or not an unload/take tile action is legal.
  /// It does this by checking if the given action number is in the meeple_map and if the take_tile_action_num_ is between 0 and 2 (inclusive).
  /// If both of those conditions are true, it gets the tile name from the boats_ array using the action number and take_tile_action_num_ as indices.
  /// If the tile name is not "None" and is not found in the checkForActionTileName function, the function returns true. Otherwise, it returns false.
  ///
  /// @param temp
  ///
  /// @return true or false
  ///
  bool checkForUnloadTakeTile(std::string temp);

  //---------------------------------------------------------------------------------------------------------------------
  /// The function first assigns the board and boats arrays returned
  /// by the parse_object.getBoard() and parse_object.getBoats() methods, respectively,
  /// to the board_ and boats_ member variables of the legalMoves object.
  ///
  //
  void intializeValues();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function appears to be checking if a swap action is valid by checking if the two positions specified in the
  /// second_swap_action_num_ and third_swap_action_num_ variables are within the range of valid positions for a boat
  /// and if the positions are not empty.
  /// It does this by looping through the meeple_map and checking if the first element of action_num_ (which may represent the index of a boat in the boats_ array)
  /// matches an element in the meeple_map. If a match is found, it checks the range of the positions specified in second_swap_action_num_ and
  /// third_swap_action_num_, and then retrieves the elements at those positions in the boats_ array and stores them in the temp and second_temp variables.
  /// If the positions are not within the valid range or if the retrieved elements are empty (i.e. equal to "None "), the function returns false.
  /// Otherwise it returns true.
  ///
  /// @param temp
  /// @param second_temp
  ///
  /// @return true or false
  ///
  bool checkForUnloadSwap(std::string temp, std::string second_temp);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function appears to be checking if the contents of action_message_vec_ are valid for a given action token, as determined by the declareActionToken function.
  /// It is using the isdigit function to check if the elements of action_message_vec_ are digits (i.e. numeric characters). If the elements are digits and
  /// the number of elements matches the expected format for the given action token, the function returns true, otherwise it returns false.
  ///
  /// @return true or false
  ///
  bool declareStateOfActionToken();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// The code defines a function named checkForUnload that is a member of the legalMoves class.
  /// The function checks if the conditions for the action of "Unloading a boat" are met.
  /// It takes no arguments and returns a boolean value of true if the conditions are met and false if they are not.
  /// The function first checks if the action message is "PlaceMeeple". If it is, the function returns true without further checking. Otherwise,
  /// it checks if the action message is "UnloadBoat" and the action number is between 0 and 5. If these conditions are met,
  /// the function checks the meeple_map for the boat with the action number and counts the number of - characters in it. If the number of - characters is
  /// less than or equal to 1 and the boat is not empty, the function returns true.
  /// If these conditions are not met, or the action message is not "PlaceMeeple" or "UnloadBoat", the function returns false.
  ///
  /// @return true or false
  ///
  bool checkForUnload();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Checks for the syntax error (invalid number of spaces , invalid digit or given string),
  // and returns true or false correspondingly.
  ///
  /// @return bool
  //
  bool checkForSyntaxError();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function sets the message that's received from the server
  /// and transforms it to action message e.g. "PlaceMeeples"
  /// and action num e.g. "3". It also checks for syntax error from server.
  ///
  /// @param message The original message from server.
  /// @param flag_for_syntax_error Syntax error received from the server.
  ///
  void set(std::string message, bool flag_for_syntax_error);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function is using to set the value of the action_tokens_vec_ member variable, which is a vector of strings.
  /// It takes three parameters: comment_pl_0, comment_pl_1, and current_player_turn.
  /// The current_player_turn parameter is used to determine which of the comment_pl_0 or comment_pl_1 vectors should be used to set
  /// the value of action_tokens_vec_. The function then loops over the chosen vector and adds each string in the vector to action_tokens_vec_.
  /// This function is used to initialize the action_tokens_vec_ variable with the list of available actions for a particular player.
  ///
  /// @param comment
  /// @param comment2
  /// @param current_player_turn
  ///
  /// @return none
  ///
  void setActionArray(std::vector<std::string> comment, std::vector<std::string> comment2, int current_player_turn);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function appears to be used to determine the type of action represented by a given set of action_message_vec_ strings.
  /// It does this by checking the size of the action_message_vec_ vector and then using the findActionTokenInMessage function to
  /// search for specific action tokens within the vector. Depending on the size of the vector and the action tokens it contains,
  /// the function returns a value indicating the type of action. If the type of action cannot be determined, the function returns -1.
  ///
  /// @return -1, 0, 1, 2, 3, 4
  ///
  int declareActionToken();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Checks if the desired boat is unloadable.
  /// If the penultimate(before the last one) boat is unloaded,
  /// it's going to be replaced with the 'None' tokens.
  ///
  /// @param token_to_find
  ///
  /// @return bool
  ///
  bool checkForActionTileName(std::string token_to_find);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function converts a given vector of strings into a single string.
  /// It uses the std::accumulate algorithm to concatenate all of the strings in the vector into a single string,
  /// and then it returns the resulting string.
  /// The std::string class's += operator is used to concatenate the strings together,
  /// and a lambda function is used to specify how the concatenation should be performed.
  ///
  /// @param vector_for_conversion
  ///
  /// @return converted_vector
  ///
  std::string convertVectorToString(std::vector<std::string> vector_for_conversion);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function converts a given string into a vector of strings, splitting the original string into separate words using whitespace as a delimiter.
  /// The stringstream class is used to iterate over each word in the string, and the getline method is used to extract each word and store it in a
  /// temporary string variable called tmp.
  /// The tmp string is then added to the converted_string vector, and when all of the words have been processed,
  /// the function returns the converted_string vector.
  ///
  /// @param string_for_conversion
  ///
  /// @return converted_string
  ///
  std::vector<std::string> convertStringToVector(std::string string_for_conversion);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function searches for the action_token within a different vector of strings called action_message_.
  /// The convertVectorToString function is likely used to convert the vector of strings into a single string,
  /// and then the find method is used to search for the action_token within this string.
  /// If the action_token is found, the function returns true, otherwise it returns false.
  ///
  /// @param action_token
  ///
  /// @return true or false

  bool findActionTokenInMessage(std::string action_token);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function is searching for a given action_token within a vector of strings called action_tokens_vec_.
  /// The convertVectorToString function is likely used to convert the vector
  /// of strings into a single string, and then the find method is used to search
  /// for the action_token within this string. If the action_token is found, the function returns true, otherwise it returns false.
  ///
  /// @param action_token
  ///
  /// @return true or false
  ///
  bool findActionTokenInComment(std::string action_token);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function is counting the number of times a given action_token appears in a vector of strings called action_message_vec_.
  /// It does this by looping over each string in the vector and using the find method to search for the action_token within each string.
  /// If the action_token is found, it increments a counter variable.
  /// When the loop is finished, the function returns the final count of how many times the action_token was found.
  ///
  /// @param action_token
  ///
  /// @return count
  ///
  int findOccurencesOfActionToken(std::string action_token);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function is setting the values of two member variables in the legalMoves class.
  /// The temporary_action_num and temporary_action_message parameters are used to set the action_num_ and action_message_ member variables.
  ///
  /// @param temporary_action_num
  /// @param temporary_action_message
  ///
  /// @return none
  ///
  void setValuesBack(std::string temporary_action_num, std::string temporary_action_message);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This code defines a function called checkForPlaceMeepleAction() in a class called legalMoves.
  /// First of all, this function determines whether the message is valid.
  /// If the condition is true, the next step depends on the vector's size.
  /// it will arrange the vector's elements into groups at specific locations.
  /// The checkformeeples and checkforunload functions will then be reviewed.
  /// Action-messages and action-numbers will be organised into a specific positions of a vector if they both return a true value in that case.
  /// and the checkforplaceunload() function will produce a true result.
  /// If any of these are false, this function will return a false result.
  ///
  /// @return true or false
  ///
  bool checkForPlaceUnload();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function determines whether the message is valid.
  /// If the condition is true, the next step depends on the vector's size.
  /// The bigger its size is, the more meeples it will place on the board
  /// The checkformeeples function and first action_num will then be reviewed.
  /// If they are all valid, the return value will be true.
  ///
  /// @return true or false
  ///
  bool checkForPlaceMeepleAction();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This code defines a function called checkForTakeTileAction() in a class called legalMoves.
  /// The function returns a bool value indicating whether a certain type of action is valid or not.
  /// This action is a "TakeTile" action, which involves taking a single tile from the boat.
  /// The method first checks if the findActionTokenInMessage() and findActionTokenInComment() methods,
  /// which are also member functions of the legalMoves class, return true when passed the strings
  /// "TakeTile" and "TakeTile", respectively.
  /// If both of these methods return true, afterwards the function will check if the size of the action_message_vec_ vector is 3
  /// If it is true, the grouping of the final message will be done. Lastly the unload condition will be checked,
  /// if the return value is true, this function
  /// will also return true, otherwise, it will return false.
  ///
  /// @return true or false
  ///
  bool checkForTakeTileAction();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This code defines a function called checkForSwapUnloadAction() in a class called legalMoves.
  /// The function returns a bool value indicating whether a "SwapUnload" action, which involves swapping a tile
  /// and unloading a boat in a single turn is, is valid or not.
  /// If the size of the vector is 6 and the return value from checkforUnload function is true, the return value from
  /// checkForSwapUnloadAction() will also be true.
  ///
  /// @return true or false
  ///
  bool checkForSwapUnloadAction();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This code defines a function called checkIfCardPlayable() in a class called legalMoves.
  /// The function returns a bool value indicating whether a card is playable or not.
  /// To determine if a card is playable, the function has to recieve a true value from a if condition.
  ///
  /// @return true or false
  ///
  bool checkIfCardPlayable();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function is used to check whether a PlaceMeeple action is legal based on the current state of the game board.
  /// It first checks that the action_message_ is indeed "PlaceMeeple". If this is the case, it checks that the value
  /// of the action_num_ variable is a valid index for the game board (between 0 and 8 inclusive) and that the board space at that index is
  /// not already occupied (-1 indicates an empty space on the board). If these conditions are met, the function returns true,
  /// indicating that the PlaceMeeple action is legal.
  /// Otherwise, the function returns false, indicating that the action is not legal.
  ///
  /// @return bool
  //
  bool checkForMeeples();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Checks for the end of the game according to the number of
  /// 'None' tokens on the boats.
  ///
  /// @return bool
  ///
  bool checkForEnd();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function calls all previous defined functions
  /// and correspondigly returned the current gamestates string.
  ///
  /// @return std::string current gamestate
  ///
  std::string checkIfLegal();
};