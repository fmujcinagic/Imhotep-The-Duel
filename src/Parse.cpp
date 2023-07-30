#include "Parse.hpp"

std::vector<std::string> parseInput::parseForStart()
{
  std::vector<std::string> copy_of_gamestate;

  for (size_t index = 0; index < gamestate_text_.size(); index++)
  {
    copy_of_gamestate.push_back(gamestate_text_[index]);
  }
  copy_of_gamestate.erase(copy_of_gamestate.begin());

  return copy_of_gamestate;
}

int parseInput::catchException(std::string string_to_check)
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

std::string parseInput::getControlState()
{
  return gamestate_text_.at(1);
}

int parseInput::getCurrentPlayer()
{
  return catchException(gamestate_text_.at(2));
}

int parseInput::getRemainigTiles()
{
  return catchException(gamestate_text_.at(3));
}

std::vector<std::vector<std::string>> parseInput::getBoats()
{
  std::vector<std::string> list_of_boats;
  std::vector<std::vector<std::string>> matrix_of_boats;

  for (size_t index = 4; index < 22; index++)
  {
    list_of_boats.push_back(gamestate_text_[index]);
  }

  matrix_of_boats.resize(6);

  for (int count_resize = 0; count_resize < 6; count_resize++)
  {
    matrix_of_boats[count_resize].resize(3);
  }

  int row = 0;
  for (size_t index_of_boats = 0; index_of_boats < 18; index_of_boats++)
  {
    if (index_of_boats % 3 == 0 and index_of_boats > 0)
    {
      row += 1;
    }
    int col = index_of_boats % 3;
    matrix_of_boats[row][col] = list_of_boats[index_of_boats];
  }

  return matrix_of_boats;
}

std::vector<std::string> parseInput::getBoard()
{
  std::vector<std::string> list_of_meeples;
  for (size_t index = 22; index < 31; index++)
  {
    list_of_meeples.push_back(gamestate_text_[index]);
  }
  return list_of_meeples;
}

std::vector<std::string> parseInput::getPl1Info()
{
  std::vector<std::string> pl1_list;

  if (gamestate_text_.at(43) == "0")
  {
    for (size_t index = 31; index < 44; index++)
    {
      pl1_list.push_back(gamestate_text_[index]);
    }
  }
  else
  {
    for (int index = 31; index < 44 + catchException(gamestate_text_.at(43)); index++)
    {
      pl1_list.push_back(gamestate_text_[index]);
    }
  }

  return pl1_list;
}

std::vector<std::string> parseInput::getPl2Info()
{
  std::vector<std::string> pl2_list;

  if (gamestate_text_.at(43) == "0")
  {
    for (size_t index = 44; index < 57; index++)
    {
      pl2_list.push_back(gamestate_text_[index]);
    }
  }
  else
  {
    for (int index = 44 + catchException(gamestate_text_.at(43)); index < 57 + catchException(gamestate_text_.at(43)) +
                                                                              catchException(gamestate_text_.at(56 + catchException(gamestate_text_.at(43))));
         index++)
    {
      pl2_list.push_back(gamestate_text_[index]);
    }
  }

  return pl2_list;
}

std::vector<std::string> parseInput::getRandomState()
{
  std::vector<std::string> random_state_list;

  for (int index = 57 + catchException(gamestate_text_.at(43)) + catchException(gamestate_text_.at(56 + catchException(gamestate_text_.at(43))));
       index < 60 + catchException(gamestate_text_.at(43)) + catchException(gamestate_text_.at(56 + catchException(gamestate_text_.at(43)))); index++)
  {
    random_state_list.push_back(gamestate_text_[index]);
  }

  return random_state_list;
}

int parseInput::getCommentArrayLength()
{
  if (catchException(gamestate_text_[gamestate_text_.size() - 1]) == 0)
  {
    return 0;
  }
  else
  {
    return catchException(gamestate_text_[gamestate_text_.size() - 2]) == 0;
  }
}

std::vector<std::string> parseInput::getCommentArray()
{
  std::vector<std::string> comment;

  if (getCommentArrayLength() != 0)
  {
    for (size_t index = gamestate_text_.size() - getCommentArrayLength(); index < gamestate_text_.size(); index++)
    {
      comment.push_back(gamestate_text_[index]);
    }
  }

  return comment;
}

std::string player::plName(int pl_num)
{
  return (player_map_.at(pl_num)).front();
}

int player::plPoints(int pl_num)
{
  return parseInput::catchException(player_map_.at(pl_num).at(1));
}

int player::plFirstObelisk(int pl_num)
{
  return parseInput::catchException(player_map_.at(pl_num).at(2));
}

int player::plMeepleCount(int pl_num)
{
  return parseInput::catchException(player_map_.at(pl_num).at(3));
}

int player::plAmountObelisks(int pl_num)
{
  return parseInput::catchException(player_map_.at(pl_num).at(4));
}

std::vector<std::string> player::plTemples(int pl_num)
{
  std::vector<std::string> temples;

  for (size_t index = 5; index < 9; index++)
  {
    temples.push_back(player_map_.at(pl_num).at(index));
  }

  return temples;
}

int player::plAmountPyramidLight(int pl_num)
{
  return parseInput::catchException(player_map_.at(pl_num).at(9));
}

int player::plAmountPyramidDark(int pl_num)
{
  return parseInput::catchException(player_map_.at(pl_num).at(10));
}

int player::plTombs(int pl_num)
{
  return parseInput::catchException(player_map_.at(pl_num).at(11));
}

int player::plLengthOfActionArray(int pl_num)
{
  return parseInput::catchException(player_map_.at(pl_num).at(12));
}

std::vector<std::string> player::plActionArray(int pl_num)
{
  std::vector<std::string> comment;

  if (plLengthOfActionArray(pl_num) != 0)
  {
    if (pl_num == 0)
    {
      for (size_t index = player_info1_.size() - plLengthOfActionArray(pl_num); index < player_info1_.size(); index++)
      {
        comment.push_back(player_map_.at(pl_num).at(index));
      }
    }
    else
    {
      for (size_t index = player_info2_.size() - plLengthOfActionArray(pl_num); index < player_info2_.size(); index++)
      {
        comment.push_back(player_map_.at(pl_num).at(index));
      }
    }
  }

  return comment;
}