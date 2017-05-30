#include <sstream>
#include <vector>
#include <stack>
#include <streambuf>
#include <fstream>
#include <stdio.H>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

struct configuration {
    string content;
    string state;
    string readHead;
    configuration& operator= (const configuration &arg) {
        content = arg.content;
        state = arg.state;
        readHead = arg.readHead;
    }
};

bool checkPossible(configuration current, configuration topOfStack, vector<vector <string>> edges, string query) {
    cout << "HELLO WORLD" << endl;
}

int main(int argc, char* argv[])
{
	string TMDefinition = argv[1]; // get the input file via command line args
    string query = argv[2];
    string passthrough = "./TM2 "+TMDefinition+" "+query; 
    cout << passthrough << endl;
    const char * c = passthrough.c_str();
    system(c);

    char buffer[128];
    string result = "";
    FILE* pipe = popen(c, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
            {
                result += buffer;
            }
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);

    ifstream TM("TM.txt");
    //TM.open("TM.txt");
    if (!TM.good()) {
        cerr << "Error: unable to open TM.txt" << endl;
        exit(-1);
    }
    int counter = 0;
    vector <string> temp;
    string s;
    while (TM >> s) {
        temp.push_back(s);
        ++counter;
    }
    for (auto i : temp) {
        cout << i << endl;
    }
    string str;
    cout << str << endl;

    cout << result << endl;
    vector <string> configurations;
    stringstream ss(result.c_str());
    string to;
    while(getline(ss,to,'\n')) {
        configurations.push_back(to);
    }

    for(int i = 0; i < configurations.size(); ++i) {
        cout << configurations[i] << endl;
    }
    cout << "\n\n\n";
    //cout << temp[0] << endl; 
    int numstates = stoi(temp[0]);
    string alphabets = temp[1];
    //vector <vector <string>> configurationsfixed;
    string configurationsfixed[numstates][alphabets.length()];
    
    bool skip3;
    int skipindex = 0;
    int index2 = 0;
    int regularindex = 0;
    string concatenate1;
    string concatenate2;
    string concatenate3;
    string catted;
    vector <string> temp2; 

    for(int i = 3, end1 = temp.size(); i < end1; i +=3) {
        //cout << temp[i] << endl;
        if (temp[i] == "_") {
            concatenate1 = temp[i];
            concatenate2 = " ";
            concatenate3 = " ";
            catted = concatenate1 + " " + concatenate2 + " " + concatenate3;
            temp2.push_back(catted);
            i -= 2;
        } else if (temp[i] == "R" or temp[i] == "A") {
            temp2.push_back(temp[i]);
            if (temp[i+1] == "_") {
                concatenate1 = temp[i+1];
                concatenate2 = " ";
                concatenate3 = " ";
                catted = concatenate1 + " " + concatenate2 + " " + concatenate3;
                temp2.push_back(catted);
                i -= 2;
            } else {
                concatenate1 = temp[i+1];
                concatenate2 = temp[i+2];
                concatenate3 = temp[i+3];
                catted = concatenate1 + " " + concatenate2 + " " + concatenate3;
                temp2.push_back(catted);
                ++i;
            }
        } else {
            concatenate1 = temp[i];
            concatenate2 = temp[i+1];
            concatenate3 = temp[i+2];
            catted = concatenate1 + " " + concatenate2 + " " + concatenate3;
            temp2.push_back(catted);
        }
    }
    int index4 = 0;
    int index3 = 1;
    for(int i = 0; i < numstates; ++i ) {
        if (temp2[index3] == "A") {
            index4 = index3;
        }
            
        for(int j = 0; j < alphabets.length(); ++j) {
            configurationsfixed[i][j] == temp2[index3];
            ++index3;
        }
        ++index3;
    }
    for(int i = 0; i < numstates; ++i ) {
        for(int j = 0; j < alphabets.length(); ++j) {
            cout << configurationsfixed[i][j] << endl;
        }
    }
    //string configurationsfixed[numstates][alphabets.length()];
    //for(int i = 0; i < numstates; ++i) {
        //for(int j = 0; j < alphabets.length(); ++j) {
            //if (skip3) {
                //if (skipindex == 3) {
                    //skip3 = false;
                    //skipindex = 0;
                //}
                //else {
                    //++skipindex;
                //}
            //}
            //if (temp[index2] == "_") {
                //skip3 = true;
                //skipindex = 1;
            //} else {
                //if (regularindex == 0) {
                    //concatenate1 = temp[index2];
                    //regularindex += 1;
                //} else if (regularindex == 1) {
                    //concatenate2 = temp[index2];
                    //regularindex += 1;
                //} else if (regularindex == 2) {
                    //concatenate3 = temp[index2];
                    //regularindex += 1;
                //} else if (regularindex == 3) {
                    //catted = concatenate1 + concatenate2 + concatenate3;
                    //configurations[i][j] == catted;
            //}
            //configurationsfixed[i][j] = temp[index2];
        //}
        //++index2;
    //}
    //for (int i = 0; i < numstates; ++i) {
        //for(int j = 0; j < alphabets.length(); ++j) {
            //cout << configurationsfixed[i][j] << endl;
        //}
    //}

    //ifstream("edges.txt")
    configuration current;
    configuration topOfStack;
    stack <configuration>configstack;
    int index = 0;
    //while (true) {
        //if (configurations[index+3] == "accepted") {
            ////some accepting condition
            //break;
        //} else {
            //if (index > 2) {
                //current.content = configurations[index];
                //current.state = configurations[index+1];
                //current.readHead = configurations[index+2];
                //topOfStack = configstack.top();
                //configstack.pop();
                //if (checkPossible(current, topOfStack, configurationsfixed, query)) {
                    //configstack.push(current);
                    //continue;
                //} else {
                    //cout << "TM M is undecidable" << endl;
                    //break;
                //}
            //} else {
                //current.content = configurations[index];
                //current.state = configurations[index+1];
                //current.readHead = configurations[index+2];
                //configstack.push(current);
            //}
        //}
        //index += 3;

    //}
	return 0;
}
