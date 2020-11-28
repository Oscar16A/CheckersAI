#include "StudentAI.h"
#include <random>
#include <iostream>
#include <math.h>

//The following part should be completed by students.
//The students can modify anything except the class name and exisiting functions and varibles.
StudentAI::StudentAI(int col,int row,int p)
	:AI(col, row, p)
{
    board = Board(col,row,p);
	winCountOfOtherPlayerB = 0;
	playOutsB = 0;
    board.initializeGame();
    player = 2;
}

/*
Node::Node(Move move_, Board board_, int turnPlayer_, int winCountOfOtherPlayer_, int playOuts_)
{
	move = move_;
	board = board_;
	turnPlayer = turnPlayer_;
	winCountOfOtherPlayer = winCountOfOtherPlayer_;
	playOuts = playOuts_;
}
*/
float StudentAI::GetUCT(Node node)//simple, parent is always root node
{
	float C = 1.4f;
	float U = node.winCountOfOtherPlayer;
	float N = node.playOuts;
	float Np = winCountOfOtherPlayerB;
	if (N != 0)
	{
		return (U / N) + C * sqrt((log(Np) / N));
	}
	return INFINITY;
}

Node StudentAI::Selection(vector<Node> nodes)
{
	Node bestNode;
	float bestUCT;
	for (int node = 0; node < nodes.size(); node++)
	{
		float currentUCT= GetUCT(nodes[node]);
		if (currentUCT > bestUCT)
		{
			bestUCT = currentUCT;
			bestNode = nodes[node];
		}
	}
	return bestNode;
}

int StudentAI::Simulate(Board boardCopy, Move move, int turnPlayer)
{
	boardCopy.makeMove(move, player);
	int result = boardCopy.isWin(player); //2 = white, 1 = black, -1 = tie, 0 = undetermined
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

void StudentAI::BackPropagate(int result, Node node)
{
	//1 == a player won, 2 == a player won, -1 = tie, 0 = none



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

	vector<Node> nodeVector;

	vector<vector<Move>> moves = board.getAllPossibleMoves(player);
	for (int checkerNum = 0; checkerNum < moves.size(); checkerNum++)
	{
		for (int moveNum = 0; moveNum < moves[checkerNum].size(); moveNum++) //for each move
		{
			Board boardCopy = board;
			Move move = moves[checkerNum][moveNum];
			boardCopy.makeMove(move, player); //make our move
			int turnPlayer = player == 1 ? 2 : 1; // opponents turn

			nodeVector.push_back(Node(move, boardCopy, turnPlayer, 0, 0));
		}
	}
	
	Move bestMove;
	int  mostWins = -1;
	{
		for (int node = 0; node < nodeVector.size(); node++)
		{
			int winCount = 0;
			for (int i = 0; i < 50; i++) //run 50 simulations
			{
				
				int turnPlayerV = nodeVector[node].turnPlayer;
				Move moveV = nodeVector[node].move;
				int result = Simulate(board, moveV, turnPlayerV);

				if (result == player || result == -1)
				{
					winCount++;
				}
			}
			//check if move is best so far
			if (winCount > mostWins)
			{
				mostWins = winCount;
				bestMove = nodeVector[node].move;
			}
		}
	}
	
	board.makeMove(bestMove,player);
    return bestMove;
}

