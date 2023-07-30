#include "PlayerStats.hpp"

void playerStats::setPlayer(int pl_num)
{
  pl_num_ = pl_num;
}

void playerStats::setActionMessage(legalMoves &object_legal_move)
{
  legal_moves_obj = object_legal_move;
}

void playerStats::setSecondActionNumForUnload(std::string second_action_num_for_unload, bool place_unload_double_active)
{
  second_action_num_for_unload_ = catchException(second_action_num_for_unload);
  place_unload_double_active_ = place_unload_double_active;
}

void playerStats::initializeValues()
{
  flag_active = false;
  place_unload_double_active_ = false;
  swap_unload_active_ = false;
  place_unload_active_ = false;
  place_2_meeples_active_ = false;
  place_3_meeples_active_ = false;
  board_ = parse_object.getBoard();
  board_map_ = {{0, board_[0] + board_[1] + board_[2]},
                {1, board_[3] + board_[4] + board_[5]},
                {2, board_[6] + board_[7] + board_[8]},
                {3, board_[2] + board_[5] + board_[8]},
                {4, board_[1] + board_[4] + board_[7]},
                {5, board_[0] + board_[3] + board_[6]}};
  pyramid_map_ = {{0, 0}, {1, 1}, {2, 3}, {3, 6}, {4, 10}, {5, 15}, {6, 21}};
  tomb_map_ = {{0, 0}, {1, 1}, {2, 4}, {3, 9}, {4, 16}, {5, 25}};
  temples_B_side_map_ = {{0, 0}, {1, 1}, {2, 4}, {3, 9}, {4, 16}};
  pyramid_B_side_map_ = {{0, -6},
                         {1, 0},
                         {2, 4},
                         {3, 10},
                         {4, 18},
                         {5, 30},
                         {6, 45}};
}

std::string playerStats::finalFirstObelisk()
{
  std::vector<std::string> tokens_on_boats;
  std::string obelisk_for_pl1;
  std::string obelisk_for_pl2;
  if (legal_moves_obj.action_message_ == "UnloadBoat" and legal_moves_obj.checkIfLegal() == "Okay")
  {
    for (int count_obelisks = 0; count_obelisks < 3; count_obelisks++)
    {
      if (parse_object.getBoats()[legal_moves_obj.action_num_[0] - '0'][count_obelisks] == "Obelisk " and player_info.plAmountObelisks(0) == 4)
      {
        obelisk_for_pl1 = "1";
        break;
      }
      else
      {
        obelisk_for_pl1 = std::to_string(player_info.plFirstObelisk(0));
      }
    }
    for (int count_obelisks = 0; count_obelisks < 3; count_obelisks++)
    {
      if (parse_object.getBoats()[legal_moves_obj.action_num_[0] - '0'][count_obelisks] == "Obelisk " and player_info.plAmountObelisks(1) == 4)
      {
        obelisk_for_pl2 = "1";
        break;
      }
      else
      {
        obelisk_for_pl2 = std::to_string(player_info.plFirstObelisk(1));
      }
    }
  }
  else if (legal_moves_obj.action_message_ == "TakeTile" and legal_moves_obj.checkIfLegal() == "Okay")
  {
    if (pl_num_ == 1)
    {
      if (finalAmountObelisks().front() == "5")
      {
        obelisk_for_pl1 = "1";
        obelisk_for_pl2 = std::to_string(player_info.plFirstObelisk(1));
      }
      else
      {
        obelisk_for_pl1 = std::to_string(player_info.plFirstObelisk(0));
        obelisk_for_pl2 = std::to_string(player_info.plFirstObelisk(1));
      }
    }
    else
    {
      if (finalAmountObelisks().back() == "5")
      {

        obelisk_for_pl2 = "1";
        obelisk_for_pl1 = std::to_string(player_info.plFirstObelisk(0));
      }
      else
      {
        obelisk_for_pl1 = std::to_string(player_info.plFirstObelisk(0));
        obelisk_for_pl2 = std::to_string(player_info.plFirstObelisk(1));
      }
    }
  }
  else
  {
    obelisk_for_pl1 = std::to_string(player_info.plFirstObelisk(0));
    obelisk_for_pl2 = std::to_string(player_info.plFirstObelisk(1));
  }
  return obelisk_for_pl1 + obelisk_for_pl2;
}

void playerStats::setNewBoard(std::vector<std::string> updated_board, bool place_unload_active, bool place_2_meeples_active, bool place_3_meeples_active)
{
  for (size_t index = 0; index < updated_board.size(); index++)
  {
    new_board_.push_back(updated_board[index]);
  }
  place_unload_active_ = place_unload_active;
  place_2_meeples_active_ = place_2_meeples_active;
  place_3_meeples_active_ = place_3_meeples_active;
  board_map_after_place_meeple_ = {
      {0, new_board_[0] + new_board_[1] + new_board_[2]},
      {1, new_board_[3] + new_board_[4] + new_board_[5]},
      {2, new_board_[6] + new_board_[7] + new_board_[8]},
      {3, new_board_[2] + new_board_[5] + new_board_[8]},
      {4, new_board_[1] + new_board_[4] + new_board_[7]},
      {5, new_board_[0] + new_board_[3] + new_board_[6]}};
}

std::string playerStats::calculateMeepleForPlaceUnload()
{
  std::string meeple_for_pl0, meeple_for_pl1;
  std::vector<std::string> meeples_on_board = parse_object.getBoard();
  int current_meeples_pl_0 = player_info.plMeepleCount(0);
  int current_meeples_pl_1 = player_info.plMeepleCount(1);
  int count_pl_0 = 0;
  int count_pl_1 = 0;
  int count_minus = 0;
  for (size_t i = 0; i < board_map_after_place_meeple_.at(legal_moves_obj.action_num_[0] - '0').size(); i++)
  {
    if (board_map_after_place_meeple_.at(legal_moves_obj.action_num_[0] - '0')[i] == '0')
    {
      count_pl_0++;
    }
  }
  for (size_t i = 0; i < board_map_after_place_meeple_.at(legal_moves_obj.action_num_[0] - '0').size(); i++)
  {
    if (board_map_after_place_meeple_.at(legal_moves_obj.action_num_[0] - '0')[i] == '1')
    {
      count_pl_1++;
    }
    if (board_map_after_place_meeple_.at(legal_moves_obj.action_num_[0] - '0')[i] == '-')
    {
      count_minus++;
    }
  }
  meeple_for_pl0 = std::to_string(current_meeples_pl_0 + count_pl_0);
  meeple_for_pl1 = std::to_string(current_meeples_pl_1 + (count_pl_1 - count_minus));
  if (place_unload_double_active_)
  {
    count_pl_0 = 0;
    count_pl_1 = 0;
    count_minus = 0;
    for (size_t i = 0; i < board_map_after_place_meeple_.at(second_action_num_for_unload_).size(); i++)
    {
      if (board_map_after_place_meeple_.at(second_action_num_for_unload_)[i] == '0')
      {
        count_pl_0++;
      }
    }
    for (size_t i = 0; i < board_map_after_place_meeple_.at(second_action_num_for_unload_).size(); i++)
    {
      if (board_map_after_place_meeple_.at(second_action_num_for_unload_)[i] == '1')
      {
        count_pl_1++;
      }
      if (board_map_after_place_meeple_.at(second_action_num_for_unload_)[i] == '-')
      {
        count_minus++;
      }
    }
    meeple_for_pl0 = std::to_string(catchException(meeple_for_pl0) + count_pl_0);
    meeple_for_pl1 = std::to_string(catchException(meeple_for_pl1) + (count_pl_1 - count_minus));
  }
  return meeple_for_pl0 + meeple_for_pl1;
}

std::string playerStats::calculateMeepleForUnloadBoat()
{
  std::string meeple_for_pl0, meeple_for_pl1;
  std::vector<std::string> meeples_on_board = parse_object.getBoard();
  int current_meeples_pl_0 = player_info.plMeepleCount(0);
  int current_meeples_pl_1 = player_info.plMeepleCount(1);
  int count_pl_0 = 0;
  int count_pl_1 = 0;
  int count_minus = 0;
  for (size_t i = 0; i < board_map_.at(legal_moves_obj.action_num_[0] - '0').size(); i++)
  {
    if (board_map_.at(legal_moves_obj.action_num_[0] - '0')[i] == '0')
    {
      count_pl_0++;
    }
  }
  for (size_t i = 0; i < board_map_.at(legal_moves_obj.action_num_[0] - '0').size(); i++)
  {
    if (board_map_.at(legal_moves_obj.action_num_[0] - '0')[i] == '1')
    {
      count_pl_1++;
    }
    if (board_map_.at(legal_moves_obj.action_num_[0] - '0')[i] == '-')
    {
      count_minus++;
    }
  }
  meeple_for_pl0 = std::to_string(current_meeples_pl_0 + count_pl_0);
  meeple_for_pl1 = std::to_string(current_meeples_pl_1 + (count_pl_1 - count_minus));
  return meeple_for_pl0 + meeple_for_pl1;
}

