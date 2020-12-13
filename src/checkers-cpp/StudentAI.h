#ifndef STUDENTAI_H
#define STUDENTAI_H
#include "AI.h"
#include "Board.h"
#pragma once

struct Node
{
	Board board; //current board
	Move move; //last move made on current board
	int turnPlayer; //turn of player for current board

	float winsOther; //win score of other player
	int playOuts; //number of simulations that passed through this node

	vector<Node*> children; //children of node
	Node* parent; // parent node of this node
	
	//constructors
	Node();

	Node(Board board_, Move move_, int turnPlayer_, int winsOther_, int playOuts_, vector<Node*> children_, Node* parent_);
	
	//destructor
	~Node();

};

class StudentAI :public AI
{
public:
    Board board;
	StudentAI(int col, int row, int p);
	virtual Move GetMove(Move board);
	
private:
	Node& Selection(Node& node);
	Node& SelectionStep(Node& node);
	float GetUCT(const Node& node, float C);
	Node& Expansion(Node& node);
	int Simulate(Board _board, int _turnPlayer);
	void BackPropagate(int result, Node* other);
};

#endif //STUDENTAI_H
