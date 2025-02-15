#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>

const int ARRAY_SIZE = 20;

// This is to generate a random number between min and max
int generateRandomNumber(int min, int max) {
    return min + (rand() % (max - min + 1));
}

// This is to find the minimum number in a range that's specified of the array
int findMinimum(const int arr[], int start, int end) {
    int minVal = arr[start];
    for (int i = start + 1; i < end; ++i) {
        if (arr[i] < minVal) {
            minVal = arr[i];
        }
    }
    return minVal;
}

int main() {
    int arr[ARRAY_SIZE];
    srand(time(0)); // This is to initalize the random number generator

    // This is to load the array with random numbers
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        arr[i] = generateRandomNumber(1, 100); // Random numbers between 1 and 100
    }

    // This is to show the array
    std::cout << "Array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    // This is to make a child process
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "The fork failed" << std::endl;
        return 1;
    } else if (pid == 0) {
        // This is to find the minimum in the second half of the array for child process
        int minChild = findMinimum(arr, ARRAY_SIZE / 2, ARRAY_SIZE);
        std::cout << "Child Process (Processor ID: " << getpid() << ") found minimum in the second half: " << minChild << std::endl;
        exit(minChild); // Return the minimum value as the exit status
    } else {
        // This is to find the minimum in the first half of the array for parent process
        int minParent = findMinimum(arr, 0, ARRAY_SIZE / 2);
        std::cout << "Parent Process (Processor ID: " << getpid() << ") found minimum in the first half: " << minParent << std::endl;

        // This is to wait for the child process to finish and get its exit status
        int childStatus;
        wait(&childStatus);
        int minChild = WEXITSTATUS(childStatus);

        // This to decide the overall minimum
        int overallMin = (minParent < minChild) ? minParent : minChild;
        std::cout << "Overall minimum number in the array: " << overallMin << std::endl;
    }

    return 0;
}