std::string playerStats::calculateMeepleActionAndPlaceUnload(std::string meeple_for_pl0, std::string meeple_for_pl1)
{
  std::vector<std::string> meeples_on_board = parse_object.getBoard();
  int current_meeples_pl_0 = player_info.plMeepleCount(0);
  int current_meeples_pl_1 = player_info.plMeepleCount(1);
  if (pl_num_ == 1 and (current_meeples_pl_0 >= 1 and current_meeples_pl_0 <= 4))
  {
    meeple_for_pl0 = std::to_string((meeple_for_pl0[0] - '0') - 1);
    meeple_for_pl1 = std::to_string(meeple_for_pl1[0] - '0');
  }
  else if (pl_num_ == 0 and (current_meeples_pl_1 >= 1 and current_meeples_pl_1 <= 4))
  {
    meeple_for_pl0 = std::to_string(meeple_for_pl0[0] - '0');
    meeple_for_pl1 = std::to_string((meeple_for_pl1[0] - '0') - 1);
  }
  else
  {
    return std::to_string(player_info.plMeepleCount(0)) + std::to_string(player_info.plMeepleCount(1));
  }
  return meeple_for_pl0 + meeple_for_pl1;
}

std::string playerStats::finalMeepleCount(bool syntax_error_flag)
{
  for (size_t i = 0; i < action_message_.length(); i++)
  {
    if (action_message_[i] == ' ')
      action_message_.pop_back();
  }
  // std::cout << "OVO SE IZVRSAVA ZA MEEPLE: " << legal_moves_obj.action_message_ << std::endl;
  if (legal_moves_obj.action_message_.back() == ' ')
    legal_moves_obj.action_message_.pop_back();
  std::string meeple_for_pl0, meeple_for_pl1;
  std::vector<std::string> meeples_on_board = parse_object.getBoard();
  int current_meeples_pl_0 = player_info.plMeepleCount(0);
  int current_meeples_pl_1 = player_info.plMeepleCount(1);
  if (syntax_error_flag == false and (current_gamestate == "Okay" or current_gamestate == "GameEnded"))
  {
    if (legal_moves_obj.action_message_ == "UnloadBoat" and (place_unload_active_ or place_unload_double_active_))
    {
      meeple_for_pl0 = calculateMeepleForPlaceUnload().front();
      meeple_for_pl1 = calculateMeepleForPlaceUnload().back();
    }
    else if (legal_moves_obj.action_message_ == "UnloadBoat" and !place_unload_active_)
    {
      meeple_for_pl0 = calculateMeepleForUnloadBoat().front();
      meeple_for_pl1 = calculateMeepleForUnloadBoat().back();
      return meeple_for_pl0 + meeple_for_pl1;
    }
    if (action_message_ == "PlaceMeeple" and (place_unload_active_ or place_unload_double_active_))
    {
      meeple_for_pl0 = calculateMeepleActionAndPlaceUnload(meeple_for_pl0, meeple_for_pl1).front();
      meeple_for_pl1 = calculateMeepleActionAndPlaceUnload(meeple_for_pl0, meeple_for_pl1).back();
    }
    else if (action_message_ == "PlaceMeeple" and !place_2_meeples_active_ and !place_3_meeples_active_)
    {
      if (pl_num_ == 1 and (current_meeples_pl_0 >= 1 and current_meeples_pl_0 <= 4))
      {
        meeple_for_pl0 = std::to_string(current_meeples_pl_0 - 1);
        meeple_for_pl1 = std::to_string(player_info.plMeepleCount(1));
      }
      else if (pl_num_ == 0 and (current_meeples_pl_1 >= 1 and current_meeples_pl_1 <= 4))
      {
        meeple_for_pl0 = std::to_string(player_info.plMeepleCount(0));
        meeple_for_pl1 = std::to_string(current_meeples_pl_1 - 1);
      }
      else
      {
        return std::to_string(player_info.plMeepleCount(0)) + std::to_string(player_info.plMeepleCount(1));
      }
    }
    else if (action_message_ == "PlaceMeeple" and place_2_meeples_active_ and !place_3_meeples_active_)
    {
      if (pl_num_ == 1 and (current_meeples_pl_0 >= 1 and current_meeples_pl_0 <= 4))
      {
        meeple_for_pl0 = std::to_string(current_meeples_pl_0 - 2);
        meeple_for_pl1 = std::to_string(player_info.plMeepleCount(1));
      }
      else if (pl_num_ == 0 and (current_meeples_pl_1 >= 1 and current_meeples_pl_1 <= 4))
      {
        meeple_for_pl0 = std::to_string(player_info.plMeepleCount(0));
        meeple_for_pl1 = std::to_string(current_meeples_pl_1 - 2);
      }
      else
      {
        return std::to_string(player_info.plMeepleCount(0)) + std::to_string(player_info.plMeepleCount(1));
      }
    }
    else if (action_message_ == "PlaceMeeple" and place_3_meeples_active_)
    {
      if (pl_num_ == 1 and (current_meeples_pl_0 >= 1 and current_meeples_pl_0 <= 4))
      {
        meeple_for_pl0 = std::to_string(current_meeples_pl_0 - 3);
        meeple_for_pl1 = std::to_string(player_info.plMeepleCount(1));
      }
      else if (pl_num_ == 0 and (current_meeples_pl_1 >= 1 and current_meeples_pl_1 <= 4))
      {
        meeple_for_pl0 = std::to_string(player_info.plMeepleCount(0));
        meeple_for_pl1 = std::to_string(current_meeples_pl_1 - 3);
      }
      else
      {
        return std::to_string(player_info.plMeepleCount(0)) + std::to_string(player_info.plMeepleCount(1));
      }
    }
    else if (action_message_ != "PlaceMeeple" and action_message_ != "UnloadBoat")
    {
      return std::to_string(player_info.plMeepleCount(0)) + std::to_string(player_info.plMeepleCount(1));
    }
  }
  else
  {
    return std::to_string(player_info.plMeepleCount(0)) + std::to_string(player_info.plMeepleCount(1));
  }
  return meeple_for_pl0 + meeple_for_pl1;
}

void playerStats::getCountOfMinusesAndMeeples(std::vector<std::string> &meeples_on_board, size_t &count_of_minuses)
{
  if (place_unload_active_ or place_unload_double_active_)
  {
    for (size_t index = 0; index < new_board_.size(); index++)
    {
      meeples_on_board.push_back(new_board_[index]);
    }
    count_of_minuses = std::count_if((board_map_after_place_meeple_.at(legal_moves_obj.action_num_[0] - '0')).begin(),
                                     (board_map_after_place_meeple_.at(legal_moves_obj.action_num_[0] - '0')).end(), [](char c)
                                     { return c == '-'; });
  }
  else
  {
    meeples_on_board = parse_object.getBoard();
    count_of_minuses = std::count_if((board_map_.at(legal_moves_obj.action_num_[0] - '0')).begin(),
                                     (board_map_.at(legal_moves_obj.action_num_[0] - '0')).end(), [](char c)
                                     { return c == '-'; });
  }
}
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
std::string playerStats::connectBoatAndBoard(std::string token_name)
{
  std::string num_pl_0, num_pl_1;
  std::vector<std::string> meeples_on_board;
  size_t count_of_minuses = 0;

  getCountOfMinusesAndMeeples(meeples_on_board, count_of_minuses);

  int count_pl_0 = 0;
  int count_pl_1 = 0;
  int meeple_index = 0;
  int count_of_empty_spaces = 0;
  count_of_empty_spaces = 3 - int(count_of_minuses);
  // if (legal_moves_obj.action_num_[0] - '0' == second_action_num_for_unload_ and flag_active)
  // {
  //   flag_active = false;
  // }
  for (int main_index = 2; main_index >= 0; main_index--)
  {
    if (parse_object.getBoats()[legal_moves_obj.action_num_[0] - '0'][main_index] == token_name and count_of_empty_spaces > 0)
    {
      meeple_index = main_index;
      if (legal_moves_obj.action_num_[0] - '0' == 0)
      {
        if (meeples_on_board[meeple_index] == "-1 " or meeples_on_board[2] == "-1 ")
        {
          meeple_index--;
        }
        if (meeples_on_board[meeple_index] == "0 ")
        {
          count_pl_0++;
        }
        if (meeples_on_board[meeple_index] == "1 ")
        {
          count_pl_1++;
        }
      }
      else if (legal_moves_obj.action_num_[0] - '0' == 1)
      {
        meeple_index += 3;
        if (meeples_on_board[meeple_index] == "-1 " or meeples_on_board[5] == "-1 ")
        {
          meeple_index--;
        }
        if (meeples_on_board[meeple_index] == "0 ")
        {
          count_pl_0++;
        }
        if (meeples_on_board[meeple_index] == "1 ")
        {
          count_pl_1++;
        }
      }
      else if (legal_moves_obj.action_num_[0] - '0' == 2)
      {
        meeple_index += 6;
        if (meeples_on_board[meeple_index] == "-1 " or meeples_on_board[8] == "-1 ")
        {
          meeple_index--;
        }
        if (meeples_on_board[meeple_index] == "0 ")
        {
          count_pl_0++;
        }
        if (meeples_on_board[meeple_index] == "1 ")
        {
          count_pl_1++;
        }
      }
      else if (legal_moves_obj.action_num_[0] - '0' == 3)
      {
        if (meeple_index == 2)
        {
          meeple_index = 8;
        }
        else if (meeple_index == 1)
        {
          meeple_index = 5;
        }
        else if (meeple_index == 0)
        {
          meeple_index = 2;
        }
        if (meeples_on_board[meeple_index] == "-1 " or meeples_on_board[8] == "-1 ")
        {
          meeple_index -= 3;
        }
        if (meeples_on_board[meeple_index] == "0 ")
        {
          count_pl_0++;
        }
        if (meeples_on_board[meeple_index] == "1 ")
        {
          count_pl_1++;
        }
      }
      else if (legal_moves_obj.action_num_[0] - '0' == 4)
      {
        if (meeple_index == 2)
        {
          meeple_index = 7;
        }
        else if (meeple_index == 1)
        {
          meeple_index = 4;
        }
        else if (meeple_index == 0)
        {
          meeple_index = 1;
        }
        if (meeples_on_board[meeple_index] == "-1 " or meeples_on_board[7] == "-1 ")
        {
          meeple_index -= 3;
        }
        if (meeples_on_board[meeple_index] == "0 ")
        {
          count_pl_0++;
        }
        if (meeples_on_board[meeple_index] == "1 ")
        {
          count_pl_1++;
        }
      }
      else if (legal_moves_obj.action_num_[0] - '0' == 5)
      {
        if (meeple_index == 1)
        {
          meeple_index = 3;
        }
        else if (meeple_index == 2)
        {
          meeple_index = 6;
        }
        if (meeples_on_board[meeple_index] == "-1 " or meeples_on_board[6] == "-1 ")
        {
          meeple_index -= 3;
        }
        if (meeples_on_board[meeple_index] == "0 ")
        {
          count_pl_0++;
        }
        if (meeples_on_board[meeple_index] == "1 ")
        {
          count_pl_1++;
        }
      }
    }
    count_of_empty_spaces--;
  }
  num_pl_0 = std::to_string(count_pl_0);
  num_pl_1 = std::to_string(count_pl_1);
  return num_pl_0 + num_pl_1;
}

