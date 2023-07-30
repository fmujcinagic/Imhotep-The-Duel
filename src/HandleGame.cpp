#include "Parse.hpp"
#include "TileDeck.hpp"
#include "network/Server.hpp"
#include "network/Exceptions.hpp"
#include "LegalMoves.hpp"
#include "PlayerStats.hpp"
#include "HandleGame.hpp"

handleGame::handleGame(std::vector<std::string> words_)
{
  words_vec_.insert(std::end(words_vec_), std::begin(words_), std::end(words_));

  std::string words = std::accumulate(words_.begin(), words_.end(), std::string{},
                                      [](std::string &single_word, const std::string &piece) -> decltype(auto)
                                      { return single_word += piece; });

  indexes_of_meeples = {{'0', "012"},
                        {'1', "345"},
                        {'2', "678"},
                        {'3', "258"},
                        {'4', "147"},
                        {'5', "036"}};

  random_map_ = {
      {0, "Obelisk"},
      {1, "PyramidLight"},
      {2, "PyramidDark"},
      {3, "Temple~1"},
      {4, "Temple~2"},
      {5, "Temple~3"},
      {6, "Temple~4"},
      {7, "Tomb~1"},
      {8, "Tomb~2"},
      {9, "Tomb~3"},
      {10, "Tomb~4"},
      {11, "Tomb~5"},
      {12, "Tomb~6"},
      {13, "Tomb~7"},
      {14, "Tomb~8"},
      {15, "Tomb~9"},
      {16, "Tomb~10"},
      {17, "Tomb~11"},
      {18, "Tomb~12"},
      {19, "TakeTile"},
      {20, "PlaceMeeples"},
      {21, "PlaceUnload"},
      {22, "SwapUnload"}};

  words.pop_back();

  flag_for_syntax_error_ = false;
  swap_is_finished = false;
  catch_error_ = false;
  number_of_iterations = 0;
  place_unload_active_ = false;
  place_unload_double_active_ = false;
  place_2_meeples_active = false;
  take_tile_active_ = false;
  place_3_meeples_active = false;
  swap_unload_active_ = false;
}

void handleGame::set(int pl_ID, std::string message)
{
  pl_ID_ = pl_ID;
  message_ = message;
}

void handleGame::setPlayingSide(char current_side)
{
  current_side_ = current_side;
}

void handleGame::setUpdatedPlayer(int num)
{
  pl_ID_ = num;
}

std::vector<std::string> handleGame::giveCards(TileDeck::TileState &tile_state, int num_of_cards)
{
  std::string intial_state_file = words_ = "";
  std::vector<std::string> cards_vec;

  int tile_id = 0;
  std::string next_tile = "";

  for (int get_three_nums = 0; get_three_nums < num_of_cards; get_three_nums++)
  {
    try
    {
      tile_id = TileDeck::drawNextTile(tile_state);
      next_tile = std::to_string(tile_id);
      cards_vec.push_back(next_tile + " ");
    }
    catch (const std::exception &e)
    {
      catch_error_ = true;
    }
  }

  return cards_vec;
}

std::string handleGame::getCardsForLoad(TileDeck::TileState &tile_state, int num_of_cards)
{
  std::vector<std::string> random_nums = giveCards(tile_state, num_of_cards);
  std::string string_of_new_tokens = "";
  int indexes_of_cards = 0;

  for (size_t index_of_tokens = 0; index_of_tokens < random_nums.size(); index_of_tokens++)
  {
    random_nums[index_of_tokens].pop_back();
    try
    {
      indexes_of_cards = catchException(random_nums[index_of_tokens]);
    }
    catch (std::invalid_argument const &e)
    {
      return "error";
    }
    catch (std::out_of_range const &e)
    {
      return "error";
    }
    string_of_new_tokens.append(random_map_.at(indexes_of_cards) + " ");
  }

  if (catch_error_)
  {
    return "None None None ";
  }
  return string_of_new_tokens;
}

