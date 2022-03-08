
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

#include <iostream>
#include <vector>
#include <array>
#include <chrono>
#include <map>

using namespace std;

// VARIABLES ---------------------------------------------------------------------- //

typedef struct piece_t
{
    int id;
    array<int, 4> sides;
    bool visited;
    bool operator<(const piece_t &other) const { return id < other.id; }
} piece;

array<array<piece, 50>, 50> puzzle;
map<piece, pair<vector<piece>, vector<piece>>> dic;
int R, C, F;
unsigned long int visited = 0;
piece p_up;

// --------------------------------------------------------------------------------- //

bool check_horizontal_pair(piece p1, piece p2)
{
    int counter = 1;
    int side1 = p2.sides[0], side2 = p2.sides[1], side3 = p2.sides[2], side4 = p2.sides[3];
    // rotates p2
    while (p1.sides[1] != side1 || p1.sides[2] != side4)
    {

        if (counter == 4)
        {
            return false;
        }
        side1 = p2.sides[(0 + counter) % 4];
        side2 = p2.sides[(1 + counter) % 4];
        side3 = p2.sides[(2 + counter) % 4];
        side4 = p2.sides[(3 + counter) % 4];
        counter++;
    }
    p2.sides[0] = side1, p2.sides[1] = side2, p2.sides[2] = side3, p2.sides[3] = side4;
    return true;
}

bool check_vertical_pair(piece p1, piece p2)
{
    int counter = 1;
    int side1 = p2.sides[0], side2 = p2.sides[1], side3 = p2.sides[2], side4 = p2.sides[3];
    // rotates p2
    while (p1.sides[3] != side1 || p1.sides[2] != side2)
    {
        if (counter == 4)
        {
            return false;
        }
        side1 = p2.sides[(0 + counter) % 4];
        side2 = p2.sides[(1 + counter) % 4];
        side3 = p2.sides[(2 + counter) % 4];
        side4 = p2.sides[(3 + counter) % 4];
        counter++;
    }
    p2.sides[0] = side1, p2.sides[1] = side2, p2.sides[2] = side3, p2.sides[3] = side4;
    return true;
}

void add_pair(int side, piece p1, piece p2)
{
    // combines with left piece
    if (side == 0)
    {
        dic[p1].first.push_back(p2);
    }
    // combines with upper piece
    if (side == 1)
    {
        dic[p1].second.push_back(p2);
    }
}

void add_values()
{
    for (auto const &[key, val] : dic)
    {
        for (auto const &[key2, val2] : dic)
        {
            if (key.id == key2.id || key2.id == 0) // we don't want to use 1st piece
            {
                continue;
            }
            if (check_horizontal_pair(key, key2) == true)
            {
                add_pair(0, key, key2);
            }
            if (check_vertical_pair(key, key2) == true)
            {
                add_pair(1, key, key2);
            }
        }
    }
}

void print_dic()
{
    for (auto const &[key, val] : dic)
    {
        cout << "Key: " << key.sides[0] << key.sides[1] << key.sides[2] << key.sides[3] << endl;
        cout << endl;

        cout << "1st Values: ";
        for (auto p : val.first)
        {
            cout << " --- ";
            for (int i = 0; i < 4; i++)
                cout << p.sides[i];
        }

        cout << endl;
        cout << "2nd Values: ";
        for (auto p : val.second)
        {
            cout << " --- ";
            for (int i = 0; i < 4; i++)
                cout << p.sides[i];
        }
        cout << endl
             << endl;
    }
}

// esta função pode até nem ser necessária
void place_piece(int row, int column, piece p)
{
    puzzle[row][column] = p;
}

