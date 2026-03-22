#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

int grid[9][9];     // sudoku grid shared by all threads
int result[27];     // stores if each thread's check passed (1) or failed (0)
int valid = 1;      // overall validity flag

// thread function - each thread checks one subgrid, row, or column
void *validation(void *arg){
    int index = (int)(long)arg;
    int isValid = 1;
    int seen[10] = {0}; // tracks which digits 1-9 have been seen

    if(index < 9){ // threads 0-8 check subgrids
        // figure out where this subgrid starts
        int startRow =  (index/3) * 3;
        int startCol = (index%3) * 3;
        
        for (int i = startRow; i < startRow + 3; i++){
            for (int j = startCol; j < startCol + 3; j++){
                int val = grid[i][j];

                if(seen[val] == 1){ // duplicate found
                    isValid = 0;
                    break;
                }
                seen[val] = 1;
            }
        }
    } 
    else if(index < 18){ // threads 9-17 check rows
        int rowNum = index - 9;

        for (int i = 0; i < 9; i++){
            int val = grid[rowNum][i];

            if(seen[val] == 1){
                isValid = 0;
                break;
            }
            seen[val] = 1;
        }
    }
    else{ // threads 18-26 check columns
        int colNum = index - 18;

        for (int j = 0; j < 9; j++){
            int val = grid[j][colNum];

            if(seen[val] == 1){
                isValid = 0;
                break;
            }
            seen[val] = 1;
        }
    }

    result[index] = isValid; // store result so main thread can read it later

    pthread_exit(NULL);
}



int main(int argc, char *argv[]){
    // make sure user provided a filename
    if(argc != 2){
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];  
    FILE *fptr;

    fptr = fopen(filename, "r");
    if(fptr == NULL){
        printf("Unable to open the file: %s\n", filename);
        return 1;
    }

    // read the 9x9 grid from file
    char line[50];
    for(int i = 0; i < 9; i++){
        fgets(line, 50, fptr);

        char *numbers = strtok(line, " \t\n");
        for(int j = 0; j < 9; j++){
            int val = atoi(numbers);
            grid[i][j] = val;
            numbers = strtok(NULL, " \t\n");
        }

    }
    fclose(fptr);

    // create 27 threads - 9 for subgrids, 9 for rows, 9 for columns
    pthread_t threads[27];

    for (int i = 0; i < 27; i++) {
        pthread_create(&threads[i], NULL, validation, (void*)(long)i);
    }

    // wait for all threads to finish
    for (int i = 0; i < 27; i++) {
        pthread_join(threads[i], NULL);
    }      

    // print results from each thread
    for(int i = 0; i < 27; i++){
        if(i < 9){
            if(result[i] == 1){
                printf("Thread # %d (subgrid %d) is valid\n", i+1, i+1);
            }
            else{
                printf("Thread # %d (subgrid %d) is INVALID\n", i+1, i+1);
                valid = 0;
            }
        }
        else if(i < 18){
            if(result[i] == 1){
                printf("Thread # %d (row %d) is valid\n",i+1, (i-9)+1);
            }
            else{
                printf("Thread # %d (row %d) is INVALID\n", i+1, (i-9)+1);
                valid = 0;
            }
        }
        else{
            if(result[i] == 1){
                printf("Thread # %d (column %d) is valid\n", i+1, (i-18)+1);
            }
            else{
                printf("Thread # %d (column %d) is INVALID\n", i+1, (i-18)+1);
                valid = 0;
            }
        }
        
    }

    // final answer
    if(valid == 1){
        printf("%s contains a valid solution\n", filename);
    }
    else{
        printf("%s contains an INVALID solution\n", filename);
    }
    

    return 0;
}