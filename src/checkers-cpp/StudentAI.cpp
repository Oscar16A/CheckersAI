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
	

	//AI 2 Sample Win-Rate

	vector<vector<Move>> moves = board.getAllPossibleMoves(player);
	Board boardCopy = board;
	Board boardCopyCopy = boardCopy;
	int winCount = 0;
	int turnPlayer = player;
	Move bestMove;
	int mostWins = -1;

	//run 100 sim per possible move
	for (int checkerNum = 0; checkerNum < moves.size(); checkerNum++)
	{
		for (int moveNum = 0; moveNum < moves[checkerNum].size(); moveNum++) //for each move
		{
			boardCopy = board;
			Move move = moves[checkerNum][moveNum];
			boardCopy.makeMove(move, player); //make our move
			int result = boardCopyCopy.isWin(player); //2 = white, 1 = black, -1 = tie, 0 = undetermined
			


			winCount = 0;
			for (int i = 0; i < 50; i++) //test 50 games
			{
				boardCopyCopy = boardCopy; //reset board
				result = boardCopyCopy.isWin(player);
				turnPlayer = player == 1 ? 2 : 1; //opponents turn
				vector<vector<Move>> possibleMoves;
				while (result == 0) //repeat until result is determined
				{
					
					possibleMoves = boardCopyCopy.getAllPossibleMoves(turnPlayer);
					int x = rand() % (possibleMoves.size());
					vector<Move> checker_moves = possibleMoves[x];
					int y = rand() % (checker_moves.size());
					Move randMove = checker_moves[y];

					boardCopyCopy.makeMove(randMove, turnPlayer);

					
					result = boardCopyCopy.isWin(turnPlayer);
					turnPlayer = turnPlayer == 1 ? 2 : 1;//switch turn after check for win
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
				bestMove = move;
			}
		}
	}
	
	//random AI 
	/*
	int i = rand() % (moves.size());
    vector<Move> checker_moves = moves[i];
    int j = rand() % (checker_moves.size());
    bestMove = checker_moves[j];
	*/
    
	board.makeMove(bestMove,player);
    return bestMove;

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
}

