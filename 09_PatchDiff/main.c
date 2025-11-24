#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int size;
int** visited;
int** grid;

int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

int is_valid(int x, int y) {
    return x >= 0 && x < size && y >= 0 && y < size;
}

void dfs(int x, int y) {
    visited[x][y] = 1;
    
    int directions[4] = {0, 1, 2, 3};
    
    for (int i = 0; i < 4; i++) {
        int ind = rand() % 4;
        while (directions[ind = (ind + 1) % 4] == -1) ;
        int dir = directions[ind];
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        
        int gridx = 2 * x + 1 + dx[dir];
        int gridy = 2 * y + 1 + dy[dir];
        
        if (is_valid(nx, ny) && visited[nx][ny] == 0 && grid) {
            grid[gridx][gridy] = 1;
            dfs(nx, ny);
        }
        directions[ind] = -1;
    }
}

void print_maze() {
    int grid_size = 2 * size + 1;
    
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            printf("%c", grid[i][j] ? '.' : '#');
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));
    
    size = 6;
    
    int startX = rand() % size;
    int startY = rand() % size;
    
    int grid_size = 2 * size + 1;
    
    visited = (int**) malloc(size * sizeof(int*));
    grid = (int**) malloc(grid_size * sizeof(int*));
    
    for (int i = 0; i < size; i++) {
        visited[i] = (int*)calloc(size, sizeof(int));
        grid[2 * i] = (int*)calloc(grid_size, sizeof(int));
        grid[2 * i + 1] = (int*)calloc(grid_size, sizeof(int));
        for (int j = 0; j < size; j++) {
            grid[2 * i + 1][2 * j + 1] = 1; // room
        }
    }
    grid[grid_size - 1] = (int*)calloc(grid_size, sizeof(int));

    dfs(startX, startY);
    
    print_maze();
    
    for (int i = 0; i < size; i++) {
        free(visited[i]);
        free(grid[2 * i]);
        free(grid[2 * i + 1]);
    }
    free(grid[2 * size]);
    free(visited);
    free(grid);
    return 0;
}