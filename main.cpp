/*
        381-24 Proj 2
        Name: Suatcan Isik
        Date: 21/02/2022
        Prof: Tsaiyun Phillips

        Description:    Given a bimodal histogram, and the two points on the two
peaks of the bimodal histogram, you are to implement the deepest concavity
method for automatic threshold selection (taught in class). AN EASY PROJ BTW

given; highest points of two curves+ array of nums ,
return; deepest concavity as a num from that array.

*/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

class Concavity {

public:
    int numRows, numCols, minVal, maxVal;
    int x1, y1, x2, y2; //points of the two peaks
    double m; //slop of the line equation, y = m*x + b
    double b; // the y-interception of the line equation, y = m*x + b.
    int* histAry; // a 1D integer array (size of maxVal + 1) to store the histogram.
    // It needs to be dynamically allocated at run time; initialize to zero.
    int maxHeight = 0; // The max of histAry[i], i = minVal to maxVal
    int bestThrVal; // the auto selected threshold value by the method.
    int** displayGraph; // a 2-D int array size of maxVal+1 by maxHeight+1, initialize to 0,
    int c = 0;
    // It needs to be dynamically allocated at run time. Within displayGraph [] []: 1 for histogram points, 
    //2 for line points and 3 for gaps points, 0 for nothing

    Concavity(string hist, string pts) {
        string line;
        ifstream inFile1(hist);
        if (inFile1.is_open()) { //read first file
            int count = 0;
            while (getline(inFile1, line)) {
                if (line.length() > 10) {
                    int s1 = line.find("\t", 0);        //s1,s2 & s3: locations of "\t" in txt file
                    int s2 = line.find("\t", line.find("\t", s1 + 1));
                    int s3 = line.find("\t", line.find("\t", s2 + 1));
                    this->numRows = stoi(line.substr(0, s1));
                    this->numCols = stoi(line.substr(s1, s2));
                    this->minVal = stoi(line.substr(s2, s3 - 1));
                    this->maxVal = stoi(line.substr(s3, 20));
                    histAry = new int[1 + maxVal];
                    for (int i = 0; i < maxVal; i++) {
                        histAry[i] = 0;
                    }
                }
                else {
                    int s1 = line.find("\t", 0);
                    histAry[count++] = stoi(line.substr(s1, 5));

                }
            }
        }
        inFile1.close();
        ifstream inFile2(pts);  //read second file
        if (inFile2.is_open()) {
            while (getline(inFile2, line)) {
                int s1 = line.find("\t", 0);//s1,s2 & s3: locations of "\t" in txt file
                int s2 = line.find("\t", line.find("\t", s1 + 1));
                int s3 = line.find("\t", line.find("\t", s2 + 1));
                this->x1 = stoi(line.substr(0, s1));
                this->y1 = stoi(line.substr(s1, s2));
                this->x2 = stoi(line.substr(s2, s3 - 1));
                this->y2 = stoi(line.substr(s3, 20));
            }
        }
        inFile2.close();
        m = (double)(y2 - y1) / double(x2 - x1);
        b = (double)y1 - (m * (double)x1);
        this->maxHeight = loadHist();
        bestThrVal = deepestConcavity();

        //initilize the double array to 0s
        displayGraph = new int* [maxVal + 1];
        for (int i = 0; i < maxVal + 1; i++) {
            displayGraph[i] = new int[maxHeight + 1];
        }
        for (int i = 0; i < maxVal + 1; i++) {
            for (int j = 0; j < maxHeight + 1; j++) {
                displayGraph[i][j] = 0;
            }
        }
        //maxVal+1 by maxHeight+1
    } // It dynamically allocates all member arrays and initialization.
    void plotOneRow() {
        //"+" line
        for (int i = x1 + 1; i < x2; i++) {
            int y = i * m + b;
            displayGraph[i][y] = 2;
        }
        //"=" gaps
        for (int i = x1 + 1; i < x2; i++) {
            int y = i * m + b;
            for (int j = histAry[i]; j < y; j++) {
                displayGraph[i][j] = 3;
            }

        }
        //"*" array
        for (int i = 0; i < maxVal + 1; i++) {
            if (histAry[i] != 0) {
                for (int j = 0; j < histAry[i]; j++) {
                    displayGraph[i][j] = 1;
                }
            }
        }
    } // Assign 1 to points of histAry[x], 2 to line points and 3 to gap points
    void printGraph() {
        for (int i = 0; i < maxVal + 1; i++) {
            cout << i << " - ";
            for (int j = 0; j < maxHeight + 1; j++) {
                if (displayGraph[i][j] == 0) { cout << " "; }
                if (displayGraph[i][j] == 1) { cout << "*"; }
                if (displayGraph[i][j] == 2) { cout << "+"; }
                if (displayGraph[i][j] == 3) { cout << "="; }
            }
            cout << endl;
        }

    }
    int loadHist() {
        int maxH = 0;
        for (int i = 0; i < maxVal; i++) {
            if (maxH <= histAry[i]) {
                maxH = histAry[i];
            }
        }
        return maxH;
    } // reads and loads the histAry from inFile and returns the max hist[i]. // On your own
    int deepestConcavity() {
        int maxGap = 0;
        int xVal;           //x cords for the biggest gap
        int gap;
        for (int i = x1; i < x2 + 1; i++) {
            gap = (m * histAry[i] + b) - histAry[i];
            if (gap >= maxGap) { maxGap = gap; xVal = i; }
        }
        return xVal;
    }  // this is the principal method that determines the best threshold selection.
    void printVal() { //print data values
        cout << "Row :" << numRows << " Colm :" << numCols << endl << "Min :" << minVal << " Max :" << maxVal << endl;
        cout << "Max Height" << " : " << maxHeight << endl;
        cout << "Max point cords : First : (" << x1 << "," << y1 << "), Second : (" << x2 << "," << y2 << ")" << endl;
        cout << "m : " << abs(m) << " b : " << b << endl;
        cout << "Best Thrashold Value:  " << bestThrVal << endl;
    }
    void histPrint() {
        for (int i = 0; i < maxVal; i++) {
            cout << i << " - " << histAry[i] << endl;
        }
    }
};
int main() {
    Concavity graph1("data1_hist.txt", "data1_pts.txt");
    graph1.printVal();
    graph1.histPrint();
    graph1.plotOneRow();
    graph1.printGraph();

    /*
    Concavity graph2("data2_hist.txt","data2_pts.txt");
    graph2.printVal();
    graph2.plotOneRow ();
    graph2.printGraph();
    */
}
