#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>

#include "point.h"

using namespace std;

// Functions declaration
string requestFile(string);
int requestMode();

map<int, Point> initPointsInMap(string, int);
vector<Point> initPointsInVector(string, int);

map<int, Point> movePointsFromMap(map<int, Point>, string, int);
vector<Point> movePointsFromVector(vector<Point>, string, int);

bool yesOrNoQuestion(string);

void printMoveFinishedMsg(string);
void printPoints(map<int, Point>);
void printPoints(vector<Point>);

bool checkFileExist(string);

//storage modes : 
// v = Vector
// m = map

int main()
{
    int maxSize = 8000;

    string initFile;
    string moveFile;

    char mode;

    vector<Point> pointsVector;
    map<int, Point> pointsMap;
    

    initFile = requestFile("Please enter the name of the point initialization file (ex: initial.txt) :");

    mode = requestMode();

    if (mode == 'v') {
        pointsVector = initPointsInVector(initFile, maxSize);
    }
    else if (mode == 'm') {
        pointsMap = initPointsInMap(initFile, maxSize);
    }

    if (yesOrNoQuestion("Do you want to print out the points positions? (yes = Y / no = Anything else)")) {
        if (mode == 'v') {
            printPoints(pointsVector);
        }
        else if (mode == 'm') {
            printPoints(pointsMap);
        }
    }
    
    moveFile = requestFile("Please enter the name of the move file (ex: move.txt) :");

    if (mode == 'v') {
        pointsVector = movePointsFromVector(pointsVector, moveFile, maxSize);
    }
    else if (mode == 'm') {
        pointsMap = movePointsFromMap(pointsMap, moveFile, maxSize);
    }

    if (yesOrNoQuestion("Do you want to print out the new points positions? (yes = Y / no = Anything else)")) {
        if (mode == 'v') {
            printPoints(pointsVector);
        }
        else if (mode == 'm') {
            printPoints(pointsMap);
        }
    }
}

string requestFile(string msg) {
    // loop until the user give a file name that exist
    string fileName;
    bool validFile = false;
    while (validFile == false) {
        cout << msg << endl;
        cin >> fileName; // get the user input and store it in a variable
        cout << endl; // just an empty line to make the console more readable

        validFile = checkFileExist(fileName); //check if the file exist, if not we loop
    }

    return fileName;
}

int requestMode() {
    // ask the user to select a mode and then check if is a valid one
    bool validMode = false;
    char mode;
    while (validMode == false) {

        cout << "In what type of container do you want to store the points?" << endl << "[v] -> vector" << endl << "[m] -> map" << endl;
        cin >> mode;

        // only used if we wanted a number
        //while (!(std::cin >> mode)) { // will loop until the user input is of the same type of the variable that it is supposed to go in
        //    std::cout << "Error! You must type an alphabetical character" << endl;
        //    cout << "In what type of container do you want to store the points?" << endl << "[1] -> vector" << endl << "[2] -> map" << endl;
        //    std::cin.clear(); // clear all errors flags of cin
        //    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //Extract and discard all characters (one by one until the limit set, here "std::numeric_limits<std::streamsize>::max()" set the limit at the max lenght of the stream(string) inputed)
        //}

        cout << endl; // just an empty line to make the console more readable

        if (mode == 'v' || mode == 'm') {
            validMode = true;
        }
        else {
            cout << "Invalid mode selected" << endl;
        }
    }
    return mode;
}

// this function is better if for any reason points need to be added or removed later (comparded to a vector(avoid the need to copy a vector in a larger or smaller one))
// takes in parameter the name and extension of the file inputed by the user and the maxSize of the "working area"
map<int, Point> initPointsInMap(string fileName, int maxSize) {
    map<int, Point> points;
    fstream file; // declaration of the variable that will contain the file that we want to interact with.
    file.open(fileName, ios::in); // open the file in read mode (ios::in)
    if (file.is_open()) { // check if the file is open

        //Loop to read each line of the file and initialize a point with the data obtained from each line
        string line; // declaration of the variable that will contain the line that we obtain.

        while (getline(file, line)) {
            stringstream linestream(line); // convert the string into a stream 
            string sId, sX, sY; // declaring the variables that will get the data as strings

            // getline extract everything until a ';' (or the end of the stream) and store it into a variable
            // the data extracted and the delimiter (';') will be removed from the stream
            getline(linestream, sId, ';');
            getline(linestream, sX, ';');
            getline(linestream, sY); // no delimiter needed since we should be at the end of the stream

            // convert the strings obtained into integer
            int id = atoi(sId.c_str());
            int x = atoi(sX.c_str());
            int y = atoi(sY.c_str());

            Point point; // create a new point object
            point.Point::Point(x, y, id); // construct the object with his values

            points[id] = point; // add the new object to the map with the object id value as key the (in order to later find it more easely rather than to check each objects values)

        }
        file.close();
    }
    else {
        cout << "could not open file" << endl;
    }

    return points;
}

