#ifndef HELPER_H
#define HELPER_H

#include "givenA1.h"

#define NUM_FEATURES 16   // not including animal name and class label
#define NUM_SAMPLES 100      // total number of samples / rows in the given zoo dataset
#define NUM_TEST_DATA 20     // total number of samples / rows in the test dataset - you need this for task 9

void findDistance(struct Animal dataZoo [NUM_SAMPLES], int newSample [NUM_FEATURES], int whichDistanceFunction, int tempNeighbors[NUM_SAMPLES], float tempNeighborsFl[NUM_SAMPLES], int indices[NUM_SAMPLES]);
void sort(int whichDistanceFunction, int tempNeighbors[NUM_SAMPLES], float tempNeighborsFl[NUM_SAMPLES], int indices[NUM_SAMPLES]);
int readCSV (char fName [], struct Animal testData [NUM_TEST_DATA]);

#endif // HELPER_H