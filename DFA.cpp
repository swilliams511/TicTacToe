#include "DFA.hpp"
#include <ctime>
#include <cstdlib>
#include "Eigen/Sparse"

DFA::DFA()
{

}

DFA::~DFA()
{
    for(size_t i = 0; i < collectionOfStates.size(); ++i)
        delete collectionOfStates[i];
}

//This function provides a simulation of a sequence of valid random moves through
//the DFA. The variable n represents the chosen state to begin in. The variable
//print decides whether output is printed to the console. It is suggested that
//this be set to true for single simulations, and false for a large number of
//simulations. As a failsafe, after 1 billion turns the simulation will declare that
//you lost. This is because it is possible (but extremely unlikely) that you will
//just start moving between two non-final states forever, causing an infinite loop.
int DFA::simulate(int n, bool print)
{
    int turnCounter = 1;
    int counter = 0;

    State* state = collectionOfStates[n];
    if(print)
            std::cout << "---Turn: " << turnCounter << "---\n";
    if(print)
        state->print();
    ++turnCounter;
    while(counter != 4) ///TODO clean up logic in this
    {
        if(print)
            std::cout << "---Turn: " << turnCounter << "---\n";
        int random = std::rand() % state->getTransitions().size();
        state = state->getTransitions().at(random);
        if(print)
            state->print();
        if(state->isFinal())
        {
            if(print)
                std::cout << "you win!!!\n";
            return 1;
        }
        ++counter;
        ++turnCounter;
    }
    if(print)
        std::cout << "---Turn: " << turnCounter << "---\n";
    int random = std::rand() % state->getTransitions().size();
    state = state->getTransitions().at(random);
    if(print)
        state->print();
    if(state->isFinal())
    {
        if(print)
            std::cout << "you loose!!!\n";
        return 0;
    }
    ++turnCounter;
    while(true)
    {
        if(print)
            std::cout << "---Turn: " << turnCounter << "---\n";
        random = std::rand() % state->getTransitions().size();
        state = state->getTransitions().at(random);
        if(print)
            state->print();
        if(state->isFinal())
        {
            if(print)
                std::cout << "you win!!!\n";
            return 1;
        }
        ++turnCounter;
        if(print)
            std::cout << "---Turn: " << turnCounter << "---\n";
        random = std::rand() % state->getTransitions().size();
        state = state->getTransitions().at(random);
        if(print)
            state->print();
        if(state->isFinal())
        {
            if(print)
                std::cout << "you loose!!!\n";
            return 0;
        }
        ++turnCounter;
        if(turnCounter > 1000000000)
            return 0;
    }
}


