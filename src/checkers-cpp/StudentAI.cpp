// Three Fourths MCTS (missing expansion)
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
    board.initializeGame();
    player = 2;
}

Node* StudentAI::Selection(Node* root)
{
	while (root->children.empty() == false)
	{
		root = SelectionStep(root);
	}
	return root; //leaf
}

Node* StudentAI::SelectionStep(Node* node) //choose best child from node's children
{
	Node* bestChild;
	float bestUCT;
	for (int i = 0; i < node->children.size(); i++)
	{
		float currentUCT = GetUCT(node->children[i], 1.4f);
		if (currentUCT > bestUCT)
		{
			bestUCT = currentUCT;
			bestChild = &node->children[i];
		}
	}
	return bestChild; 
}

float StudentAI::GetUCT(Node& node, float C)//assumes parent is always root node, C= 1.4f for selection
{
	float U = node.winsOther;
	float N = node.playOuts;
	float Np = node.parent->playOuts;
	if (N != 0)
	{
		return (U / N) + C * sqrt((log(Np) / N));
	}
	return INFINITY;
}

Node* StudentAI::Expansion(Node* leaf)
{
	int _turnPlayer = leaf->turnPlayer == 1 ? 2 : 1;
	int _result = leaf->board.isWin(_turnPlayer);
	if (_result == 0) //check for terminal node, 0 is not terminal
	{
		//expand
		vector<vector<Move>> moves = leaf->board.getAllPossibleMoves(leaf->turnPlayer);
		for (int checkerNum = 0; checkerNum < moves.size(); checkerNum++)
		{
			for (int moveNum = 0; moveNum < moves[checkerNum].size(); moveNum++) //for each move
			{
				Board childBoard = leaf->board;
				Move childMove = moves[checkerNum][moveNum];
				childBoard.makeMove(childMove, leaf->turnPlayer);
				int childTurnPlayer = _turnPlayer;

				vector<Node> childVector;
				Node* childParent = leaf;

				leaf->children.push_back(Node(childBoard, childMove, childTurnPlayer, 0, 0, childVector, childParent));
			}
		}
		//selection step
		leaf = SelectionStep(leaf);
	}
	return leaf;
}

int StudentAI::Simulate(Node* child)
{
	Board boardCopy = child->board;
	int _turnPlayer = child->turnPlayer == 1 ? 2 : 1;
	int result = boardCopy.isWin(_turnPlayer); //2 = white, 1 = black, -1 = tie, 0 = undetermined
	_turnPlayer = _turnPlayer == 1 ? 2 : 1;
	vector<vector<Move>> possibleMoves;
	while (result == 0) //repeat until result is determined
	{

		possibleMoves = boardCopy.getAllPossibleMoves(_turnPlayer);
		int x = rand() % (possibleMoves.size());
		vector<Move> checker_moves = possibleMoves[x];
		int y = rand() % (checker_moves.size());
		Move randMove = checker_moves[y];

		boardCopy.makeMove(randMove, _turnPlayer);


		result = boardCopy.isWin(_turnPlayer);
		_turnPlayer = _turnPlayer == 1 ? 2 : 1;//switch turn after check for win
	}
	return result;
}

void StudentAI::BackPropagate(int result, Node* child) //assume parent is root node
{
	//1 == a player won, 2 == a player won, -1 = tie, 0 = none

	while (child != nullptr)
	{
		//update result
		if (child->turnPlayer == player) //node belongs to us
		{
			if (result == player)
			{
				child->playOuts += 1;
			}
			else if (result == -1)
			{
				child->playOuts += 1;
			}
			else if (result == player == 1 ? 2 : 1)
			{
				child->winsOther += 1;
				child->playOuts += 1;
			}
		}
		else //node belongs to other player
		{
			if (result == player)
			{
				child->winsOther += 1;
				child->playOuts += 1;
			}
			else if (result == -1)
			{
				child->winsOther += 0.5f;
				child->playOuts += 1;
			}
			else if (result == player == 1 ? 2 : 1)
			{
				child->playOuts += 1;
			}
		}
		
		//go up tree
		child = child->parent;
	}
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
	
	//set-up root
	Board _board = board;
	Move _move = move;
	int _player = player;

	vector<Node> rootVector;

	Node rootNode = Node(_board, _move, _player, 0, 0, rootVector, nullptr);

	Node* root = &rootNode;

	for (int i = 0; i < 100; i++) //run 500 simulations for the turn
	{
		Node* leaf = Selection(root);
		Node* child = Expansion(leaf);
		int result = Simulate(child);
		BackPropagate(result, child);
	}

	Node* bestNode;
	int highestPlayOuts = 0;
	for (int i = 0; i < root->children.size(); i++) //run 500 simulations for the turn
	{
		if (highestPlayOuts < root->children[i].playOuts)
		{
			bestNode = &root->children[i];
			highestPlayOuts = root->children[i].playOuts;
		}
		
	}

	board.makeMove(bestNode->move, player);
	return bestNode->move;
}

