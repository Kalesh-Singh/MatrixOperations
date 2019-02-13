#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 4

int matA[MAX][MAX];
int matB[MAX][MAX];

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX];
int matProductResult[MAX][MAX];

void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
void* computeSum(void* args) {
    int row = *((int*) args);
    int col = *(((int*) args)+1);
    matSumResult[row][col] = matA[row][col] + matB[row][col];
    free(args);
    return NULL;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) {
    int row = *((int*) args);
    int col = *(((int*) args)+1);
    matDiffResult[row][col] = matA[row][col] - matB[row][col];
    free(args);
    return NULL;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) {
    int row = *((int*) args);
    int col = *(((int*) args)+1);

    int sum = 0;
    for (int i = 0, j = 0; i < MAX && j < MAX;) {
        sum += matA[row][i] * matB[j][col];
        i++;
        j++;
    }

    matProductResult[row][col] = sum;

    return NULL;
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() {
    srand(time(0));  // Do Not Remove. Just ignore and continue below.

    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);

    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);

    // 3. Create pthread_t objects for our threads.
    pthread_t sumThreads[MAX * MAX];
    for (int row = 0; row < MAX; row++) {
        for (int col = 0; col < MAX; col++) {
            int *args = (int*) malloc(2 * sizeof(int));
            *args = row;
            *(args+1) = col;
            pthread_create(&sumThreads[row * MAX + col], NULL, computeSum, (void*) args);

        }
    }

    pthread_t diffThreads[MAX * MAX];
    for (int row = 0; row < MAX; row++) {
        for (int col = 0; col < MAX; col++) {
            int *args = (int*) malloc(2 * sizeof(int));
            *args = row;
            *(args+1) = col;
            pthread_create(&diffThreads[row * MAX + col], NULL, computeDiff, (void*) args);
        }
    }

    pthread_t prodThreads[MAX * MAX];
    for (int row = 0; row < MAX; row++) {
        for (int col = 0; col < MAX; col++) {
            int *args = (int*) malloc(2 * sizeof(int));
            *args = row;
            *(args + 1) = col;
            pthread_create(&prodThreads[row * MAX + col], NULL, computeProduct, (void*) args);
        }
    }
    // 4. Create a thread for each cell of each matrix operation.
    //
    // You'll need to pass in the coordinates of the cell you want the thread
    // to compute. One way to do this is to "encode" the coordinates into a number
    // and then pass it into the thread function. For example,
    //
    // long coordinates = MAX*row + col;
    //
    // Then the thread will have to "decode" that number back into the row and col.
    //
    // Another option is to malloc memory for the coordinates, populate the coordinates
    // into that space, and pass that address to the thread. The thread will then have
    // to free that space when it's done with what's in that memory.

    // 5. Wait for all threads to finish.
    for (int i = 0; i < MAX * MAX; ++i) {
        pthread_join(sumThreads[i], NULL);
        pthread_join(diffThreads[i], NULL);
        pthread_join(prodThreads[i], NULL);
    }

    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;
}