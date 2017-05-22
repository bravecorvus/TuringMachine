/* This program simulates the TM assigned in class on April 13, 2017
 * NOTE:  PLEASE COMPILE WITH C++11
 */

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

using namespace std;


// The following enum makes the tuple element retrieval syntax much more readable
enum Key {WRITE = 0, MOVE = 1, NEXT = 2};

class turingMachine
{
private:
	int head = 1; // this var simulates the head which reads the tape; will always start at the 1-index of the tape
	vector<char> tape; // simulates the tape
	int accepted; // this is the index of the Q which is the accept state
	int currentState = 0; // this var keeps track of which state we are in when processing a string

	// The following vector of unordered_maps represents the action of the TM.
	// The index in vector correspends to the state number (see the JFLAP for state numbers)
	// and the unordered map at each index returns a tuple containing info about what the TM should
	// do depending on what symbol was read in on the tape.
	// Tuple format:  <WRITE, MOVE (-1 for left, 0 for stay, 1 for right), NEXT_STATE>
	vector<unordered_map<char, tuple<char, int, int> > > delta;

public:
	turingMachine(){} // default constructor
	turingMachine(ifstream & in); // file-based constructor

	void initializeTape(string); // reads in the input string into the tape
	bool process(); // process the content of the tape; return true if accepted and false if rejected
};


// This constructor reads in a file which represents a TM.  
// The format is as follows (note that [char]-connection represents what the machine is supposed to do when reading in [char]
// i.e. "d -1 1" means write a delta, move one space to the left, and go to state 1; also, ALPHABET is the alphabet accepted 
// by this TM and REJECT_CHAR is any char not accepted by this turing machine

/*
   NUMBER_OF_NODES
   ALPHABET
   REJECT_CHAR

   [the following for each node]
   R/A (is this state accepted or rejected)
   [first-alphabet-char]-connection
   [second-alphabet-char]-connection
   [third-alphabet-char]-connection
   [fourth-alphabet-char]-connection
   .
   .
   .
*/
turingMachine::turingMachine(ifstream& in)
{
	int numStates = -1; // number of states in the TM; initialized to -1 to indicate nothing read in yet
	in >> numStates; // read in the number of states
	delta.resize(numStates); // give the delta function a hash map (unordered_map) for each state

	// all possible characters in the alphabet
	// MUST be in the same order as the connections listed for each node
	string alphabet = ""; // start initialized to nothing
	char reject = '\0'; // the char not in the alphabet which can serve as an indication that we should reject
	in >> alphabet >> reject;

	char maybeAccepted, write; // temp vars for reading in info about each state in the TM
	int move, nextState; // another temp var for reading in state info

	// read in the info about each state
	for (int i = 0; i < delta.size(); ++i)
	{
		in >> maybeAccepted; // discern whether the current state being read in will be accepted
		if (maybeAccepted == 'A') // if so, set the 'accepted' state variable accordingly
		{
			accepted = i;
		}

		// read in the info about each of the transitions
		for (int j = 0; j < alphabet.size(); j++)
		{
			in >> write; // read in the char that we would write to the tape
			if (write == '_') // if it is a '_', then there is no such transition in our TM
			{
				// add an invalid entry to the unordered map to indicate that there is no such transition
				delta.at(i).insert(make_pair(alphabet.at(j), make_tuple(reject, -2, -1)));
			}
			else
			{
				in >> move >> nextState; // find where to move and what the next state is

				// insert into the hashmap
				delta.at(i).insert(make_pair(alphabet.at(j), make_tuple(write, move, nextState))); 
			}
		}
	}
}

// load the input string into the TM's tape
void turingMachine::initializeTape(string query)
{
	tape.resize(0); // make sure the tape is empty
	tape.push_back('d'); // let the first symbol be a 'd' to represent a delta
	for (string::iterator it = query.begin(); it != query.end(); ++it)
	{
		tape.push_back((char) *it); // push back each char to be processed to the tape
	}

	tape.push_back('d'); // push back the final 'd' to represent another delta

}

// process the input string (which has already been read into the tape)
bool turingMachine::process()
{
	currentState = 0; // make sure we are starting at the start state
	head = 1; // make sure the head is starting at index 1
	tuple<char, int, int> tmpTuple; // temporary tuple to make the while loop more readable

	// this while loop represents the processing of the string on the tape
	while (true)
	{
		// get the tuple corresponding to the symbol read on the tape
		tmpTuple = delta.at(currentState).at( tape.at(head) );

		// if we are to write a 'q' (an invalid tape character), then we know 
		// we require a transition that doesn't exist and may reject it
		if ( get<WRITE> (tmpTuple) == 'q')
			return false;

		tape.at(head) = get<WRITE> (tmpTuple); // write the appropriate char to the tape
		head += get<MOVE> (tmpTuple); // move the head
		currentState = get<NEXT> (tmpTuple); // get the next state

		// if the current state is the accepted state, return true
		if ( currentState == accepted)
			return true;
	}
}

int main(int argc, char* argv[])
{
	if (argc != 3) // check that the user provided two arguments to the command line
	{
		cout << "Please provide a file to read in and an input string file" << endl;
		return 0;
	}

	string file = argv[1]; // get the input file with the TM encoded from the command line
	//string inputStrings = argv[2]; // get the file with input strings

	ifstream saintPeter(file); // create an ifstream object to read in the Turing machine
	turingMachine saintOfBrokenObjects(saintPeter); // create the Turing machine

	//ifstream saintFrancis(inputStrings); // create an ifstream object for input strings
	string query = argv[2]; // an individual query string
	bool wasEmpty = false; // bool to tell whether the input was the empty string; this will help us format output
	// read in each query string and process it
	//while (saintFrancis >> query)
	//{
		if (query == "[]") // "[]" means empty string
		{
			wasEmpty = true;
			query = "";
		}

		saintOfBrokenObjects.initializeTape(query); // initialize saintOfBrokenObjects's tape

		// output result
		//cout << "input ";
		//(wasEmpty) ? cout << "[empty-string]" : cout << query;
		//cout << " was ";

		// process the current string and indicate accepted or rejected
		if ( saintOfBrokenObjects.process() ) 
			cout << "accepted" << endl;
		else 
			cout << "rejected" << endl;

		wasEmpty = false; // reset wasEmpty
	//}

	return 0;
}
