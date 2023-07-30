#include "PrimaryGame.hpp"
#include "Parse.hpp"
#include "TileDeck.hpp"
#include "network/Server.hpp"
#include "network/Exceptions.hpp"
#include "LegalMoves.hpp"
#include "PlayerStats.hpp"
#include "HandleGame.hpp"

bool primaryGame::checkIfValid(char *word)
{
  while (*word)
  {
    if (isdigit(*word++) == false)
    {
      return false;
    }
  }
  return true;
}

std::string primaryGame::parseFunc(int num)
{
  std::string intial_state_file = argv_[3];

  std::ifstream input_file(intial_state_file);
  std::string file_content;
  file_content = std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());

  std::string curr_buffer;
  std::stringstream tile_state_ss(file_content);

  std::vector<std::string> tile_state_vec;

  while (tile_state_ss >> curr_buffer)
  {
    tile_state_vec.push_back(curr_buffer);
  }

  int size = tile_state_vec.size();
  return tile_state_vec[size - num];
}

int primaryGame::checkArgv(char &current_side)
{
  if (argc_ != 4)
  {
    std::cerr << "Invalid number of arguments: [port_number] [a_or_b_site] [intial_state_file] expected\n";
    return 1;
  }
  std::stringstream value_argv;
  value_argv << argv_[1];

  unsigned short port_number = 0;
  value_argv >> port_number;

  std::string a_or_b_side = argv_[2];

  if (1 > port_number or port_number > 65535 or checkIfValid((char *)argv_[1]) == false)
  {
    return 2;
  }
  if (a_or_b_side[0] != 'A' and a_or_b_side[0] != 'B')
  {
    return 3;
  }
  else
  {
    current_side = a_or_b_side[0];
  }

  std::string *intial_state_file = new (std::nothrow) std::string("");

  if (!intial_state_file)
  {
    return -1;
  }
  else
  {
    *intial_state_file = argv_[3];
  }

  if (std::filesystem::exists(*intial_state_file) == false)
  {
    return 4;
  }
  else
  {
    delete intial_state_file;
  }

  return 0;
}

std::vector<std::string> primaryGame::parseTheWords(const char *textfile)
{
  std::string text_from_file;
  std::ifstream readfile(textfile);
  std::vector<std::string> parts_n;
  while (readfile >> text_from_file)
  {
    parts_n.push_back(text_from_file + " ");
  }
  return parts_n;
}

std::string primaryGame::convertVectorToString(std::vector<std::string> gamestate_vec)
{
  std::string converted_vector = std::accumulate(gamestate_vec.begin(), gamestate_vec.end(), std::string{},
                                                 [](std::string &s, const std::string &piece) -> decltype(auto)
                                                 { return s += piece; });
  converted_vector.pop_back();
  return converted_vector;
}

int primaryGame::originalGame()
{
  int term = checkArgv(current_side_);
  if (term != 0)
  {
    return term;
  }
  std::stringstream value_argv;
  value_argv << argv_[1];
  unsigned short port_number = 0;
  value_argv >> port_number;
  std::vector<std::string> words_vec = parseTheWords(argv_[3]);
  parseInput obj(words_vec);
  std::vector<std::string> start = obj.parseForStart();
  std::string converted_vector = convertVectorToString(start);
  int new_player = 0;
  try
  {
    net::Server server(port_number);
    std::cout << converted_vector << '\n';
    server.send(converted_vector + '\n');
    std::string string_of_players = "";
    auto message = server.waitForNewInput();
    string_of_players.append(std::to_string(message.player_id));
    std::cout << message.message << message.player_id << std::endl;
    new_player = message.player_id;
    std::vector<std::string> new_message;
    new_message.insert(std::end(new_message), std::begin(words_vec), std::end(words_vec));
    std::string updated_text = "";
    bool flag_for_syntax_error = false;
    std::string current_gamestate_message = "";
    TileDeck::TileState tile_state;
    tile_state.remaining_cards[0] = stoi(parseFunc(4));
    tile_state.remaining_cards[1] = stoi(parseFunc(3));
    tile_state.seed = stoi(parseFunc(2));
    while (true)
    {
      if ((new_message.at(2)[0] - '0') != new_player)
      {
        flag_for_syntax_error = true;
        new_player = -new_player + 1;
      }
      handleGame handle_input(new_message);
      handle_input.setPlayingSide(current_side_);
      handle_input.set(new_player, message.message);
      updated_text = handle_input.game(new_player, flag_for_syntax_error, current_gamestate_message, tile_state);
      flag_for_syntax_error = false;
      std::cout << updated_text << std::endl;
      server.send(updated_text + '\n');
      std::string tmp = "";
      std::stringstream ss(updated_text);
      new_message.clear();
      while (getline(ss, tmp, ' '))
      {
        new_message.push_back(tmp + " ");
      }
      new_message.insert(new_message.begin(), "GameState ");
      message = server.waitForNewInput();
      std::cout << message.message << std::endl;
      string_of_players.append(std::to_string(message.player_id));
      if (string_of_players.length() >= 2)
      {
        if (current_gamestate_message == "Okay")
        {
          if (string_of_players[string_of_players.length() - 1] == string_of_players[string_of_players.length() - 2])
          {
            flag_for_syntax_error = true;
          }
        }
      }
    }
  }
  catch (net::NetworkException &e)
  {
    return 3;
  }
  return 0;
}