// peça p2 é a peça que já inseri no puzzle
// agora quero ver que peça posso inserir a seguir a esse
void look_pairs(int row, int column, piece p2)
{
    p2.visited = true;
    visited++;

    // muda para a linha seguinte quando chega ao fim da linha
    if (column == C)
    {
        p_up = puzzle[row][0]; // já não vou pegar na peça p2 mas sim na peça p_up para comparar se posso colocar em baixo desta
        row++;
        column = 0;
    }

    // ----------------------------------------------------------------------------------------------- //
    // na 1º linha percorre apenas os valores do 1º array da peça à esquerda

    if (row == 0 && !dic[p2].first.empty())
    {
        for (unsigned long int i = 0; i < dic[p2].first.size(); i++)
        {
            if (dic[p2].first[i].visited == false)
            {
                place_piece(row, column, dic[p2].first[i]);
                column++;
                look_pairs(row, column, dic[p2].first[i]);
            }
        }
    }

    // ----------------------------------------------------------------------------------------------- //
    // condição para quando chegámos à última coluna e queremos comparar a peça com a 1ª dessa linha, antes de ir para a linha seguinte
    // na 1º coluna de qualquer linha percorre apenas os valores do 2º array da peça acima da que queremos colocar

    if (row != 0 && !dic[p_up].first.empty())
    {
        if (column == 0)
        {
            for (unsigned long int i = 0; i < dic[p_up].second.size(); i++)
            {
                if (dic[p_up].second[i].visited == false)
                {
                    place_piece(row, column, dic[p_up].second[i]);
                    column++;
                    look_pairs(row, column, dic[p_up].second[i]);
                }
            }
        }

        // ----------------------------------------------------------------------------------------------- //
        // percorre os valores do 2º array da peça de cima, porque já temos uma peça à esquerda
        // se a peça encaixar tanto de um lado como do outro, é porque podemos usar essa peça no meio daquelas duas

        if (column != 0 && !dic[p_up].first.empty())
        {

            p_up = puzzle[row - 1][column - 1]; // como já tenho uma peça à esquerda, agora só preciso de comparar com a de cima

            for (unsigned long int i = 0; i < dic[p_up].second.size(); i++)
            {
                if (dic[p_up].second[i].visited == false)
                {
                    place_piece(row, column, dic[p_up].second[i]);
                    column++;
                    look_pairs(row, column, dic[p_up].second[i]);
                }
            }
        }
    }

    // ----------------------------------------------------------------------------------------------- //
    // se a peça não preenche nenhum dos requisitos, é porque não a podemos colocar
    // por isso, voltamos atrás na recursão, voltamos à coluna de trás
    // se já estávamos na coluna 0 e queríamos inserir a peça aí, então voltamos à linha de trás, última coluna

    
    p2.visited = false;
    visited--;

    if (column == 0)
    {
        column = C - 1;
        row--;
    }

    else
        column--;
    return;
}

void print_puzzle()
{
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            cout << "(ID, " << puzzle[i][j].id << "  " << "visited= " << puzzle[i][j].visited << ")" << endl;
        }
        cout << endl
             << endl;
    }
}

int main()
{
    int n_test_cases, N;
    cin >> n_test_cases;

    for (int i = 0; i < n_test_cases; i++)
    {
        cin >> N >> R >> C;

        for (int j = 0; j < N; j++)
        {
            piece p;
            p.visited = false;
            if (j == 0){
                p.visited = true; // 1st piece will never be used again
                puzzle[0][0] = p;
            }
            p.id = j;
            cin >> p.sides[0] >> p.sides[1] >> p.sides[2] >> p.sides[3];
            dic[p] = make_pair(vector<piece>(), vector<piece>());
        }


        // 1 --> pre-processing
        add_values();
        print_dic();

        
        // 2 --> recursive step
        auto iter = dic.begin();

        for (unsigned long int i = 0; i < iter->second.first.size(); i++){
            look_pairs(0, 1, iter->second.first[i]); // 1st key, 1st value of 1st array of 1st key
        }

        // 3 --> printing result
        
        //if (visited != dic.size())
            //cout << "IMPOSSIBLE PUZZLE!" << endl;
        //else
        //print_puzzle();
        
        // 4 --> clear current dictionary
        dic.clear();
        visited = 0;

        cout << endl << endl << "-- NEXT ITERATION --" << endl << endl;
    }

    return 0;
}
