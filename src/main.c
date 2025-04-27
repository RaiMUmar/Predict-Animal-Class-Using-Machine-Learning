#include "../headers/givenA1.h"

int main(int argc, char *argv[]){
    // Initialize Variables
    int status, hammingDistance, value = 0, vector1[16] = {1,1,1,0,1,0,1,1,1,0,0,0,0,1,1,1}, vector2[16] = {1,0,0,1,0,0,1,1,1,1,0,0,4,0,0,1};
    int kNearestNeighbors [NUM_SAMPLES], newSample[16] = {1,1,1,0,1,0,1,1,1,0,0,0,0,1,1,1}, read = 0;
    float euclideanDistance, jaccardSimilarity, percentage;
    struct Animal dataZoo [NUM_SAMPLES], testData [NUM_TEST_DATA];

    // Do loop to repeat all options, exit when a correct option is not inputted
    do { 
        printf("Here is the menu - enter a number between 1 and 5\n\n");
        printf("Enter your choice: ");
        scanf("%d", &value);

        if (value == 1){ // Read file given through argv[1]
            status = readFromFile (argv[1], dataZoo);
            if (status == -1) printf("Failed Reading File\n");
            // Print read data
            for (int i = 0; i < NUM_SAMPLES; i++){
                printf("%s", dataZoo[i].animalName);
                for (int j = 0; j < 16; j++){
                    printf(" %d", dataZoo[i].features[j]);
                }
            printf(" %d\n", dataZoo[i].classLabel);
            }
            printf("\n");
            read = 1; // File has been read
        } 
        if (value == 2){ // Finds distance between two vectors and prints them out
            distanceFunctions (vector1, vector2, &euclideanDistance, &hammingDistance, &jaccardSimilarity);
            printf("Euclidean Distance: %f\n", euclideanDistance);
            printf("Hamming Distance: %d\n", hammingDistance);
            printf("Jaccard Similarity: %f\n", jaccardSimilarity);
            printf("\n");
        }
        if (value == 3){ // Find the k nearest neighbours of a given new sample and prints them out
            if (!read){ // File has not been read yet, function can't work properly
                printf("Error: File Not Read!\n");
                return 0;
            }
            for (int i = 1; i < 4; i++){
                findKNearestNeighbors (dataZoo , newSample, 5, i, kNearestNeighbors);
                printf("Neighbors: ");
                for (int j = 0; j < 5; j++){
                    printf("%d ", kNearestNeighbors[j]);
                }
                printf("\n");
            }
            printf("\n");
        }
        if (value == 4){ // Predict class of a new sample using the 3 distances and prints it out
            if (!read){ // File has not been read yet, function can't work properly
                printf("Error: File Not Read!\n");
                return 0;
            }
            for (int i = 1; i < 4; i++){
                int class = predictClass (dataZoo, newSample, i, 5);
                printf("\nThe predicted class is: %d\n", class);
            }
            printf("\n");
        }
        if (value == 5){ // Finds the accuracy of predicted classes for test data and prints it out
            if (!read){ // File has not been read yet, function can't work properly
                printf("Error: File Not Read!\n");
                return 0;
            }
            for (int i = 1; i < 4; i++){
                percentage = findAccuracy (dataZoo, i, testData, 5);
                printf("The accuracy for the test data is %f\n", percentage);
            }
            printf("\n");
        }

    } while (value == 1 || value == 2 || value == 3 || value == 4 || value == 5);
    return 0;
}