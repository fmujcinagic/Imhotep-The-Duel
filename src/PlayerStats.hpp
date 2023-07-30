//---------------------------------------------------------------------------------------------------------------------
// PlayerStats.cpp
//
// In this file, player tokens are being constantly updated through the rounds.
//
// Group: Gruppe 15
// Authors: Faris Mujcinagic 12127517, Milan Mejic 12034456, Fedja Mulabegovic 12127976
//---------------------------------------------------------------------------------------------------------------------
//

#pragma once
#include "LegalMoves.hpp"
#include "Parse.hpp"

class playerStats
{
protected:
  legalMoves legal_moves_obj;
  parseInput parse_object;
  player player_info;
  int pl_num_;
  int action_num_ = legal_moves_obj.action_num_[0] - '0';
  std::string action_message_ = legal_moves_obj.action_message_;
  std::map<int, std::string> board_map_after_place_meeple_;
  bool flag_active;
  int second_action_num_for_unload_;
  bool place_unload_double_active_;
  bool swap_unload_active_;
  int sum_player_0_, sum_player_1_;
  bool place_unload_active_;
  bool place_2_meeples_active_;
  bool place_3_meeples_active_;
  std::vector<std::string> board_;

public:
  playerStats(legalMoves obj1, parseInput obj2, player obj3) : legal_moves_obj(obj1), parse_object(obj2), player_info(obj3){};