//This function generates all the states needed in our DFA. The states are put into the container
//"collectionOfStates" and in addition there are seven other vectors to keep track of states
//that are generated at a certain level in the tree.
//The Datastructure and algorithm part of the report will explain in more detail what happens
void DFA::createStates()
{
    State* firstState = new State();           //create the starting state which is an empty board
    firstState->setStateNumber(collectionOfStates.size());
    collectionOfStates.push_back(firstState);
//--------------------------------------------------------
//block of code that generates all first turn states
    for(int i = 0; i < 3; ++i)//for each empty square on the board, create a new state instance where that empty
        for(int j = 0; j < 3; ++j) //state is occupied by a 0
        {
            std::array<std::array<int,3>,3> firstStateBoard = firstState->getBoard();
            firstStateBoard[i][j] = 0;
            State* currentState = new State(firstStateBoard);
            currentState->setStateNumber(collectionOfStates.size());
            collectionOfStates.push_back(currentState);
            firstTurnStates.push_back(currentState);
            firstState->addTransiton(currentState);//link the starting state to the newly made first turn state
        }//end for
//--------------------------------------------------------
//block of code that generates all second turn states
    for(size_t i = 0; i < firstTurnStates.size(); ++i)
        for(unsigned j = 0; j < 3; ++j)
            for(unsigned k = 0; k < 3; ++k)
            {
                std::array<std::array<int,3>,3> stateBoard = firstTurnStates[i]->getBoard();
                if(stateBoard[j][k] == -1)            //if the square we are looking at doesn't contain an X or O
                {
                    stateBoard[j][k] = 1;
                    State* currentState = new State(stateBoard);
                    currentState->setStateNumber(collectionOfStates.size());
                    collectionOfStates.push_back(currentState);
                    secondTurnStates.push_back(currentState);
                    firstTurnStates[i]->addTransiton(currentState);
                }//end if
            }//end for
//--------------------------------------------------------
//block of code that generates all third turn states
    for(size_t i = 0; i < secondTurnStates.size(); ++i)
        for(unsigned j = 0; j < 3; ++j)
            for(unsigned k = 0; k < 3; ++k)
            {
                std::array<std::array<int,3>,3> stateBoard = secondTurnStates[i]->getBoard();
                if(stateBoard[j][k] == -1)
                {
                    bool inCollection = false; //assume the generated state isn't in the collection of third states
                    stateBoard[j][k] = 0;
                    State* currentState = new State(stateBoard);
                    for(size_t m = 0; m < thirdTurnStates.size(); ++m)
                        if(*currentState == *thirdTurnStates[m])
                        {                       //dont add it to the collection
                            secondTurnStates[i]->addTransiton(thirdTurnStates[m]);
                            inCollection = true;
                            delete currentState;
                            break;              //no reason to continue looping at this point
                        }//end if
                    if(inCollection == false)
                    {
                        currentState->setStateNumber(collectionOfStates.size());
                        collectionOfStates.push_back(currentState);
                        thirdTurnStates.push_back(currentState);
                        secondTurnStates[i]->addTransiton(currentState);
                    }//end if
                }//end if
            }//end for
//--------------------------------------------------------
//block of code that generates all 4th turn states
    for(size_t i = 0; i < thirdTurnStates.size(); ++i)
        for(unsigned j = 0; j < 3; ++j)
            for(unsigned k = 0; k < 3; ++k)
            {
                std::array<std::array<int,3>,3> stateBoard = thirdTurnStates[i]->getBoard();
                if(stateBoard[j][k] == -1)
                {
                    bool inCollection = false;  //assume the generated state isn't in the collection of third states
                    stateBoard[j][k] = 1;
                    State* currentState = new State(stateBoard);
                    for(size_t m = 0; m < fourthTurnStates.size(); ++m)
                        if(*currentState == *fourthTurnStates[m])
                        {                               //dont add it to the collection
                            thirdTurnStates[i]->addTransiton(fourthTurnStates[m]);
                            inCollection = true;
                            delete currentState;
                            break;                      //no reason to continue looping at this point
                        }//end if
                    if(inCollection == false)
                    {
                        currentState->setStateNumber(collectionOfStates.size());
                        collectionOfStates.push_back(currentState);
                        fourthTurnStates.push_back(currentState);
                        thirdTurnStates[i]->addTransiton(currentState);
                    }//end if
                }//end if
            }//end for
//--------------------------------------------------------
//block of code that generates all 5th turn states
    for(size_t i = 0; i < fourthTurnStates.size(); ++i)
        for(unsigned j = 0; j < 3; ++j)
            for(unsigned k = 0; k < 3; ++k)
            {
                std::array<std::array<int,3>,3> stateBoard = fourthTurnStates[i]->getBoard();
                if(stateBoard[j][k] == -1)
                {
                    bool inCollection = false;          //assume the generated state isn't in the collection of third states
                    stateBoard[j][k] = 0;
                    State* currentState = new State(stateBoard);
                    for(size_t m = 0; m < fifthTurnStates.size(); ++m)
                        if(*currentState == *fifthTurnStates[m])
                        {   //dont add it to the collection
                            fourthTurnStates[i]->addTransiton(fifthTurnStates[m]);
                            inCollection = true;
                            delete currentState;
                            break;                      //no reason to continue looping at this point
                        }//end if
                    if(inCollection == false)
                    {
                        currentState->isFinal();
                        currentState->setStateNumber(collectionOfStates.size());
                        collectionOfStates.push_back(currentState);
                        fifthTurnStates.push_back(currentState);
                        fourthTurnStates[i]->addTransiton(currentState);
                    }//end if
                }//end if
            }//end for
//-----------------------------------------------------------------
//block of code that generates all 6th turn states
    for(size_t i = 0; i < fifthTurnStates.size(); ++i)
        for(unsigned j = 0; j < 3; ++j)
            for(unsigned k = 0; k < 3; ++k)
            {
                std::array<std::array<int,3>,3> stateBoard = fifthTurnStates[i]->getBoard();
                if(stateBoard[j][k] == -1 && !fifthTurnStates[i]->isFinal()) //if the state is final, we know we dont
                {                                                            //transition from it
                    bool inCollection = false; //assume the generated state isn't in the collection of third states
                    stateBoard[j][k] = 1;
                    State* currentState = new State(stateBoard);
                    for(size_t m = 0; m < sixthTurnStates.size(); ++m)
                        if(*currentState == *sixthTurnStates[m])
                        {   //dont add it to the collection
                            fifthTurnStates[i]->addTransiton(sixthTurnStates[m]);
                            inCollection = true;
                            delete currentState;
                            break;   //no reason to continue looping at this point
                        }//end if
                    if(inCollection == false)
                    {
                        currentState->isFinal();
                        currentState->setStateNumber(collectionOfStates.size());
                        collectionOfStates.push_back(currentState);
                        sixthTurnStates.push_back(currentState);
                        fifthTurnStates[i]->addTransiton(currentState);
                    }//end if
                }//end if
            }//end for
//-----------------------------------------------------------------------------------------------
//block of code that sets up O's transitioning to adjacent cells
    for(size_t i = 0; i < sixthTurnStates.size(); ++i)
        for(int j = 0; j < 3; ++j)
            for(int k = 0; k < 3; ++k)
            {
                std::array<std::array<int,3>,3> stateBoard = sixthTurnStates[i]->getBoard();
                if(stateBoard[j][k] == 0 && !sixthTurnStates[i]->isFinal()) //if the state is final, we know we dont
                    for(int a = j-1; a < j+2; ++a) //transition from it
                        for(int b = k-1; b < k+2; ++b)
                            if(stateBoard[a][b] == -1 && a != -1 && a != 3 && b != -1 && b != 3)
                            {
                                bool inCollection = false;
                                std::array<std::array<int,3>,3> moveBoard = stateBoard;
                                moveBoard[j][k] = -1;
                                moveBoard[a][b] = 0;
                                State* currentState = new State(moveBoard);
                                for(size_t m = 0; m < seventhTurnStates.size(); ++m)
                                    if(*currentState == *seventhTurnStates[m])
                                    {
                                        //dont add it to the collection
                                        sixthTurnStates[i]->addTransiton(seventhTurnStates[m]);
                                        inCollection = true;
                                        delete currentState;
                                        break;   //no reason to continue looping at this point
                                    }//end if
                                if(inCollection == false)
                                {
                                    currentState->isFinal();
                                    currentState->setStateNumber(collectionOfStates.size());
                                    collectionOfStates.push_back(currentState);
                                    seventhTurnStates.push_back(currentState);
                                    sixthTurnStates[i]->addTransiton(currentState);
                                }//end if
                            }//end if
            }//end for
//-----------------------------------------------------------------------------------------------
//block of code that sets up X's transitioning to adjacent cells
    for(size_t i = 0; i < seventhTurnStates.size(); ++i)
        for(int j = 0; j < 3; ++j)
            for(int k = 0; k < 3; ++k)
            {
                std::array<std::array<int,3>,3> stateBoard = seventhTurnStates[i]->getBoard();
                if(stateBoard[j][k] == 1 && !seventhTurnStates[i]->isFinal()) //if the state is final, we know we dont
                    for(int a = j-1; a < j+2; ++a)  //transition from it
                        for(int b = k-1; b < k+2; ++b)
                            if(stateBoard[a][b] == -1 && a != -1 && a != 3 && b != -1 && b != 3)
                            {
                                bool inCollection = false;
                                std::array<std::array<int,3>,3> moveBoard = stateBoard;
                                moveBoard[j][k] = -1;
                                moveBoard[a][b] = 1;
                                State* currentState = new State(moveBoard);
                                for(size_t m = 0; m < sixthTurnStates.size(); ++m)
                                    if(*currentState == *sixthTurnStates[m])
                                    {   //dont add it to the collection
                                        seventhTurnStates[i]->addTransiton(sixthTurnStates[m]);
                                        inCollection = true;
                                        delete currentState;
                                        break;   //no reason to continue looping at this point
                                    }//end if
                                if(inCollection == false)
                                {
                                    currentState->isFinal();
                                    currentState->setStateNumber(collectionOfStates.size());
                                    collectionOfStates.push_back(currentState);
                                    sixthTurnStates.push_back(currentState);
                                    seventhTurnStates[i]->addTransiton(currentState);
                                }//end if
                            }//end if
            }//end for
}