void handleGame::declareBoats(parseInput parse_obj)
{
  boats = parse_obj.getBoats();

  first_boat = boats[0][0] + boats[0][1] + boats[0][2];
  second_boat = boats[1][0] + boats[1][1] + boats[1][2];
  third_boat = boats[2][0] + boats[2][1] + boats[2][2];
  fourth_boat = boats[3][0] + boats[3][1] + boats[3][2];
  fifth_boat = boats[4][0] + boats[4][1] + boats[4][2];
  sixth_boat = boats[5][0] + boats[5][1] + boats[5][2];

  vec_of_boats.push_back(first_boat);
  vec_of_boats.push_back(second_boat);
  vec_of_boats.push_back(third_boat);
  vec_of_boats.push_back(fourth_boat);
  vec_of_boats.push_back(fifth_boat);
  vec_of_boats.push_back(sixth_boat);

  boat_map = {
      {'0', first_boat},
      {'1', second_boat},
      {'2', third_boat},
      {'3', fourth_boat},
      {'4', fifth_boat},
      {'5', sixth_boat}};
}

void handleGame::replaceTokensSwap(size_t index, legalMoves &legal_moves_obj, std::string &replaced_string,
                                   std::string &new_boat, TileDeck::TileState &tile_state)
{
  if (vec_of_boats[index] == boat_map.at(legal_moves_obj.action_num_[0]))
  {
    std::string replace_on_boat_str = vec_of_boats[index];
    std::vector<std::string> replace_on_boat_vec = legal_moves_obj.convertStringToVector(replace_on_boat_str);
    std::string token_to_take_first = "";
    std::string token_to_take_second = "";
    token_to_take_first.append(replace_on_boat_vec.at(legal_moves_obj.third_swap_action_num_[0] - '0'));
    token_to_take_second.append(replace_on_boat_vec.at(legal_moves_obj.second_swap_action_num_[0] - '0'));
    replace_on_boat_str.replace(replace_on_boat_str.find(replace_on_boat_vec.at(legal_moves_obj.third_swap_action_num_[0] - '0')),
                                replace_on_boat_vec.at(legal_moves_obj.third_swap_action_num_[0] - '0').length(), token_to_take_second);
    replace_on_boat_str.replace(replace_on_boat_str.find(replace_on_boat_vec.at(legal_moves_obj.second_swap_action_num_[0] - '0')),
                                replace_on_boat_vec.at(legal_moves_obj.second_swap_action_num_[0] - '0').length(), token_to_take_first);
    replaced_string.append(replace_on_boat_str);
    swap_is_finished = true;
  }
  else
  {
    replaced_string.append(vec_of_boats[index]);
  }
  if (swap_is_finished)
  {
    changeCurrentActionNum(legal_moves_obj, 2);
    if (legal_moves_obj.action_message_ == "UnloadBoat" and gamestate == "Okay")
    {
      new_boat = getCardsForLoad(tile_state, 3);
    }
  }
}

void handleGame::replaceTokensUnload(size_t index, legalMoves &legal_moves_obj, std::string &replaced_string,
                                     std::string &new_boat, bool &boat_changed, TileDeck::TileState &tile_state)
{
  if (vec_of_boats[index] == boat_map.at(legal_moves_obj.action_num_[0]))
  {
    replaced_string.append(new_boat);
    if (place_unload_double_active_)
    {
      new_boat = getCardsForLoad(tile_state, 3);
      boat_changed = true;
    }
  }
  else
  {
    if (place_unload_double_active_ and vec_of_boats[index] == boat_map.at(second_action_num_for_unload_[0]))
    {
      if (boat_changed == false)
      {
        new_boat = getCardsForLoad(tile_state, 3);
        boat_changed = false;
      }
      replaced_string.append(new_boat);
    }
    else
    {
      replaced_string.append(vec_of_boats[index]);
    }
  }
}

