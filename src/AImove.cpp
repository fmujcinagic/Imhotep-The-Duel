#include "AImove.hpp"

void AImove::setNewObject(handleGame handle_obj)
{
  handle_game_obj_ = handle_obj;
}

void AImove::setPlayerTurn(int player_turn, int AI_id)
{
  player_turn_ = player_turn;
  AI_id_ = AI_id;
}

int AImove::random(int min, int max)
{
  static bool first = true;
  if (first)
  {
    srand(time(NULL));
    first = false;
  }
  return min + rand() % ((max + 1) - min);
}

std::vector<std::string> AImove::convertStringToVector(std::string string_for_conversion)
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

void AImove::evaluationfListForTombs(int current_number_of_tombs)
{
  int count_tombs = 0, max = 0, best_tomb_to_take = 0;
  std::vector<int> tombs;
  std::vector<int> other_tombs;
  if (current_number_of_tombs == 0)
  {
    for (int i = 3; i <= 10; i++)
    {
      for (const auto &row : parse_obj_.getBoats())
      {
        if (std::find(row.begin(), row.end(), ("Tomb~" + std::to_string(i) + " ")) != row.end())
        {
          tombs.push_back(i);
          break;
        }
      }
    }
    for (size_t index = 0; index < tombs.size(); index++)
    {
      for (int j = tombs.at(index) - 2; j <= tombs.at(index) + 2; j++)
      {
        for (const auto &row : parse_obj_.getBoats())
        {
          count_tombs += std::count(row.begin(), row.end(), ("Tomb~" + std::to_string(j) + " "));
        }
        if (count_tombs > max)
        {
          max = count_tombs;
          best_tomb_to_take = tombs.at(index);
        }
        count_tombs = 0;
      }
    }
    evaluation_list_.push_back("Tomb~" + std::to_string(best_tomb_to_take) + " ");
    other_tombs.push_back(best_tomb_to_take);
  }
  else
  {
    int count_tombs_around = 0, max_second = 0;
    std::bitset<12> bin(current_number_of_tombs);
    std::string string_bin = bin.to_string();
    best_tomb_to_take = 0;
    for (int index = 11; index >= 1; index--)
    {
      if (string_bin[index] == '1')
      {
        current_number_of_tombs = string_bin[index] - '0';
        for (int j = current_number_of_tombs - 2; j <= current_number_of_tombs + 2; j++)
        {
          for (const auto &row : parse_obj_.getBoats())
          {
            count_tombs_around += std::count(row.begin(), row.end(), ("Tomb~" + std::to_string(j) + " "));
          }
          if (count_tombs_around > max_second)
          {
            max = count_tombs;
            best_tomb_to_take = 13 - index;
          }
          count_tombs_around = 0;
        }
      }
    }
    for (int current_tomb_index = best_tomb_to_take - 2; current_tomb_index <= best_tomb_to_take + 2; current_tomb_index++)
    {
      evaluation_list_.push_back("Tomb~" + std::to_string(current_tomb_index) + " ");
      other_tombs.push_back(current_tomb_index);
    }
  }
}

void AImove::evaluationfListForPyramids(int current_amount_of_pyramid_light, int current_amount_of_pyramid_dark)
{
  if (current_amount_of_pyramid_light == current_amount_of_pyramid_dark)
  {
    int count_darks = 0, count_lights = 0;
    for (const auto &row : parse_obj_.getBoats())
    {
      count_darks += std::count(row.begin(), row.end(), "PyramidDark ");
      count_lights += std::count(row.begin(), row.end(), "PyramidLight ");
    }
    if (count_darks > count_lights)
    {
      evaluation_list_.push_back("PyramidDark ");
      evaluation_list_.push_back("PyramidLight ");
    }
    else
    {
      evaluation_list_.push_back("PyramidLight ");
      evaluation_list_.push_back("PyramidDark ");
    }
  }
  else if (current_amount_of_pyramid_light > current_amount_of_pyramid_dark)
  {
    evaluation_list_.push_back("PyramidLight ");
    evaluation_list_.push_back("PyramidDark ");
  }
  else if (current_amount_of_pyramid_light < current_amount_of_pyramid_dark)
  {
    evaluation_list_.push_back("PyramidDark ");
    evaluation_list_.push_back("PyramidLight ");
  }
}

