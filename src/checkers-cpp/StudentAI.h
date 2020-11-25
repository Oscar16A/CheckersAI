#ifndef STUDENTAI_H
#define STUDENTAI_H
#include "AI.h"
#include "Board.h"
#pragma once

//The following part should be completed by students.
//Students can modify anything except the class name and exisiting functions and varibles.
struct Node
{
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

class StudentAI :public AI
{
public:
    Board board;
	StudentAI(int col, int row, int p);
	virtual Move GetMove(Move board);
	
private:
	
	Node Selection(Node node);
	int Simulate(Board boardCopy, Move move, int turnPlayer);
	
};

#endif //STUDENTAI_H
