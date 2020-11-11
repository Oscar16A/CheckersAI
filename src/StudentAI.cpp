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
	//update our board
    if (move.seq.empty())
    {
        player = 1;
    } else{
        board.makeMove(move,player == 1?2:1);
    }
	
	vector<vector<Move> > moves = board.getAllPossibleMoves(player);
	//random AI for fisrt move selection
	//int i = rand() % (moves.size());
	//vector<Move> checker_moves = moves[i];
	//int j = rand() % (checker_moves.size());
	
	//choosing a random move to start
	//Move bestMove = checker_moves[j];
	Move bestMove;
	int bestScore = -999;
	
	Board boardCopy = board;
	/*
	boardCopy.makeMove(checker_moves[j], player);
	if (player == 1)
	{
		bestScore = boardCopy.blackCount - boardCopy.whiteCount;
	}
	else
	{
		bestScore = boardCopy.whiteCount - boardCopy.blackCount;
	}*/

	//check all moves for best move
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
	
	//random AI 
	//int i = rand() % (moves.size());
    //vector<Move> checker_moves = moves[i];
    //int j = rand() % (checker_moves.size());
    //Move res = checker_moves[j];
    
	Move res = bestMove;
	board.makeMove(res,player);
    return res;
}

