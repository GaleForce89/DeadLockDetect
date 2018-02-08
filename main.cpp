/** @file main.cpp
 *
 * @author GaleForce
 *
 * @assg   Programming Assignment 
 *
 * @desc Implement the deadlock detection algorithm.  Given a file
 *         that describes the current allocation A of resources in the
 *         system, and the current set of outstanding requests Q in
 *         the system, determine if a deadlock is present or not. 
 *
 */
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
using namespace std;


// global constants
const int MAX_PROCESSES = 10; // I won't test your algorithm with simulations with more than 10 processes
const int MAX_RESOURCES = 10; // nor will I give a simulation to test with more than 10 resources


// simple struct to read in and hold the state of the system
typedef struct
{
  int numResources;
  int numProcesses;
  int available[MAX_RESOURCES]; // V available vector
  int alloc[MAX_PROCESSES][MAX_RESOURCES]; // A allocation matrix
  int request[MAX_PROCESSES][MAX_RESOURCES]; // Q request matrix
} State;


/** Read system state from file.
 * Given a file, read the current system state from the file.
 * The system state file is expected to hold the available vector V
 * the allocation matrix A and the request matrix Q.  
 *
 * @param simfilename The name of the file to open and read state & request
 *         from.
 * @return state A new State structure is allocated and filled with the
 *         system state from the file.  A pointer to this allocated system
 *         state structure is returned as a result of calling this function.
 */
State* readSystemState(char* statefilename)
{
  ifstream simstatefile(statefilename);
  State* state;
  int r, p;

  // If we can't open file, abort and let the user know problem
  if (!simstatefile.is_open())
  {
    cout << "Error: could not open system state file: " << statefilename
         << endl;
    exit(1);
  }

  // dynamically allocate a new State structure, to be filled in and returned
  state = new State;

  // Format of file is this (where m = numResource n = numProcesses
  //                         V = available vector
  //                         A = allocation matrix and
  //                         Q = request matrix)
  // m n
  // V1 V2 V3 ... Vm
  // A11 A12 ... A1m
  // ...
  // An1 An2 ... Anm
  // Q11 Q12 ... Q1m
  // ...
  // Qn1 Qn2 ... Qnm

  // First line, get m (numResources) and n (numProcesses)
  simstatefile >> state->numResources >> state->numProcesses;

  // Next line contains the available vector V
  for (r = 0; r < state->numResources; r++)
  {
    simstatefile >> state->available[r];
  }

  // Next n lines contain the allocation matrix A
  for (p = 0; p < state->numProcesses; p++)
  {
    for (r = 0; r < state->numResources; r++)
    {
      simstatefile >> state->alloc[p][r];
    }
  }

  // Next n lines contain the request matrix Q
  for (p = 0; p < state->numProcesses; p++)
  {
    for (r = 0; r < state->numResources; r++)
    {
      simstatefile >> state->request[p][r];
    }
  }

  // return the newly allocated and filled in system state
  return state;
}

/** Display a vector
 * Display a state vector to standard output
 *
 * @param len The number of items in the vector
 * @param v An array of integers of len items
 */
void displayVector(int len, int v[])
{
  int i;

  // Display a header
  for (i = 0; i < len; i++)
  {
    cout << "R" << i << " ";
  }
  cout << endl;

  // Display values
  for (i = 0; i < len; i++)
  {
    cout << setw(2) << v[i] << " ";
  }
  cout << endl;
}

/** Display a matrix
 * Display a state matrix to standard output
 *
 * @param rows The number of rows in the matrix
 * @param cols The number of cols in the matrix
 * @param m A 2 dimensional array of rows x cols integers
 */
void displayMatrix(int rows, int cols, int v[MAX_PROCESSES][MAX_RESOURCES])
{
  int r, c;

  // display column headers
  cout << "   "; // extra space over for row labels
  for (c = 0; c < cols; c++)
  {
    cout << "R" << c << " ";
  }
  cout << endl;

  // now display data in matrix
  for (r = 0; r < rows; r++)
  {
    cout << "P" << r << " ";
    for (c = 0; c < cols; c++)
    {
      cout << setw(2) << v[r][c] << " ";
    }
    cout << endl;
  }
  cout << endl;
}

