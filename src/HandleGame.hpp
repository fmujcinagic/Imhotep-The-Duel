
//---------------------------------------------------------------------------------------------------------------------
// HandleGame.hpp
//
// Here we check for available cards, connect the numbers with tokens based on three random numbers to load the boats if needed.
// Also we update the tokens on boats and meeples on board. In the last process we update gamestate message based on called
// functions and checks for legal moves.
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
#include "Parse.hpp"
#include "TileDeck.hpp"
#include "network/Server.hpp"
#include "network/Exceptions.hpp"
#include "LegalMoves.hpp"
#include "PlayerStats.hpp"
#include "HandleGame.hpp"

class handleGame
{
private:
  //--------------------------------------------------------------------------------------------------------------------
  /// Variables
  ///
  int pl_ID_;
  char current_side_;
  int comment_pl0_beginning_, comment_pl1_beginning_;
  std::string message_, words_;
  std::vector<std::string> words_vec_;
  bool flag_for_syntax_error_, swap_is_finished, swap_unload_active_;
  bool catch_error_;
  int number_of_iterations, place_unload_double_active_;
  bool place_2_meeples_active, take_tile_active_, place_3_meeples_active;
  std::string second_action_num_for_unload_;
  std::vector<std::string> comment_array_pl0_vec_, comment_array_pl1_vec_;
  std::vector<std::vector<std::string>> boats;
  std::vector<std::string> vec_of_boats;
  std::map<char, std::string> boat_map;
  std::string first_boat, second_boat, third_boat;
  std::string fourth_boat, fifth_boat, sixth_boat;
  std::vector<std::string> vec_after_place_meeples, vec_to_be_compared_for_final;
  std::vector<std::string> vec_after_place_double_meeples, vec_after_place_triple_meeples;
  std::string final_meeple_string, string_after_place_unload, updated_player_turn;
  std::string string_after_place_unload_double, current_list_of_indexes;
  std::string updated_text_file;
  std::string num_of_tile_deck, updated_boats;
  int num_of_reserved_cards;
  std::map<char, std::string> indexes_of_meeples;
  std::map<int, std::string> random_map_;

public:
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This is the constructor for the handleGame class. It initializes the words_vec_ member variable with a given words_ vector of strings,
  /// and then uses the std::accumulate algorithm to concatenate all of the strings in the words_ vector into a single string.
  /// The resulting string is then stored in the words variable.
  /// The handleGame class also has a member variable called random_map_,
  /// which is initialized in the constructor with a map containing a list of possible actions and corresponding action numbers.
  /// This map is used to determine the meaning of a given action number within the context of the game.
  //
  handleGame(std::vector<std::string> words_);