void AImove::setEvaluationList()
{
  player_stats_obj_.initializeValues();

  int obelisks_for_pl0 = 0, obelisks_for_pl1 = 0;
  int current_amount_of_pyramid_light = 0, current_amount_of_pyramid_dark = 0, current_number_of_tombs = 0;
  if (AI_id_)
  {
    obelisks_for_pl0 = stoi(convertStringToVector(player_stats_obj_.PlayerStats0(0, 'A')).at(4));
    obelisks_for_pl1 = stoi(convertStringToVector(player_stats_obj_.PlayerStats1(0, 'A')).at(4));

    current_amount_of_pyramid_light = stoi(convertStringToVector(player_stats_obj_.PlayerStats1(0, 'A')).at(9));
    current_amount_of_pyramid_dark = stoi(convertStringToVector(player_stats_obj_.PlayerStats1(0, 'A')).at(10));
    current_number_of_tombs = stoi(convertStringToVector(player_stats_obj_.PlayerStats1(0, 'A')).at(11));
  }
  else
  {
    obelisks_for_pl1 = stoi(convertStringToVector(player_stats_obj_.PlayerStats0(0, 'A')).at(4));
    obelisks_for_pl0 = stoi(convertStringToVector(player_stats_obj_.PlayerStats1(0, 'A')).at(4));

    current_amount_of_pyramid_light = stoi(convertStringToVector(player_stats_obj_.PlayerStats0(0, 'A')).at(9));
    current_amount_of_pyramid_dark = stoi(convertStringToVector(player_stats_obj_.PlayerStats0(0, 'A')).at(10));
    current_number_of_tombs = stoi(convertStringToVector(player_stats_obj_.PlayerStats0(0, 'A')).at(11));
  }
  evaluationfListForTombs(current_number_of_tombs);
  evaluation_list_.push_back("Temple~4 ");
  evaluationfListForPyramids(current_amount_of_pyramid_light, current_amount_of_pyramid_dark);
  if (obelisks_for_pl0 >= obelisks_for_pl1)
  {
    evaluation_list_.push_back("Obelisk ");
  }
  evaluation_list_.push_back("Temple~3 ");
  evaluation_list_.push_back("Temple~2 ");
  if (obelisks_for_pl1 > obelisks_for_pl0)
  {
    evaluation_list_.push_back("Obelisk ");
  }
  for (int current_tomb_index = 1; current_tomb_index <= 12; current_tomb_index++)
  {
    auto it = std::find(evaluation_list_.begin(), evaluation_list_.end(), ("Tomb~" + std::to_string(current_tomb_index) + " "));

    if (it == evaluation_list_.end())
    {
      evaluation_list_.push_back("Tomb~" + std::to_string(current_tomb_index) + " ");
    }
  }

  evaluation_list_.push_back("Temple~1 ");
  evaluation_list_.push_back("SwapUnload ");
  evaluation_list_.push_back("PlaceMeeples ");
  evaluation_list_.push_back("TakeTile ");
  evaluation_list_.push_back("PlaceUnload ");
}

