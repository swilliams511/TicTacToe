#include "State.hpp"



State::State()
: stateNumber(0), finalState(false), winner(-1)
{
    //-1 represents an empty square on the board
    //0 represents an O's place
    //1 represents an X's place
    for(int i = 0; i < 3; ++i)
        board[i].fill(-1);


}

State::State(const std::array<std::array<int,3>,3>& otherBoard)
: stateNumber(0), finalState(false), winner(-1)
{
    board = otherBoard;
    //-1 represents an empty square on the board
    //0 represents an O's place
    //1 represents an X's place
}

State::~State()
{

}

void State::setStateNumber(int number)
{
    stateNumber = number;
}

int State::getStateNumber() const
{
    return stateNumber;
}

//checks if the board is in a winning state. Winning states have been drawn next to their
//respective logic statements.
//If the board is a winning board (or final dfa state) then we mark it as such and return true
bool State::isFinal()
{
    if(finalState == true) //if we already know the state being checked is final, dont bother checking
        return true;
    //if any of these 8 combinations are true, the state is a final state. The winner is recorded
    if(board[0][0] != -1 && (board[0][0] == board[0][1]) && (board[0][1] == board[0][2]))  //[x][x][x]
       {                                                                                   //[ ][ ][ ]
           if(board[0][0] == 0) //determine who won                                        //[ ][ ][ ]
                winner = 0;
           else
                winner = 1;
           finalState = true;
           return true;
       }
       if(board[1][0] != -1 &&(board[1][0] == board[1][1]) && (board[1][1] == board[1][2])) //[ ][ ][ ]
       {                                                                                    //[x][x][x]
           if(board[1][0] == 0) //determine who won                                         //[ ][ ][ ]
                winner = 0;
           else
                winner = 1;
           finalState = true;
           return true;
       }
       if(board[2][0] != -1 &&(board[2][0] == board[2][1]) && (board[2][1] == board[2][2])) //[ ][ ][ ]
       {                                                                                    //[ ][ ][ ]
           if(board[2][0] == 0) //determine who won                                         //[x][x][x]
                winner = 0;
           else
                winner = 1;
           finalState = true;
           return true;
       }
       if(board[0][0] != -1 &&(board[0][0] == board[1][0]) && (board[1][0] == board[2][0])) //[x][ ][ ]
       {                                                                                    //[x][ ][ ]
           if(board[0][0] == 0) //determine who won                                         //[x][ ][ ]
                winner = 0;
           else
                winner = 1;

           finalState = true;
           return true;
       }
       if(board[0][1] != -1 &&(board[0][1] == board[1][1]) && (board[1][1] == board[2][1])) //[ ][x][ ]
       {                                                                                    //[ ][x][ ]
           if(board[0][1] == 0) //determine who won                                         //[ ][x][ ]
                winner = 0;
           else
                winner = 1;

           finalState = true;
           return true;
       }
       if(board[0][2] != -1 &&(board[0][2] == board[1][2]) && (board[1][2] == board[2][2])) //[ ][ ][x]
       {                                                                                    //[ ][ ][x]
           if(board[0][2] == 0) //determine who won                                         //[ ][ ][x]
                winner = 0;
           else
                winner = 1;
           finalState = true;
           return true;
       }
       if(board[0][0] != -1 &&(board[0][0] == board[1][1]) && (board[1][1] == board[2][2])) //[x][ ][ ]
       {                                                                                    //[ ][x][ ]
           if(board[0][0] == 0) //determine who won                                         //[ ][ ][x]
                winner = 0;
           else
                winner = 1;
           finalState = true;
           return true;
       }
       if(board[0][2] != -1 &&(board[0][2] == board[1][1]) && (board[1][1] == board[2][0]))  //[ ][ ][x]
        {                                                                                    //[ ][x][ ]
            if(board[0][2] == 0) //determine who won                                         //[x][ ][ ]
                winner = 0;
            else
                winner = 1;
           finalState = true;
           return true;
        }
    return false;   //if we make it here, the state is not a final state

}

bool State::operator == (const State& otherState) const
{
    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j)
            if(board[i][j] != otherState.board[i][j])
                return false;
    return true;
}

std::vector<State*> State::getTransitions() const
{
    return transitions;
}

std::array<std::array<int,3>,3> State::getBoard() const
{
    return board;
}

void State::addTransiton(State* transitionState)
{
    transitions.push_back(transitionState);
}

bool State::getFinal() const
{
    return finalState;
}

int State::getWinner() const
{
    return winner;
}

void State::print() const
{
    //std::cout << "State number: " << stateNumber << "\n";
    for(int i = 0; i < 3; ++i)
    {
        std::cout << "  ";
        for(int j = 0; j < 3; ++j)
        {
            if(board[i][j] == 0)
                std::cout << "[O]";
            else if(board[i][j] == 1)
                std::cout << "[X]";
            else
                std::cout << "[ ]";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