  std::string gamestate;
  std::string updated_board;
  bool place_unload_active_;

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we set member variables new definition through passed parameters pl_id and message (action command)
  ///
  /// @param pl_ID number of players turn
  /// @param message represent the command (string) command of user.
  ///
  /// @return none
  //
  void set(int pl_ID, std::string message);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function is used to set the value of the current_side_ member variable in the handleGame class.
  /// The current_side parameter specifies the value to be assigned to current_side_,
  /// which is used to keep track of which player's turn it is in the game. This function is called at the beginning of each player's turn to
  /// update the current_side_ variable.
  ///
  /// @param current_side
  ///
  /// @return none
  //
  void setPlayingSide(char current_side);

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
  std::string parseFunc(int num);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// The function first uses the legal_moves_obj object to extract the action_message_vec vector of strings from the message_ member variable.
  /// It then uses the size of this vector to determine the type of action represented by the message_ string. Depending on the size of the vector
  /// and the action tokens it contains, the function sets one or more of the following member variables to
  /// true: place_2_meeples_active, take_tile_active_, place_unload_active_, place_3_meeples_active, or place_unload_double_active_.
  /// It looks like these member variables are used to keep track of which actions are currently available to the player, and
  /// this function is used to update the values of these variables based on the contents of the message_ string.
  /// @param legal_moves_obj
  ///
  /// @return none
  ///
  void checkForChangeCurrentActionNum(legalMoves &legal_moves_obj);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// The checkForChangeCurrentActionNum() method checks the given message to see if it is a PlaceMeeple or TakeTile or PlaceUnload action.
  /// If it is, it sets the corresponding _active flag to true. The changeCurrentActionNum() method then uses the _active flags and the round number
  /// to determine which elements of the action message vector should be used to update the action_num_ and action_message_ variables of the legalMoves object.
  /// This is used to ensure that the action_num_ and action_message_ variables always contain the correct values for the current round.
  ///
  /// @param egal_moves_obj
  /// @param round_number
  ///
  /// @return none
  ///
  void changeCurrentActionNum(legalMoves &legal_moves_obj, int round_number);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we take object tilestate as a parameter to get three random numbers.
  /// We iterate through for loop get the number, convert it to string and store it in new vector of strings.
  /// Now this vector contains three random numbers.
  ///
  /// @param tile_state object with three random numbers that helps us get next three random numbers for cards
  ///
  ///
  /// @return vector of strings loaded with three random numbers
  //
  std::vector<std::string> giveCards(TileDeck::TileState &tile_state, int num_of_cards);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we determine three random cards based on three random numbers that we get from function giveCards().
  /// We iterate through for loop to fill the the string with actions based on numbers connected with the map of actions.
  ///
  /// @param tile_state serves as a parameter to giveCards() function.
  ///
  /// @return string of new tokens (loaded with three new tokens for the boat)
  //
  std::string getCardsForLoad(TileDeck::TileState &tile_state, int num_of_cards);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we replace tokens on the boats with the new ones if the command from user is "UnloadBoat" and if that
  /// action is legal move. Legal move action means that control state needs to be okay or gameended. In case that both command
  /// UnloadBoat and legal move are false, the function will return the same state of boats as in previous gamestate (string).
  ///
  /// @param tile_state needed for givecardsforload() function as a parameter.
  ///
  /// @return string with new loaded tokens or nones, or the same state of boats as in previous gamestate (string data type).
  //
  std::string replaceTokensOnBoat(TileDeck::TileState &tile_state, parseInput parse_obj, legalMoves legal_moves_obj);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// method called setUpdatedPlayer that takes an
  /// integer argument num and sets the value of pl_ID_(private variable) to the value of num.
  ///
  /// @param num
  ///
  /// @return none
  //
  void setUpdatedPlayer(int num);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Function called catchException takes a string argument string_to_check and attempts to convert it to an integer using the stoi function.
  /// If the conversion is successful, the method returns the resulting integer value.
  /// However, if the conversion fails, the method catches any exceptions that are thrown by the stoi function,
  /// prints an error message to the standard error stream, and returns 0.
  ///
  /// @param string_to_check
  /// @return converted string to integer
  //
  int catchException(std::string string_to_check);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// The declareBoats method retrieves an array of boats from the parseInput object using the getBoats method.
  /// It then calculates the sizes of each boat and stores them in several member variables, such as first_boat,
  /// second_boat, and so on. The sizes of the boats are calculated
  /// by summing the values of the three elements in each boat's array.The sizes of the boats are then added
  /// to a vector called vec_of_boats, and a map called boat_map is created that associates each boat with its size.
  /// The keys in the map are the boat numbers as characters(e.g., '0', '1', '2') and the values are the sizes of the boats.
  ///
  /// @param parse_obj
  ///
  /// @return none
  //
  void declareBoats(parseInput parse_obj);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// The checkForMeeplesOnBeginning function appears to be used to check if it is possible
  /// to place meeples on the game board at the beginning of a turn. The method checks if the action_message_ member variable
  /// of the legalMoves object is equal to "PlaceMeeple". If it is, the method calls the
  /// finalMeepleCount method on the playerStats object and checks the resulting string to see if it contains values that are within the range 0-4.
  ///
  /// @param player_stats_new
  /// @param legal_moves_obj
  /// @param flag_for_syntax_error_
  /// @param num
  /// @param gamestate
  ///
  /// @return  none
  //
  void checkForMeeplesOnBeginning(playerStats player_stats_new, legalMoves legal_moves_obj, bool &flag_for_syntax_error_, int &num, std::string &gamestate);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// The function iterates over the elements of vec_after_place_meeples and updates them based on the player's actions,
  /// storing the updated game board in one of two std::vectors:
  /// vec_after_place_double_meeples or vec_after_place_triple_meeples.
  /// The function then returns a string representation of the updated game board.
  ///
  /// @param vec_after_place_meeples
  /// @param parse_obj
  /// @param legal_moves_obj
  ///
  /// @return updated game board
  std::string boardPlaceMeeples(std::vector<std::string> vec_after_place_meeples, parseInput parse_obj, legalMoves &legal_moves_obj);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// The function updates a string representation of the game board by replacing certain elements with the string "-1 ".
  /// If the place_unload_double_active_ flag is set,
  /// the function updates the string again by replacing additional elements with "-1 .
  /// Finally, the function returns the updated game board string.
  ///
  /// @param legal_moves_obj
  ///
  /// @return updated game board
  //
  std::string boardPlaceUnload(legalMoves &legal_moves_obj);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// The function begins by checking if the current boat (the string at the index-th position in the vec_of_boats vector)
  /// is the same as the boat specified by the legal_moves_obj object. If it is, the function performs a swap operation on
  /// the boat string, swapping the tokens at the positions specified by the legal_moves_obj object.
  /// The resulting string is appended to the replaced_string string.
  /// If the swap operation is successful, the function sets the swap_is_finished flag to true
  /// and updates the legal_moves_obj object to reflect the change in the game state.
  /// If the action_message_ field of the legal_moves_obj
  /// object is set to "UnloadBoat" and the game state is "Okay",
  /// the function also updates the new_boat string by calling the getCardsForLoad
  /// function with the tile_state and the number 3 as arguments.
  ///
  /// @param index
  /// @param legal_moves_obj
  /// @param replaced_string
  /// @param new_boat
  /// @param tile_state
  ///
  /// @return none
  //
  void replaceTokensSwap(size_t index, legalMoves &legal_moves_obj, std::string &replaced_string, std::string &new_boat, TileDeck::TileState &tile_state);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// The function loops through the indices from 0 to 8, and for each index, it checks if it is present in
  ///  the current_list_of_indexes string.
  ///  If the index is found in the string, the function appends "-1 " to the final_meeple_string string,
  ///  and if the place_unload_active_ or place_unload_double_active_ flags are set, it appends "-1 "
  /// to the vec_to_be_compared_for_final vector.
  ///  If the index is not found in the current_list_of_indexes string, the function appends the corresponding element
  ///  of the parse_obj.getBoard() array to the final_meeple_string string, and if the place_unload_active_
  /// or place_unload_double_active_ flags are set, it appends "x " to the vec_to_be_compared_for_final vector.
  /// @param final_meeple_string
  /// @param vec_to_be_compared_for_final
  /// @param parse_obj
  ///
  //
  void boardUnloadBoat(std::string &final_meeple_string, std::vector<std::string> &vec_to_be_compared_for_final, parseInput parse_obj);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// The function checks if the index-th element of the vec_of_boats vector is equal to the value mapped to
  /// the first element of the legal_moves_obj.action_num_ vector in the boat_map map. If they are equal,
  /// the function appends the new_boat string to the replaced_string string, and if the place_unload_double_active_ flag
  /// is set, it sets the new_boat string to the result of calling the getCardsForLoad function with the tile_state
  /// object and the integer 3, and sets the boat_changed flag to true.
  ///
  ///  @param index
  ///  @param legal_moves_obj
  ///  @param replaced_string
  ///  @param new_boat
  ///  @param boat_changed
  ///  @param tile_state
  //
  void replaceTokensUnload(size_t index, legalMoves &legal_moves_obj, std::string &replaced_string, std::string &new_boat, bool &boat_changed, TileDeck::TileState &tile_state);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// The function first calls the PlayerStats0 and PlayerStats1 methods of the player_stats_new object
  /// with the flag_for_syntax_error_ and current_side_ arguments, and stores the returned strings
  /// in the updated_pl_0 and updated_pl_1 variables, respectively.
  /// Next, if the legal_moves_obj.action_message_ string is equal to "TakeTile" and the
  /// gamestate variable is equal to "Okay" or "GameEnded", the function decrements the num_of_reserved_cards variable,
  /// converts it to a string, and stores it in the num_of_tile_deck variable.
  ///
  /// @param player_stats_new
  /// @param legal_moves_obj
  /// @param tile_state
  /// @param game_length
  ///
  /// @return The function returns the updated_text_file string.
  //
  std::string appendFinalGameStateMessage(playerStats player_stats_new, legalMoves legal_moves_obj, TileDeck::TileState &tile_state, std::string game_length);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function program updates the game board with meeples based on command action number from the user.
  /// if the user command was "PlaceMeeple" program will take action number(place where the player wants to put meeple) and set 0 or 1 based on pl_id.
  /// In case of command unloadboat the program will replace players meeples and set "-1" based on action number that he selected
  /// (action num - selected boat in this case).
  ///
  /// @param parse_obj
  /// @param legal_moves_obj
  /// @param player_stats_new
  ///
  /// @return new updated board of meeples (string data type).
  //
  std::string updateBoard(parseInput parse_obj, legalMoves &legal_moves_obj,
                          playerStats &player_stats_new);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This is the function where we check for some legal moves and update our new game state string. We call all functions needed
  /// for new gamestate message and append them in to new gamestate based on checkiflegal() - function. Here we also check for players command
  /// if the players turn didn't change as it should be with variable flag_for_syntax_error.
  ///
  /// @param num int number of players turn
  /// @param _flag_for_syntax_error passing argument to check if syntax error appeared
  /// @param current_gamestate_message passed argument that tells current gamestate command
  /// @param tile_state object that we need for appending three random numbers
  ///
  /// @return string of new updated gamestate message
  //
  std::string game(int &num, bool flag_for_syntax_error_, std::string &current_gamestate_message, TileDeck::TileState &tile_state);
};