#include "LegalMoves.hpp"
#include "PlayerStats.hpp"
#include "PrimaryGame.hpp"
#include <iostream>
#include <vector>
#include <map>

#define PLACEUNLOAD 0
#define PLACEUNLOAD_DOUBLE 1
#define SWAP 2
#define PLACEMEEPLE 3
#define PLACEMEEPLE_DOUBLE 4
#define TAKETILE 5

void legalMoves::intializeValues()
{
  board_ = parse_object.getBoard();
  boats_ = parse_object.getBoats();
  first_row_ = board_[0] + board_[1] + board_[2];
  second_row_ = board_[3] + board_[4] + board_[5];
  third_row_ = board_[6] + board_[7] + board_[8];
  first_col_ = board_[2] + board_[5] + board_[8];
  second_col_ = board_[1] + board_[4] + board_[7];
  third_col_ = board_[0] + board_[3] + board_[6];

  meeple_map = {
      {'0', first_row_},
      {'1', second_row_},
      {'2', third_row_},
      {'3', first_col_},
      {'4', second_col_},
      {'5', third_col_}};
}

void legalMoves::set(std::string message, bool flag_for_syntax_error)
{
  size_t count = std::count_if(message_content_.begin(), message_content_.end(), [](char c)
                               { return c == ' '; });

  message_content_ = message;
  action_message_ = message_content_.substr(0, message_content_.find(" "));
  action_message_vec_ = convertStringToVector(message_content_);

  if (action_message_vec_.size() > 2)
  {
    action_message_ = action_message_vec_.at(0);
    action_num_ = action_message_vec_.at(1);
  }
  else if (flag_for_syntax_error == false and int(count) == 1)
  {
    action_num_ = message_content_.substr(message_content_.find_last_of(" ") + 1);
  }
  else
  {
    action_num_ = "0";
  }
}

bool legalMoves::declareStateOfActionToken()
{
  switch (declareActionToken())
  {
  case PLACEUNLOAD:
    if (isdigit(action_message_vec_.at(1)[0]) and isdigit(action_message_vec_.at(3)[0]))
    {
      return true;
    }
    break;
  case PLACEUNLOAD_DOUBLE:
    if (isdigit(action_message_vec_.at(1)[0]) and isdigit(action_message_vec_.at(3)[0]) and isdigit(action_message_vec_.at(4)[0]))
    {
      return true;
    }
    break;
  case SWAP:
    if (isdigit(action_message_vec_.at(1)[0]) and isdigit(action_message_vec_.at(2)[0]) and isdigit(action_message_vec_.at(3)[0]))
    {
      if (isdigit(action_message_vec_.at(5)[0]))
      {
        return true;
      }
    }
    break;
  case PLACEMEEPLE:
    if (isdigit(action_message_vec_.at(1)[0]) and isdigit(action_message_vec_.at(2)[0]))
    {
      return true;
    }
    break;
  case PLACEMEEPLE_DOUBLE:
    if (isdigit(action_message_vec_.at(1)[0]) and isdigit(action_message_vec_.at(2)[0]) and isdigit(action_message_vec_.at(3)[0]))
    {
      return true;
    }
    break;
  default:
    break;
  }
  return false;
}