  std::string current_gamestate;
  std::vector<std::string> new_board_;
  std::vector<std::string> new_board_map_;
  std::vector<std::string> private_vec_;
  std::map<int, int> temples_B_side_map_;
  std::map<int, int> pyramid_B_side_map_;
  std::map<int, int> pyramid_map_;
  std::map<int, int> tomb_map_;
  std::map<int, std::string> board_map_;
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This code initializes the values of the playerStats class in a game. It sets the initial values of several member variables,
  /// such as flag_active and place_unload_double_active_, to false. It also creates two maps, board_map_ and pyramid_map_,
  /// which store information about the game board and pyramids, respectively.
  /// Finally, it initializes the board_ member variable by calling the get_board() method of the parse_object object.
  ///
  //
  void initializeValues();
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function sets passed current player to
  /// the protected vairable of our class.
  ///
  /// @param pl_num Current player
  ///
  //
  void setPlayer(int pl_num);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function sets passed object of legalMoves class to the protected variable of our class.
  ///
  /// @param object_legal_move object from legalMoves class
  ///
  //
  void setActionMessage(legalMoves &object_legal_move);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This funciton returns "1" for the first player
  /// that achieves 5 obelisks. This is going to be important
  /// for the further calculation of the points (B side).
  ///
  /// @return std::string 0 or 1 if player has achieved 5 obelisk
  //
  std::string finalFirstObelisk();
  // ---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function calculates the final meeple number of both players that are available to play in case that action message is unloadboat
  /// and action placeunload and placeunload double are active (see the function finalMeepleCOunt()). It has no parameters
  /// and its purpose to short the code of finalMeepleCount() function.
  ///
  /// @return string of a number of meeples of both players.
  //
  std::string calculateMeepleForPlaceUnload();
  // ---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function calculates the final meeple number of both players that are available to play in case that action message is unloadboat
  /// and no other actions are active. It has no parameters and its purpose to short the code of finalMeepleCount() function.
  ///
  /// @return string of a number of meeples of both players.
  //
  std::string calculateMeepleForUnloadBoat();
  // ---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function calculates the final meeple number of both players that are available to play in case that action message is placemeeple
  /// and action placeunload or placeunload double are active (see the function finalMeepleCOunt()). It has no parameters
  /// and its purpose to short the code of finalMeepleCount() function.
  ///
  /// @return string of a number of meeples of both players.
  //
  std::string calculateMeepleActionAndPlaceUnload(std::string meeple_for_pl0, std::string meeple_for_pl1);
  // ---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function returns current meeple count for the both of the players.
  /// It's calculated depending on the current action message that has been
  /// previously received from the server. Based on condition, either corresponding function is called to give number of meeples,
  /// or the calculation is done in the body of condition. Also the syntax error has been
  /// taken into the account.
  ///
  /// @param syntax_error_flag Optional syntax error from the server, in case of that function returns back previous defined numbers.
  ///
  /// @return std::string calculation of the meeples for the both of the players.
  std::string finalMeepleCount(bool flag_for_syntax_error);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function returns current obelisk count for the both of the players.
  /// It's calculated depending on the current action message that has been
  ///. previously received from the server. Also the previous amount has been
  /// taken into the account (constantly updating according to the current gamestate).
  ///
  /// @return std::vector<std::string> Vector of strings filled with the
  ///                                  corresponding amount of obelisks
  //
  std::vector<std::string> finalAmountObelisks();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function returns current temple count for the both of the players.
  /// It's calculated depending on the current action message that has been
  ///. previously received from the server. Also the previous amount has been
  /// taken into the account (constantly updating according to the current gamestate).
  ///
  /// @return std::string Current updated string that contains
  ///                     the number of temlpes for both players
  //
  std::string finalTemples();
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function helps us determine the number of tempel that is taken after the taketile action.
  /// Based on action number(tells us on which boat) and take tile action number(tells us which tile to take from the boat)
  /// program compares possible tempels and in case of matchig add one to the final amount.
  ///
  /// @return string that contains number of tiles taken from the boat for both players.
  //
  std::string tempelsForTakeTileAction();
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This is a helper function, that we use to catch exception in case that stoi() throws one.
  ///
  /// @return in case valid convertion, then number. On other hand terminates the program with return value 0.
  //
  int catchException(std::string string_to_check);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function returns current pyramid light count for the both of the players.
  /// It's calculated depending on the current action message that has been
  ///. previously received from the server. Also the previous amount has been
  /// taken into the account (constantly updating according to the current gamestate).
  ///
  /// @return std::string Current updated string that contains
  ///                     the number of light pyramids for both players
  //
  std::string finalAmountPyramidLight();
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function returns current pyramid dark count for the both of the players.
  /// It's calculated depending on the current action message that has been
  /// previously received from the server. Also the previous amount has been
  /// taken into the account (constantly updating according to the current gamestate).
  ///
  /// @return std::string Current updated string that contains
  ///                     the number of dark pyramids for both players
  //
  std::string finalAmountPyramidDark();
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function returns current count of the tombs for the both of the players.
  /// It's calculated depending on the current action message that has been
  /// previously received from the server. Number from the gamestate has been
  /// converted to the 12 digit binary number which has been updated
  /// and correspondingly usigned long integers are merged into two strings
  /// that represent updated tomb count for each player individually.
  ///
  ///
  /// @return std::string Current updated string that contains
  ///                     the number(decimal) of tombs for both players
  //
  std::string finalTombs();
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function calculates the number of action toknes,
  /// wihch represents the length of the player comment.
  ///
  /// @return std::string Current updated string that contains
  ///                     the final comment length for the both of the players
  //
  std::string lengthOfPlayerComment();
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function returns number of take tile (Take 1 Cargo Token)action tokens
  /// after action message is received from the server.
  /// Current terms and validity is taken into account.
  ///
  /// @param comment_pl_0 Occurences of action tokens for the player 0
  /// @param comment_pl_1 Occurences of action tokens for the player 1
  ///
  /// @return std::string Current updated string that contains
  ///                     the final number of "Take Tile" action tokens
  //
  std::string playerCommentTakeTile(int &comment_pl_0, int &comment_pl_1);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function returns number of place meeples action tokens
  /// after action message is received from the server.
  /// Current terms and validity is taken into account.
  ///
  /// @param comment_pl_0 Occurences of action tokens for the player 0
  /// @param comment_pl_1 Occurences of action tokens for the player 1
  ///
  /// @return std::string Current updated string that contains
  ///                     the final number of "Place 2-3 Meeples" action tokens
  //
  std::string playerCommentPlaceMeeples(int &comment_pl_0, int &comment_pl_1);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function returns number of place unload action tokens
  /// after action message is received from the server.
  /// Current terms and validity is taken into account.
  ///
  /// @param comment_pl_0 Occurences of action tokens for the player 0
  /// @param comment_pl_1 Occurences of action tokens for the player 1
  ///
  /// @return std::string Current updated string that contains
  ///                     the final number of "Place 1 Meeple and Unload 1–2 boats" action tokens
  //
  std::string playerCommentPlaceUnload(int &comment_pl_0, int &comment_pl_1);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function returns number of place SwapUnload action tokens
  /// after action message is received from the server.
  /// Current terms and validity is taken into account.
  ///
  /// @param comment_pl_0 Occurences of action tokens for the player 0
  /// @param comment_pl_1 Occurences of action tokens for the player 1
  ///
  /// @return std::string Current updated string that contains
  ///                     the final number of "Swap 2 Cargo Tokens and Unload" action tokens
  //
  std::string playerCommentSwapUnload(int &comment_pl_0, int &comment_pl_1);
  // ---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function helps us to determine which board of meeples we need to use for giving the tiles to the players.
  /// Also counts the minuses on the board with the connection to the boat -> with this information we determine how many tiles are
  /// gonna be given to the players.
  ///
  /// @param meeples_on_board here we determine which board of meeples we are gonna use in the progress of program.
  /// @param count_of_minuses here we determine number of minuses in the corresponding row or column on the board
  ///
  void getCountOfMinusesAndMeeples(std::vector<std::string> &meeples_on_board, size_t &count_of_minuses);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function is the base alogrithm that has been implemented.
  /// It is based on the finding given token name and returning its
  /// occurence on the desired boat. It is achieved thorugh the board
  /// and boat map that has already been implemented.
  ///
  /// @param token_name Desired name of the token
  ///
  /// @return std::string Occurences of the desired token for the both of players
  //
  std::string connectBoatAndBoard(std::string name);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function program updates the comment array of first player. Based on given action messages from the server,
  /// program takes the action message from comment or it gets it during the play turn. In case of taking action,
  /// program deletes it from the comment, in case of unloading boat and getting a action tile, program adds it to the current
  /// comment array.
  ///
  /// @param comment_pl_0 Occurences of action tokens for the player 0
  /// @param comment_pl_1 Occurences of action tokens for the player 1
  ///
  /// @return vector of strings filled with action messages for the first player
  //
  std::vector<std::string> commentArrayFirstPlayer(int &comment_pl_0, int &comment_pl_1);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function program updates the comment array of second player. Based on given action messages from the server,
  /// program takes the action message from comment or it gets it during the play turn. In case of taking action,
  /// program deletes it from the comment, in case of unloading boat and getting a action tile, program adds it to the current
  /// comment array.
  ///
  /// @param comment_pl_0 Occurences of action tokens for the player 0
  /// @param comment_pl_1 Occurences of action tokens for the player 1
  ///
  /// @return vector of strings filled with action messages for the second player
  //
  std::vector<std::string> commentArraySecondPlayer(int &comment_pl_0, int &comment_pl_1);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function sets passed second number of the boat to unload to the protected variable of our class, beside that
  /// it sets passed value of boolen varible, which tells us whether the action placeunload double is active, to the
  /// protected variable of our class.
  ///
  /// @param second_action_num_for_unload passed number inside of strings which tells the number of second boat to operate
  /// @param place_unload_double_active passed boolen variable that tells us whether the action placeunload double is active.
  //
  void setSecondActionNumForUnload(std::string second_action_num_for_unload, bool place_unload_double_active);
  // ---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function sets passed board after placed meeple on the board and passed boolen values that determine what actions
  /// are currently activeted, to the protected variables of playerStats class. Based on new board we create a map so we can
  /// access at indexes more easily.
  ///
  ///
  /// @param updated_board passed argument after placed meeple on the board in case of placeunload action
  /// @param place_unload_active passed argument that tells, whether placeunload action is active or not
  /// @param place_2_meeples_active passed argument that tells, whether place 2 meeples action is active or not
  /// @param place_3_meeples_active passed argument that tells, whether place 3 meeples action is active or not
  ///
  void setNewBoard(std::vector<std::string> updated_board, bool place_unload_active, bool place_2_meeples_active, bool place_3_meeples_active);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function program calculates part of the whole points needed for A side evaluation.
  /// Part of the functions are called here, their return values are summed up and returned back as parted calculated points.
  ///
  /// @param syntax_error_flag Optional syntax error needed for finalmeeplecount()
  ///
  /// @return current updated string with partially calculated points of A side.
  //
  std::vector<std::string> partOfPointsASide(bool syntax_error_flag);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function returns final number of player points. They use partially calculated points from previous function (A side)
  /// and they are calculating according to current gamestate string
  /// and also the official rules for the A side.
  ///
  /// @param syntax_error_flag Optional syntax error needed as parameter for partofpointsAside()
  ///
  /// @return std::string Current updated string that contains
  ///                     the final points for both of the players
  //
  std::vector<std::string> playerPointsASide(bool syntax_error_flag);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function program calculates first part of the whole points needed for B side evaluation.
  /// Part of the functions are called here, their return values are summed up and returned back as parted calculated points.
  /// Because of the amount of actions that are taken into account we needed to split calculation of Points for B side into three functions.
  ///
  /// @param syntax_error_flag Optional syntax error needed for finalmeeplecount()
  ///
  /// @return current updated string with partially calculated points of B side.
  //
  std::vector<std::string> firstPartOfPointsBSide(bool syntax_error_flag);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function program calculates second part of the whole points needed for B side evaluation.
  /// Part of the functions are called here, their return values are summed up and returned back as parted calculated points.
  /// Based on currently updated number of points for both players program calculate the next steps and returns back second part
  /// of calculation.
  ///
  /// @param sum_pl_0 the program gets points for first player that is summed up until now
  /// @param sum_pl_0 the program gets points for second player that is summed up until now
  ///
  /// @return current updated string with partially calculated points of B side.
  //
  std::vector<std::string> secondPartOfPointsBSide(int sum_pl_0, int sum_pl_1);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function returns final number of player points. They use partially calculated points from previous function (B side)
  /// and they are calculating according to current gamestate string
  /// and also the official rules for the B side.
  ///
  /// @param syntax_error_flag Optional syntax error needed as parameter for partofpointsBside()
  ///
  /// @return std::string Current updated string that contains
  ///                     the final points for both of the players
  //
  std::vector<std::string> playerPointsBSide(bool syntax_error_flag);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function program calculates only the part of points needed for the stats of player 0.
  /// Function that calculates temples is called here, from the function return value is summed up and returned back as parted calculated points.
  ///
  /// @param player_stats0 the program gets currently updated playerstats0 and gives it back after its done with calculating
  /// in this function.
  //
  void partOfPlayerStats0ForTemples(std::string &player_stats0);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function program calculates only the part of points needed for the stats of player 0.
  /// Function that calculates occurences pyramidDark is called here, from the function return value is summed up
  /// and returned back as parted calculated points.
  ///
  /// @param player_stats0 the program gets currently updated playerstats0 and gives it back after its done with calculating
  /// in this function.
  //
  void partOfPlayerStats0ForPyramid(std::string &player_stats0);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// In this function program calculates only the part of points needed for the stats of player 1.
  /// Many functions needed for calculation final points are called here. Here the program summs up all given points
  /// and returns it back as a string for the second player.
  ///
  /// @param syntax_error_flag Optional syntax error
  /// @param current_side determine which side of points needs to be taken into account
  ///
  /// @return std::string Filled string with the info for the desired player
  //
  std::string partOfPlayerStats1(bool flag_for_syntax_error, char current_side);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Primary function for the player 0 stats, which
  /// is responsible for calling all of the functions above
  /// and correspondingly merging all of the info into the string
  ///
  /// @param syntax_error_flag Optional syntax error
  /// @param current_side determine which side of points needs to be taken into account
  ///
  /// @return std::string Filled string with the info for the desired player
  //
  std::string PlayerStats0(bool flag_for_syntax_error, char current_side);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Primary function for the player 1 stats, which
  /// is responsible for calling all of the functions above
  /// and correspondingly merging all of the info into the string
  ///
  /// @param syntax_error_flag Optional syntax error
  ///
  /// @return std::string Filled string with the info for the desired player
  //
  std::string PlayerStats1(bool flag_for_syntax_error, char current_side);
};