std::vector<std::string> playerStats::finalAmountObelisks()
{
  std::string pl_0_obelisks = "", pl_1_obelisks = "";
  std::vector<std::string> meeples_on_board = parse_object.getBoard();
  int current_obelisks_pl_0 = player_info.plAmountObelisks(0);
  int current_obelisks_pl_1 = player_info.plAmountObelisks(1);
  std::string obelisks = "";
  int obelisks_pl_0 = 0;
  int obelisks_pl_1 = 0;
  if (legal_moves_obj.action_message_ == "UnloadBoat" and (legal_moves_obj.checkIfLegal() == "Okay" or legal_moves_obj.checkIfLegal() == "GameEnded"))
  {
    obelisks = connectBoatAndBoard("Obelisk ");
    obelisks_pl_0 = obelisks[0] - '0';
    obelisks_pl_1 = obelisks[1] - '0';
    pl_0_obelisks = std::to_string(current_obelisks_pl_0 + obelisks_pl_0);
    pl_1_obelisks = std::to_string(current_obelisks_pl_1 + obelisks_pl_1);
  }
  else if (legal_moves_obj.action_message_ == "TakeTile" and legal_moves_obj.checkIfLegal() == "Okay")
  {
    obelisks.append(legal_moves_obj.boats_[legal_moves_obj.action_num_[0] - '0'][legal_moves_obj.take_tile_action_num_[0] - '0']);
    obelisks.pop_back();
    if (obelisks == "Obelisk")
    {
      if (pl_num_ == 1)
      {
        pl_0_obelisks = std::to_string(current_obelisks_pl_0 + 1);
        pl_1_obelisks = std::to_string(current_obelisks_pl_1);
      }
      else
      {
        pl_0_obelisks = std::to_string(current_obelisks_pl_0);
        pl_1_obelisks = std::to_string(current_obelisks_pl_1 + 1);
      }
    }
    else
    {
      pl_0_obelisks = std::to_string(current_obelisks_pl_0);
      pl_1_obelisks = std::to_string(current_obelisks_pl_1);
    }
  }
  else
  {
    pl_0_obelisks = std::to_string(current_obelisks_pl_0);
    pl_1_obelisks = std::to_string(current_obelisks_pl_1);
  }
  std::vector<std::string> return_vector_of_obelisks;
  return_vector_of_obelisks.push_back(pl_0_obelisks);
  return_vector_of_obelisks.push_back(pl_1_obelisks);
  return return_vector_of_obelisks;
}

int playerStats::catchException(std::string string_to_check)
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

std::string playerStats::tempelsForTakeTileAction()
{
  std::vector<std::string> current_temples_pl_0 = player_info.plTemples(0);
  std::vector<std::string> current_temples_pl_1 = player_info.plTemples(1);
  std::vector<char> temples_pl0;
  std::vector<char> temples_pl1;
  std::string updated_temples = "";
  std::string current_temple_index = "";
  std::string token_on_boat = "";
  int temples_at_index = 0;
  token_on_boat.append(legal_moves_obj.boats_[legal_moves_obj.action_num_[0] - '0'][legal_moves_obj.take_tile_action_num_[0] - '0']);
  token_on_boat.pop_back();
  if (pl_num_ == 1)
  {
    for (int temple_index = 1; temple_index < 5; temple_index++)
    {
      if (temple_index == token_on_boat.back() - '0')
      {
        temples_pl0.push_back('1');
      }
      else
      {
        temples_pl0.push_back('0');
      }
    }
    for (int temple_index = 0; temple_index < 4; temple_index++)
    {
      temples_at_index = catchException(current_temples_pl_0[temple_index]);
      updated_temples.append(std::to_string((temples_pl0[temple_index] - '0') + temples_at_index));
    }
    updated_temples.append(" ");
    for (int temple_index = 0; temple_index < 4; temple_index++)
    {
      temples_at_index = catchException(current_temples_pl_1[temple_index]);
      updated_temples.append(std::to_string(temples_at_index));
    }
  }
  else
  {
    for (int temple_index = 1; temple_index < 5; temple_index++)
    {
      if (temple_index == token_on_boat.back() - '0')
      {
        temples_pl1.push_back('1');
      }
      else
      {
        temples_pl1.push_back('0');
      }
    }
    for (int temple_index = 0; temple_index < 4; temple_index++)
    {
      temples_at_index = catchException(current_temples_pl_0[temple_index]);
      updated_temples.append(std::to_string(temples_at_index));
    }
    updated_temples.append(" ");
    for (int temple_index = 0; temple_index < 4; temple_index++)
    {
      temples_at_index = catchException(current_temples_pl_1[temple_index]);
      updated_temples.append(std::to_string((temples_pl1[temple_index] - '0') + temples_at_index));
    }
  }
  return updated_temples;
}

std::string playerStats::finalTemples()
{
  std::vector<std::string> current_temples_pl_0 = player_info.plTemples(0);
  std::vector<std::string> current_temples_pl_1 = player_info.plTemples(1);
  std::vector<char> temples_pl0;
  std::vector<char> temples_pl1;
  std::string updated_temples = "";
  std::string current_temple_index = "";
  std::string token_on_boat = "";
  int temples_at_index = 0;
  if (legal_moves_obj.action_message_ == "UnloadBoat" and (legal_moves_obj.checkIfLegal() == "Okay" or legal_moves_obj.checkIfLegal() == "GameEnded"))
  {
    for (int temple_index = 1; temple_index < 5; temple_index++)
    {
      current_temple_index = "Temple~" + std::to_string(temple_index) + " ";
      temples_pl0.push_back(connectBoatAndBoard(current_temple_index)[0]);
      temples_pl1.push_back(connectBoatAndBoard(current_temple_index)[1]);
    }
    for (int temple_index = 0; temple_index < 4; temple_index++)
    {
      temples_at_index = catchException(current_temples_pl_0[temple_index]);
      updated_temples.append(std::to_string((temples_pl0[temple_index] - '0') + temples_at_index));
    }

    updated_temples.append(" ");
    for (int temple_index = 0; temple_index < 4; temple_index++)
    {
      temples_at_index = catchException(current_temples_pl_1[temple_index]);
      updated_temples.append(std::to_string((temples_pl1[temple_index] - '0') + temples_at_index));
    }
  }
  else if (legal_moves_obj.action_message_ == "TakeTile" and legal_moves_obj.checkIfLegal() == "Okay")
  {
    updated_temples = tempelsForTakeTileAction();
  }
  else
  {
    for (int index = 0; index < 4; index++)
    {
      temples_at_index = catchException(current_temples_pl_0[index]);
      updated_temples.append(std::to_string(temples_at_index));
    }
    updated_temples.append(" ");
    for (int index = 0; index < 4; index++)
    {
      temples_at_index = catchException(current_temples_pl_1[index]);
      updated_temples.append(std::to_string(temples_at_index));
    }
  }
  return updated_temples;
}