/** Display state
 * Display the values of the resource vectors and matrices in the indicated
 * state structure
 *
 * @param state A pointer to a State struct whose info we should display on stdout.
 */
void displayState(State* s)
{
  cout << "numResources (m) = " << s->numResources << " ";
  cout << "numProcesses (n) = " << s->numProcesses << endl << endl;

  cout << "Available vector V:" << endl;
  displayVector(s->numResources, s->available);
  cout << endl;

  cout << "Allocation matrix A: " << endl;
  displayMatrix(s->numProcesses, s->numResources, s->alloc);
  cout << endl;

  cout << "Request matrix Q: " << endl;
  displayMatrix(s->numProcesses, s->numResources, s->request);
  cout << endl;

}

/** markOff
 * This is where processes with no resources allocated are determined and marked in the vector
 *
 * @param mark The mark vector used to mark off processes that have no resources allocated
 * @param rows  The number of rows
 * @param cols The number of columns
 * @param v a 2 dimensional array
 * @return returns the vector
 */
vector<int> markOff(vector<int> mark, int rows, int cols, int v[MAX_PROCESSES][MAX_RESOURCES])
{

  int r, c, check; //rows, columns, and check feature

  for(r = 0; r < rows; r++)
  {

    //set check to 1
    check = 0;
    for(c = 0; c < cols; c++)
    {
      if(v[r][c] == 0)
        check++;
    }

    if(check == cols)
      mark.push_back(r);
  }

  return mark;
}

/** lockCheck
 * This will return true if there are deadlocks detected, false if none detected
 *
 * @param deadLocks The deadLocks vector that stores detected deadlocks
 * @return Checks if deadLocks is > 0 and returns true or else false
 */
bool lockCheck(vector<int> deadLocks)
{
  return (deadLocks.size() > 0 ? true : false); //could be deadLocks.size() > 0
  //wanted experience with the Ternary operator
}

/** displayLock
 * Displays any detected deadlocks
 *
 * @param deadLocks The deadLocks vector that stores detected deadlocks
 */
void displayLock(vector<int> deadLocks){
  cout << "Deadlock: ";
  for(int x = 0; x < deadLocks.size();x++){
    cout << "P" << deadLocks[x] << ", ";
  }
}

/** Detect
 *  This is where we check available resources and mark any processes that will not
 *  cause a deadlock
 *
 * @param mark The mark vector to keep track of processes that will not cause a deadlock
 * @param state The data matrix
 * @return Will return the mark vector
 */
vector<int> detect(vector<int> &mark, State* &state)
{
  int r, rows, c, cols, x, markcheck;

  rows = state->numProcesses; //sets rows
  cols = state->numResources; //sets columns

  for(r = 0; r < rows; r++)
  {
    //make sure not to check same rows we have marked out
    for(markcheck = 0; markcheck < mark.size();markcheck++){
      if(mark[markcheck] == r)
        r++;
    }

    //if skipping are marked rows causes end of rows then break
    if(r == rows)
      break;

    for(c = 0; c < cols; c++)
    {
      if(state->request[r][c] > state->available[c])
        break;
      else if(cols  == c + 1 && state->request[r][c] <= state->available[c])
      {
        mark.push_back(r); //mark the process/row
        //probably should push this to a function
        for(x = 0;x < cols;x++){
          state->available[x] += state->alloc[r][x];
        }
        //call detect to check again
        detect(mark, state);
      }
    }
  }
  return mark;
}

/** determineLocks
 *  calculates and pushes any deadlocks to the deadlocks vector
 *
 * @param mark The marked processes that are not going to deadlock
 * @param deadLocks The vector to hold deadlocks
 * @param state The data matrix (provides the rows)
 */
void determineLocks(vector<int> mark, vector<int> &deadLocks, State* state)
{
  int r, rows, markcount, lockcheck;
  rows = state->numProcesses;

  for(r = 0; r < rows; r++){

    lockcheck = 0;
    for(markcount = 0; markcount < mark.size(); markcount++)
    {
      if(mark[markcount] == r)
      {
        lockcheck++;
      }
    }
    if(lockcheck == 0)
      deadLocks.push_back(r);
  }

}

/** The deadlock detector
 * The starting point for implementation of the deadlock detection algorithm.
 * We open and read in the allocation matrices here, then perform the deadlock detection.
 *
 * @ param statefilename A string with the name of the file holding the A and Q system state matrices
 */
