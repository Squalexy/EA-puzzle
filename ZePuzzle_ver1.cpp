#include <iostream>
#include <vector>
#include <array>
#include <chrono>

typedef std::array<int, 4> piece;
typedef std::vector<piece> pieceQueue;
typedef std::array<std::array<piece, 50>, 50> boardMatrix;

int  n, rows, cols;

bool checkBoardState(boardMatrix board, int x, int y){
    // [0 1 2 3] north edge is indexes 0 and 1  |p0 p1|
    // [0 1 2 3] west edge is indexes 0 and 3   |p3   |
    int p0 = board[x][y][0];
    int p1 = board[x][y][1];
    int p3 = board[x][y][3];

    // check North ^ if possible
    if (x > 0){
        // [0 1 2 3] south edge is indexes 3 and 2
        int s0 = board[x - 1][y][3];
        int s1 = board[x - 1][y][2];
        if (p0 != s0 || p1 != s1) return false;
    }
    // check West < if possible
    if (y > 0){
        // [0 1 2 3] east edge is indexes 1 and 2
        int e0 = board[x][y - 1][1];
        int e1 = board[x][y - 1][2];
        if (p0 != e0 || p3 != e1) return false;
    }

    return true;
}

bool placePiece(boardMatrix& board, pieceQueue& pieces, int x, int y){
    // base case
    if (x >= rows){
        // if all pieces got used then the puzzle is not impossible
        return true;
    }

    // test all pieces still available
    for (long unsigned int i = 0; i < pieces.size(); i++){
        // next position to place piece
        int nx = y + 1 >= cols ? x + 1 : x;
        int ny = y + 1 >= cols ? 0 : y + 1;

        // take piece from queue
        //piece p = pieces.front();
        //pieces.pop();
        piece p = pieces[i];
        if (p[0] == -1) continue;
        pieces[i][0] = -1;

        // place piece on board and test if it was a valid move
        board[x][y][0] = p[0];
        board[x][y][1] = p[1];
        board[x][y][2] = p[2];
        board[x][y][3] = p[3];
        if (checkBoardState(board, x, y)){
            if (placePiece(board, pieces, nx, ny)) return true;
        }

        // if it was not a valid move try with another rotation
        board[x][y][0] = p[1];
        board[x][y][1] = p[2];
        board[x][y][2] = p[3];
        board[x][y][3] = p[0];
        if (checkBoardState(board, x, y)){
            if (placePiece(board, pieces, nx, ny)) return true;
        }

        board[x][y][0] = p[2];
        board[x][y][1] = p[3];
        board[x][y][2] = p[0];
        board[x][y][3] = p[1];
        if (checkBoardState(board, x, y)){
            if (placePiece(board, pieces, nx, ny)) return true;
        }

        board[x][y][0] = p[3];
        board[x][y][1] = p[0];
        board[x][y][2] = p[1];
        board[x][y][3] = p[2];
        if (checkBoardState(board, x, y)){
            if (placePiece(board, pieces, nx, ny)) return true;
        }

        // if no rotation was valid, return piece to queue and try next one
        pieces[i][0] = p[0];
    }
    return false;
}


int main(){
    auto begin = std::chrono::high_resolution_clock::now();

    int numTestCases;

    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    
    std::cin >> numTestCases;
    for (int i = 0; i < numTestCases; i++){
        piece p;
        pieceQueue pieces;
        boardMatrix board;
        
        std::cin >> n >> rows >> cols;
        for (int j = 0; j < n; j++){
            std::cin >> p[0] >> p[1] >> p[2] >> p[3];
            pieces.push_back(p);
        }

        // place first piece as is on top left corner
        p = pieces[0];
        pieces[0][0] = -1;
        board[0][0] = p;

        // solve puzzle
        if (placePiece(board, pieces, 0, 1)){
            for (int j = 0; j < rows; j++){
                for (int k = 0; k < cols - 1; k++)
                    std::cout << board[j][k][0] << " " << board[j][k][1] << "  ";
                std::cout << board[j][cols-1][0] << " " << board[j][cols - 1][1];
                std::cout<< std::endl;

                for (int k = 0; k < cols - 1; k++)
                    std::cout << board[j][k][3] << " " << board[j][k][2] << "  ";
                std::cout << board[j][cols - 1][3] << " " << board[j][cols - 1][2];
                if (j < rows-1 ) std::cout << std::endl << std::endl;
                else std::cout << std::endl;
            }
        }
        else {
            std::cout << "impossible puzzle!" << std::endl;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    // printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);

    return 0;
}