std::string handleGame::replaceTokensOnBoat(TileDeck::TileState &tile_state, parseInput parse_obj, legalMoves legal_moves_obj)
{
  declareBoats(parse_obj);

  std::string new_boat = "";
  std::string new_boat_take_tile;
  std::string boat_take_tile = "";

  if (swap_unload_active_)
  {
    checkForChangeCurrentActionNum(legal_moves_obj);
    changeCurrentActionNum(legal_moves_obj, 1);
  }

  if (legal_moves_obj.action_message_ == "UnloadBoat" and gamestate == "Okay")
  {
    new_boat = getCardsForLoad(tile_state, 3);
  }
  if (legal_moves_obj.action_message_ == "TakeTile" and gamestate == "Okay")
  {
    new_boat_take_tile = getCardsForLoad(tile_state, 1);
  }

  std::string replaced_string = "";
  bool boat_changed = false;
  for (size_t index = 0; index < vec_of_boats.size(); index++)
  {
    if (legal_moves_obj.action_message_ == "UnloadBoat" and gamestate == "Okay")
    {
      replaceTokensUnload(index, legal_moves_obj, replaced_string, new_boat, boat_changed, tile_state);
    }
    else if (legal_moves_obj.action_message_ == "UnloadBoat" and legal_moves_obj.checkIfLegal() == "GameEnded")
    {
      if (vec_of_boats[index] == boat_map.at(legal_moves_obj.action_num_[0]))
      {
        replaced_string.append("None None None ");
      }
      else
      {
        replaced_string.append(vec_of_boats[index]);
      }
    }
    else if (legal_moves_obj.action_message_ == "TakeTile" and gamestate == "Okay")
    {
      if (take_tile_active_)
      {
        checkForChangeCurrentActionNum(legal_moves_obj);
        changeCurrentActionNum(legal_moves_obj, 1);
      }
      if (vec_of_boats[index] == boat_map.at(legal_moves_obj.action_num_[0]))
      {
        std::string replacement = vec_of_boats[index];
        std::vector<std::string> token_to_replace = legal_moves_obj.convertStringToVector(boat_map.at(legal_moves_obj.action_num_[0]));

        replacement.replace(replacement.find(token_to_replace.at(legal_moves_obj.take_tile_action_num_[0] - '0')), token_to_replace.at(legal_moves_obj.take_tile_action_num_[0] - '0').length(), new_boat_take_tile);

        replaced_string.append(replacement);
      }
      else
      {
        replaced_string.append(vec_of_boats[index]);
      }
    }
    else if (legal_moves_obj.action_message_ == "Swap" and gamestate == "Okay")
    {
      replaceTokensSwap(index, legal_moves_obj, replaced_string, new_boat, tile_state);
    }
    else
    {
      replaced_string.append(vec_of_boats[index]);
    }
  }
  int count = 0;
  size_t pos = 0;
  std::string substr = "None ";
  while ((pos = replaced_string.find(substr, pos)) != std::string::npos)
  {
    ++count;
    pos += substr.length();
  }

  if (count == 15)
  {
    gamestate = "GameEnded";
  }
  return replaced_string;
}

std::string handleGame::boardPlaceMeeples(std::vector<std::string> vec_after_place_meeples, parseInput parse_obj, legalMoves &legal_moves_obj)
{
  for (size_t index = 0; index < parse_obj.getBoard().size(); index++)
  {

    if ((legal_moves_obj.action_num_[0] - '0') == int(index))
    {
      if (pl_ID_ == 1)
      {
        vec_after_place_double_meeples.push_back("1 ");
      }
      else
      {
        vec_after_place_double_meeples.push_back("0 ");
      }
    }
    else
    {
      vec_after_place_double_meeples.push_back(vec_after_place_meeples[index]);
    }

    if (place_3_meeples_active)
    {
      changeCurrentActionNum(legal_moves_obj, 3);

      if ((legal_moves_obj.action_num_[0] - '0') == int(index))
      {
        if (pl_ID_ == 1)
        {
          vec_after_place_triple_meeples.push_back("1 ");
        }
        else
        {
          vec_after_place_triple_meeples.push_back("0 ");
        }
      }
      else
      {
        vec_after_place_triple_meeples.push_back(vec_after_place_double_meeples[index]);
      }
    }
  }

  if (place_3_meeples_active)
  {
    return legal_moves_obj.convertVectorToString(vec_after_place_triple_meeples);
  }

  return legal_moves_obj.convertVectorToString(vec_after_place_double_meeples);
}

