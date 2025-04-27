#include "../headers/givenA1.h"
#include "../headers/helper.h"

/* Function which reads a data file and stories its info in a struct array called dataZoo */
int readFromFile (char fName [30], struct Animal dataZoo [NUM_SAMPLES]){
    // Initialize Variables
    int line = 0;

    // Start Reading File
    FILE *fptr = fopen(fName, "r");
    if (fptr == NULL) return -1; // Could not open or find file

    // Read file and store the animal names, features, and class label. 
    while (!feof(fptr)){
        fscanf(fptr, "%s", dataZoo[line].animalName);
        for (int i = 0; i < 16; i++){
            fscanf(fptr, "%d", &dataZoo[line].features[i]);
        }
        fscanf(fptr, "%d", &dataZoo[line].classLabel);
        line++;
    }
    fclose(fptr); // Close file
    return 1; // Read Succesfully 
}

/* Function which compares two vectors, and finds their eucilidean distance, hamming distance, and jaccard similarity and returns them through 
call by reference */
void distanceFunctions (int vector1 [NUM_FEATURES], int vector2 [NUM_FEATURES], float * euclideanDistance, int * hammingDistance, float * jaccardSimilarity){
    // Initialize Variables
    float sum = 0, oneMatch = 0, zeroMatch = 0;
    int count = 0;

    for (int i = 0; i < NUM_FEATURES; i++){
        sum = sum + ((vector1[i] - vector2[i]) * (vector1[i] - vector2[i])); // Calculate sum of euclidean distance before find it's square root
        if (vector1[i] != vector2[i]) count++; // Find number of features which do not match
        if (vector1[i] == 1 && vector2[i] == 1) oneMatch++; // Find one to one matches
        if (vector1[i] == 0 && vector2[i] == 0) zeroMatch++; // Find zero to zero matches
    }

    // Save values to the correct variables and return through call by reference
    *euclideanDistance = sqrtf(sum);
    *hammingDistance = count;
    *jaccardSimilarity = oneMatch/(NUM_FEATURES - zeroMatch);
}

/* Function to find distance between a new sample and all samples in dataZoo, then sort those distances and their indecies together.
Afterwards save the indicies of the first k sorted distances in kNearestNeighbors */
void findKNearestNeighbors (struct Animal dataZoo [NUM_SAMPLES], int newSample [NUM_FEATURES], int k, int whichDistanceFunction, int kNearestNeighbors [NUM_SAMPLES]){
    // Initialize Variables
    int tempNeighbors[NUM_SAMPLES], indices[NUM_SAMPLES];
    float tempNeighborsFl[NUM_SAMPLES];

    if (whichDistanceFunction != 1 && whichDistanceFunction != 2 && whichDistanceFunction != 3) return; // Check for valid case

    findDistance(dataZoo, newSample, whichDistanceFunction, tempNeighbors, tempNeighborsFl, indices); // Find distance function
    sort(whichDistanceFunction, tempNeighbors, tempNeighborsFl, indices); // Sort the distances

    // Save first k distances indicy values in kNearestNeighbors
    for (int i = 0; i < k; i++){
        kNearestNeighbors[i] = indices[i];
    }
}

/* Function finds the first k closest neighbors, and predicts the class of newSample using those neighbor's classes. 
The most repeated class, is predicted */
int predictClass (struct Animal dataZoo [NUM_SAMPLES], int newSample [NUM_FEATURES], int whichDistanceFunction, int k){
    // Initialize Variables
    int kNearestNeighbors [NUM_SAMPLES], classes[NUM_SAMPLES], temp, amounts[NUM_SAMPLES], bestIndex;

    findKNearestNeighbors (dataZoo, newSample, k, whichDistanceFunction, kNearestNeighbors); // Function to find the k closest neighbors

    for (int i = 0; i < k; i++){
        classes[i] = dataZoo[kNearestNeighbors[i]].classLabel; // Stores the classes of those neighbors into variable classes
    }
    // Sort the classes array from smallest to largest values
    for (int i = 0; i < k - 1; i++) {
        bestIndex = i;
        for (int j = i + 1; j < k; j++) {
            if (classes[j] < classes[bestIndex]) {
                bestIndex = j;
            }
        }
        if (bestIndex != i) {
            // Swap the classes
            temp = classes[i];
            classes[i] = classes[bestIndex];
            classes[bestIndex] = temp;
        }
    }

    // Find how many times each class was repeated and save it in amounts
    for (int i = 0; i < k; i++){
        amounts[i] = 0;
        for (int j = 0; j < k; j++){
            if (classes[i] == classes[j]){
                amounts[i]++;
            }
        }
    }

    // Using the values in amount resort the classes array to put the most repeated class at classes[0]
    for (int i = 0; i < k - 1; i++) {
        bestIndex = i;
        for (int j = i + 1; j < k; j++) {
            if (amounts[j] > amounts[bestIndex]) {
                bestIndex = j;
            }
        }
        if (bestIndex != i) {
            // Swap the amount values.
            temp = amounts[i];
            amounts[i] = amounts[bestIndex];
            amounts[bestIndex] = temp;
            // Swap the corresponding classes.
            temp = classes[i];
            classes[i] = classes[bestIndex];
            classes[bestIndex] = temp;
        }
    }

    return classes[0]; // Return the class which was the most repeated and smallest
}

