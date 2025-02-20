#include <windows.h>
#include <stdio.h>

#define NUM_THREADS 4

int dataArray[100];
int resultArray[100];

DWORD WINAPI ProcessData(LPVOID lpParam) {
    int threadID = *(int*)lpParam;
    int start = threadID * (100 / NUM_THREADS);
    int end = (threadID + 1) * (100 / NUM_THREADS);

    for (int i = start; i < end; i++) {
        resultArray[i] = dataArray[i] * dataArray[i];
    }

    printf("Thread %d processed elements %d to %d\n", threadID, start, end - 1);
    return 0;
}

int main() {
    for (int i = 0; i < 100; i++) {
        dataArray[i] = i + 1;
    }

    HANDLE threads[NUM_THREADS];
    int threadIDs[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        threadIDs[i] = i;
        threads[i] = CreateThread(
            NULL,                   
            0,                      
            ProcessData,            
            &threadIDs[i],          
            0,                      
            NULL                    
        );

        if (threads[i] == NULL) {
            printf("Failed to create thread %d\n", i);
            return 1;
        }
    }

    WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);

    for (int i = 0; i < NUM_THREADS; i++) {
        CloseHandle(threads[i]);
    }

    printf("Results:\n");
    for (int i = 0; i < 100; i++) {
        printf("%d ", resultArray[i]);
    }
    printf("\n");

    return 0;
}