std::string handleGame::boardPlaceUnload(legalMoves &legal_moves_obj)
{
  for (size_t i = 0; i < vec_to_be_compared_for_final.size(); i++)
  {
    if (vec_to_be_compared_for_final[i] == "-1 ")
    {
      string_after_place_unload.append(vec_to_be_compared_for_final[i]);
    }
    else
    {
      string_after_place_unload.append(vec_after_place_meeples[i]);
    }
  }

  if (place_unload_double_active_)
  {
    changeCurrentActionNum(legal_moves_obj, 3);
    current_list_of_indexes.clear();
    vec_to_be_compared_for_final.clear();
    vec_to_be_compared_for_final = legal_moves_obj.convertStringToVector(string_after_place_unload);

    current_list_of_indexes = indexes_of_meeples.at(legal_moves_obj.action_num_[0]);

    for (size_t index = 0; index <= 8; index++)
    {
      size_t found = current_list_of_indexes.find(std::to_string(index));
      if (found != std::string::npos)
      {
        string_after_place_unload_double.append("-1 ");
      }
      else
      {
        string_after_place_unload_double.append(vec_to_be_compared_for_final[index]);
      }
    }
    return string_after_place_unload_double;
  }

  return string_after_place_unload;
}

void handleGame::boardUnloadBoat(std::string &final_meeple_string, std::vector<std::string> &vec_to_be_compared_for_final, parseInput parse_obj)
{
  for (size_t index = 0; index <= 8; index++)
  {
    size_t found = current_list_of_indexes.find(std::to_string(index));
    if (found != std::string::npos)
    {
      final_meeple_string.append("-1 ");
      if (place_unload_active_ or place_unload_double_active_)
      {
        vec_to_be_compared_for_final.push_back("-1 ");
      }
    }
    else
    {
      final_meeple_string.append(parse_obj.getBoard()[index]);
      if (place_unload_active_ or place_unload_double_active_)
      {
        vec_to_be_compared_for_final.push_back("x ");
      }
    }
  }
}

int handleGame::catchException(std::string string_to_check)
{
  try
  {
    return stoi(string_to_check);
  }
  catch (std::invalid_argument const &e)
  {
    std::cerr << e.what() << '\n';
  }
  catch (std::out_of_range const &e)
  {
    std::cerr << e.what() << '\n';
  }
  return 0;
}

