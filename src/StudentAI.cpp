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

Move StudentAI::GetMove(Move move)
{
	//update our board, "player" is us, "player == 1?2:1" is them
    if (move.seq.empty())
    {
        player = 1;
    } else{
        board.makeMove(move,player == 1?2:1);
    }
	
	//AI 1 Min-Max with simple utility
	/*
	vector<vector<Move> > moves = board.getAllPossibleMoves(player);
	Move bestMove;
	int bestScore = -999;
	Board boardCopy = board;

	//select move with best score (score = our pieces - their pieces)
	for (int checkerNum = 0; checkerNum < moves.size(); checkerNum++) 
	{
		for (int moveNum = 0; moveNum < moves[checkerNum].size(); moveNum++)
		{
			boardCopy = board;
			boardCopy.makeMove(moves[checkerNum][moveNum], player);
			if (player == 1)
			{
				int moveScore = boardCopy.blackCount - boardCopy.whiteCount;
				if (moveScore > bestScore)
				{
					bestScore = moveScore;
					bestMove = moves[checkerNum][moveNum];
				}
			}
			else
			{
				int moveScore = boardCopy.whiteCount - boardCopy.blackCount;
				if (moveScore > bestScore)
				{
					bestScore = moveScore;
					bestMove = moves[checkerNum][moveNum];
				}
			}
		}
	}
	*/

	//AI 2 Sample Win-Rate

	vector<tuple<int, Move>> winCounts;
	vector<vector<Move>> moves = board.getAllPossibleMoves(player);
	Board boardCopy = board;
	Board boardCopyCopy = boardCopy;
	int winCount = 0;
	int turnPlayer = player;

	//run 100 sim per possible move
	for (int checkerNum = 0; checkerNum < moves.size(); checkerNum++)
	{
		for (int moveNum = 0; moveNum < moves[checkerNum].size(); moveNum++) //for each move
		{
			boardCopy = board;
			Move move = moves[checkerNum][moveNum];
			boardCopy.makeMove(move, player); //make our move
			turnPlayer = player == 1 ? 2 : 1; //enemy turn now


			winCount = 0;
			boardCopyCopy = boardCopy;
			int result = boardCopyCopy.isWin(turnPlayer); //2 = white, 1 = black, 0 = tie, -1 = undetermined
			for (int i = 0; i < 100; i++) //test 100 games
			{
				
				boardCopyCopy = boardCopy;
				result = boardCopyCopy.isWin(turnPlayer);
				vector<vector<Move>> possibleMoves;
				while (result == -1) //repeat until result is determined
				{
					possibleMoves = boardCopyCopy.getAllPossibleMoves(turnPlayer);
					int x = rand() % (possibleMoves.size());
					vector<Move> checker_moves = possibleMoves[x];
					int y = rand() % (checker_moves.size());
					Move randMove = checker_moves[y];

					boardCopyCopy.makeMove(randMove, turnPlayer);

					turnPlayer = turnPlayer == 1 ? 2 : 1;
					result = boardCopyCopy.isWin(turnPlayer);
				}
				if (result == player)
				{
					winCount++;
				}
			}
			winCounts.push_back(tuple<winCount, move>);
		}
	}
	//select move with most wins
	Move bestMove;
	int mostWins = -1;
	for (int moveNum = 0; checkerNum < winCounts.size(); moveNum++)
	{
		int numOfWins = winCounts[moveNum][0];
		if (numOfWins > mostWins)
		{
			mostWins = numOfWins;
			bestMove = winCounts[moveNum][1];
		}
	}


	//random AI 
	/*
	int i = rand() % (moves.size());
    vector<Move> checker_moves = moves[i];
    int j = rand() % (checker_moves.size());
    Move res = checker_moves[j];
	*/
    
	Move res = bestMove;
	board.makeMove(res,player);
    return res;
}