std::string playerStats::finalAmountPyramidLight()
{
  std::string pl_0_pyramid_light = "", pl_1_pyramid_light = "";
  std::vector<std::string> meeples_on_board = parse_object.getBoard();
  int curr_pyramid_light_pl_0 = player_info.plAmountPyramidLight(0);
  int curr_pyramid_light_pl_1 = player_info.plAmountPyramidLight(1);
  std::string pyramid_light = "";
  int pyramid_light_pl_0 = 0;
  int pyramid_light_pl_1 = 0;
  if (legal_moves_obj.action_message_ == "UnloadBoat" and (legal_moves_obj.checkIfLegal() == "Okay" or legal_moves_obj.checkIfLegal() == "GameEnded"))
  {
    pyramid_light = connectBoatAndBoard("PyramidLight ");
    pyramid_light_pl_0 = pyramid_light[0] - '0';
    pyramid_light_pl_1 = pyramid_light[1] - '0';

    pl_0_pyramid_light = std::to_string(curr_pyramid_light_pl_0 + pyramid_light_pl_0);
    pl_1_pyramid_light = std::to_string(curr_pyramid_light_pl_1 + pyramid_light_pl_1);
  }
  else if (legal_moves_obj.action_message_ == "TakeTile" and legal_moves_obj.checkIfLegal() == "Okay")
  {
    pyramid_light.append(legal_moves_obj.boats_[legal_moves_obj.action_num_[0] - '0'][legal_moves_obj.take_tile_action_num_[0] - '0']);
    pyramid_light.pop_back();

    if (pyramid_light == "PyramidLight")
    {
      if (pl_num_ == 1)
      {
        pl_0_pyramid_light = std::to_string(curr_pyramid_light_pl_0 + 1);
        pl_1_pyramid_light = std::to_string(curr_pyramid_light_pl_1);
      }
      else
      {
        pl_0_pyramid_light = std::to_string(curr_pyramid_light_pl_0);
        pl_1_pyramid_light = std::to_string(curr_pyramid_light_pl_1 + 1);
      }
    }
    else
    {
      pl_0_pyramid_light = std::to_string(curr_pyramid_light_pl_0);
      pl_1_pyramid_light = std::to_string(curr_pyramid_light_pl_1);
    }
  }
  else
  {
    pl_0_pyramid_light = std::to_string(curr_pyramid_light_pl_0);
    pl_1_pyramid_light = std::to_string(curr_pyramid_light_pl_1);
  }
  return pl_0_pyramid_light + pl_1_pyramid_light;
}

std::string playerStats::finalAmountPyramidDark()
{
  std::string pl_0_pyramid_dark = "", pl_1_pyramid_dark = "";
  std::vector<std::string> meeples_on_board = parse_object.getBoard();
  int curr_pyramid_dark_pl_0 = player_info.plAmountPyramidDark(0);
  int curr_pyramid_dark_pl_1 = player_info.plAmountPyramidDark(1);
  std::string pyramid_dark = "";
  int pyramid_dark_pl_0 = 0;
  int pyramid_dark_pl_1 = 0;
  if (legal_moves_obj.action_message_ == "UnloadBoat" and (legal_moves_obj.checkIfLegal() == "Okay" or legal_moves_obj.checkIfLegal() == "GameEnded"))
  {
    pyramid_dark = connectBoatAndBoard("PyramidDark ");
    pyramid_dark_pl_0 = pyramid_dark[0] - '0';
    pyramid_dark_pl_1 = pyramid_dark[1] - '0';
    pl_0_pyramid_dark = std::to_string(curr_pyramid_dark_pl_0 + pyramid_dark_pl_0);
    pl_1_pyramid_dark = std::to_string(curr_pyramid_dark_pl_1 + pyramid_dark_pl_1);
  }
  else if (legal_moves_obj.action_message_ == "TakeTile" and legal_moves_obj.checkIfLegal() == "Okay")
  {
    pyramid_dark.append(legal_moves_obj.boats_[legal_moves_obj.action_num_[0] - '0'][legal_moves_obj.take_tile_action_num_[0] - '0']);
    pyramid_dark.pop_back();

    if (pyramid_dark == "PyramidDark")
    {
      if (pl_num_ == 1)
      {
        pl_0_pyramid_dark = std::to_string(curr_pyramid_dark_pl_0 + 1);
        pl_1_pyramid_dark = std::to_string(curr_pyramid_dark_pl_1);
      }
      else
      {
        pl_0_pyramid_dark = std::to_string(curr_pyramid_dark_pl_0);
        pl_1_pyramid_dark = std::to_string(curr_pyramid_dark_pl_1 + 1);
      }
    }
    else
    {
      pl_0_pyramid_dark = std::to_string(curr_pyramid_dark_pl_0);
      pl_1_pyramid_dark = std::to_string(curr_pyramid_dark_pl_1);
    }
  }
  else
  {
    pl_0_pyramid_dark = std::to_string(curr_pyramid_dark_pl_0);
    pl_1_pyramid_dark = std::to_string(curr_pyramid_dark_pl_1);
  }
  return pl_0_pyramid_dark + pl_1_pyramid_dark;
}

std::string playerStats::finalTombs()
{
  int pl0_tombs = player_info.plTombs(0);
  int pl1_tombs = player_info.plTombs(1);
  std::string binary_pl0 = std::bitset<12>(pl0_tombs).to_string();
  std::string binary_pl1 = std::bitset<12>(pl1_tombs).to_string();
  std::string current_tomb_index = "";
  std::vector<char> tombs_pl0;
  std::vector<char> tombs_pl1;
  std::string final_string1 = "", final_string2 = "";
  std::string current_tomb_from_boat = "";
  if (legal_moves_obj.action_message_ == "UnloadBoat" and (legal_moves_obj.checkIfLegal() == "Okay" or legal_moves_obj.checkIfLegal() == "GameEnded"))
  {
    for (int tomb_index = 1; tomb_index <= 12; tomb_index++)
    {
      current_tomb_index = "Tomb~" + std::to_string(tomb_index) + " ";
      tombs_pl0.push_back(connectBoatAndBoard(current_tomb_index)[0]);
      tombs_pl1.push_back(connectBoatAndBoard(current_tomb_index)[1]);
    }
    for (int tomb_index = 0; tomb_index < 12; tomb_index++)
    {
      final_string1.append(std::to_string((tombs_pl0[11 - tomb_index] - '0') + (binary_pl0[tomb_index] - '0')));
      final_string2.append(std::to_string((tombs_pl1[11 - tomb_index] - '0') + (binary_pl1[tomb_index] - '0')));
    }
  }
  else if (legal_moves_obj.action_message_ == "TakeTile" and legal_moves_obj.checkIfLegal() == "Okay") // dodao sam za take tile
  {
    std::string some_variable = "";
    some_variable.append(legal_moves_obj.boats_[legal_moves_obj.action_num_[0] - '0'][legal_moves_obj.take_tile_action_num_[0] - '0']);
    some_variable.pop_back();
    if (pl_num_ == 1)
    {
      for (int tomb_index = 1; tomb_index <= 12; tomb_index++)
      {
        if (tomb_index == some_variable.back() - '0')
        {
          tombs_pl0.push_back('1');
        }
        else
        {
          tombs_pl0.push_back('0');
        }
      }
      for (int tomb_index = 0; tomb_index < 12; tomb_index++)
      {
        final_string1.append(std::to_string((tombs_pl0[11 - tomb_index] - '0') + (binary_pl0[tomb_index] - '0')));
      }
      final_string2.append(binary_pl1);
    }
    else
    {
      final_string1.append(binary_pl0);
      for (int tomb_index = 1; tomb_index <= 12; tomb_index++)
      {
        if (tomb_index == some_variable.back() - '0')
        {
          tombs_pl1.push_back('1');
        }
        else
        {
          tombs_pl1.push_back('0');
        }
      }
      for (int tomb_index = 0; tomb_index < 12; tomb_index++)
      {
        final_string2.append(std::to_string((tombs_pl1[11 - tomb_index] - '0') + (binary_pl1[tomb_index] - '0')));
      }
    }
  }
  else
  {
    final_string1.append(binary_pl0);
    final_string2.append(binary_pl1);
  }
  unsigned long decimal_pl0 = std::bitset<12>(final_string1).to_ulong();
  unsigned long decimal_pl1 = std::bitset<12>(final_string2).to_ulong();
  return std::to_string(decimal_pl0) + " " + std::to_string(decimal_pl1);
}