std::string handleGame::updateBoard(parseInput parse_obj, legalMoves &legal_moves_obj,
                                    playerStats &player_stats_new)
{
  if (legal_moves_obj.action_message_.back() == ' ')
    legal_moves_obj.action_message_.pop_back();
  if (legal_moves_obj.action_num_.back() == ' ')
    legal_moves_obj.action_num_.pop_back();

  if (legal_moves_obj.action_message_ == "PlaceMeeple")
  {
    for (size_t index = 0; index < parse_obj.getBoard().size(); index++)
    {
      if ((legal_moves_obj.action_num_[0] - '0') == int(index))
      {
        if (pl_ID_ == 1)
        {
          final_meeple_string.append("1 ");
          vec_after_place_meeples.push_back("1 ");
        }
        else
        {
          final_meeple_string.append("0 ");
          vec_after_place_meeples.push_back("0 ");
        }
      }
      else
      {
        final_meeple_string.append(parse_obj.getBoard()[index]);
        vec_after_place_meeples.push_back(parse_obj.getBoard()[index]);
      }
    }
  }

  if (place_unload_active_ or place_unload_double_active_ or place_2_meeples_active or place_3_meeples_active)
  {
    // for (auto i : vec_after_place_meeples)
    // {
    //   std::cout << i << " ";
    // }
    player_stats_new.setNewBoard(vec_after_place_meeples, place_unload_active_, place_2_meeples_active, place_3_meeples_active);
    checkForChangeCurrentActionNum(legal_moves_obj);
    changeCurrentActionNum(legal_moves_obj, 2);
  }

  if (place_2_meeples_active)
  {
    return boardPlaceMeeples(vec_after_place_meeples, parse_obj, legal_moves_obj);
  }

  if (swap_unload_active_)
  {
    changeCurrentActionNum(legal_moves_obj, 2);
  }

  if (legal_moves_obj.action_message_ == "UnloadBoat")
  {
    current_list_of_indexes = indexes_of_meeples.at(legal_moves_obj.action_num_[0]);

    boardUnloadBoat(final_meeple_string, vec_to_be_compared_for_final, parse_obj);
  }

  if (place_unload_active_ or place_unload_double_active_)
  {
    return boardPlaceUnload(legal_moves_obj);
  }

  if (legal_moves_obj.action_message_ == "TakeTile")
  {
    final_meeple_string = legal_moves_obj.convertVectorToString(parse_obj.getBoard());
  }

  return final_meeple_string;
}

void handleGame::checkForChangeCurrentActionNum(legalMoves &legal_moves_obj)
{
  std::vector<std::string> action_message_vec = legal_moves_obj.convertStringToVector(message_);
  int size_of_action_message_vec = action_message_vec.size();
  switch (size_of_action_message_vec)
  {
  case 3:
    if (legal_moves_obj.findActionTokenInMessage("PlaceMeeple") and !(legal_moves_obj.findActionTokenInMessage("UnloadBoat")))
    {
      place_2_meeples_active = true;
    }
    if (legal_moves_obj.findActionTokenInMessage("TakeTile"))
    {
      take_tile_active_ = true;
    }
    break;
  case 4:
    if (legal_moves_obj.findActionTokenInMessage("PlaceMeeple") and legal_moves_obj.findActionTokenInMessage("UnloadBoat"))
    {
      place_unload_active_ = true;
    }
    if (legal_moves_obj.findActionTokenInMessage("PlaceMeeple") and !(legal_moves_obj.findActionTokenInMessage("UnloadBoat")))
    {
      place_3_meeples_active = true;
    }
    break;

  case 5:
    if (legal_moves_obj.findActionTokenInMessage("PlaceMeeple") and legal_moves_obj.findActionTokenInMessage("UnloadBoat"))
    {
      place_unload_double_active_ = true;
      place_unload_active_ = true;
    }
    break;
  case 6:
    if (legal_moves_obj.findActionTokenInMessage("Swap") and legal_moves_obj.findActionTokenInMessage("UnloadBoat"))
    {
      swap_unload_active_ = true;
    }
    break;

  default:
    break;
  }
}