bool legalMoves::checkForSyntaxError()
{
  size_t count = std::count_if(message_content_.begin(), message_content_.end(), [](char c)
                               { return c == ' '; });
  std::string valid_names = "PlaceMeeple PlaceUnload Swap TakeTile";
  if (int(count) == 1 and action_message_vec_.size() == 2)
  {
    if (isdigit(action_num_[0]) and (action_message_ == "PlaceMeeple" or action_message_ == "UnloadBoat"))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else if (action_message_vec_.size() >= 3 and action_message_vec_.size() <= 6 and action_tokens_vec_.size() != 0)
  {
    return declareStateOfActionToken();
  }
  return false;
}

int legalMoves::declareActionToken()
{
  switch (action_message_vec_.size())
  {
  case 3:
    if (findActionTokenInMessage("PlaceMeeple") and !(findActionTokenInMessage("UnloadBoat")))
    {
      return 3;
    }
    if (findActionTokenInMessage("TakeTile"))
    {
      return 3;
    }
    break;
  case 4:
    if (findActionTokenInMessage("PlaceMeeple") and !(findActionTokenInMessage("UnloadBoat")))
    {
      return 4;
    }
    if (findActionTokenInMessage("PlaceMeeple") and (findActionTokenInMessage("UnloadBoat")))
    {
      return 0;
    }
    break;
  case 5:
    if (findActionTokenInMessage("PlaceMeeple") and (findActionTokenInMessage("UnloadBoat")))
    {
      return 1;
    }
  case 6:
    if (findActionTokenInMessage("Swap") and (findActionTokenInMessage("UnloadBoat")))
    {
      return 2;
    }
  default:
    break;
  }
  return -1;
}

bool legalMoves::checkForMeeples()
{
  std::vector<std::string> board = parse_object.getBoard();

  if (action_message_ == "PlaceMeeple")
  {
    if ((action_num_[0] - '0') >= 0 and (action_num_[0] - '0') <= 8 and
        board.at(action_num_[0] - '0') == "-1 ")
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  return true;
}

bool legalMoves::checkForActionTileName(std::string token_to_find)
{
  std::string action_tokens = "PlaceMeeples SwapUnload PlaceUnload TakeTile ";
  size_t found = action_tokens.find(token_to_find);

  if (found != std::string::npos)
  {
    return true;
  }
  return false;
}

bool legalMoves::checkForUnloadTakeTile(std::string temp)
{
  for (auto item : meeple_map)
  {
    if (action_num_[0] == item.first)
    {
      if ((take_tile_action_num_[0] - '0') >= 0 and (take_tile_action_num_[0] - '0') <= 2)
      {
        temp = boats_[action_num_[0] - '0'][take_tile_action_num_[0] - '0'];
        break;
      }
      else
      {
        return false;
      }
    }
  }
  if (temp != "None " and !(checkForActionTileName(temp)))
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool legalMoves::checkForUnloadSwap(std::string temp, std::string second_temp)
{
  for (auto item : meeple_map)
  {
    if (action_num_[0] == item.first)
    {
      if (((second_swap_action_num_[0] - '0') >= 0 and (second_swap_action_num_[2] - '0') <= 2) and
          ((third_swap_action_num_[0] - '0') >= 0 and (third_swap_action_num_[0] - '0') <= 2))
      {
        temp = boats_[action_num_[0] - '0'][second_swap_action_num_[0] - '0'];
        second_temp = boats_[action_num_[0] - '0'][third_swap_action_num_[0] - '0'];
      }
      else
      {
        return false;
      }
    }
  }
  if (temp != "None " and second_temp != "None ")
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool legalMoves::checkForUnload()
{
  int sum_of_minuses = 0;
  std::string temp = "";
  std::string second_temp = "";

  if (term_for_placed_meeple_by_action_token_ == true)
  {
    if (board_.at(meeple_place_by_action_token_[0] - '0') == "-1 ")
    {
      term_for_placed_meeple_by_action_token_ = true;
    }
    else
    {
      term_for_placed_meeple_by_action_token_ = false;
    }
  }
  if (action_message_ == "PlaceMeeple")
  {
    return true;
  }
  else if (action_message_ == "UnloadBoat" and (action_num_[0] - '0') >= 0 and (action_num_[0] - '0') <= 5)
  {
    for (auto item : meeple_map)
    {
      if (action_num_[0] == item.first)
      {
        for (auto iterator : item.second)
        {
          if (iterator == '-')
          {
            sum_of_minuses += 1;
          }
        }
        for (size_t i = 0; i < 3; i++)
        {
          temp += boats_[action_num_[0] - '0'][i];
        }
      }
    }
    if (term_for_placed_meeple_by_action_token_ == true)
    {
      sum_of_minuses -= 1;
    }
    if (sum_of_minuses <= 1 and temp != "None None None ")
    {
      term_for_placed_meeple_by_action_token_ = false;
      return true;
    }
    else
    {
      return false;
    }
  }
  else if (action_message_ == "TakeTile" and (action_num_[0] - '0') >= 0 and (action_num_[0] - '0') <= 5)
  {
    return checkForUnloadTakeTile(temp);
  }
  else if (action_message_ == "Swap" and (action_num_[0] - '0') >= 0 and (action_num_[0] - '0') <= 5)
  {
    return checkForUnloadSwap(temp, second_temp);
  }
  else
  {
    return false;
  }
  term_for_placed_meeple_by_action_token_ = false;
  return true;
}

bool legalMoves::checkForEnd()
{
  size_t sum = 0;
  for (auto &v : parse_object.getBoats())
  {
    sum += std::count(v.begin(), v.end(), "None ");
  }

  if (action_message_ == "UnloadBoat" and sum >= 12)
  {
    return false;
  }
  return true;
}

void legalMoves::setActionArray(std::vector<std::string> comment_pl_0, std::vector<std::string> comment_pl_1, int current_player_turn)
{
  std::vector<std::string> new_vec;
  current_player_turn_ = current_player_turn;
  if (current_player_turn == 0)
  {
    for (size_t i = 0; i < comment_pl_0.size(); i++)
    {
      action_tokens_vec_.push_back(comment_pl_0[i]);
    }
  }
  else
  {
    for (size_t i = 0; i < comment_pl_1.size(); i++)
    {
      action_tokens_vec_.push_back(comment_pl_1[i]);
    }
  }
}

std::string legalMoves::convertVectorToString(std::vector<std::string> vector_for_conversion)
{
  std::string converted_vector = std::accumulate(vector_for_conversion.begin(), vector_for_conversion.end(), std::string{},
                                                 [](std::string &s, const std::string &piece) -> decltype(auto)
                                                 { return s += piece; });
  return converted_vector;
}

std::vector<std::string> legalMoves::convertStringToVector(std::string string_for_conversion)
{
  std::string tmp = "";
  std::vector<std::string> converted_string;

  std::stringstream ss(string_for_conversion);

  while (getline(ss, tmp, ' '))
  {
    converted_string.push_back(tmp + " ");
  }

  return converted_string;
}

bool legalMoves::findActionTokenInComment(std::string action_token)
{
  size_t found = convertVectorToString(action_tokens_vec_).find(action_token);

  if (found != std::string::npos)
  {
    return true;
  }
  return false;
}

bool legalMoves::findActionTokenInMessage(std::string action_token)
{
  size_t found = convertVectorToString(action_message_vec_).find(action_token);

  if (found != std::string::npos)
  {
    return true;
  }
  return false;
}

int legalMoves::findOccurencesOfActionToken(std::string action_token)
{
  int searchWordSize = action_token.size();
  int count = 0;

  for (std::vector<std::string>::iterator iter = action_message_vec_.begin(); iter != action_message_vec_.end(); ++iter)
  {
    for (size_t pos = 0; pos < (*iter).length(); pos += searchWordSize)
    {
      pos = (*iter).find(action_token, pos);
      if (pos != std::string::npos)
        ++count;
      else
        break;
    }
  }

  return count;
}

void legalMoves::setValuesBack(std::string temporary_action_num, std::string temporary_action_message)
{
  action_num_ = temporary_action_num;
  action_message_ = temporary_action_message;
}

bool legalMoves::checkForPlaceUnload()
{
  if (findActionTokenInComment("PlaceUnload") and findActionTokenInMessage("PlaceMeeple") and findActionTokenInMessage("UnloadBoat"))
  {
    if (action_message_vec_.size() == 4 or action_message_vec_.size() == 5)
    {
      action_num_ = action_message_vec_.at(1);
      action_message_ = action_message_vec_.at(0);
      action_message_.pop_back();

      if (checkForMeeples())
      {
        term_for_placed_meeple_by_action_token_ = true;
        meeple_place_by_action_token_ = action_message_vec_.at(1);

        action_num_ = action_message_vec_.at(3);
        action_message_ = action_message_vec_.at(2);
        action_message_.pop_back();

        if (checkForUnload())
        {
          if (action_message_vec_.size() == 5)
          {
            if (action_message_vec_.at(3)[0] == action_message_vec_.at(4)[0])
            {
              return false;
            }
            action_num_ = action_message_vec_.at(4);
            if (checkForUnload())
            {
              return true;
            }
          }
          else
          {
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool legalMoves::checkForPlaceMeepleAction()
{
  if (findActionTokenInMessage("PlaceMeeple") and findActionTokenInComment("PlaceMeeple") and !findActionTokenInMessage("UnloadBoat"))
  {
    if (action_message_vec_.size() == 3 or action_message_vec_.size() == 4)
    {
      action_message_ = action_message_vec_.at(0);
      action_num_ = action_message_vec_.at(1);
      action_message_.pop_back();
      if (checkForMeeples() and (action_num_[0] != action_message_vec_.at(2)[0]))
      {
        action_num_ = action_message_vec_.at(2);
        if (checkForMeeples())
        {
          if (action_message_vec_.size() == 4)
          {
            action_num_ = action_message_vec_.at(3);
            if (checkForMeeples() and (action_num_[0] != action_message_vec_.at(2)[0]) and (action_num_[0] != action_message_vec_.at(1)[0]))
            {
              return true;
            }
            else
            {
              return false;
            }
          }
          return true;
        }
      }
    }
  }
  return false;
}

bool legalMoves::checkForTakeTileAction()
{
  if (findActionTokenInMessage("TakeTile") and findActionTokenInComment("TakeTile"))
  {
    if (action_message_vec_.size() == 3)
    {
      action_message_ = action_message_vec_.at(0);
      action_num_ = action_message_vec_.at(1);
      take_tile_action_num_ = action_message_vec_.at(2);
      action_message_.pop_back();
      if (checkForUnload())
      {
        return true;
      }
    }
  }
  return false;
}

bool legalMoves::checkForSwapUnloadAction()
{
  if (findActionTokenInMessage("Swap") and findActionTokenInComment("SwapUnload"))
  {
    if (action_message_vec_.size() == 6)
    {
      action_message_ = action_message_vec_.at(0);
      action_num_ = action_message_vec_.at(1);
      second_swap_action_num_ = action_message_vec_.at(2);
      third_swap_action_num_ = action_message_vec_.at(3);
      action_message_.pop_back();
      if (checkForUnload())
      {
        action_message_ = action_message_vec_.at(4);
        action_num_ = action_message_vec_.at(5);
        action_message_.pop_back();
        if (checkForUnload())
        {
          return true;
        }
      }
    }
  }
  return false;
}

bool legalMoves::checkIfCardPlayable()
{
  std::string temp_action_num = action_num_;
  std::string temp_action_message = action_message_;

  if (checkForPlaceUnload() or checkForPlaceMeepleAction() or checkForTakeTileAction() or checkForSwapUnloadAction())
  {
    setValuesBack(temp_action_num, temp_action_message);
    return true;
  }
  setValuesBack(temp_action_num, temp_action_message);

  return false;
}

std::string legalMoves::checkIfLegal()
{
  intializeValues();

  if (action_message_vec_.size() >= 3)
  {
    if (!checkForSyntaxError())
    {
      return "SyntaxError";
    }
    if (!checkIfCardPlayable())
    {
      return "CardNotPlayable";
    }
    if (!checkForEnd())
    {
      return "GameEnded";
    }
  }
  else if (action_message_vec_.size() < 3)
  {
    if (!checkForSyntaxError())
    {
      return "SyntaxError";
    }
    if (!checkForEnd())
    {
      return "GameEnded";
    }
    if (!checkForMeeples())
    {
      return "MeepleNotPlayable";
    }
    if (!checkForUnload())
    {
      return "BoatNotUnloadable";
    }
  }
  return "Okay";
}