/* Function which reads a csv file containing testData and predicts their class, afterwards checking the test data to find accuracies
and finding the accuracy of our predicted classes */
float findAccuracy (struct Animal dataZoo [NUM_SAMPLES], int whichDistanceFunction, struct Animal testData [NUM_TEST_DATA], int k){
    // Initialize Variables
    float count = 0; 
    int result, status;

    status = readCSV ("testData.csv", testData); // Read csv file containing test data
    if (status == -1) return -1; // Check if it's read

    // Predict class of test data and check if it's accurate 
    for (int i = 0; i < NUM_TEST_DATA; i++){
        result = predictClass (dataZoo, testData[i].features, whichDistanceFunction, k);
        if (result == testData[i].classLabel) count++;
    }

    return (count/(1.0 * NUM_TEST_DATA)); // Return percentage of correct perdictions
}

/* Go through features in dataZoo and a newSample, find euclidean distance, hamming distance, and jaccard similarities of
the samples. Store the distances into tempNeighbors array. Also store 0 to NUM_SAMPLES - 1 values into indices array. 
Return all values through call by reference. */
void findDistance(struct Animal dataZoo [NUM_SAMPLES], int newSample [NUM_FEATURES], int whichDistanceFunction, int tempNeighbors[NUM_SAMPLES], float tempNeighborsFl[NUM_SAMPLES], int indices[NUM_SAMPLES]){
    // Initialize Variables
    int hammingDistance = 0;
    float euclideanDistance = 0, jaccardSimilarity = 0;

    for (int i = 0; i < NUM_SAMPLES; i++){
        distanceFunctions(dataZoo[i].features, newSample, &euclideanDistance, &hammingDistance, &jaccardSimilarity); // Find Distance
        // Store distance into array
        if (whichDistanceFunction == 1){
            tempNeighborsFl[i] = euclideanDistance;
        }
        if (whichDistanceFunction == 2){
            tempNeighbors[i] = hammingDistance;
        }
        if (whichDistanceFunction == 3){
            tempNeighborsFl[i] = jaccardSimilarity;
        }
        indices[i] = i; // Store indicy value
    }

}

/* Sort function made by Chat GPT, edited by Rai Umar. Takes in an array of values and an array of indices, 
sorts out the array of values and moves the indicies to correspond to the position of it's value */
void sort(int whichDistanceFunction, int tempNeighbors[NUM_SAMPLES], float tempNeighborsFl[NUM_SAMPLES], int indices[NUM_SAMPLES]) {
    // Initialize Variables
    int i, j, bestIndex, tempIndex;
    float tempFloat;
    int tempInt;

    if (whichDistanceFunction == 1) {
        // Euclidean: sort tempNeighborsFl (floats) in ascending order.
        for (i = 0; i < NUM_SAMPLES - 1; i++) {
            bestIndex = i;
            for (j = i + 1; j < NUM_SAMPLES; j++) {
                if (tempNeighborsFl[j] < tempNeighborsFl[bestIndex]) {
                    bestIndex = j;
                }
            }
            if (bestIndex != i) {
                // Swap the float values.
                tempFloat = tempNeighborsFl[i];
                tempNeighborsFl[i] = tempNeighborsFl[bestIndex];
                tempNeighborsFl[bestIndex] = tempFloat;
                // Swap the corresponding indices.
                tempIndex = indices[i];
                indices[i] = indices[bestIndex];
                indices[bestIndex] = tempIndex;
            }
        }
    }
    else if (whichDistanceFunction == 2) {
        // Hamming: sort tempNeighbors (ints) in ascending order.
        for (i = 0; i < NUM_SAMPLES - 1; i++) {
            bestIndex = i;
            for (j = i + 1; j < NUM_SAMPLES; j++) {
                if (tempNeighbors[j] < tempNeighbors[bestIndex]) {
                    bestIndex = j;
                }
            }
            if (bestIndex != i) {
                // Swap the integer values.
                tempInt = tempNeighbors[i];
                tempNeighbors[i] = tempNeighbors[bestIndex];
                tempNeighbors[bestIndex] = tempInt;
                // Swap the corresponding indices.
                tempIndex = indices[i];
                indices[i] = indices[bestIndex];
                indices[bestIndex] = tempIndex;
            }
        }
    }
    else if (whichDistanceFunction == 3) {
        // Jaccard similarity: sort tempNeighborsFl (floats) in descending order.
        for (i = 0; i < NUM_SAMPLES - 1; i++) {
            bestIndex = i;
            for (j = i + 1; j < NUM_SAMPLES; j++) {
                if (tempNeighborsFl[j] > tempNeighborsFl[bestIndex]) {
                    bestIndex = j;
                }
            }
            if (bestIndex != i) {
                // Swap the float values.
                tempFloat = tempNeighborsFl[i];
                tempNeighborsFl[i] = tempNeighborsFl[bestIndex];
                tempNeighborsFl[bestIndex] = tempFloat;
                // Swap the corresponding indices.
                tempIndex = indices[i];
                indices[i] = indices[bestIndex];
                indices[bestIndex] = tempIndex;
            }
        }
    }
}

// Function which reads a csv file containing test data
int readCSV (char fName [], struct Animal testData [NUM_TEST_DATA]){
    FILE *fptr = fopen(fName, "r"); // Open file to read
    if (fptr == NULL) return -1; // File not read
    // Read line by line and store data into corresponding variables 
    for (int i = 0; i < NUM_TEST_DATA; i++) {
        fscanf(fptr, "%49[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                   testData[i].animalName,
                   &testData[i].features[0], &testData[i].features[1], &testData[i].features[2], &testData[i].features[3],
                   &testData[i].features[4], &testData[i].features[5], &testData[i].features[6], &testData[i].features[7],
                   &testData[i].features[8], &testData[i].features[9], &testData[i].features[10], &testData[i].features[11],
                   &testData[i].features[12], &testData[i].features[13], &testData[i].features[14], &testData[i].features[15],
                   &testData[i].classLabel); 
        }
    fclose(fptr); // Close file
    return 1; // File read correctly
}