void handleGame::changeCurrentActionNum(legalMoves &legal_moves_obj, int round_number)
{
  std::vector<std::string> action_message_vec = legal_moves_obj.convertStringToVector(message_);
  // int size_of_action_message_vec = action_message_vec.size();

  switch (round_number)
  {
  case 1:
    if (place_unload_active_ or place_unload_double_active_)
    {
      legal_moves_obj.action_num_ = action_message_vec.at(1);
      legal_moves_obj.action_message_ = action_message_vec.at(0);
    }
    if (place_2_meeples_active or place_3_meeples_active)
    {
      legal_moves_obj.action_message_ = action_message_vec.at(0);
      legal_moves_obj.action_num_ = action_message_vec.at(1);
    }

    if (take_tile_active_)
    {
      legal_moves_obj.action_message_ = action_message_vec.at(0);
      legal_moves_obj.action_num_ = action_message_vec.at(1);
    }
    if (swap_unload_active_)
    {
      legal_moves_obj.action_message_ = action_message_vec.at(0);
      legal_moves_obj.action_num_ = action_message_vec.at(1);
    }
    break;
  case 2:
    if (place_unload_active_ or place_unload_double_active_)
    {
      legal_moves_obj.action_num_ = action_message_vec.at(3);
      legal_moves_obj.action_message_ = action_message_vec.at(2);
    }
    if (place_2_meeples_active or place_3_meeples_active)
    {
      legal_moves_obj.action_message_ = action_message_vec.at(0);
      legal_moves_obj.action_num_ = action_message_vec.at(2);
    }
    if (take_tile_active_)
    {
      legal_moves_obj.action_message_ = action_message_vec.at(0);
      legal_moves_obj.action_num_ = action_message_vec.at(1);
    }
    if (swap_unload_active_)
    {
      legal_moves_obj.action_message_ = action_message_vec.at(4);
      legal_moves_obj.action_num_ = action_message_vec.at(5);
    }
    break;
  case 3:
    if (place_unload_active_ or place_unload_double_active_)
    {
      legal_moves_obj.action_num_ = action_message_vec.at(4);
    }
    if (place_3_meeples_active)
    {
      legal_moves_obj.action_num_ = action_message_vec.at(3);
    }
  default:
    break;
  }
  if (action_message_vec.size() > 2)
    legal_moves_obj.action_message_.pop_back();
  // legal_moves_obj.action_message_.pop_back();
}

std::string handleGame::appendFinalGameStateMessage(playerStats player_stats_new, legalMoves legal_moves_obj,
                                                    TileDeck::TileState &tile_state, std::string game_length)
{
  // std::cout << "da nije ovde pt1" << std::endl;
  std::string updated_pl_0 = player_stats_new.PlayerStats0(flag_for_syntax_error_, current_side_);
  // std::cout << "da nije ovde pt2" << std::endl;
  std::string updated_pl_1 = player_stats_new.PlayerStats1(flag_for_syntax_error_, current_side_);
  // std::cout << "da nije ovde pt2" << std::endl;
  if (legal_moves_obj.action_message_ == "TakeTile" and (gamestate == "Okay" or gamestate == "GameEnded"))
  {
    num_of_reserved_cards--;
    num_of_tile_deck = std::to_string(num_of_reserved_cards);
  }

  updated_player_turn = std::to_string(pl_ID_);
  updated_text_file.append(gamestate);
  updated_text_file.append(" ");
  updated_text_file.append(updated_player_turn);
  updated_text_file.append(" ");
  updated_text_file.append(num_of_tile_deck);
  updated_text_file.append(" ");
  updated_text_file.append(updated_boats);
  updated_text_file.append(updated_board);
  updated_text_file.append(updated_pl_0);
  updated_text_file.append(updated_pl_1);
  // std::cout << "da nije ovde pt3" << std::endl;
  updated_text_file.append(std::to_string(tile_state.remaining_cards[0]) + " ");
  updated_text_file.append(std::to_string(tile_state.remaining_cards[1]) + " ");
  updated_text_file.append(std::to_string(tile_state.seed) + " ");
  updated_text_file.append(game_length);

  return updated_text_file;
}

void handleGame::checkForMeeplesOnBeginning(playerStats player_stats_new, legalMoves legal_moves_obj,
                                            bool &flag_for_syntax_error_, int &num, std::string &gamestate)
{
  if (legal_moves_obj.action_message_ == "PlaceMeeple")
  {
    if (((player_stats_new.finalMeepleCount(flag_for_syntax_error_)[0] - '0') <= 0 or (player_stats_new.finalMeepleCount(flag_for_syntax_error_)[0] - '0') > 4) and pl_ID_ == 0)
    {
      num = -num + 1;
      gamestate = "MeepleNotPlayable";
      flag_for_syntax_error_ = true;
    }
    else if (((player_stats_new.finalMeepleCount(flag_for_syntax_error_)[1] - '0') <= 0 or (player_stats_new.finalMeepleCount(flag_for_syntax_error_)[1] - '0') > 4) and pl_ID_ == 1)
    {
      num = -num + 1;
      gamestate = "MeepleNotPlayable";
      flag_for_syntax_error_ = true;
    }
  }
}

