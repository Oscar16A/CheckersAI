#include "StudentAI.h"
#include <random>

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
	
	//

    vector<vector<Move> > moves = board.getAllPossibleMoves(player);
	for (int checkerNum = 0; chekcerNum < moves.size(); checkerNum++) 
	{
		for (int moveNum = 0; moveNum < moves[checkerNum].size(); moveNum++)
		{
			//make child of root
		}
	}
    
	//random AI 
	int i = rand() % (moves.size());
    vector<Move> checker_moves = moves[i];
    int j = rand() % (checker_moves.size());
    Move res = checker_moves[j];
    
	
	
	board.makeMove(res,player);
    return res;
}

