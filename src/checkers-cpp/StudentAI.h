#ifndef STUDENTAI_H
#define STUDENTAI_H
#include "AI.h"
#include "Board.h"
#pragma once

//The following part should be completed by students.
//Students can modify anything except the class name and exisiting functions and varibles.
struct Node
{
	Board board; //current board
	Move move; //last move made on current board
	int turnPlayer; //turn of player for current board

	float winsOther; //win score of other player
	int playOuts; //number of simulations that passed through this node

	vector<Node> children; //children of this node
	Node* parent; //parent of this node
	
	Node() {}

	Node(Board board_, Move move_, int turnPlayer_, int winsOther_, int playOuts_, vector<Node> children_, Node* parent_)
	:
	board(board_),
	move(move_),
	turnPlayer(turnPlayer_),

	winsOther(winsOther_),
	playOuts(playOuts_),

	children(children_),
	parent(parent_)
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
	Node* Selection(Node* root);
	Node* SelectionStep(Node* node);
	float GetUCT(Node& node, float C);
	Node* Expansion(Node* leaf);
	int Simulate(Node* child);
	void BackPropagate(int result, Node* child);
};

#endif //STUDENTAI_H