std::string handleGame::game(int &num, bool _flag_for_syntax_error, std::string &current_gamestate_message, TileDeck::TileState &tile_state)
{
  // std::cout << "ovde nije core dump pt1 " << std::endl;
  parseInput parse_obj(words_vec_);

  num_of_tile_deck = std::to_string(parse_obj.getRemainigTiles());
  num_of_reserved_cards = parse_obj.getRemainigTiles();

  legalMoves legal_moves_obj(parse_obj);

  std::vector<std::string> random_state = parse_obj.getRandomState();
  flag_for_syntax_error_ = _flag_for_syntax_error;
  // std::cout << "ovde nije core dump pt2 " << std::endl;
  legal_moves_obj.set(message_, flag_for_syntax_error_);

  player player_info(parse_obj);
  playerStats player_stats(legal_moves_obj, parse_obj, player_info);
  player_stats.initializeValues();
  comment_array_pl0_vec_ = player_stats.commentArrayFirstPlayer(comment_pl0_beginning_, comment_pl1_beginning_);
  comment_array_pl1_vec_ = player_stats.commentArraySecondPlayer(comment_pl0_beginning_, comment_pl1_beginning_);
  legal_moves_obj.setActionArray(comment_array_pl0_vec_, comment_array_pl1_vec_, num);
  legal_moves_obj.set(message_, flag_for_syntax_error_);
  playerStats player_stats_new(legal_moves_obj, parse_obj, player_info);
  player_stats_new.initializeValues();
  int length_of_game_comment = parse_obj.getCommentArrayLength();
  std::string length_game_comment = std::to_string(length_of_game_comment);
  // std::cout << "ovde nije core dump pt3 " << std::endl;

  if (flag_for_syntax_error_ == true)
  {
    gamestate = "SyntaxError";
  }
  else
  {
    gamestate = legal_moves_obj.checkIfLegal();
  }

  if (gamestate == "Okay" or gamestate == "GameEnded")
  {
    num = -num + 1;
  }
  player_stats_new.setPlayer(num);
  checkForMeeplesOnBeginning(player_stats_new, legal_moves_obj, flag_for_syntax_error_, num, gamestate);
  current_gamestate_message = gamestate;

  if ((gamestate == "Okay" or gamestate == "GameEnded") and flag_for_syntax_error_ == false)
  {
    checkForChangeCurrentActionNum(legal_moves_obj);
    changeCurrentActionNum(legal_moves_obj, 1);
    updated_board = updateBoard(parse_obj, legal_moves_obj, player_stats_new);
  }
  else
  {
    for (auto index : parse_obj.getBoard())
    {
      updated_board.append(index);
    }
  }
  // std::cout << "ovde nije core dump pt4 " << std::endl;
  if ((gamestate == "Okay" or gamestate == "GameEnded") and flag_for_syntax_error_ == false)
  {
    checkForChangeCurrentActionNum(legal_moves_obj);
    changeCurrentActionNum(legal_moves_obj, 2);
    player_stats_new.setActionMessage(legal_moves_obj);
  }
  if (place_unload_double_active_)
  {
    second_action_num_for_unload_ = legal_moves_obj.action_message_vec_.at(4);
    player_stats_new.setSecondActionNumForUnload(second_action_num_for_unload_, place_unload_double_active_);
  }
  updated_boats = replaceTokensOnBoat(tile_state, parse_obj, legal_moves_obj);

  // if (catch_error_)
  // {
  //   gamestate = "GameEnded";
  // }
  // std::cout << "ovde nije core dump pt5 " << std::endl;
  player_stats_new.current_gamestate = gamestate;
  setUpdatedPlayer(num);
  // std::cout << "ovde nije core dump pt6 " << std::endl;
  return appendFinalGameStateMessage(player_stats_new, legal_moves_obj, tile_state, length_game_comment);
}