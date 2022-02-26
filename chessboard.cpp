/*

N = R*C
1 <= N <= 2500
1 <= R ou C <= 50

n_test_cases
N R C
1 2 3 4
5 8 1 3
etc

exemplo:

3
3 1 3
2 5 1 3
1 5 4 5
3 2 5 4
2 2 1
1 2 3 4
4 1 3 4
4 2 2
1 2 3 4
1 3 2 5
4 2 3 1
5 4 3 2

PIECE PIECE A B x
  x     x   x x x

C PIECE PIECE PIECE PIECE
B   x     x     x     x

PIECE PIECE PIECE C PIECE
PIECE PIECE   A   B   x

*/

#include <bits/stdc++.h>

using namespace std;

void insert(auto chessboard, auto set_pieces)
{
    // r = row atual; c = column atual

    // se chegou ao fim da linha, muda de linha
    if (c == C)
    {
        r++;
        c = 0;
    }

    pieceA = chessboard[r][c - 1]; // a peça à ESQUERDA da que quero inserir
    pieceB = chessboard[r][c];     // a que quero INSERIR
    pieceC = chessboard[r - 1][c]; // a peça EM CIMA da que quero inserir
    int upper_left = pieceB[0];
    int upper_right = pieceB[1];
    int bottom_left = pieceB[3];

    int counter = 1;
    if (r == 0)
    {
        while (upper_left != pieceA[1] && down_left != pieceA[2])
        {
            if (counter == 4)
                return;
            // roda a peça
            upper_left = pieceB[0 + counter % 4];
            bottom_left = pieceB[3 + counter % 4];
            counter++;
        }
    }

    else
    {
        if (c == 0)
        {
            while (upper_left != pieceC[3] && upper_right != pieceC[2])
            {
                if (counter == 4)
                    return;
                // roda a peça
                upper_left = pieceB[0 + counter % 4];
                upper_right = pieceB[1 + counter % 4];
                counter++;
            }
        }

        else
        {
            while (upper_left != (pieceC[4] || pieceA[1]) && upper_right != pieceC[3] && bottom_left != pieceA[3])
            {
                if (counter == 4)
                    return;
                // roda a peça
                upper_left = pieceB[0 + counter % 4];
                upper_right = pieceB[1 + counter % 4];
                bottom_left = pieceB[3 + counter % 4];
                counter++;
            }
        }
    }

    // se números coincidem. chama recursivamente
    chessboard[r++][c++];
    insert(chessboard, pieceC);

    // acrescentar peça ao tabuleiro
    chessboard[r][c] = index da peça;
}

int main()
{
    int n_test_cases, N, R, C, side;
    vector<int> piece;
    vector<vector<int>> set_pieces;

    cin >> n_test_cases;

    for (int i = 0; i < n_test_cases; i++)
    {

        // leitura de dados
        cin >> N >> R >> C;
        int chessboard[R][C];
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                cin >> side;
                piece.push_back(side);
            }
            if (j == 0)
            {
                chessboard[0][0] = 0;
            }
            set_pieces.push_back(piece);
        }

        // chamar recursão aqui para cada caso de teste
        insert(chessboard, set_pieces);

        // isto é para verificar se um index já foi colocado no tabuleiro, não sei se
        // podemos otimizar isto melhor
        bool piece_exists_in_chessboard = find(begin(chessboard), end(chessboard), piece) != end(chessboard);
    }
    return 0;
}