void detectDeadlock(char* statefilename)
{
  State* state;
  //Create a vector to mark any allocation set to 0 continuously
  //and deadLocks vector if any are detected
  vector<int> mark, deadLocks;
  //reserve 10 elements
  mark.reserve(10);
  deadLocks.reserve(10);

  //Read in the matrix
  state = readSystemState(statefilename);

  //display test
  //displayState(state);

  //Mark off any rows that have no resources allocated
  mark = markOff(mark, state->numProcesses, state->numResources, state->alloc);

  //**********DEBUGGING FEATURE TO TEST MARK VECTOR********
  //cout << mark.size() << endl;
  //if(mark.size() > 0)
  //  for(int x = 0; x < mark.size();x++)
  //    cout << mark[x] << endl;
  //********************
  mark = detect(mark, state);

  //Check the vector
  //displayVector(state->numResources, state->available);

  //determine locks
  determineLocks(mark, deadLocks, state);

  //**********DEBUGGING FEATURE TO TEST MARK VECTOR********
  //cout << mark.size() << endl;
  //if(mark.size() > 0)
  //  for(int x = 0; x < mark.size();x++)
  //    cout << mark[x] << endl;

  //**** DEBUGGING
  //Check if deadLocks has any size
  // cout << deadLocks.size() << lockCheck(deadLocks);
  //********************
  //Display results
  if(lockCheck(deadLocks))
    displayLock(deadLocks);
  else
    cout << "No Deadlock" << endl;

  // I have provided some example routines to read and display system state, implemented as a plain
  // C struct using C 1 and 2 dimensional arrays.  You can uncomment out the following, and/or use
  // the displayMatrix() and displayVector() functions to help you debug.  But make sure you
  // remove or comment back up any statements after you are done debugging.
  //displayState(state);

  // You need to implement your solution here.  I would recommend you use functions for each of
  // these steps.

  // Step 1: Set up a data structure that records marked/unmarked
  //         processes.  All processes are initially unmarked Search
  //         through the allocation matrix to find rows of all 0, and
  //         mark corresponding processes in your mark structure

  // Step 2: Create a temporary vector W.  Copy contents of available
  //    vector V to W.  Suggestion: create a function called
  //    copyVector, that takes a vector as its parameter, and returns
  //    a new vector.

  // Need to put Steps 3 and 4 in a loop

  // Step 3: Find index i such that process i is currently unmarked,
  //         and the ith row of Q is less than or equal to W.  If no
  //         such process is found, need to terminate algorithm/loop.
  //         Suggestion: write a function that takes Q and W, and
  //         returns either i (index of process meeting criteria) or
  //         -1

  // Step 4: If a row was found (e.g. i was a valid process that met
  //         criteria of step 3), mark process i and add the
  //         correspoinding row of allocation matrix to W.  Loop back
  //         to beginning of step 3.

  // Step 5: after loop finishes, 
  // if (your marked/unmarked processes contains unmarked processes)
  // {
  //   cout << "Deadlock";
  //   // loop through your marked/unmarked structure, print out all unmarked processes as P1, P2, etc.
  //   cout << endl;
  // }
  // else // all processes were marked, so no deadlock
  // {
  //   cout << "No Deadlock" << endl;
  // }
}


/** Main entry point of deadlock detection.
 * The main entry point of the deadlock detection program.  This function
 * checks the command line arguments, and calls the detection function if correct
 * arguments were supplied.   We expect a single command line argument
 * which is the name of the file holding the allocation and request matrices
 * of the current state of the system.
 *
 * @param argc The argument count
 * @param argv The command line argument values. We expect argv[1] to be the
 *              name of a file in the current directory holding A and Q matrices.
 */
int main(int argc, char** argv)
{
  if (argc != 2)
  {
    cout << "Error: expecting state matrix file as first command line parameter" << endl;
    cout << "Usage: " << argv[0] << " system-state.sim" << endl;
    exit(1);
  }

  detectDeadlock(argv[1]);
  // if don't want to use command line do following. Need to recompile by hand since file 
  // name to get simulated events from is hard coded.
  // Make sure you revert back to using command line before submitting your program.
  //detectDeadlock("state-01.sim");  
}
