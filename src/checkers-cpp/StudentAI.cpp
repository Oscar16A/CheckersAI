// Pure MC
#include "StudentAI.h"
#include <random>
#include <iostream>
#include <math.h>

Node::Node()
{}

Node::Node(Board board_, Move move_, int turnPlayer_, int winsOther_, int playOuts_, vector<Node*> children_, Node* parent_)
	:
	board(board_),
	move(move_),
	turnPlayer(turnPlayer_),

	winsOther(winsOther_),
	playOuts(playOuts_),

	children(children_),
	parent(parent_)
{}

Node::~Node()
{
	//clean up memory
	for (int i = 0; i < children.size(); i++)
	{
		delete children.at(i);
	}
}

StudentAI::StudentAI(int col,int row,int p)
	:AI(col, row, p)
{
    board = Board(col,row,p);
    board.initializeGame();
    player = 2;
}

Node* StudentAI::Selection(Node* node)
{
	if ((node->children.empty()))
	{
		return node;
	}
	else
	{
		return Selection(SelectionStep(node));
	}
}

Node* StudentAI::SelectionStep(Node* node)
{
	if (node->children.empty())
	{
		return node;
	}
	int bestChildIndex;
	float bestUCT;
	for (int i = 0; i < node->children.size(); i++)
	{
		float currentUCT = GetUCT(node->children.at(i), 1.4f);
		if (currentUCT > bestUCT)
		{
			bestUCT = currentUCT;
			bestChildIndex = i;
		}
	}
	return node->children.at(bestChildIndex);
}

float StudentAI::GetUCT(const Node* node, float C)//C= 1.4f for selection
{
	float U = node->winsOther;
	float N = node->playOuts;
	float Np = node->parent->playOuts;
	if (N != 0)
	{
		return (U / N) + (C * sqrt((log(Np) / N)));
	}
	return INFINITY;
}

Node* StudentAI::Expansion(Node* node)
{
	vector<vector<Move>> moves = node->board.getAllPossibleMoves(node->turnPlayer);
	for (int checkerNum = 0; checkerNum < moves.size(); checkerNum++) //for each checker
	{
		for (int moveNum = 0; moveNum < moves.at(checkerNum).size(); moveNum++) //for each move for each checker
		{
			Board childBoard = node->board;
			Move childMove = moves.at(checkerNum).at(moveNum);
			childBoard.makeMove(childMove, node->turnPlayer);
			int childTurnPlayer = node->turnPlayer == 1 ? 2 : 1;

			Node* childNode = new Node(childBoard, childMove, childTurnPlayer, 0, 0, vector<Node*>(), node);

			node->children.push_back(childNode);
		}
	}
	return SelectionStep(node);
}

int StudentAI::Simulate(Board _board, int _turnPlayer) //2 = white, 1 = black, -1 = tie, 0 = undetermined
{
	_turnPlayer = _turnPlayer == 1 ? 2 : 1; //flip player to previous player who just made a move
	int result = _board.isWin(_turnPlayer); //check isWin for just made move
	_turnPlayer = _turnPlayer == 1 ? 2 : 1; //flip player back to current player
	vector<vector<Move>> possibleMoves;
	while (result == 0) //repeat until result is determined
	{

		possibleMoves = _board.getAllPossibleMoves(_turnPlayer);
		int x = rand() % (possibleMoves.size());
		vector<Move> checker_moves = possibleMoves.at(x);
		int y = rand() % (checker_moves.size());
		Move randMove = checker_moves.at(y);

		_board.makeMove(randMove, _turnPlayer);


		result = _board.isWin(_turnPlayer);
		_turnPlayer = _turnPlayer == 1 ? 2 : 1;//pass turn
	}
	return result;
}

void StudentAI::BackPropagate(int result, Node* other) //assume child is a child of root
{
	while (!(other == nullptr))
	{
		if (other->turnPlayer == (player == 1 ? 2 : 1)) //if other is opponent turn
		{
			if (result == player)
			{
				other->winsOther += 1;
				other->playOuts += 1;

				//root.playOuts += 1;
			}
			else if (result == -1)
			{
				other->winsOther += 0.5f;
				other->playOuts += 1;

				//root.playOuts += 1;
			}
			else if (result == player == 1 ? 2 : 1)
			{
				other->playOuts += 1;

				//root.winsOther += 1;
				//root.playOuts += 1;
			}
		}
		else if (other->turnPlayer == player) //if other our turn
		{
			if (result == player)
			{
				other->playOuts += 1;
			}
			else if (result == -1)
			{
				other->playOuts += 1;

			}
			else if (result == player == 1 ? 2 : 1)
			{
				other->winsOther += 1;
				other->playOuts += 1;
			}
		}

		other = other->parent;
	}
}

Move StudentAI::GetMove(Move move)
{
	//update our board, "player" is us, "player == 1?2:1" is them
    if (move.seq.empty())
    {
        player = 1;
    }
	else
	{
        board.makeMove(move,player == 1?2:1);
    }

	//root setup
	Node* root = new Node(board, move, player, 0, 0, vector<Node*>(), nullptr);
	//root.board = board;
	//root.move = move;
	//root.turnPlayer = player;
	//root.winsOther = 0;
	//root.playOuts = 0;
	//root.children = vector<Node*>();
	//root.parent = nullptr;

	//Expansion(root);

	for (int i = 0; i < 500; i++) //run 500 simulations for the turn
	{
		//Node& leaf = SelectionStep(root.children);
		Node* leaf = Selection(root);
		Node* child = Expansion(leaf);
		int result = Simulate(child->board, child->turnPlayer);
		BackPropagate(result, child);
	}

	Move bestMove; //best move among root's children
	int highestPlayOuts = 0;
	for (int i = 0; i < root->children.size(); i++) 
	{
		if (highestPlayOuts < root->children.at(i)->playOuts)
		{
			bestMove = root->children.at(i)->move;
			highestPlayOuts = root->children.at(i)->playOuts;
		}
	}

	delete root;

	board.makeMove(bestMove, player);
	return bestMove;
}