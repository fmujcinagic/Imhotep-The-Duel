//---------------------------------------------------------------------------------------------------------------------
// AImove.hpp
//
// In summary, here we compute the best possible options for AI to perform an action. Our main priority for the AI is.
// To place meeple as long as it is not possible or it already has 3 meeple in a row or column. Then, based on calculations
// it unloads the best boat. To do some of the specific calculations, we look at the information in each player's stats
// so that we can update them in each next round.
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
#include "LegalMoves.hpp"
#include "HandleGame.hpp"

class AImove
{
private:
  handleGame handle_game_obj_;
  int player_turn_;
  std::string current_gamestate_message;
  TileDeck::TileState tile_state_;
  parseInput parse_obj_;
  legalMoves legal_moves_obj_;
  playerStats player_stats_obj_;
  std::map<std::string, int> unsorted_board;
  std::vector<std::string> best_combinations_for_pl1;
  std::vector<std::string> best_combinations_for_pl0;
  bool choice_for_unload_not_found = false;
  std::vector<int> best_choices_;
  std::map<int, std::string> options_place_meeple_map;
  std::multimap<int, std::vector<std::string>> corresponding_position_map_;
  std::multimap<std::vector<std::string>, int> corresponding_position_map_second_;
  std::multimap<std::vector<std::string>, int> final_map;
  std::vector<int> possible_choices_place_meeple_;
  std::vector<std::string> evaluation_list_;
  int AI_id_;
  int possible_points_from_unload;
  std::string token_to_take_;
  bool placeable_on_edge_ = false;
  std::vector<std::string> tokens_from_boats;
  std::vector<std::string> tokens_from_boats_second;
  std::multimap<int, std::vector<std::string>> map_for_next_meeple_;
  std::map<int, std::vector<int>> map_for_second_next_meeple_;
  std::multimap<int, std::vector<std::string>> map_for_next_meeple;
  std::map<int, std::vector<int>> map_for_second_next_meeple;
  std::multimap<int, std::vector<std::string>> connect_tokens_and_boats, connect_tokens_and_boats_second;

public:
  AImove(handleGame handle_obj, TileDeck::TileState &tile_state, parseInput parse_obj, legalMoves legal_moves_obj,
         playerStats player_stats_obj) : handle_game_obj_{handle_obj},
                                         tile_state_{tile_state},
                                         parse_obj_{parse_obj},
                                         legal_moves_obj_{legal_moves_obj},
                                         player_stats_obj_(player_stats_obj){};
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we are setting the actuall player turn and Ai id, we are setiing them to our private variables, so we can use them
  /// as part of our program. This function takes as a parameter player turn and ai id and this function is called in primaryGameAI.cpp
  ///
  /// @param player_turn here we are getting actuall player turn
  /// @param AI_id here we are passing AI ID.
  ///
  //
  void setPlayerTurn(int player_turn, int AI_id);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we have implemented the process to check whether a two meeples from ai are in a row or column,
  /// so that we can add another one in that line. We loop through the columns and rows and count the minuses,
  /// based on that we determine the number of current placed meeples. Then we send the needed information in function
  /// declarethirdoption in order to get index of board that needs to be placed.
  ///
  /// @param check_for_two_meeples here we pass bool variable and set it to true if there is two meeples in a row or column
  /// @param best_option after we find the column or row with two meeples we then calculate and get index for third meeple
  ///
  /// @return 0 when the function is done
  //
  int checkForTwoMeeples(bool &check_for_two_meeples, int &best_option);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we are looking to place meeple on a edge based on evaluation list and free places on this edges of a board.
  /// We are doing it so because once we set meeple on edge, we know for sure that we will get a token based on a evaluation list.
  ///
  /// @return the index on board, where the meeple needs to be placed, otherwise 0 -> when the conditions are not true.
  //
  int placeOnEdges();
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we only initalize our private variables (maps) for later usage in order to detemrine which tokens
  /// on boat we can get. This function takes no parameter.
  ///
  ///
  //
  void initializeMaps();
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we determine in what order we want to sort pyramids in our evaluation list.
  /// Whether it is going to be light or dark, it depends on how many pyramids the ai has already.
  ///
  /// @param current_amount_of_pyramid_light current number of pyramid light for ai
  /// @param current_amount_of_pyramid_dark current number of pyramid dark for ai
  ///
  //
  void evaluationfListForPyramids(int current_amount_of_pyramid_light, int current_amount_of_pyramid_dark);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// The same process is for tombs. We determine the number of tomb that we would like to store as a priority in our evaluation
  /// list,. For the first iteration that tomb is a single tomb, as soon as we get one tomb we than add to our evaluation list
  /// tombs that can be built around.
  ///
  /// @param current_number_of_tombs current number of tombs for ai (in decimal)
  ///
  //
  void evaluationfListForTombs(int current_number_of_tombs);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we fill the map with the tokens that need to help in order to determine what boat is best to unload.
  /// We are iterating through the best choices that we can have, than check whether it can be unloaded and based on all
  /// this information we fill the map.
  ///
  //
  void fillTheMapsForBestBoat();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we initializing the object of class handleGame, as we need the object during a game for getting correct and updated values.
  ///
  /// @param handle_obj we are passing the actuall object of the class and setting to our private variable
  ///
  //
  void setNewObject(handleGame handle_obj);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we are calculating the best choice of the possible choices in order to unload boat.
  /// At first we determine, based on placed meeple on board, which player would get which token from the boat. After it,
  /// we are comparing in our evaulation list which token is best to get. Based on that information, we return the number of boat
  /// that contains best token to take.
  ///
  /// @return index of boat that needs to be unloaded
  //
  int newBestChoiceForUnloadBoat();
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we have initalized map that contains right indexes of every column or row. Based on passed arguments,
  /// we define, on what row or column, which place on board to return as an index of board. This function has a purpose when we have two meeple
  /// in a row or column so we can put a third meeple after these two.
  ///
  /// @param boat represent the number of boat
  /// @param index represent the place on board
  ///
  /// @return the index of place on a board if it is possible, otherwise 10 number if it is not possible.
  //
  int declareThirdOptionPlace(int boat, int index);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we determine the move that needs to be taken from the ai. We are calculating and comparing whether a placemeeple or unloadboat
  /// is better option. In the end we place meeple as many times as at least there is 3 meeples of ai in a row or a column, or there is no meeple to place
  /// in that round anymore, after it based on calculations ai unloads the bests boat.
  ///
  ///
  /// @return it returns the action message that needs to be taken
  //
  std::string getMove();
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function has its purpose to serve as an algorithm for getting tokens from boat based on placed meeple. The algorithm
  /// looks at current situation on board based on passed arguments and returns back the actuall index on a boat that tells what token
  /// you can get.
  ///
  /// @param second_nearest_place Number to check if its empty on board or not
  /// @param first_nearest_place Number to check if its empty on board or not
  /// @param get_first here we use this and set to false when to position where are checking for token or place for meeple is not fixed.
  ///
  /// @return returns exact position on a boat
  //
  int getExactPosition(int second_nearest_place, int first_nearest_place, bool get_first);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This funciton is the brain of our ai. Here we determine what tokens are best to search for. The evaulations list is updated after every turn
  /// and based on this list we determine the move of placemeeple and unloadboat. It contains all possible tokens from boats but in
  /// priority order.
  ///
  //
  void setEvaluationList();
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function, based on positions of placed meeple we determine what is the best combination of the given ones to unload boat.
  /// If the boat is unloadable we determine the best combination and return it.
  ///
  /// @return it returns 0 if the best option doesn't exist, otherwise it returns best calculated option to unload.
  //
  int bestChoiceForUnloadBoat();
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we just check if there is option to unload, if not we than set a flag that there is no options and that placemeeple
  /// action needs to be taken, otherwise we go to the function that calculates best boat to unload. This function takes no parameter.
  ///
  ///
  /// @return it returns the return value of called function (best option to unload)
  //
  int calculateBestChoiceForUnloadBoat();
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function initializes maps, which are used for algorithms
  /// in other functions in this whole class.
  ///
  void updateCorrespondingMap();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function It does this by first updating and initializing "evaluation_list_"
  /// and "map_for_next_meeple_", which are used in the decision-making process.
  /// Then, it checks if it's possible for the AI to place two meeples in one move,
  /// and if so, it returns the best option for doing so. If not, it checks for meeples
  /// that are already placed on the board and tries to find a move that is adjacent to one of them.
  ///
  /// @param gamestate std::string Current gamestate
  ///
  /// @return int Best index on the board
  //
  int bestChoiceForPlaceMeeple(std::string gamestate);

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
  /// This function populates board_vec with best possible options
  /// and combinations on the board. Depending on the AI_id,
  /// combinations change.
  ///
  /// @param board_ std::vector<std::string> Vector of strings(combinations as strings)
  ///
  /// @return std::vector<std::string> Populated vector
  //
  std::vector<std::string> getUpadtedBoard(std::vector<std::string> board_);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function takes in two integers, "min" and "max", as parameters.
  /// It is used in case there isn't good option for AI move, so random
  /// number is retrurned. The function uses the C++ standard library function
  /// "rand()" to generate the random number.
  ///
  /// @param min Set to 1
  /// @param max Set to 8
  ///
  /// @return int Random num
  //
  int random(int min, int max);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This funcion gets the best option from  bestChoiceForPlaceMeeple() and then sends that option
  /// to the game object "handle_game_obj_" to be executed. The function continues
  /// to loop until a valid unload option is found. If a valid option is found,
  /// the function returns the message "PlaceMeeple + best index on board".
  ///
  ///
  /// @return std::string "PlaceMeeple + best index on board"
  //
  std::string getFinalPlaceMeeple();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This funcion gets the best option from bestChoiceForUnloadBoat() and then sends that option
  /// to the game object "handle_game_obj_" to be executed. The function continues
  /// to loop until a valid unload option is found. If a valid option is found,
  /// the function returns the message "UnloadBoat [best_unload_option]" else it returns an empty string.
  ///
  ///
  /// @return std::string "UnloadBoat + number"
  //
  std::string getFinalBoat();
};