std::vector<std::string> playerStats::partOfPointsASide(bool syntax_error_flag)
{
  std::vector<std::string> final_player_points;
  int sum_pl_0 = 0, sum_pl_1 = 0;
  std::string meeples_on_board = finalMeepleCount(syntax_error_flag);
  sum_pl_0 += 4 - (meeples_on_board[0] - '0');
  sum_pl_1 += 4 - (meeples_on_board[1] - '0');
  int final_obelisk_at_front = 0, final_obelisk_at_back = 0;
  final_obelisk_at_front = catchException(finalAmountObelisks().front());
  final_obelisk_at_back = catchException(finalAmountObelisks().back());

  // std::cout << "\n front i back obelisci: " << final_obelisk_at_front << " --- " << final_obelisk_at_back << std::endl;

  if (final_obelisk_at_front > final_obelisk_at_back)
  {
    sum_pl_0 += 6;
    sum_pl_0 += final_obelisk_at_front;
    sum_pl_1 += final_obelisk_at_back;
  }
  else if (final_obelisk_at_front < final_obelisk_at_back)
  {
    sum_pl_1 += 6;
    sum_pl_0 += final_obelisk_at_front;
    sum_pl_1 += final_obelisk_at_back;
  }
  else
  {
    sum_pl_0 += final_obelisk_at_front;
    sum_pl_1 += final_obelisk_at_back;
  }

  std::string temple_count = finalTemples();
  sum_pl_0 += (temple_count[0] - '0') + 2 * (temple_count[1] - '0') + 3 * (temple_count[2] - '0') + 4 * (temple_count[3] - '0');
  sum_pl_1 += (temple_count[5] - '0') + 2 * (temple_count[6] - '0') + 3 * (temple_count[7] - '0') + 4 * (temple_count[8] - '0');
  std::string final_light = finalAmountPyramidLight();
  std::string final_dark = finalAmountPyramidDark();
  sum_pl_0 += pyramid_map_.at(final_light[0] - '0');
  sum_pl_1 += pyramid_map_.at(final_light[1] - '0');
  sum_pl_0 += pyramid_map_.at(final_dark[0] - '0');
  sum_pl_1 += pyramid_map_.at(final_dark[1] - '0');
  final_player_points.push_back(std::to_string(sum_pl_0));
  final_player_points.push_back(std::to_string(sum_pl_1));
  return final_player_points;
}

std::vector<std::string> playerStats::playerPointsASide(bool syntax_error_flag)
{
  std::vector<std::string> final_player_points;
  int sum_pl_0 = 0, sum_pl_1 = 0;
  sum_pl_0 = catchException(partOfPointsASide(syntax_error_flag).front());
  sum_pl_1 = catchException(partOfPointsASide(syntax_error_flag).back());
  std::string current_tomb_0 = "", current_tomb_1 = "";
  for (size_t tomb_outer_counter = 0; tomb_outer_counter < finalTombs().length(); tomb_outer_counter++)
  {
    if (finalTombs()[tomb_outer_counter] != ' ')
    {
      current_tomb_0.append(std::to_string(finalTombs()[tomb_outer_counter] - '0'));
    }
    else
    {
      break;
    }
  }
  int beginning_index = 0;
  while (1)
  {
    if (finalTombs()[beginning_index] == ' ')
    {
      for (size_t tomb_inner_counter = beginning_index + 1; tomb_inner_counter < finalTombs().length(); tomb_inner_counter++)
      {
        current_tomb_1.append(std::to_string(finalTombs()[tomb_inner_counter] - '0'));
      }
      break;
    }
    beginning_index++;
  }
  std::vector<int> pl_0_ones;
  std::vector<int> pl_1_ones;
  int current_tom_pl0 = 0, current_tom_pl1 = 0, count_ones = 0;
  current_tom_pl0 = catchException(current_tomb_0);
  current_tom_pl1 = catchException(current_tomb_1);
  current_tomb_0 = std::bitset<12>(current_tom_pl0).to_string();
  current_tomb_1 = std::bitset<12>(current_tom_pl1).to_string();
  for (size_t index = 0; index < current_tomb_0.length(); index++)
  {
    if (current_tomb_0[index] != '0')
    {
      count_ones++;
    }
    else
    {
      if (count_ones > 0)
        pl_0_ones.push_back(count_ones);
      count_ones = 0;
    }
  }
  count_ones = 0;
  for (size_t index = 0; index < current_tomb_1.length(); index++)
  {
    if (current_tomb_1[index] != '0')
    {
      count_ones++;
    }
    else
    {
      if (count_ones > 0)
        pl_1_ones.push_back(count_ones);
      count_ones = 0;
    }
  }
  for (size_t index = 0; index < pl_0_ones.size(); index++)
  {
    if (pl_0_ones[index] > 5)
    {
      sum_pl_0 += 25;
    }
    else
    {
      sum_pl_0 += tomb_map_.at(pl_0_ones[index]);
    }
  }
  for (size_t index = 0; index < pl_1_ones.size(); index++)
  {
    if (pl_1_ones[index] > 5)
    {
      sum_pl_1 += 25;
    }
    else
    {
      sum_pl_1 += tomb_map_.at(pl_1_ones[index]);
    }
  }
  // if (legal_moves_obj.checkIfLegal() == "GameEnded")
  // {
  //   final_player_points.push_back(std::to_string(sum_pl_0));
  //   final_player_points.push_back(std::to_string(sum_pl_1));
  // }
  // else
  // {
  //   final_player_points.push_back("0");
  // }
  if (current_gamestate == "GameEnded")
  {
    sum_player_0_ = sum_pl_0;
    sum_player_1_ = sum_pl_1;
    final_player_points.push_back("points");
  }
  else
  {
    final_player_points.push_back("0");
  }
  return final_player_points;
}

std::vector<std::string> playerStats::firstPartOfPointsBSide(bool syntax_error_flag)
{
  std::vector<std::string> final_player_points;
  int sum_pl_0 = 0, sum_pl_1 = 0;
  std::string meeples_on_board = finalMeepleCount(syntax_error_flag);
  sum_pl_0 += 4 - (meeples_on_board[0] - '0');
  sum_pl_1 += 4 - (meeples_on_board[1] - '0');
  int final_obelisk_at_front = 0, final_obelisk_at_back = 0;
  final_obelisk_at_front = catchException(finalAmountObelisks().front());
  final_obelisk_at_back = catchException(finalAmountObelisks().back());
  if (finalFirstObelisk()[0] == '1')
  {
    sum_pl_0 += 12;
    if (final_obelisk_at_back >= 5)
    {
      sum_pl_1 += 6;
      if (final_obelisk_at_back >= 10)
      {
        sum_pl_1 += 12;
      }
    }
    if (final_obelisk_at_front >= 10)
    {
      sum_pl_0 += 6;
    }
  }
  else if (finalFirstObelisk()[1] == '1')
  {
    sum_pl_1 += 12;
    if (final_obelisk_at_front >= 5)
    {
      sum_pl_0 += 6;
      if (final_obelisk_at_front >= 10)
      {
        sum_pl_0 += 12;
      }
    }
    if (final_obelisk_at_back >= 10)
    {
      sum_pl_1 += 6;
    }
  }
  final_player_points.push_back(std::to_string(sum_pl_0));
  final_player_points.push_back(std::to_string(sum_pl_1));
  return final_player_points;
}

std::vector<std::string> playerStats::secondPartOfPointsBSide(int sum_pl_0, int sum_pl_1)
{
  std::vector<std::string> final_player_points;
  std::string temple_count = finalTemples();
  std::string temples_for_first_player = std::to_string(temple_count[0] - '0') + std::to_string(temple_count[1] - '0') +
                                         std::to_string(temple_count[2] - '0') + std::to_string(temple_count[3] - '0');
  std::string temples_for_second_player = std::to_string(temple_count[5] - '0') + std::to_string(temple_count[6] - '0') +
                                          std::to_string(temple_count[7] - '0') + std::to_string(temple_count[8] - '0');
  int final_sum_of_meeples_for_first = 0, final_sum_of_meeples_for_second = 0, current_sum = 0;
  while (1)
  {
    for (int i = 0; i < 4; i++)
    {
      if (temples_for_first_player[i] != '0')
      {
        current_sum += 1;
        temples_for_first_player[i] = temples_for_first_player[i] - 1;
      }
    }
    if (current_sum == 0)
    {
      break;
    }
    final_sum_of_meeples_for_first += temples_B_side_map_.at(current_sum);
    current_sum = 0;
  }
  while (1)
  {
    for (int i = 0; i < 4; i++)
    {
      if (temples_for_second_player[i] != '0')
      {
        current_sum += 1;
        temples_for_second_player[i] = temples_for_second_player[i] - 1;
      }
    }
    if (current_sum == 0)
    {
      break;
    }
    final_sum_of_meeples_for_second += temples_B_side_map_.at(current_sum);
    current_sum = 0;
  }
  sum_pl_0 += final_sum_of_meeples_for_first;
  sum_pl_1 += final_sum_of_meeples_for_second;
  std::string final_light = finalAmountPyramidLight();
  std::string final_dark = finalAmountPyramidDark();
  if ((final_light[0] - '0') <= (final_dark[0] - '0'))
  {
    sum_pl_0 += pyramid_B_side_map_.at(final_light[0] - '0');
  }
  else
  {
    sum_pl_0 += pyramid_B_side_map_.at(final_dark[0] - '0');
  }
  if ((final_light[1] - '0') <= (final_dark[1] - '0'))
  {
    sum_pl_1 += pyramid_B_side_map_.at(final_light[1] - '0');
  }
  else
  {
    sum_pl_1 += pyramid_B_side_map_.at(final_dark[1] - '0');
  }
  final_player_points.push_back(std::to_string(sum_pl_0));
  final_player_points.push_back(std::to_string(sum_pl_1));
  return final_player_points;
}

