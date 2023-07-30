//---------------------------------------------------------------------------------------------------------------------
// Parse.hpp
//
// In the constructor of parseInput class we get our gamestate message.
// After that we split each argument of that gamestate into separate functions.
// Since each of the players has a lot of data with him, we created another player class and called two functions in it
// with the help of the constructor to get information from the first and second player.
// Each individual data from the players is divided by member functions.
//
// Group: Gruppe 15
// Authors: Faris Mujcinagic 12127517, Milan Mejic 12034456, Fedja Mulabegovic 12127976
//---------------------------------------------------------------------------------------------------------------------
//
#pragma once

#include <iostream>
#include <vector>
#include <map>

class parseInput
{
public:
  //----------------------------------------------------------------------------
  // Constructor
  //
  parseInput(std::vector<std::string> intial_state_file) : gamestate_text_(std::move(intial_state_file)) {}

  //--------------------------------------------------------------------------------------------------------------------
  /// Variables
  ///
  std::vector<std::string> gamestate_text_;

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Since the game starts without the first word of the original gamestate,
  /// in this function we remove that first word "GameState" with function erase().
  ///
  ///
  /// @return it returns new vector of strings with loaded gamestate informations only without first word from original gamestate
  //
  std::vector<std::string> parseForStart();

  static int catchException(std::string string_to_check);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// With the help of this function, we get a word that tells us the state of the game: okay, syntaxerror, etc.
  /// As we already know on which index is that word, we use function at() - at(some specific index).
  ///
  ///
  /// @return it returns current state of the game
  //
  std::string getControlState();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function tells us the current player turn. We already know on which index is the number so again we use at().
  ///
  ///
  /// @return it returns the number of the players whose turn is it
  //
  int getCurrentPlayer();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function tells us current available cards to load the the empty boat after its unloaded. The index is known so again at().
  ///
  ///
  /// @return it returns the number of available cards
  //
  int getRemainigTiles();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// in this function we save all data for boats in a 2d vector of strings.In the first for loop program iterates through
  /// gamestate and in range of specific index program stores the cards for boats in new vector of strings. After it,
  /// we define the size of 2d vector of strings and fill it through 2 for loops with with already loaded "list_of_boats" vector.
  ///
  ///
  /// @return it returns 2d vector of strings loaded with tokens for all 6 boats
  //
  std::vector<std::vector<std::string>> getBoats();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function, inside of for loop we save informations about meeples on the board in the range of specific index.
  /// So the new vector of strings is loaded with 1, -1, 0, depending on what is in the gamestate.
  ///
  ///
  /// @return int age of spouse
  //
  std::vector<std::string> getBoard();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we store the information about first player from original game stare in the new vector of strings
  /// First we check if at the index 43 is 0, because the number of that index determines our upper limit until counter goes.
  /// Number at that index tells us how many action tiles are there.
  /// If it is 0 then the upper limit is as usual, if it is not equal to 0 then the upper limit is number at index 43 + 44.
  ///
  ///
  ///
  /// @return vector of strings loaded with information of player 1.
  //
  std::vector<std::string> getPl1Info();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function the same is happening as in function get_pl1_info(), but here we are dependent also on index at 56 position.
  /// Numbers at these indexes tell us how many action tiles are there for both players.
  /// Depending on number at index 43 and 56 we determine the size to fill the new vector of strings with information of player 2.
  ///
  ///
  ///
  /// @return vector of strings loaded with information of player 2
  //
  std::vector<std::string> getPl2Info();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we take the three numbers needed for tile state remaining cards from original gamestate.
  /// For that we need to determine the right index, again depending on number at 43 and 56.
  ///
  ///
  /// @return it returns vector of strings loaded with three random numbers from gamestate
  //
  std::vector<std::string> getRandomState();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we get the number of comments of the game. By determining the exact index from original gamestate
  /// we can get the number we search for.
  ///
  /// @return it returns int number of game comments
  //
  int getCommentArrayLength();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we store the comment of the game if exitsts. If length of comment of the game is not 0, then
  /// we determine exact index to store the comment inside of new initiliazed vector of strings.
  ///
  ///
  ///
  /// @return vector of strings loaded with comment of the game
  //
  std::vector<std::string> getCommentArray();
};

class player
{
protected:
  //--------------------------------------------------------------------------------------------------------------------
  /// Variables
  ///
  parseInput obj;

  std::vector<std::string> player_info1_ = obj.getPl1Info();
  std::vector<std::string> player_info2_ = obj.getPl2Info();

  std::map<int, std::vector<std::string>> player_map_ =
      {{0, player_info1_}, {1, player_info2_}};

public:
  //----------------------------------------------------------------------------
  // Constructor
  //
  player(parseInput obj1) : obj(obj1){};

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we determine the player name based on player id -> pl_num.
  ///
  /// @param pl_num passes the player id (0 or 1)
  ///
  /// @return string name of player
  //
  std::string plName(int pl_num);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we determine current player points based on pl_num (player on turn).
  ///
  /// @param pl_num passes the player id (0 or 1)
  ///
  /// @return int number of points
  //
  int plPoints(int pl_num);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we determine current points of firstobelisk (important for B Implementation) of a specific player, based on players turn (pl_num).
  ///
  /// @param pl_num passes the player id (0 or 1)
  ///
  /// @return int number of points for first obelisk
  //
  int plFirstObelisk(int pl_num);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we determine current amount of available meeples of a specific player, based on players turn (pl_num).
  ///
  /// @param pl_num passes the player id (0 or 1)
  ///
  /// @return int number of available meeples
  //
  int plMeepleCount(int pl_num);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we determine current number of points for obelisks of a specific player, based on players turn (pl_num).
  ///
  /// @param pl_num passes the player id (0 or 1)
  ///
  /// @return int number of current obelisks
  //
  int plAmountObelisks(int pl_num);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we store current collected tokens for temples of a specific player, based on players turn (pl_num).
  ///
  /// @param pl_num passes the player id (0 or 1)
  ///
  /// @return vector of strings loaded with current collected tokens of temples
  //
  std::vector<std::string> plTemples(int pl_num);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we determine current number of points for pyramidlight of a specific player, based on players turn (pl_num).
  ///
  /// @param pl_num passes the player id (0 or 1)
  ///
  /// @return int number of current pyramidlights
  //
  int plAmountPyramidLight(int pl_num);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we determine current number of points for pyramiddark of a specific player, based on players turn (pl_num).
  ///
  /// @param pl_num passes the player id (0 or 1)
  ///
  /// @return int number of current pyramiddark
  //
  int plAmountPyramidDark(int pl_num);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we determine current number of points for tombs of a specific player, based on players turn (pl_num).
  ///
  /// @param pl_num passes the player id (0 or 1)
  ///
  /// @return int number of current tombs.
  //
  int plTombs(int pl_num);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we determine current number of action comments of a specific player, based on players turn (pl_num).
  ///
  /// @param pl_num passes the player id (0 or 1)
  ///
  /// @return int number (length) of players action comment.
  //
  int plLengthOfActionArray(int pl_num);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function we store action comments of a specific player if exist. FIrst we check if comment of action is not equal to 0,
  /// based on that we determine the limits of player_info to take action comments and store them in new vector of strings if they exist.
  ///
  /// @param pl_num passes the player id (0 or 1)
  ///
  /// @return vector of strings loaded with action comment if exist, else empty.
  //
  std::vector<std::string> plActionArray(int pl_num);
};