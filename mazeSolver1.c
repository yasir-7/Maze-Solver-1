#include <stdio.h>
#include <stdlib.h>
//Creating Deque
struct deque{
    int size;
    int f;
    int b;
    int * arr;
};
//checks is deque is full
int isFull(struct deque * q){
    if((q->b + 1)%(q->size) == q->f){
        return 1;
    }
    
    return 0;
}
//checks is deque is empty
int isEmpty(struct deque * q){
    if(q->b == q->f){
        return 1;
    }
    
    return 0;
}
//Enqueues value to the start
void add_first(struct deque * q, int value){
    if(isFull(q)){
        printf("Overflow\n");
    }
    else{
        q->arr[q->f] = value;        
        q->f -- ;
        if(q->f < 0){
            q->f = q->size + q->f ;
        }
    }
}
//Enqueues value to end
void add_last(struct deque * q, int value){
    if(isFull(q)){
        printf("Overflow\n");
    }
    else{
        q->b =(q->b +1)%(q->size) ;
        q->arr[q->b] = value;
    }
}
//Dequeues element from starting
void remove_first(struct deque * q){
    if(isEmpty(q)){
        printf("Underflow\n");
    }
    else{       
        q->f = (q->f +1)%(q->size) ;
        int value = q->arr[q->f];
    }
}
//Dequeues element from the end
int remove_last(struct deque * q){
    if(isEmpty(q)){
        printf("Underflow\n");
    }
    else{       
        int value = q->arr[q->b];
        q->b --;
        if(q->b < 0){
            q->b = q->size + q->b ;
        }
        return value;
    }
}
//Calculates distance
int distance(int x, int y, int xe, int ye){
    return (abs(x-xe) + abs(y-ye)) ;
}

int maze_solve(char ** maze, int n_Rows, int n_Cols){

    int xs,ys,xe,ye;
    //Initializing Deque
    struct deque * q = (struct deque* )malloc(sizeof(struct deque));
    q->size = n_Rows*n_Cols;
    q->f = q->b = 0;
    q->arr = (int*)malloc(n_Cols*n_Rows*sizeof(int));
    //Calculating start and end coordinates.
    for (int i = 0; i < n_Rows; i++)
    {
        for (int j = 0; j < n_Cols; j++)
        {
            if(maze[i][j] == 'S'){
                xs = i;
                ys = j;
            }
            if(maze[i][j] == 'E'){
                xe = i;
                ye = j;
            }
        }
    }
    int current;
    int cur_x = xs;
    int cur_y = ys;
    
    //adding coordinates of S to deque
    maze[xs][ys] = '.';
    add_last(q,xs*n_Cols+ys);

    //Defining array for incrementing in a direction
    int inc_x[] = {1,-1,0,0};
    int inc_y[] = {0,0,1,-1};

    int new_x;
    int new_y;

    //Running loop till deque is empty OR deque reaches the end point
    while(!isEmpty(q)){

        current = remove_last(q);
        cur_x = current/n_Cols;
        cur_y = current%n_Cols;

        maze[cur_x][cur_y] = '.';
        
        if (cur_x == xe && cur_y == ye)
        {
            maze[xs][ys] = 'S';
            maze[xe][ye] = 'E';
            return 1;
        }
        
        //Running a loop and adding coordinates to end or start of the deque according to its distance from current coordinates
        for (int i = 0; i < 4; i++)
        {
            new_x = cur_x + inc_x[i];
            new_y = cur_y + inc_y[i];

            if(maze[new_x][new_y] != '.' && maze[new_x][new_y] != '#' &&  (distance(cur_x,cur_y,xe,ye)) > distance(new_x,new_y,xe,ye)){
                add_last(q,new_x*n_Cols + new_y);
            }
            if(maze[new_x][new_y] != '.' && maze[new_x][new_y] != '#' &&  (distance(cur_x,cur_y,xe,ye)) < distance(new_x,new_y,xe,ye)){
                add_first(q,new_x*n_Cols + new_y);
            }
        }    
    }

    free(q->arr);
    return 0;
}

int main(){
    printf("Enter the maze file name that is to be solved: ");
    char str[20];
    scanf("%s",str);

    FILE *file_Ptr;
    file_Ptr = fopen(str, "r");

    int n_Rows = 0;
    int n_Cols = 0;
    int n_Char = 0;
    char current_Char;

    //Calculating number of rows and columns
    while ((current_Char = fgetc(file_Ptr)) != EOF) {
        if (current_Char == '\n') {
            n_Rows++;
            n_Cols = n_Char;
            n_Char = 0;
        } else {
            n_Char++;
        }
    }

    //Placing the cursor back to beginning of file
    fseek(file_Ptr, 0, SEEK_SET);

    //Dynamically allocating memory to store characters from the file
    char **maze = (char **)malloc(n_Rows * sizeof(char *));
    for (int i = 0; i < n_Rows; i++) {
        maze[i] = (char *)malloc((n_Cols + 1) * sizeof(char)); // +1 for null terminator
    }

    int row = 0, col = 0;

    //Storing the characters in the allocated memory
    while ((current_Char = fgetc(file_Ptr)) != EOF) {
        if (current_Char != '\n') {
            maze[row][col++] = current_Char;
        } else {
            maze[row][col] = '\0';
            row++;
            col = 0;
        }
    }

    fclose(file_Ptr);

    //printing the solved maze if solution exists 
    if(maze_solve(maze, n_Rows, n_Cols)){
        for (int i = 0; i < n_Rows; i++)
        {
            for (int j = 0; j < n_Cols; j++)
            {
                printf("%c",maze[i][j]);
            }
            printf("\n");           
        }
    }
    else{
        printf("Maze unsolvable\n");
    }
}