std::vector<std::string> playerStats::playerPointsBSide(bool syntax_error_flag)
{
  std::vector<std::string> final_player_points;
  int sum_pl_0 = 0, sum_pl_1 = 0;
  sum_pl_0 = catchException(firstPartOfPointsBSide(syntax_error_flag).front());
  sum_pl_1 = catchException(firstPartOfPointsBSide(syntax_error_flag).back());
  sum_pl_0 = catchException(secondPartOfPointsBSide(sum_pl_0, sum_pl_1).front());
  sum_pl_1 = catchException(secondPartOfPointsBSide(sum_pl_0, sum_pl_1).back());
  std::string current_tomb_0 = "", current_tomb_1 = "";
  for (size_t tomb_outer_counter = 0; tomb_outer_counter < finalTombs().length(); tomb_outer_counter++)
  {
    if (finalTombs()[tomb_outer_counter] != ' ')
    {
      current_tomb_0.append(std::to_string(finalTombs()[tomb_outer_counter] - '0'));
    }
    else
    {
      break;
    }
  }
  int beginning_index = 0;
  while (1)
  {
    if (finalTombs()[beginning_index] == ' ')
    {
      for (size_t tomb_inner_counter = beginning_index + 1; tomb_inner_counter < finalTombs().length(); tomb_inner_counter++)
      {
        current_tomb_1.append(std::to_string(finalTombs()[tomb_inner_counter] - '0'));
      }
      break;
    }
    beginning_index++;
  }
  std::vector<int> pl_0_ones;
  std::vector<int> pl_1_ones;
  int current_tom_pl0 = 0, current_tom_pl1 = 0, count_ones = 0;
  current_tom_pl0 = catchException(current_tomb_0);
  current_tom_pl1 = catchException(current_tomb_1);
  current_tomb_0 = std::bitset<12>(current_tom_pl0).to_string();
  current_tomb_1 = std::bitset<12>(current_tom_pl1).to_string();
  for (size_t index = 0; index < current_tomb_0.length(); index++)
  {
    if (current_tomb_0[index] != '0')
    {
      count_ones++;
    }
    else
    {
      if (count_ones > 0)
        pl_0_ones.push_back(count_ones);
      count_ones = 0;
    }
  }
  if (count_ones != 0)
  {
    pl_0_ones.push_back(count_ones);
  }
  count_ones = 0;
  for (size_t index = 0; index < current_tomb_1.length(); index++)
  {
    if (current_tomb_1[index] != '0')
    {
      count_ones++;
    }
    else
    {
      if (count_ones > 0)
      {
        pl_1_ones.push_back(count_ones);
      }
      count_ones = 0;
    }
  }
  if (count_ones != 0)
  {
    pl_1_ones.push_back(count_ones);
  }
  for (size_t index = 0; index < pl_0_ones.size(); index++)
  {
    if (pl_0_ones[index] > 5)
    {
      sum_pl_0 += 4;
    }
    else
    {
      sum_pl_0 += 4;
    }
  }
  for (size_t index = 0; index < pl_1_ones.size(); index++)
  {
    if (pl_1_ones[index] > 5)
    {
      sum_pl_1 += 4;
    }
    else
    {
      sum_pl_1 += 4;
    }
  }
  if (current_gamestate == "GameEnded")
  {
    sum_player_0_ = sum_pl_0;
    sum_player_1_ = sum_pl_1;
    final_player_points.push_back("points");
  }
  else
  {
    final_player_points.push_back("0");
  }
  return final_player_points;
}

std::string playerStats::playerCommentTakeTile(int &comment_pl_0, int &comment_pl_1)
{
  std::string pl_0_comment = "", pl_1_comment = "";
  std::string comment_0_operate = "", comment_1_operate = "";
  comment_0_operate = "TakeTile";
  comment_1_operate = "TakeTile";
  std::string comment_game = "";
  comment_pl_0 = 0;
  comment_pl_1 = 0;
  if (legal_moves_obj.action_message_ == "UnloadBoat" and (legal_moves_obj.checkIfLegal() == "Okay" or legal_moves_obj.checkIfLegal() == "GameEnded"))
  {
    comment_game = connectBoatAndBoard("TakeTile ");
    comment_pl_0 = comment_game[0] - '0';
    comment_pl_1 = comment_game[1] - '0';
    for (int i = 0; i < comment_pl_0; i++)
    {
      pl_0_comment += comment_0_operate;
    }
    for (int i = 0; i < comment_pl_1; i++)
    {
      pl_1_comment += comment_1_operate;
    }
  }
  return pl_0_comment + " " + pl_1_comment;
}

std::string playerStats::playerCommentPlaceMeeples(int &comment_pl_0, int &comment_pl_1)
{
  std::string pl_0_comment = "", pl_1_comment = "";
  std::string comment_0_operate, comment_1_operate;
  comment_0_operate = "PlaceMeeples";
  comment_1_operate = "PlaceMeeples";
  std::string comment_game = "";
  comment_pl_0 = 0;
  comment_pl_1 = 0;
  if (legal_moves_obj.action_message_ == "UnloadBoat" and (legal_moves_obj.checkIfLegal() == "Okay" or legal_moves_obj.checkIfLegal() == "GameEnded"))
  {
    comment_game = connectBoatAndBoard("PlaceMeeples ");
    comment_pl_0 = comment_game[0] - '0';
    comment_pl_1 = comment_game[1] - '0';
    for (int index = 0; index < comment_pl_0; index++)
    {
      pl_0_comment += comment_0_operate;
    }
    for (int index = 0; index < comment_pl_1; index++)
    {
      pl_1_comment += comment_1_operate;
    }
  }
  return pl_0_comment + " " + pl_1_comment;
}

std::string playerStats::playerCommentPlaceUnload(int &comment_pl_0, int &comment_pl_1)
{
  std::string pl_0_comment = "", pl_1_comment = "";
  std::string comment_0_operate = "", comment_1_operate = "";
  comment_0_operate = "PlaceUnload";
  comment_1_operate = "PlaceUnload";
  std::string comment_game = "";
  comment_pl_0 = 0;
  comment_pl_1 = 0;
  if (legal_moves_obj.action_message_ == "UnloadBoat" and (legal_moves_obj.checkIfLegal() == "Okay" or legal_moves_obj.checkIfLegal() == "GameEnded"))
  {
    comment_game = connectBoatAndBoard("PlaceUnload ");
    comment_pl_0 = comment_game[0] - '0';
    comment_pl_1 = comment_game[1] - '0';
    for (int index = 0; index < comment_pl_0; index++)
    {
      pl_0_comment += comment_0_operate;
    }
    for (int index = 0; index < comment_pl_1; index++)
    {
      pl_1_comment += comment_1_operate;
    }
  }
  return pl_0_comment + " " + pl_1_comment;
}

std::string playerStats::playerCommentSwapUnload(int &comment_pl_0, int &comment_pl_1)
{
  std::string pl_0_comment = "", pl_1_comment = "";
  std::string comment_0_operate = "", comment_1_operate = "";
  comment_0_operate = "SwapUnload";
  comment_1_operate = "SwapUnload";
  std::string comment_game = "";
  comment_pl_0 = 0;
  comment_pl_1 = 0;
  if (legal_moves_obj.action_message_ == "UnloadBoat" and (legal_moves_obj.checkIfLegal() == "Okay" or legal_moves_obj.checkIfLegal() == "GameEnded"))
  {
    comment_game = connectBoatAndBoard("SwapUnload ");
    comment_pl_0 = comment_game[0] - '0';
    comment_pl_1 = comment_game[1] - '0';
    for (int index = 0; index < comment_pl_0; index++)
    {
      pl_0_comment += comment_0_operate;
    }
    for (int index = 0; index < comment_pl_1; index++)
    {
      pl_1_comment += comment_1_operate;
    }
  }
  return pl_0_comment + " " + pl_1_comment;
}

std::string playerStats::lengthOfPlayerComment()
{
  std::string num_of_comment_pl_0 = "", num_of_comment_pl_1 = "";
  int curr_comm_pl_0 = player_info.plLengthOfActionArray(0);
  int curr_comm_pl_1 = player_info.plLengthOfActionArray(1);
  int tile_pl_0 = 0;
  int tile_pl_1 = 0;
  int place_meeple_pl_0 = 0;
  int place_meeple_pl_1 = 0;
  int place_unload_pl_0 = 0;
  int place_unload_pl_1 = 0;
  int swap_unload_pl_0 = 0;
  int swap_unload_pl_1 = 0;
  playerCommentTakeTile(tile_pl_0, tile_pl_1);
  playerCommentPlaceMeeples(place_meeple_pl_0, place_meeple_pl_1);
  playerCommentPlaceUnload(place_unload_pl_0, place_unload_pl_1);
  playerCommentSwapUnload(swap_unload_pl_0, swap_unload_pl_1);
  num_of_comment_pl_0 = std::to_string(tile_pl_0 + curr_comm_pl_0 + place_meeple_pl_0 + place_unload_pl_0 + swap_unload_pl_0);
  num_of_comment_pl_1 = std::to_string(tile_pl_1 + curr_comm_pl_1 + place_meeple_pl_1 + place_unload_pl_1 + swap_unload_pl_1);
  return num_of_comment_pl_0 + " " + num_of_comment_pl_1;
}