int AImove::getExactPosition(int second_nearest_place, int first_nearest_place, bool get_first)
{
  std::vector<std::string> board = parse_obj_.getBoard();

  if (get_first)
  {
    if (board.at(first_nearest_place) == "-1 ")
    {
      return 2;
    }

    return 1;
  }

  if (board.at(second_nearest_place) == "-1 ")
  {
    if (board.at(first_nearest_place) == "-1 ")
    {
      return 2;
    }
    else
    {
      return 1;
    }
  }
  else if (board.at(first_nearest_place) == "-1 ")
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

bool compare(int a, int b)
{
  if (a == 2)
    return true;
  if (b == 2)
    return false;
  if (a == 5)
    return true;
  if (b == 5)
    return false;
  if (a == 8)
    return true;
  if (b == 8)
    return false;
  if (a == 7)
    return true;
  if (b == 7)
    return false;
  if (a == 6)
    return true;
  if (b == 6)
    return false;

  return a < b;
}

void AImove::updateCorrespondingMap()
{
  std::vector<std::vector<std::string>> boats = parse_obj_.getBoats();

  std::vector<std::string> board = parse_obj_.getBoard();

  for (size_t i = 0; i < board.size(); i++)
  {
    if (board[i].find("-1 ") != std::string::npos)
    {
      possible_choices_place_meeple_.push_back(i);
    }
  }
  std::sort(possible_choices_place_meeple_.begin(), possible_choices_place_meeple_.end(), compare);

  corresponding_position_map_ = {
      {2, {boats[0][2], boats[3][getExactPosition(5, 8, false)]}},
      {5, {boats[1][2], boats[3][getExactPosition(0, 8, true)]}},
      {8, {boats[2][2], boats[3][2]}},
      {7, {boats[2][getExactPosition(0, 8, true)], boats[4][2]}},
      {6, {boats[2][getExactPosition(8, 7, false)], boats[5][2]}}};

  corresponding_position_map_second_ =
      {{{boats[0][getExactPosition(2, 1, false)], boats[5][getExactPosition(6, 3, false)]}, 0},
       {{boats[0][getExactPosition(0, 2, true)], boats[4][getExactPosition(7, 4, false)]}, 1},
       {{boats[1][getExactPosition(5, 4, false)], boats[5][getExactPosition(0, 6, true)]}, 3},
       {{boats[1][getExactPosition(0, 5, true)], boats[4][getExactPosition(0, 7, true)]}, 4}};

  final_map = {{{boats[0][getExactPosition(2, 1, false)], boats[5][getExactPosition(6, 3, false)]}, 0},
               {{boats[0][getExactPosition(0, 2, true)], boats[4][getExactPosition(7, 4, false)]}, 1},
               {{boats[1][getExactPosition(5, 4, false)], boats[5][getExactPosition(0, 6, true)]}, 3},
               {{boats[1][getExactPosition(0, 5, true)], boats[4][getExactPosition(0, 7, true)]}, 4},
               {{boats[0][2], boats[3][getExactPosition(5, 8, false)]}, 2},
               {{boats[1][2], boats[3][getExactPosition(0, 8, true)]}, 5},
               {{boats[2][2], boats[3][2]}, 8},
               {{boats[2][getExactPosition(0, 8, true)], boats[4][2]}, 7},
               {{boats[2][getExactPosition(8, 7, false)], boats[5][2]}, 6}};
}

int AImove::declareThirdOptionPlace(int boat, int index)
{
  std::multimap<int, std::vector<int>> map = {{0, {0, 1, 2}},
                                              {1, {3, 4, 5}},
                                              {2, {6, 7, 8}},
                                              {3, {2, 5, 8}},
                                              {4, {1, 4, 7}},
                                              {5, {0, 3, 6}}};
  auto it = map.find(boat);
  if (it != map.end())
  {
    return it->second[index];
  }
  return 10;
}

int AImove::checkForTwoMeeples(bool &check_for_two_meeples, int &best_option)
{
  for (int i = 0; i < 6; i++)
  {
    std::string str = player_stats_obj_.board_map_.at(i);
    std::string minuses = "-";
    std::string ai_symbol = std::to_string(AI_id_);
    int count_minuses = 0;
    int count_ai_ocurr = 0;
    size_t pos = 0;

    while ((pos = str.find(minuses, pos)) != std::string::npos)
    {
      ++count_minuses;
      pos += minuses.length();
    }

    pos = 0;

    while ((pos = str.find(ai_symbol, pos)) != std::string::npos)
    {
      ++count_ai_ocurr;
      pos += ai_symbol.length();
    }

    if (AI_id_)
    {
      if (count_minuses == 1 and (count_ai_ocurr - count_minuses) == 2)
      {
        for (size_t index = 0; index < convertStringToVector(str).size(); index++)
        {
          if (convertStringToVector(str)[index].find("-1 ") != std::string::npos)
          {
            check_for_two_meeples = true;
            best_option = declareThirdOptionPlace(i, index);
            return 0;
          }
        }
      }
    }
    else
    {
      if (count_minuses == 1 and count_ai_ocurr == 2)
      {

        for (size_t index = 0; index < convertStringToVector(str).size(); index++)
        {
          if (convertStringToVector(str)[index].find("-1 ") != std::string::npos)
          {
            check_for_two_meeples = true;
            best_option = declareThirdOptionPlace(i, index);
            return 0;
          }
        }
      }
    }
  }
  return 0;
}

int AImove::placeOnEdges()
{
  for (size_t i = 0; i < evaluation_list_.size(); i++)
  {
    for (auto x : possible_choices_place_meeple_)
    {
      if (std::find(possible_choices_place_meeple_.begin(), possible_choices_place_meeple_.end(), x) != possible_choices_place_meeple_.end())
      {
        if (x == 2 or x == 5 or x == 6 or x == 7 or x == 8)
        {
          auto it = corresponding_position_map_.find(x);
          std::vector<std::string> &vec = it->second;
          std::string specific_string_null = vec[0];
          std::string specific_string_one = vec[1];

          if (evaluation_list_.at(i) == specific_string_null or evaluation_list_.at(i) == specific_string_one)
          {
            if (parse_obj_.getBoard().at(x) == "-1 ")
            {
              token_to_take_ = evaluation_list_.at(i);
              placeable_on_edge_ = true;
              return x;
            }
          }
        }
      }
    }
  }

  return 0;
}

void AImove::initializeMaps()
{
  std::vector<std::vector<std::string>> boats = parse_obj_.getBoats();

  map_for_next_meeple_ = {{2, {boats[0][getExactPosition(0, 2, true)], boats[3][getExactPosition(0, 8, true)]}},
                          {5, {boats[3][getExactPosition(5, 8, false)], boats[1][getExactPosition(0, 5, true)], boats[3][2]}},
                          {8, {boats[2][getExactPosition(0, 8, true)], boats[3][getExactPosition(0, 8, true)]}},
                          {7, {boats[2][getExactPosition(8, 7, false)], boats[4][getExactPosition(0, 7, true)], boats[2][2]}},
                          {6, {boats[5][getExactPosition(0, 6, true)], boats[2][getExactPosition(0, 8, true)]}}};

  map_for_second_next_meeple_ = {
      {2, {1, 5}},
      {5, {2, 4, 8}},
      {8, {5, 7}},
      {7, {6, 4, 8}},
      {6, {3, 7}}};
}

int AImove::bestChoiceForPlaceMeeple(std::string gamestate)
{
  updateCorrespondingMap();
  setEvaluationList();
  std::vector<int> meeples_on_side = {2, 5, 8, 7, 6};
  initializeMaps();
  bool check_for_two_meeples = false;
  int best_option = 0;
  checkForTwoMeeples(check_for_two_meeples, best_option);
  if (check_for_two_meeples)
  {
    return best_option;
  }
  std::vector<int> next_meeple;
  for (size_t i = 0; i < parse_obj_.getBoard().size(); i++)
  {
    if (parse_obj_.getBoard().at(i) == (std::to_string(AI_id_) + " "))
    {
      if (std::find(meeples_on_side.begin(), meeples_on_side.end(), i) != meeples_on_side.end())
      {
        next_meeple.push_back(i);
      }
    }
  }
  size_t min_find = legal_moves_obj_.convertVectorToString(evaluation_list_).length();
  std::vector<int> make_choice_to_place_around_meeple;
  std::string best_move_to_play = "";
  int counter = 0, position_to_match_place_meeple = 0;
  int current_placed_meeple_for_next_meeple = 0, counter_for_placed_meeples = 0;
  if (!next_meeple.empty())
  {
    for (size_t i = 0; i < next_meeple.size(); i++)
    {
      for (auto x : map_for_next_meeple)
      {
        if (x.first == next_meeple.at(i))
        {
          for (auto y : x.second)
          {
            if ((legal_moves_obj_.convertVectorToString(evaluation_list_).find(y) < min_find) and (parse_obj_.getBoard().at(map_for_second_next_meeple.at(x.first).at(counter_for_placed_meeples)) == "-1 "))
            {
              make_choice_to_place_around_meeple.push_back(map_for_second_next_meeple.at(x.first).at(counter_for_placed_meeples));
              counter++;
              min_find = legal_moves_obj_.convertVectorToString(evaluation_list_).find(y);
              current_placed_meeple_for_next_meeple = x.first;
              token_to_take_ = y;
            }
            counter_for_placed_meeples++;
          }
          if (position_to_match_place_meeple <= counter)
          {
            position_to_match_place_meeple = counter;
          }
          counter = 0;
          counter_for_placed_meeples = 0;
        }
      }
    }
  }
  if (current_placed_meeple_for_next_meeple != 0)
  {
    return make_choice_to_place_around_meeple.at(position_to_match_place_meeple - 1);
  }
  int choice_on_edge = placeOnEdges();
  if (placeable_on_edge_ and choice_on_edge != 0)
  {
    return choice_on_edge;
  }
  while (true)
  {
    int temp = random(0, 8);
    if (parse_obj_.getBoard().at(temp) == "-1 ")
    {
      return temp;
    }
  }
}

std::string AImove::getFinalPlaceMeeple()
{
  std::string current_gamestate = "";
  while (true)
  {
    std::string message = "PlaceMeeple " + std::to_string(bestChoiceForPlaceMeeple(current_gamestate));

    handle_game_obj_.set(player_turn_, message);
    std::string updated_text = handle_game_obj_.game(player_turn_, 0, current_gamestate_message, tile_state_);

    if (handle_game_obj_.gamestate != "GameEnded")
    {
      return message;
    }
    else
    {
      current_gamestate = handle_game_obj_.gamestate;
    }
  }
}

std::vector<std::string> AImove::getUpadtedBoard(std::vector<std::string> board_)
{
  std::vector<std::string> board_vec;

  if (AI_id_)
  {
    best_combinations_for_pl1 = {"1 1 1 ", "-1 1 1 ", "0 1 1 ", "1 -1 1 ", "1 0 1 ",
                                 "1 1 -1 ", "1 1 0 ", "1 -1 0 ", "1 0 -1 ", "-1 0 1 ",
                                 "-1 1 0 ", "0 -1 1 ", "0 1 -1 "};
  }
  else
  {
    best_combinations_for_pl1 = {"0 0 0 ", "-1 0 0 ", "1 0 0 ", "0 -1 0 ", "0 1 0 ",
                                 "0 0 -1 ", "0 0 1 ", "0 -1 1 ", "0 1 -1 ", "-1 1 0 ",
                                 "-1 0 1 ", "1 -1 0 ", "1 0 -1 "};
  }
  board_vec.push_back(board_[0] + board_[1] + board_[2]);
  board_vec.push_back(board_[3] + board_[4] + board_[5]);
  board_vec.push_back(board_[6] + board_[7] + board_[8]);
  board_vec.push_back(board_[2] + board_[5] + board_[8]);
  board_vec.push_back(board_[1] + board_[4] + board_[7]);
  board_vec.push_back(board_[0] + board_[3] + board_[6]);

  for (int i = 0; i < 6; i++)
  {
    unsorted_board.insert(std::make_pair(board_vec.at(i), i));
  }

  return board_vec;
}

void AImove::fillTheMapsForBestBoat()
{
  std::vector<std::string> board = parse_obj_.getBoard();

  std::vector<std::string> vec_of_best_tokens, vec_of_best_tokens_second;
  std::vector<int> vec_of_best_boats, vec_of_best_boats_second;

  int counter = 0, counter_second = 0;
  for (auto current_best_boat_index : best_choices_)
  {
    for (auto map_index : player_stats_obj_.board_map_)
    {
      if (current_best_boat_index == map_index.first)
      {

        for (size_t frist_map_index = 0; frist_map_index < convertStringToVector(map_index.second).size(); frist_map_index++)
        {
          if (convertStringToVector(map_index.second)[frist_map_index] == (std::to_string(AI_id_) + " "))
          {

            for (size_t drugi = frist_map_index + 1; drugi < convertStringToVector(map_index.second).size(); drugi++)
            {
              if (convertStringToVector(map_index.second)[drugi] == (std::to_string(AI_id_) + " ") or convertStringToVector(map_index.second)[drugi] == (std::to_string(-AI_id_ + 1) + " "))
              {
                counter++;
              }
            }
            vec_of_best_boats.push_back(current_best_boat_index);
            vec_of_best_tokens.push_back(parse_obj_.getBoats()[current_best_boat_index][2 - counter]);
            counter = 0;
          }
          else if (convertStringToVector(map_index.second)[frist_map_index] == (std::to_string(-AI_id_ + 1) + " "))
          {
            for (size_t drugi = frist_map_index + 1; drugi < convertStringToVector(map_index.second).size(); drugi++)
            {
              if (convertStringToVector(map_index.second)[drugi] == (std::to_string(-AI_id_ + 1) + " ") or convertStringToVector(map_index.second)[drugi] == (std::to_string(AI_id_) + " "))
              {
                counter_second++;
              }
            }
            vec_of_best_boats_second.push_back(current_best_boat_index);
            vec_of_best_tokens_second.push_back(parse_obj_.getBoats()[current_best_boat_index][2 - counter_second]);
            counter_second = 0;
          }
        }
      }
      if (!vec_of_best_tokens.empty())
      {
        connect_tokens_and_boats.insert(std::make_pair(vec_of_best_boats.front(), vec_of_best_tokens));

        vec_of_best_boats.clear();
        vec_of_best_tokens.clear();
      }
      if (!vec_of_best_tokens_second.empty())
      {
        connect_tokens_and_boats_second.insert(std::make_pair(vec_of_best_boats_second.front(), vec_of_best_tokens_second));
        vec_of_best_boats_second.clear();
        vec_of_best_tokens_second.clear();
      }
    }
  }
}

int AImove::newBestChoiceForUnloadBoat()
{
  fillTheMapsForBestBoat();
  int boat_to_unload = 0;
  int min = evaluation_list_.size();
  int temporary_min = 0;
  for (const auto &kv : connect_tokens_and_boats)
  {
    for (const auto &boat : kv.second)
    {
      for (size_t index = 0; index < evaluation_list_.size(); index++)
      {
        if (boat == evaluation_list_.at(index))
        {
          temporary_min = index;
          if (min > temporary_min)
          {
            min = temporary_min;
            boat_to_unload = kv.first;
          }
        }
      }
    }
  }
  for (const auto &kv : connect_tokens_and_boats)
  {
    if (kv.first == boat_to_unload)
    {
      tokens_from_boats.insert(tokens_from_boats.begin(), kv.second.begin(), kv.second.end());
    }
  }

  int boat_to_unload_second = 0;
  int min_second = evaluation_list_.size();
  int temporary_min_second = 0;
  for (const auto &kv : connect_tokens_and_boats_second)
  {
    for (const auto &boat : kv.second)
    {
      for (size_t index = 0; index < evaluation_list_.size(); index++)
      {
        if (boat == evaluation_list_.at(index))
        {
          temporary_min_second = index;
          if (min_second > temporary_min_second)
          {
            min_second = temporary_min_second;
            boat_to_unload_second = kv.first;
          }
        }
      }
    }
  }
  for (const auto &kv : connect_tokens_and_boats_second)
  {
    if (kv.first == boat_to_unload_second)
    {
      tokens_from_boats_second.insert(tokens_from_boats_second.begin(), kv.second.begin(), kv.second.end());
    }
  }
  return boat_to_unload;
}

int AImove::bestChoiceForUnloadBoat()
{
  std::vector<int> best_choices;

  std::vector<std::string> vec = getUpadtedBoard(parse_obj_.getBoard());

  for (size_t i = 0; i < best_combinations_for_pl1.size(); i++)
  {
    auto it = std::find(vec.begin(), vec.end(), best_combinations_for_pl1.at(i));

    if (it != vec.end())
    {
      if (parse_obj_.getBoats()[unsorted_board.at(best_combinations_for_pl1.at(i))][0] != "None ")
      {
        best_choices.push_back(unsorted_board.at(best_combinations_for_pl1.at(i)));
      }
      else
      {

        for (int current_boat_index = unsorted_board.at(best_combinations_for_pl1.at(i)) + 1; current_boat_index < 6; current_boat_index++)
        {
          if (i + 1 != best_combinations_for_pl1.size())
          {
            auto it2 = std::find(vec.begin(), vec.end(), best_combinations_for_pl1.at(i + 1));
            if (parse_obj_.getBoats()[current_boat_index][0] != "None " and it2 != vec.end())
            {
              best_choices.push_back(current_boat_index);
            }
          }
        }
      }
    }
  }

  best_choices.empty() ? choice_for_unload_not_found = true : choice_for_unload_not_found = false;

  if (choice_for_unload_not_found)
  {
    return 0;
  }

  best_choices_.insert(best_choices_.end(), best_choices.begin(), best_choices.end());

  player_stats_obj_.initializeValues();

  return calculateBestChoiceForUnloadBoat();
}

int AImove::calculateBestChoiceForUnloadBoat()
{
  if (best_choices_.empty())
  {
    choice_for_unload_not_found = true;
    return 0;
  }
  else
  {
    return newBestChoiceForUnloadBoat();
  }
  return 0;
}

std::string AImove::getFinalBoat()
{
  while (true)
  {
    int best_unload_option = bestChoiceForUnloadBoat();

    if (!choice_for_unload_not_found)
    {
      std::string message = "UnloadBoat " + std::to_string(best_unload_option);

      handle_game_obj_.set(player_turn_, message);

      std::string updated_text = handle_game_obj_.game(player_turn_, 0, current_gamestate_message, tile_state_);
      if (handle_game_obj_.gamestate == "Okay" or handle_game_obj_.gamestate == "GameEnded")
      {
        return message;
      }
      else
      {
        return "";
      }
    }
    else
    {
      return "";
    }
  }
}

std::string AImove::getMove()
{
  std::string temp_second = getFinalPlaceMeeple();

  std::string temp = getFinalBoat();

  int min = evaluation_list_.size();
  int temporary_min = 0;

  for (size_t evaluation_list_index = 0; evaluation_list_index < evaluation_list_.size(); evaluation_list_index++)
  {
    for (size_t second_index = 0; second_index < tokens_from_boats.size(); second_index++)
    {
      if (tokens_from_boats.at(second_index) == evaluation_list_.at(evaluation_list_index))
      {
        temporary_min = evaluation_list_index;
        if (min > temporary_min)
        {
          min = temporary_min;
        }
      }
    }
  }
  int min_second = evaluation_list_.size();
  int temporary_min_second = 0;
  for (size_t evaluation_list_index = 0; evaluation_list_index < evaluation_list_.size(); evaluation_list_index++)
  {
    for (size_t second_index = 0; second_index < tokens_from_boats_second.size(); second_index++)
    {
      if (tokens_from_boats_second.at(second_index) == evaluation_list_.at(evaluation_list_index))
      {
        temporary_min_second = evaluation_list_index;
        if (min_second > temporary_min_second)
        {
          min_second = temporary_min_second;
        }
      }
    }
  }

  for (int current_boat_index = 0; current_boat_index < 6; current_boat_index++)
  {
    if (player_stats_obj_.board_map_.at(current_boat_index) == (std::to_string(AI_id_) + " " + std::to_string(AI_id_) + " " + std::to_string(AI_id_) + " "))
    {
      if (parse_obj_.getBoats().at(current_boat_index)[0] != "None ")
      {
        return temp;
      }
    }
  }

  if (AI_id_ and convertStringToVector(player_stats_obj_.PlayerStats1(false, 'A')).at(3) == "0 ")
  {
    return temp;
  }
  else if ((AI_id_ == 0) and convertStringToVector(player_stats_obj_.PlayerStats0(false, 'A')).at(3) == "0 ")
  {
    return temp;
  }

  else
  {
    return temp_second;
  }
  return temp;
}
