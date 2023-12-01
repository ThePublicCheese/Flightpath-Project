#include "flightplan.h"

FlightPlan parseData(string flightDataFile);
tuple<string*, string*, char*, string> inputData(string pathsToCalculateFile);

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <FlightDataFile> <PathsToCalculateFile> <OutputFile>" << endl;
        return 1;
    }

    //magic ???
    ofstream outputFile(argv[3]);
    if (!outputFile.is_open()) {
        cerr << "Error opening output file: " << argv[3] << endl;
        return 1;
    }

    //redirecting cout to the output file
    streambuf* originalCout = cout.rdbuf();
    cout.rdbuf(outputFile.rdbuf());

    string flightDataFile = argv[1];
    string pathsToCalculateFile = argv[2];
  
    Stack pathStack;
    vector<string> currentPath;
    unordered_map<string, bool> visited;
    string* origin, * destination, key;

    char* specification;
    double minCost = -1.0; 
    double minTime = -1.0;  //initialize to a value we cant get

    FlightPlan flightPlan = parseData(flightDataFile); //parsing all of our data into flightPlan
   
    tie(origin, destination, specification, key) = inputData(pathsToCalculateFile); //retrieving our input arrays

    for (const auto& entry : flightPlan.getAdjacencyList()) { //marking every entry as not visited/infinity
        visited[entry.first] = false;
    }

    for (int count = 0; count < stoi(key); count++) { //processing each location
        //cout << origin[count] + "\n" + destination[count] + "\n" + specification[count] + "\n";
        flightPlan.shortestPaths(origin[count], destination[count], visited, specification[count]);
    }

    //restore cout to the original streambuf
    cout.rdbuf(originalCout);
    outputFile.close();
   
    return 0;
}

//oddly enough this was one of the most annoying parts of this project
FlightPlan parseData(string flightDataFile) {
    FlightPlan flightPlan;
    ifstream flightData;
    string amountOfData;
    char type;
    double cost, time;
    int count = 0, timeInt = 0, costInt = 0;
    bool fullParse = false;

    flightData.open(flightDataFile, ios::in);
    if (!flightData.is_open()) {
        cerr << "\n\nERROR DATA FILE COULD NOT BE READ\n\n";
        exit(1);
    }

    //read the number of locations from the first line
    else getline(flightData, amountOfData);
    try {
        cout << "The number of locations in your data file is: " << amountOfData << endl;
        cout << "Reading...\n";

        string* origin = new string[stoi(amountOfData)];
        string* destination = new string[stoi(amountOfData)];
        string* costString = new string[stoi(amountOfData)];
        string* timeString = new string[stoi(amountOfData)];
        //is this archaic? yes.. do I enjoy this? yes.
        for (int count = 0; count != stoi(amountOfData); count++) {
            //cout << count << "TEST \n";

            getline(flightData, origin[count], ',');
            getline(flightData, destination[count], ',');
            getline(flightData, costString[count], ',');
            getline(flightData, timeString[count], '\n');

            if (stoi(timeString[count]) < 0) {
                cout << "Warning: Invalid time value found for the leg from " << origin << " to " << destination << ". Skipping entry.\n";
                continue;  //skipping this entry and move to the next one
            }
            if (stoi(costString[count]) < 0) {
                cout << "Warning: Invalid cost value found for the leg from " << origin << " to " << destination << ". Skipping entry.\n";
                continue; 
            }
        }


        //cost = 1.0 * stoi(costString[count]);
        //time = 1.0 * stoi(timeString[count]);

        //everything else was so nice, I figured we needed some absolute chaos
        for (int i = 0; i != stoi(amountOfData);) {

            for (int j = 0; j != stoi(amountOfData); j++) {

                if (((origin[i] == origin[j]) && (destination[i] == destination[j])) || ((origin[i] == destination[j]) && (destination[i] == origin[j]))) {
                    if (costString[i] != "DELETED") {
                        if (stoi(costString[i]) > stoi(costString[j])) {
                            costString[i] = "DELETED";
                        }
                    }
                    if (timeString[i] != "DELETED") {
                        if (stoi(timeString[i]) > stoi(timeString[j])) {
                            timeString[i] = "DELETED";
                        }
                    }
                }
            }

            i++;
        }

        for (int count = 0; count != stoi(amountOfData); count++) {
            if ((costString[count] != "DELETED") && (timeString[count] != "DELETED")) {
                //cout << origin << " " << destination << " " << time << " " << cost << "\n";
                flightPlan.addFlight(origin[count], destination[count], time = 1.0 * stoi(timeString[count]), cost = 1.0 * stoi(costString[count]));
                //cout << destination << " " << origin << " " << time << " " << cost << "\n";
                flightPlan.addFlight(destination[count], origin[count], time = 1.0 * stoi(timeString[count]), cost = 1.0 * stoi(costString[count]));

            }
        }
        flightData.close();
    }
    catch (const invalid_argument& e) {
        cerr << "\n - Error: Invalid input, please check input files - \n" << "Here is a description of the error that could be correct: " << e.what() << endl;
        exit(1);
    }
    return flightPlan;
}

tuple<string*, string*, char*, string> inputData(string pathsToCalculateFile) {

        ifstream inputData;

        string amountOfData;
        int dataType = 0;


        //check if file is open
        inputData.open(pathsToCalculateFile, ios::in);
        if (!inputData.is_open()) {
            cout << "\n\nERROR INPUT FILE COULD NOT BE READ\n\n" << endl;
            exit(1);
        }
        else getline(inputData, amountOfData);
        cout << "The number of locations in your input file is: " << amountOfData << endl;
        cout << "Reading...\n";

        string *origin = new string[stoi(amountOfData)];
        string* destination = new string[stoi(amountOfData)];
        char* specification = new char[stoi(amountOfData)];

        for (int count = 0; count != stoi(amountOfData); count++) {
            //cout << count << "TEST \n";

            getline(inputData, origin[count], ',');
            getline(inputData, destination[count], ',');
            inputData >> specification[count]; //magic
            inputData.ignore(numeric_limits<streamsize>::max(), '\n'); //so we reset to the next line
        }
        
        return make_tuple(origin, destination, specification, amountOfData);
}