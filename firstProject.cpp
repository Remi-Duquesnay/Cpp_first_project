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

void printPoints(map<int, Point>);
void printPoints(vector<Point>);

bool checkFileExist(string);


int main()
{
    //storage modes : 
    // v = Vector
    // m = map

    int maxSize = 8000;

    string initFile;
    string moveFile;
    char mode;
    bool validPrintInput = false;
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

    if (yesOrNoQuestion("Do you want to print out the points? (yes = Y / no = Anything else)")) {
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

    if (yesOrNoQuestion("Do you want to print out the new points? (yes = Y / no = Anything else)")) {
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
        
        while (!(std::cin >> mode)) { // will loop until the user input is of the same type of the variable that it is supposed to go in
            std::cout << "Error! You must type a number" << endl;
            cout << "In what type of container do you want to store the points?" << endl << "[v] -> vector" << endl << "[m] -> map" << endl;
            std::cin.clear(); // clear all errors flags of cin
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //Extract and discard all characters (one by one until the limit set, here "std::numeric_limits<std::streamsize>::max()" set the limit at the max lenght of the stream(string) inputed)
        }

        if (mode == 'v' || mode == 'm') {
            validMode = true;
        }
        else {
            cout << "Invalid mode selected" << endl;
        }
    }
    return mode;
}

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

            Point point;
            point.Point::Point(x, y, id);

            points[id] = point;

        }
        file.close();
    }
    else {
        cout << "could not open file" << endl;
    }

    return points;
}

vector<Point> initPointsInVector(string fileName, int maxSize) {
    vector<Point> points; // declaration of the vector that will contain all the points
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

            Point point;
            point.Point::Point(x, y, id);

            points.push_back(point);

        }
        file.close();
    }
    else {
        cout << "could not open file" << endl;
    }

    return points;
}

map<int, Point> movePointsFromMap(map<int, Point> points, string fileName, int maxSize) {
    fstream file; // declaration of the variable that will contain the file that we want to interact with.
    file.open(fileName, ios::in); // open the file in read mode (ios::in)
    if (file.is_open()) { // check if the file is open

        //Loop to read each line of the file and and midify the coordonates a point with the data obtained from each line
        string line; // declaration of the variable that will contain the line that we obtain.

        while (getline(file, line)) {
            stringstream linestream(line); // convert the string into a stream 
            string sId, sX, sY; // declaring the variables that will get the data as strings
            int newX;
            int newY;

            // getline extract everything until a ';' (or the end of the stream) and store it into a variable
            // the data extracted and the delimiter (';') will be removed from the stream
            getline(linestream, sId, ';');
            getline(linestream, sX, ';');
            getline(linestream, sY); // no delimiter needed since we should be at the end of the stream

            // convert the strings obtained into integer
            int id = atoi(sId.c_str());
            int x = atoi(sX.c_str());
            int y = atoi(sY.c_str());

            map<int, Point>::iterator itr = points.find(id);
            if (itr != points.end())
            {
                
                newX = (*itr).second.Point::X() + x;
                newY = (*itr).second.Point::Y() + y;
                if (newX > maxSize) {
                    newX = maxSize - (newX - maxSize);
                }
                if (newY > maxSize) {
                    newY = maxSize - (newY - maxSize);
                }

                //cout << "Moving point " << id << " from x:" << (*itr).second.Point::X() << ", y:" << (*itr).second.Point::Y() << " to x:" << newX << ", y:" << newY << endl;
                (*itr).second.Point::X(newX);
                (*itr).second.Point::X(newY);
            }
            else {
                cout << "point not found" << endl;
            }

        }
        file.close();
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

            vector<Point>::iterator itr = find_if(points.begin(), points.end(), [id](const Point& obj) {return obj.Point::Id() == id; });
            if (itr != points.end())
            {
                (*itr).Point::X(x);
                (*itr).Point::Y(y);
            }
            else 
            { 
                cout << "point not found" << endl;
            }

        }
        file.close();
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
    if (userInput == 'Y' || userInput == 'y') {
        return true;
    }
    return false;
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