// this function is better if the number pf points do not change. (it's less expensive to search a vector than a map)
// takes in parameter the name and extension of the file inputed by the user and the maxSize of the "working area"
vector<Point> initPointsInVector(string fileName, int maxSize) {
    vector<Point> points; // declaration of the vector that will contain all the points
    fstream file; // declaration of the variable that will contain the file that we want to interact with.
    file.open(fileName, ios::in); // open the file in read mode (ios::in)
    if (file.is_open()) { // check if the file is open

        string line; // declaration of the variable that will contain the line that we obtain.

        // counting the numbers of lines of the file to reserve the final CAPACITY of the vector in memory
        int nbLine = 0;
        while (getline(file, line)) {
            nbLine++;
        }
        file.clear(); // clear the flags of the stream (in this case remove the End-of-File flag)
        file.seekg(0, std::ios::beg); // reposition the "iterator" of the stream at the first character (in this case the start of the file)
        
        points.reserve(nbLine); // change/reserve the capacity of the vector to match the number of lignes of the file

        //Loop to read each line of the file and initialize a point with the data obtained from each line
        while (getline(file, line)) {
            stringstream linestream(line); // convert the string into a stream 
            string sId, sX, sY; // declaring the variables that will get the data as strings
            // getline extract everything until a ';' (or the end of the stream) and store it into a variable
            // the data extracted and the delimiter (';') will be removed from the stream
            getline(linestream, sId, ';');
            getline(linestream, sX, ';');
            getline(linestream, sY); // no delimiter needed since we should be at the end of the stream

            // convert the strings obtained into integer
            int id = atoi(sId.c_str());
            int x = atoi(sX.c_str());
            int y = atoi(sY.c_str());

            Point point; // create a new point object
            point.Point::Point(x, y, id); // construct the object with his values

            points.push_back(point); // add the new object at the end of the vector

        }
        file.close(); // close the file
    }
    else {
        cout << "could not open file" << endl;
    }

    return points;
}

// only called if the mode selected was [m] map
// takes in parameter the name and exention of the file inputed by the user and the maxSize of the "working area"
map<int, Point> movePointsFromMap(map<int, Point> points, string fileName, int maxSize) {
    fstream file; // declaration of the variable that will contain the file that we want to interact with.
    file.open(fileName, ios::in); // open the file in read mode (ios::in)
    if (file.is_open()) { // check if the file is open

        //Loop to read each line of the file and and midify the coordonates a point with the data obtained from each line
        string line; // declaration of the variable that will contain the line that we obtain.
        string pointNotFound = ""; // string that will contain the ids of the points that could not be found if any to print them in the console

        while (getline(file, line)) { // get each line as a string
            stringstream linestream(line); // convert the string into a stream 
            string sId, sX, sY; // declaring the variables that will get the data as strings
            int newX;
            int newY;

            // getline extract everything until a ';' delimiter (or the end of the stream) and store it into a variable
            // the data extracted and the delimiter (';') will be removed from the stream
            getline(linestream, sId, ';');
            getline(linestream, sX, ';');
            getline(linestream, sY); // no delimiter needed since we should be at the end of the stream

            // convert the strings obtained into integer
            // atoi convert a string into an integer
            // it will discards as many whitespace characters as necessary until the first non-whitespace character is found
            // then takes an optional initial plus or minus sign followed by as many base - 10 digits as possible
            // then it will stop at the first non digit character and return the value obtained
            int id = atoi(sId.c_str());
            int x = atoi(sX.c_str());
            int y = atoi(sY.c_str());

            map<int, Point>::iterator itr = points.find(id); // find the object corresponding and set the iterator to it (the key was set to the id of the point at initialisation)
            if (itr != points.end()) // check if we are not at the end of the map
            {
                
                // get the value of x and y (.second is because the object is in the second portion of the map element as the first is the key)
                newX = (*itr).second.Point::X() + x;
                newY = (*itr).second.Point::Y() + y;

                // check if the new values are within the size of set area
                // if over we take the new value and substract the maxSize to obtain how much it go over and then substract that to the maxSize to "bounce of the wall"
                if (newX > maxSize) {
                    newX = maxSize - (newX - maxSize);
                }
                else if (newX < 0) {
                    newX = abs(newX); // here we just need to get the absolute value to remove the negative in order to "bouce of the wall"
                }

                // same but for y
                if (newY > maxSize) {
                    newY = maxSize - (newY - maxSize);
                }
                else if (newY < 0) {
                    newX = abs(newX); // here we just need to get the absolute value to remove the negative in order to "bouce of the wall"
                }

                //for debug
                //cout << "Moving point " << id << " from x:" << (*itr).second.Point::X() << ", y:" << (*itr).second.Point::Y() << " to x:" << newX << ", y:" << newY << endl;

                // assign the new values to the object using the setter function of the object
                (*itr).second.Point::X(newX);
                (*itr).second.Point::X(newY);
            }
            else {
                // if a point is not found add its id to the "pointNotFound" string
                if (pointNotFound == "") {
                    pointNotFound = to_string(id);
                }
                else {
                    pointNotFound += ", " + to_string(id);
                }
            }
        }
        file.close();

        printMoveFinishedMsg(pointNotFound);

        return points;
    }
    else {
        cout << "could not open file";
    }

    return points;
}