std::vector<std::string> playerStats::commentArrayFirstPlayer(int &comment_pl_0, int &comment_pl_1)
{
  std::vector<std::string> comment_arr_pl0;
  std::string length_of_player_comment = lengthOfPlayerComment();
  int tile_0 = 0, tile_1 = 0;
  std::string comment_tile = playerCommentTakeTile(tile_0, tile_1);
  int meeple_0 = 0, meeple_1 = 0;
  std::string comment_meeple = playerCommentPlaceMeeples(meeple_0, meeple_1);
  int unload_0 = 0, unload_1 = 0;
  std::string comment_unload = playerCommentPlaceUnload(unload_0, unload_1);
  int swap_0 = 0, swap_1 = 0;
  std::string comment_swap = playerCommentSwapUnload(swap_0, swap_1);
  for (auto index : player_info.plActionArray(0))
  {
    comment_arr_pl0.push_back(index);
  }

  if (legal_moves_obj.current_player_turn_ == 0)
  {
    if (legal_moves_obj.findActionTokenInMessage("PlaceMeeple") and legal_moves_obj.findActionTokenInMessage("UnloadBoat") and legal_moves_obj.checkIfLegal() == "Okay")
    {
      for (size_t index = 0; index < comment_arr_pl0.size(); index++)
      {
        if (comment_arr_pl0[index] == ("PlaceUnload "))
        {
          comment_arr_pl0.erase(comment_arr_pl0.begin() + index);
        }
      }
    }
    else if (legal_moves_obj.findActionTokenInMessage("PlaceMeeple") and legal_moves_obj.action_message_vec_.size() > 2 and !(legal_moves_obj.findActionTokenInMessage("UnloadBoat")) and legal_moves_obj.checkIfLegal() == "Okay")
    {
      for (size_t index = 0; index < comment_arr_pl0.size(); index++)
      {
        if (comment_arr_pl0[index] == ("PlaceMeeples "))
        {
          comment_arr_pl0.erase(comment_arr_pl0.begin() + index);
        }
      }
    }
    else if (legal_moves_obj.findActionTokenInMessage("Swap") and legal_moves_obj.findActionTokenInMessage("UnloadBoat") and legal_moves_obj.checkIfLegal() == "Okay")
    {
      for (size_t index = 0; index < comment_arr_pl0.size(); index++)
      {
        if (comment_arr_pl0[index] == ("SwapUnload "))
        {
          comment_arr_pl0.erase(comment_arr_pl0.begin() + index);
        }
      }
    }
    else if (legal_moves_obj.findActionTokenInMessage(legal_moves_obj.action_message_) and legal_moves_obj.checkIfLegal() == "Okay")
    {
      if (legal_moves_obj.action_message_.back() != ' ')
      {
        legal_moves_obj.action_message_ = legal_moves_obj.action_message_ + " ";
      }

      for (size_t index = 0; index < comment_arr_pl0.size(); index++)
      {
        if (comment_arr_pl0[index] == (legal_moves_obj.action_message_))
        {
          comment_arr_pl0.erase(comment_arr_pl0.begin() + index);
        }
      }
    }
  }
  if (legal_moves_obj.action_message_.back() == ' ')
    legal_moves_obj.action_message_.pop_back();

  std::string token_to_add = "";
  if (length_of_player_comment[0] != '0' and legal_moves_obj.action_message_ == "UnloadBoat")
  {
    for (int index = 0; index < tile_0; index++)
    {
      token_to_add.append(comment_tile);
      comment_arr_pl0.push_back(token_to_add);
      token_to_add.clear();
    }
    for (int index = 0; index < meeple_0; index++)
    {
      token_to_add.append(comment_meeple);
      comment_arr_pl0.push_back(token_to_add);
      token_to_add.clear();
    }
    for (int index = 0; index < unload_0; index++)
    {
      token_to_add.append(comment_unload);
      comment_arr_pl0.push_back(token_to_add);
      token_to_add.clear();
    }
    for (int index = 0; index < swap_0; index++)
    {
      token_to_add.append(comment_swap);
      comment_arr_pl0.push_back(token_to_add);
      token_to_add.clear();
    }
  }
  return comment_arr_pl0;
}

std::vector<std::string> playerStats::commentArraySecondPlayer(int &comment_pl_0, int &comment_pl_1)
{
  std::vector<std::string> comment_arr_pl1;
  std::string length_of_player_comment = lengthOfPlayerComment();
  int tile_0 = 0, tile_1 = 0;
  std::string comment_tile = playerCommentTakeTile(tile_0, tile_1);
  int meeple_0 = 0, meeple_1 = 0;
  std::string comment_meeple = playerCommentPlaceMeeples(meeple_0, meeple_1);
  int unload_0 = 0, unload_1 = 0;
  std::string comment_unload = playerCommentPlaceUnload(unload_0, unload_1);
  int swap_0 = 0, swap_1 = 0;
  std::string comment_swap = playerCommentSwapUnload(swap_0, swap_1);
  for (auto index : player_info.plActionArray(1))
  {
    comment_arr_pl1.push_back(index);
  }
  if (legal_moves_obj.current_player_turn_ == 1)
  {
    if (legal_moves_obj.findActionTokenInMessage("PlaceMeeple") and legal_moves_obj.findActionTokenInMessage("UnloadBoat") and (legal_moves_obj.checkIfLegal() == "Okay" or legal_moves_obj.checkIfLegal() == "GameEnded"))
    {
      for (size_t index = 0; index < comment_arr_pl1.size(); index++)
      {
        if (comment_arr_pl1[index] == ("PlaceUnload "))
        {
          comment_arr_pl1.erase(comment_arr_pl1.begin() + index);
        }
      }
    }
    else if (legal_moves_obj.findActionTokenInMessage("PlaceMeeple") and legal_moves_obj.action_message_vec_.size() > 2 and !(legal_moves_obj.findActionTokenInMessage("UnloadBoat")) and (legal_moves_obj.checkIfLegal() == "Okay" or legal_moves_obj.checkIfLegal() == "GameEnded"))
    {
      for (size_t index = 0; index < comment_arr_pl1.size(); index++)
      {
        if (comment_arr_pl1[index] == ("PlaceMeeples "))
        {
          comment_arr_pl1.erase(comment_arr_pl1.begin() + index);
        }
      }
    }
    else if (legal_moves_obj.findActionTokenInMessage("Swap") and legal_moves_obj.findActionTokenInMessage("UnloadBoat") and (legal_moves_obj.checkIfLegal() == "Okay" or legal_moves_obj.checkIfLegal() == "GameEnded"))
    {
      for (size_t index = 0; index < comment_arr_pl1.size(); index++)
      {
        if (comment_arr_pl1[index] == ("SwapUnload "))
        {
          comment_arr_pl1.erase(comment_arr_pl1.begin() + index);
        }
      }
    }
    else if (legal_moves_obj.findActionTokenInMessage(legal_moves_obj.action_message_) and (legal_moves_obj.checkIfLegal() == "Okay" or legal_moves_obj.checkIfLegal() == "GameEnded"))
    {
      if (legal_moves_obj.action_message_.back() != ' ')
      {
        legal_moves_obj.action_message_ = legal_moves_obj.action_message_ + " ";
      }

      for (size_t index = 0; index < comment_arr_pl1.size(); index++)
      {
        if (comment_arr_pl1[index] == legal_moves_obj.action_message_)
        {
          comment_arr_pl1.erase(comment_arr_pl1.begin() + index);
        }
      }
    }
  }
  std::string token_to_add = "";
  if (legal_moves_obj.action_message_.back() == ' ')
    legal_moves_obj.action_message_.pop_back();
  if (length_of_player_comment[2] != '0' and legal_moves_obj.action_message_ == "UnloadBoat")
  {
    for (int index = 0; index < tile_1; index++)
    {
      token_to_add.append("TakeTile ");
      comment_arr_pl1.push_back(token_to_add);
      token_to_add.clear();
    }
    for (int index = 0; index < meeple_1; index++)
    {
      token_to_add.append("PlaceMeeples ");
      comment_arr_pl1.push_back(token_to_add);
      token_to_add.clear();
    }
    for (int index = 0; index < swap_1; index++)
    {
      token_to_add.append("SwapUnload ");
      comment_arr_pl1.push_back(token_to_add);
      token_to_add.clear();
    }
    for (int index = 0; index < unload_1; index++)
    {
      token_to_add.append("PlaceUnload ");
      comment_arr_pl1.push_back(token_to_add);
      token_to_add.clear();
    }
  }
  return comment_arr_pl1;
}

void playerStats::partOfPlayerStats0ForTemples(std::string &player_stats0)
{
  std::string current_temples = "";
  if (place_unload_double_active_)
  {
    std::string temp = legal_moves_obj.action_num_;
    current_temples = finalTemples();
    legal_moves_obj.action_num_ = std::to_string(second_action_num_for_unload_);
    std::string new_temple_count = finalTemples();
    std::string temp_string = "";
    if (current_temples.substr(0, 4) != new_temple_count.substr(0, 4))
    {
      for (int i = 0; i < 4; i++)
      {
        temp_string.append(std::to_string((current_temples[i] - '0') + (new_temple_count[i] - '0')) + " ");
      }
      player_stats0.append(temp_string);
    }
    else
    {
      current_temples.clear();
      for (int index = 0; index < 4; index++)
      {
        current_temples.append(std::to_string(finalTemples()[index] - '0') + " ");
      }
      player_stats0.append(current_temples);
    }
    legal_moves_obj.action_num_ = temp;
  }
  else
  {
    for (int index = 0; index < 4; index++)
    {
      current_temples.append(std::to_string(finalTemples()[index] - '0') + " ");
    }
    player_stats0.append(current_temples);
  }
}

