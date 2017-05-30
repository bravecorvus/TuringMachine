// std=c++17
//Heavily based on Phil Nadolny's Implementation with some changes

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

using namespace std;
enum Key {WRITE = 0, MOVE = 1, NEXT = 2};

class turingMachine
{
private:
	int readHead = 1;
	vector<char> inputTape;
	int acceptIndex;
	int whichState = 0;
	vector<unordered_map<char, tuple<char, int, int> > > delta;

public:
	turingMachine(){}
	turingMachine(ifstream & in);

	void bootStrap(string); // initialize input string to the TM
	bool runTM(); // runTM the content of the inputTape; return true if accepted and false if rejected
};

turingMachine::turingMachine(ifstream& arg)
{
	int numStates = -1; // number of states in the TM; initialized to -1 to indicate nothing read in yet
	arg >> numStates; // read in the number of states
	delta.resize(numStates); // give the delta function a hash map (unordered_map) for each state

	string alphabet = ""; 
	char reject = '\0'; 
	arg >> alphabet >> reject;

	char maybeAccepted, write; // temp vars needed for reading in info about each state in the TM
	int move, nextState; // another temp var for reading in state info

	// read in the info about each state
	for (int i = 0; i < delta.size(); ++i)
	{
		arg >> maybeAccepted; // discern whether the current state being read in will be acceptIndex
		if (maybeAccepted == 'A') // if so, set the 'acceptIndex' state variable accordingly
		{
			acceptIndex = i;
		}

		// read in the info about each of the transitions
		for (int j = 0; j < alphabet.size(); j++)
		{
			arg >> write; // read in the char that we would write to the inputTape
			if (write == '_') // if it is a '_', then there is no such transition in our TM
			{
				// add an invalid entry to the unordered map to indicate that there is no such transition
				delta.at(i).insert(make_pair(alphabet.at(j), make_tuple(reject, -2, -1)));
			}
			else
			{
				arg >> move >> nextState; // find where to move and what the next state is

				// insert into the hashmap
				delta.at(i).insert(make_pair(alphabet.at(j), make_tuple(write, move, nextState))); 
			}
		}
	}
}

// load the input string into the TM's inputTape
void turingMachine::bootStrap(string arg)
{
	inputTape.resize(0); // make sure the inputTape is empty
	inputTape.push_back('d'); // let the first symbol be a 'd' to represent a delta
	for (string::iterator it = arg.begin(); it != arg.end(); ++it)
	{
		inputTape.push_back((char) *it); // push back each char to be processed to the inputTape
	}

	inputTape.push_back('d'); // push back the final 'd' to represent another delta

}

bool turingMachine::runTM()
{
	whichState = 0; // make sure we are starting at the start state
	readHead = 1; // make sure the readHead is starting at index 1
	tuple<char, int, int> tupleStore; // temporary tuple to make the while loop more readable

	// this while loop represents the processing of the string on the inputTape
	while (true)
	{
		// get the tuple corresponding to the symbol read on the inputTape
		tupleStore = delta.at(whichState).at( inputTape.at(readHead) );

		// if we are to write a 'q' (an invalid inputTape character), then we know 
		// we require a transition that doesn't exist and may reject it
		if ( get<WRITE> (tupleStore) == 'q')
			return false;

		inputTape.at(readHead) = get<WRITE> (tupleStore); // write the appropriate char to the inputTape
		readHead += get<MOVE> (tupleStore); // move the readHead
		whichState = get<NEXT> (tupleStore); // get the next state

		if ( whichState == acceptIndex)
			return true;
	}
}

int main(int argc, char* argv[])
{
	string TMDefinition = argv[1]; // get the input file via command line args
	ifstream filename(TMDefinition); // ifstream
	turingMachine test(filename); // initialize the Turing machine
	string query = argv[2];
	bool wasEmpty = false;
		if (query == "[]")
		{
			wasEmpty = true;
			query = "";
		}

		test.bootStrap(query);
		if ( test.runTM() ) 
			cout << "accepted" << endl;
		else 
			cout << "rejected" << endl;

		wasEmpty = false;
	return 0;
}