vector<Point> movePointsFromVector(vector<Point> points, string fileName, int maxSize) {
    fstream file; // declaration of the variable that will contain the file that we want to interact with.
    file.open(fileName, ios::in); // open the file in read mode (ios::in)
    if (file.is_open()) { // check if the file is open

        //Loop to read each line of the file and and midify the coordonates a point with the data obtained from each line
        string line; // declaration of the variable that will contain the line that we obtain.
        string pointNotFound = ""; // string that will contain the ids of the points that could not be found if any to print them in the console

        while (getline(file, line)) {
            stringstream linestream(line); // convert the string into a stream 
            string sId, sX, sY; // declaring the variables that will get the data as strings

            // getline extract everything until a ';' (or the end of the stream) and store it into a variable
            // the data extracted and the delimiter (';') will be removed from the stream
            getline(linestream, sId, ';');
            getline(linestream, sX, ';');
            getline(linestream, sY); // no delimiter needed since we should be at the end of the stream

            // convert the strings obtained into integer
            // atoi convert a string into an integer
            // it will discards as many whitespace characters as necessary until the first non-whitespace character is found
            // then takes an optional initial plus or minus sign followed by as many base - 10 digits as possible
            // then it will stop at the first non digit character and return the value obtained
            int id = atoi(sId.c_str());
            int x = atoi(sX.c_str());
            int y = atoi(sY.c_str());

            // search in the vector for an object with the corresponding id
            // find_if(search from, to, Lambda expression)
            // Lambda expression (unnamed function object capable of capturing variables in scope) : 
            //   - [id] capture the value of the variable that we need in the expression to find the right object
            //   - (const Point& obj) parameter of the lambda(/function)
            //          i changed the getter functions in the Point class to be const member functions to prevent them to ever change any value of the object
            //          so here i need to use a constant of the object to acces the const member functions
            //   - {return obj.Point::Id() == id; } return true if the id varible captured by the lambda is equal to the value returned by the ID() function of the object
            vector<Point>::iterator itr = find_if(points.begin(), points.end(), [id](const Point& obj) {return obj.Point::Id() == id; });
            if (itr != points.end()) // check if we are not at the end of the vector
            {
                // assign the new values to the object using the setter funtions of the object
                (*itr).Point::X(x);
                (*itr).Point::Y(y);
            }
            else 
            { 
                // if a point is not found add its id to the "pointNotFound" string
                if (pointNotFound == "") {
                    pointNotFound = to_string(id);
                }
                else {
                    pointNotFound += ", " + to_string(id);
                }
            }

        }
        file.close();

        printMoveFinishedMsg(pointNotFound);
        
        return points;
    }
    else {
        cout << "could not open file";
    }

    return points;
}

bool yesOrNoQuestion(string msg) {
    char userInput;

    cout << msg << endl;
    cin >> userInput;
    cout << endl; // just an empty line to make the console more readable
    if (userInput == 'Y' || userInput == 'y') {
        return true;
    }
    return false;
}

void printMoveFinishedMsg(string pointNotFound) {

    cout << "Points positions updated ";
    if (pointNotFound != "") {
        cout << "but the points with the id: " << pointNotFound << " where not found" << endl;
        cout << endl;
    }
    else {
        cout << "with no errors." << endl;
        cout << endl;
    }
}

void printPoints(map<int, Point> points) {
    for (auto itr = points.begin(); itr != points.end(); itr++) {
        cout << "Id : " << (*itr).second.Point::Id() << ", X : " << (*itr).second.Point::X() << ", Y : " << (*itr).second.Point::Y() << endl;
    }
    return;
}

void printPoints(vector<Point> points) {
    for (auto itr = points.begin(); itr != points.end(); itr++) {
        cout << "Id : " << (*itr).Point::Id() << ", X : " << (*itr).Point::X() << ", Y : " << (*itr).Point::Y() << endl;
    }
    return;
}

bool checkFileExist(string fileName) {
    fstream file; // declaration of the variable that will contain the file that we want to interact with.
    file.open(fileName, ios::in); // open the file in read mode (ios::in)
    if (file.is_open()) { // check if the file is opened (mean that the file exist)
        return  true;
    }
    cout << "File not found" << endl;
    return false;
}