void playerStats::partOfPlayerStats0ForPyramid(std::string &player_stats0)
{
  int pyramid_double_active = 0;

  if (place_unload_double_active_)
  {
    std::string temp = legal_moves_obj.action_num_;
    pyramid_double_active = finalAmountPyramidDark()[0] - '0';
    legal_moves_obj.action_num_ = std::to_string(second_action_num_for_unload_);
    int temp_integer = finalAmountPyramidDark()[0] - '0';
    if (temp_integer != pyramid_double_active)
    {
      player_stats0.append(std::to_string(temp_integer) + " ");
    }
    else
    {
      player_stats0.append(std::to_string(pyramid_double_active) + " ");
    }
    legal_moves_obj.action_num_ = temp;
  }
  else
  {
    player_stats0.append(std::to_string(finalAmountPyramidDark()[0] - '0') + " ");
  }
}

std::string playerStats::PlayerStats0(bool flag_for_syntax_error, char current_side)
{
  std::string player_stats0 = "";
  player_stats0.append(player_info.plName(0));
  if (current_side == 'A')
  {
    player_stats0.append(playerPointsASide(flag_for_syntax_error).front() + " ");
  }
  else if (current_side == 'B')
  {
    player_stats0.append(playerPointsBSide(flag_for_syntax_error).front() + " ");
  }
  player_stats0.append(std::to_string(finalFirstObelisk()[0] - '0') + " ");
  player_stats0.append(std::to_string(finalMeepleCount(flag_for_syntax_error)[0] - '0') + " ");
  player_stats0.append(finalAmountObelisks().front() + " ");

  partOfPlayerStats0ForTemples(player_stats0);
  int pyramid_light_double_active = 0;
  if (place_unload_double_active_)
  {
    std::string temp = legal_moves_obj.action_num_;
    pyramid_light_double_active = finalAmountPyramidLight()[0] - '0';
    legal_moves_obj.action_num_ = std::to_string(second_action_num_for_unload_);
    int temp_integer = finalAmountPyramidLight()[0] - '0';
    if (temp_integer != pyramid_light_double_active)
    {
      player_stats0.append(std::to_string(temp_integer) + " ");
    }
    else
    {
      player_stats0.append(std::to_string(pyramid_light_double_active) + " ");
    }
    legal_moves_obj.action_num_ = temp;
  }
  else
  {
    player_stats0.append(std::to_string(finalAmountPyramidLight()[0] - '0') + " ");
  }

  partOfPlayerStats0ForPyramid(player_stats0);

  for (size_t tomb_index = 0; tomb_index < finalTombs().length(); tomb_index++)
  {
    if (finalTombs()[tomb_index] != ' ')
    {
      player_stats0.append(std::to_string(finalTombs()[tomb_index] - '0'));
    }
    else
    {
      player_stats0.append(" ");
      break;
    }
  }
  std::string length_of_player_comment = lengthOfPlayerComment();
  int first = 0, second = 0;
  std::vector<std::string> my_vec = commentArrayFirstPlayer(first, second);
  std::string converted_vector = std::accumulate(my_vec.begin(), my_vec.end(), std::string{},
                                                 [](std::string &s, const std::string &piece) -> decltype(auto)
                                                 { return s += piece; });

  player_stats0.append(std::to_string(int(my_vec.size())) + " ");

  // brisati karaktere dok ne nadje space sa kraja
  // 27.12. fix radi "UnloadBoatBob"

  if (converted_vector.back() != ' ')
  {
    size_t pos = converted_vector.find_last_of(' ');

    // If a space was found, erase everything after it
    if (pos != std::string::npos)
    {
      converted_vector.erase(pos);
      converted_vector.append(" ");
    }
  }

  player_stats0.append(converted_vector);

  if (current_gamestate == "GameEnded")
    player_stats0.replace(player_stats0.find("points"), 6, std::to_string(sum_player_0_ + int(my_vec.size())));

  return player_stats0;
}

std::string playerStats::partOfPlayerStats1(bool flag_for_syntax_error, char current_side)
{
  std::string player_stats1 = "", current_temples = "";
  player_stats1.append(player_info.plName(1));
  if (current_side == 'A')
  {
    player_stats1.append(playerPointsASide(flag_for_syntax_error).back() + " ");
  }
  else if (current_side == 'B')
  {
    player_stats1.append(playerPointsBSide(flag_for_syntax_error).back() + " ");
  }
  player_stats1.append(std::to_string(finalFirstObelisk()[1] - '0') + " ");
  player_stats1.append(std::to_string(finalMeepleCount(flag_for_syntax_error)[1] - '0') + " ");
  player_stats1.append(finalAmountObelisks().back() + " ");
  if (place_unload_double_active_)
  {
    std::string temp = legal_moves_obj.action_num_;
    current_temples = finalTemples();
    legal_moves_obj.action_num_ = std::to_string(second_action_num_for_unload_);
    std::string new_temple_count = finalTemples();
    std::string temp_string = "";
    if (current_temples.substr(5, 4) != new_temple_count.substr(5, 4))
    {
      for (int i = 5; i < 9; i++)
      {
        temp_string.append(std::to_string((current_temples[i] - '0') + (new_temple_count[i] - '0')) + " ");
      }
      player_stats1.append(temp_string);
    }
    else
    {
      current_temples.clear();
      for (int index = 5; index < 9; index++)
      {
        current_temples.append(std::to_string(finalTemples()[index] - '0') + " ");
      }
      player_stats1.append(current_temples);
    }
    legal_moves_obj.action_num_ = temp;
  }
  else
  {
    for (int index = 5; index < 9; index++)
    {
      current_temples.append(std::to_string(finalTemples()[index] - '0') + " ");
    }
    player_stats1.append(current_temples);
  }
  return player_stats1;
}
std::string playerStats::PlayerStats1(bool flag_for_syntax_error, char current_side)
{
  std::string player_stats1 = "", current_temples = "";
  player_stats1.append(partOfPlayerStats1(flag_for_syntax_error, current_side));
  int pyramid_light_double_active = 0;
  if (place_unload_double_active_)
  {
    std::string temp = legal_moves_obj.action_num_;
    pyramid_light_double_active = finalAmountPyramidLight()[1] - '0';
    legal_moves_obj.action_num_ = std::to_string(second_action_num_for_unload_);
    int temp_integer = finalAmountPyramidLight()[1] - '0';
    if (temp_integer != pyramid_light_double_active)
    {
      player_stats1.append(std::to_string(temp_integer) + " ");
    }
    else
    {
      player_stats1.append(std::to_string(pyramid_light_double_active) + " ");
    }
    legal_moves_obj.action_num_ = temp;
  }
  else
  {
    player_stats1.append(std::to_string(finalAmountPyramidLight()[1] - '0') + " ");
  }
  int pyramid_double_active;
  if (place_unload_double_active_)
  {
    std::string temp = legal_moves_obj.action_num_;
    pyramid_double_active = finalAmountPyramidDark()[1] - '0';
    legal_moves_obj.action_num_ = std::to_string(second_action_num_for_unload_);
    int temp_integer = finalAmountPyramidDark()[1] - '0';
    if (temp_integer != pyramid_double_active)
    {
      player_stats1.append(std::to_string(temp_integer) + " ");
    }
    else
    {
      player_stats1.append(std::to_string(pyramid_double_active) + " ");
    }
    legal_moves_obj.action_num_ = temp;
  }
  else
  {
    player_stats1.append(std::to_string(finalAmountPyramidDark()[1] - '0') + " ");
  }
  int counter = 0;
  while (1)
  {
    if (finalTombs()[counter] == ' ')
    {
      for (size_t index_of_current_tomb = counter + 1; index_of_current_tomb < finalTombs().length(); index_of_current_tomb++)
      {
        player_stats1.append(std::to_string(finalTombs()[index_of_current_tomb] - '0'));
      }
      break;
    }
    counter++;
  }
  std::string length_of_player_comment = lengthOfPlayerComment();
  int first = 0, second = 0;
  std::vector<std::string> my_vec = commentArraySecondPlayer(first, second);
  std::string converted_vector = std::accumulate(my_vec.begin(), my_vec.end(), std::string{},
                                                 [](std::string &s, const std::string &piece) -> decltype(auto)
                                                 { return s += piece; });
  player_stats1.append(" " + std::to_string(int(my_vec.size())) + " ");

  if (converted_vector.back() != ' ')
  {
    size_t pos = converted_vector.find_last_of(' ');

    // If a space was found, erase everything after it
    if (pos != std::string::npos)
    {
      converted_vector.erase(pos);
      converted_vector.append(" ");
    }
  }

  player_stats1.append(converted_vector);
  if (current_gamestate == "GameEnded")
  {
    player_stats1.replace(player_stats1.find("points"), 6, std::to_string(sum_player_1_ + int(my_vec.size())));
  }
  return player_stats1;
}