//Pre condition: firstMoveBoard is an integer in [0,8]
std::vector<double> DFA::calculateProbability(int firstMoveBoard) const
{
    std::cout << "Starting calculations...\n";
    //in order to solve this problem, we want to build an equation in the form
    // Ax = b, where A is an NxN matrix and N = number of total states, x is the probability of
    //"winning for a specific state ie x0 would be
    int n = collectionOfStates.size();

    //A (NxN matrix)
    Eigen::SparseMatrix<double> A(n,n);

    //b (Nx1 matrix)
    Eigen::VectorXd b(n);

    //Format for assigning values to a specific cell in the matrix A(0,0) = 1;
    // given [0][0][0]                             [0][2][0]
    //       [0][0][0]                             [0][0][0]
    //       [0][0][0], saying A(0,1) = 2; makes   [0][0][0]
    //thus A(row,column)
    for(int i = 0; i < n; ++i)
    {
        State* current = collectionOfStates.at(i);
        A.coeffRef(current->getStateNumber(),current->getStateNumber()) = 1;
        if(current->getFinal())
        {
              if(current->getWinner() == 0)
                    b(current->getStateNumber()) = 1;
        }
        else
        {
            double k = -1.0/current->getTransitions().size();
            for(size_t j = 0; j < current->getTransitions().size(); ++j)
            {
                State* transitionState = current->getTransitions().at(j);
                A.coeffRef(current->getStateNumber(),transitionState->getStateNumber()) = k;
            }
        }
    }
    std::cout << "matrices generated...\n";
    A.makeCompressed();
    std::cout << "compressed...\n";
    Eigen::SparseLU<Eigen::SparseMatrix<double>, Eigen::COLAMDOrdering<int> > solver;
    solver.analyzePattern(A);
    std::cout << "pattern analyzed...\n";
    solver.factorize(A);
    std::cout << "factorized...\n";
    Eigen::VectorXd x(n);
    x = solver.solve(b);
    std::cout << "solved!\n";
    std::vector<double> results;
    for(int i = 1; i < 10; ++i)
        results.push_back(x(i,0));
    return results;
}


size_t DFA::numStates() const
{
    return collectionOfStates.size();
}


void DFA::print() const
{
    std::cout << "states on turn 1: " <<firstTurnStates.size() << "\n";
    std::cout << "states on turn 2: " <<secondTurnStates.size() << "\n";
    std::cout << "states on turn 3: " <<thirdTurnStates.size() << "\n";
    std::cout << "states on turn 4: " <<fourthTurnStates.size() << "\n";
    std::cout << "states on turn 5: " <<fifthTurnStates.size() << "\n";
    std::cout << "states on turn 6: " <<sixthTurnStates.size() << "\n";
    std::cout << "states on turn 7: " <<seventhTurnStates.size() << "\n";
    std::cout << "total number of states: " <<collectionOfStates.size() << "\n";
}
