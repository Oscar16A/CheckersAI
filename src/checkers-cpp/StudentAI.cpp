#include "StudentAI.h"
#include <random>
#include <iostream>

//The following part should be completed by students.
//The students can modify anything except the class name and exisiting functions and varibles.
StudentAI::StudentAI(int col,int row,int p)
	:AI(col, row, p)
{
    board = Board(col,row,p);
    board.initializeGame();
    player = 2;
}

int StudentAI::Simulate(Board boardCopy, int turnPlayer)
{
	int result = 0; //2 = white, 1 = black, -1 = tie, 0 = undetermined
	vector<vector<Move>> possibleMoves;
	while (result == 0) //repeat until result is determined
	{

		possibleMoves = boardCopy.getAllPossibleMoves(turnPlayer);
		int x = rand() % (possibleMoves.size());
		vector<Move> checker_moves = possibleMoves[x];
		int y = rand() % (checker_moves.size());
		Move randMove = checker_moves[y];

		boardCopy.makeMove(randMove, turnPlayer);


		result = boardCopy.isWin(turnPlayer);
		turnPlayer = turnPlayer == 1 ? 2 : 1;//switch turn after check for win
	}
	return result;
}

Move StudentAI::GetMove(Move move)
{
	//update our board, "player" is us, "player == 1?2:1" is them
    if (move.seq.empty())
    {
        player = 1;
    } else{
        board.makeMove(move,player == 1?2:1);
    }
	
	
	struct Node {
		Move move;
		Board board;
		int turnPlayer;
		int winCountOfOtherPlayer; 
		int playOuts;

		Node(Move move_, Board board_, int turnPlayer_, int winCountOfOtherPlayer_, int playOuts_)
			:
			move(move_),
			board(board_),
			turnPlayer(turnPlayer_),
			winCountOfOtherPlayer(winCountOfOtherPlayer_),
			playOuts(playOuts_)
		{
		}
	};

	vector<Node> NodeVector;

	vector<vector<Move>> moves = board.getAllPossibleMoves(player);
	for (int checkerNum = 0; checkerNum < moves.size(); checkerNum++)
	{
		for (int moveNum = 0; moveNum < moves[checkerNum].size(); moveNum++) //for each move
		{
			Board boardCopy = board;
			Move move = moves[checkerNum][moveNum];
			boardCopy.makeMove(move, player); //make our move
			int turnPlayer = player == 1 ? 2 : 1; // opponents turn

			Node node = Node(move, boardCopy, turnPlayer, 0, 0);

			NodeVector.push_back(node);
		}
	}
	
	//vector<vector<Move>> moves = board.getAllPossibleMoves(player);
	Move bestMove;
	int  mostWins = -1;
	//for (int checkerNum = 0; checkerNum < moves.size(); checkerNum++)
	{
		//for (int moveNum = 0; moveNum < moves[checkerNum].size(); moveNum++) //for each move
		for (int node = 0; node < NodeVector.size(); node++)
		{
			int winCount = 0;
			for (int i = 0; i < 50; i++) //run 50 simulations
			{
				int result = board.isWin(player);

				Board board = NodeVector[node].board;
				int turnPlayer = NodeVector[node].turnPlayer;
				//Board boardF = board;
				//int turnPlayerF = player == 1 ? 2 : 1;

				if (result == 0)
				{
					result = Simulate(board, turnPlayer);
				}

				if (result == player || result == -1)
				{
					winCount++;
				}
			}
			//check if move is best so far
			if (winCount > mostWins)
			{
				mostWins = winCount;
				bestMove = NodeVector[node].move;
				//bestMove = moves[checkerNum][moveNum];
			}
		}
	}
	/*
	for (int node = 0; node < NodeVector.size(); node++)
	{
		delete NodeVector[node];
	}
	*/
	board.makeMove(bestMove,player);
    return bestMove;
}

