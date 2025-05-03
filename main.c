#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int sudoku[9][9];
int valid[11];

void *check_rowcol(void *arg) {
    int is_row = *(int *)arg;
    for (int i = 0; i < 9; i++) {
        int nums[10] = {0};
        for (int j = 0; j < 9; j++) {
            int val = is_row ? sudoku[i][j] : sudoku[j][i];
            if (val < 1 || val > 9 || nums[val]) return NULL;
            nums[val] = 1;
        }
    }
    valid[is_row] = 1;
    return NULL;
}

void *check_box(void *arg) {
    int index = *(int *)arg;
    int row = (index / 3) * 3;
    int col = (index % 3) * 3;
    int nums[10] = {0};
    for (int i = row; i < row + 3; i++) {
        for (int j = col; j < col + 3; j++) {
            int val = sudoku[i][j];
            if (val < 1 || val > 9 || nums[val]) return NULL;
            nums[val] = 1;
        }
    }
    valid[2 + index] = 1;
    return NULL;
}

void read_sudoku() {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            scanf("%d", &sudoku[i][j]);
}

int main() {
    
    pthread_t threads[11];
    int indexes[9];
    int row_flag = 1;
    int col_flag = 0;
    
    read_sudoku();
    
    /// Thread creation
    pthread_create(&threads[0], NULL, check_rowcol, &row_flag);// it uses void *(*start_routine)(void *)
    pthread_create(&threads[1], NULL, check_rowcol, &col_flag);//thread address,attribute,func,func arg
    for (int i = 0; i < 9; i++) {
        indexes[i] = i;
        pthread_create(&threads[2 + i], NULL, check_box, &indexes[i]);
    }
    ////
    /// Join
    for (int i = 0; i < 11; i++)
        pthread_join(threads[i], NULL);
    ////
    ////outout
    for (int i = 0; i < 11; i++)
        if (!valid[i]) {
            printf("invalid\n");
            return 0;
        }

    printf("valid\n");
    return 0;
}
