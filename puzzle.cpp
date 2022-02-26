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

vector<vector<int>> puzzle[51][51];
int R, C;

void insert(set_pieces, row, column)
{
    // se chegou ao fim da linha, muda de linha
    if (column == C - 1)
    {
        row++;
        column = 0;
    }

    if (row == R - 1 && column == C - 1)
    {
        // colocar última peça
        return;
    }

    pieceA = puzzle[row][column - 1]; // a peça à ESQUERDA da que quero inserir
    pieceB = puzzle[row][column];     // a que quero INSERIR
    pieceC = puzzle[row - 1][column]; // a peça EM CIMA da que quero inserir
    int upper_left = pieceB[0];
    int upper_right = pieceB[1];
    int bottom_left = pieceB[3];

    int counter = 1;
    if (row == 0)
    {
        while (upper_left != pieceA[1] && down_left != pieceA[2])
        {
            if (counter == 4)
                if (column != 1)
                    return insert(set_pieces, row, column - 1);
                else
                {
                    cout << "impossible puzzle!" << endl;
                    return;
                }
            // roda a peça
            upper_left = pieceB[0 + counter % 4];
            bottom_left = pieceB[3 + counter % 4];
            counter++;
        }
    }

    else
    {
        if (column == 0)
        {
            while (upper_left != pieceC[3] && upper_right != pieceC[2])
            {
                if (counter == 4)
                    return insert(set_pieces, row - 1, C);
                // roda a peça
                upper_left = pieceB[0 + counter % 4];
                upper_right = pieceB[1 + counter % 4];
                counter++;
            }
        }

        else
        {
            while ((upper_left != pieceC[4] || upper_left != pieceA[1]) && upper_right != pieceC[3] && bottom_left != pieceA[3])
            {
                if (counter == 4)
                    return insert(set_pieces, row - 1, column - 1);
                // roda a peça
                upper_left = pieceB[0 + counter % 4];
                upper_right = pieceB[1 + counter % 4];
                bottom_left = pieceB[3 + counter % 4];
                counter++;
            }
        }
    }

    // se números coincidem. chama recursivamente
    insert(set_pieces, row + 1, column + 1);

    // acrescentar peça ao tabuleiro
    puzzle[r][c] = index da peça;
}

int main()
{
    int n_test_cases, N, color;
    vector<int> piece;
    vector<vector<int>> set_pieces;

    cin >> n_test_cases;

    for (int i = 0; i < n_test_cases; i++)
    {

        // leitura de dados
        cin >> N >> R >> C;

        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                cin >> color;
                piece.push_back(color);
            }
            set_pieces.push_back(piece);

            // coloca a primeira peça no puzzle
            if (j == 0)
                puzzle[0][0] = set_pieces[0];
        }

        // chamar recursão aqui para cada caso de teste
        int row = 0;
        int column = 1;
        insert(set_pieces, row, column);

        // isto é para verificar se um index já foi colocado no tabuleiro, não sei se
        // podemos otimizar isto melhor
        bool piece_exists_in_puzzle = find(begin(puzzle), end(puzzle), piece) != end(puzzle);
    }
    return 0;
}