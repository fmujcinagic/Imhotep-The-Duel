#include "PrimaryGameAI.hpp"

bool PrimaryGameAI::checkIfValid(char *word)
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

int PrimaryGameAI::playerTurn(char turn)
{
  return turn == '1' ? 1 : 0;
}

std::vector<std::string> PrimaryGameAI::convertStringToVector(std::string string_for_conversion)
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

std::string PrimaryGameAI::parseFunc(int num, std::string string_for_parse)
{
  std::vector<std::string> tile_state_vec = convertStringToVector("GameState " + string_for_parse);

  int size = tile_state_vec.size();
  return tile_state_vec[size - num];
}

auto PrimaryGameAI::setObjectsForGame(TileDeck::TileState &tile_state, std::string gameStateMessage)
{

  parseInput parse_obj(convertStringToVector("GameState " + gameStateMessage));
  handleGame handle_game_obj(convertStringToVector("GameState " + gameStateMessage));
  legalMoves legal_obj(parse_obj);
  player player_obj(parse_obj);
  playerStats player_stats_obj(legal_obj, parse_obj, player_obj);

  handle_game_obj.setPlayingSide('A');

  AImove determine_move(handle_game_obj, tile_state, parse_obj, legal_obj, player_stats_obj);

  return determine_move;
}

int PrimaryGameAI::originalGame()
{
  std::string ip = argv_[2];
  std::stringstream value_argv;
  value_argv << argv_[1];

  unsigned short port = 0;
  value_argv >> port;

  if (1 > port or port > 65535 or checkIfValid((char *)argv_[1]) == false)
  {
    return 2;
  }

  net::Client client(port, ip, "Gruppe15");

  std::string joinedGameMessage = client.waitForInput();
  std::cout << "Received from server: " << joinedGameMessage << std::endl;

  std::stringstream sstr(joinedGameMessage);

  std::string message_type;
  sstr >> message_type;
  if (message_type != "JoinedGame")
  {
    return -1; // internal error, should never happen
  }

  int ai_id{};
  sstr >> ai_id;

  std::cout << "We are player with id " << ai_id << std::endl;

  std::string gameStateMessage = client.waitForInput(); // "GameState " +
  gameStateMessage = gameStateMessage.substr(gameStateMessage.find(' ') + 1);
  std::cout << gameStateMessage << std::endl;
  TileDeck::TileState tile_state;
  tile_state.remaining_cards[0] = stoi(parseFunc(4, gameStateMessage));
  tile_state.remaining_cards[1] = stoi(parseFunc(3, gameStateMessage));
  tile_state.seed = stoi(parseFunc(2, gameStateMessage));

  while (true)
  {
    int AI_turn = playerTurn(gameStateMessage[5]);
    if (ai_id == AI_turn)
    {
      auto ai_obj = setObjectsForGame(tile_state, gameStateMessage);
      ai_obj.setPlayerTurn(playerTurn(gameStateMessage[5]), ai_id);
      std::string move = ai_obj.getMove() + "\n";
      std::cout << move;
      client.send(move);

      if (gameStateMessage[0] == 'G')
      {
        return 0;
      }
      if (gameStateMessage[0] == 'Q')
      {
        return 0;
      }
    }
    gameStateMessage = client.waitForInput(); // "GameState " +
    gameStateMessage = gameStateMessage.substr(gameStateMessage.find(' ') + 1);
    if (gameStateMessage[0] == 'Q')
    {
      return 0;
    }

    if (gameStateMessage[0] == 'G')
    {
      return 0;
    }
  }
}