
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
#include <cstring>

using namespace std;

// VARIABLES ---------------------------------------------------------------------- //

typedef struct piece_t
{
    array<int, 2> id;
    array<int, 4> sides;
    bool visited;
    bool operator<(const piece_t &other) const { return id < other.id; }
} piece;

array<array<piece, 50>, 50> puzzle;
map<piece, pair<vector<piece>, vector<piece>>> dic;
int N, R, C, F;
unsigned long int visited = 0;
piece p_up;
int personnal_id;
array<bool, 2500> is_visited;
int pieces_on_board = 0;

// --------------------------------------------------------------------------------- //

bool check_horizontal_pair(piece p1, piece p2)
{
    if (p1.sides[1] != p2.sides[0] || p1.sides[2] != p2.sides[3])
        return false;
    else
        return true;
}

bool check_vertical_pair(piece p1, piece p2)
{
    if (p1.sides[3] != p2.sides[0] || p1.sides[2] != p2.sides[1])
        return false;
    else
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
            if (key.id[0] == key2.id[0] || key2.id[0] == 0) // we don't want to use 1st piece
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
        cout << "ID(" << key.id[0] << "," << key.id[1] << ") --- { " << key.sides[0] << key.sides[1] << key.sides[2] << key.sides[3] << "}" << endl;

        cout << "Left -->";
        for (auto p : val.first)
        {
            cout << "[";
            for (int i = 0; i < 4; i++)
                cout << p.sides[i];
            cout << "]   ";
        }

        cout << endl;
        cout << "Down -->";
        for (auto p : val.second)
        {
            cout << "[";
            for (int i = 0; i < 4; i++)
                cout << p.sides[i];
            cout << "]   ";
        }
        cout << endl
             << endl;
    }
}

// esta função pode até nem ser necessária
void place_piece(int row, int column, piece p)
{
    puzzle[row][column] = p;
    pieces_on_board++;
    // cout << endl << "Added piece ID(" << p.id[0] << "," << p.id[1] << ") on puzzle[" << row << "][" << column << "]" << endl;
    // cout << "Current pieces on board: " << pieces_on_board << endl;
}

// peça p2 é a peça que já inseri no puzzle
// agora quero ver que peça posso inserir a seguir a esse
void look_pairs(int row, int column, piece p2)
{

    if (pieces_on_board == N)
        return;

    // cout << "ID(" << p2.id[0] << "," << p2.id[1] << ") has now been visited!" << endl;
    is_visited[p2.id[0]] = true;
    // cout << "ID(" << p2.id[0] << "," << p2.id[1] << ") is now true(" << is_visited[p2.id[0]] << ")" << endl;

    place_piece(row, column, p2);

    column++;
    // cout << "[BEFORE IF] Row " << row << " Col " << column << endl;

    // muda para a linha seguinte quando chega ao fim da linha
    if (column == C && row != R - 1)
    {
        row++;
        column = 0;
    }

    // cout << "[AFTER IF] Row " << row << " Col " << column << endl << endl;

    // ----------------------------------------------------------------------------------------------- //
    // na 1º linha percorre apenas os valores do 1º array da peça à esquerda

    if (row == 0 && !dic[p2].first.empty())
    {
        for (unsigned long int i = 0; i < dic[p2].first.size(); i++)
        {
            // cout << "[ROWS == 0] Checking if ID(" << dic[p2].first[i].id[0] << "," << dic[p2].first[i].id[1] << ") has been visited" << endl;
            if (is_visited[dic[p2].first[i].id[0]] == false)
            {
                // cout << "[ROWS == 0] ID(" << dic[p2].first[i].id[0] << "," << dic[p2].first[i].id[1] << ") hasn't been visited yet!" << endl;
                look_pairs(row, column, dic[p2].first[i]);
            }
        }
    }

    // ----------------------------------------------------------------------------------------------- //
    // condição para quando chegámos à última coluna e queremos comparar a peça com a 1ª dessa linha, antes de ir para a linha seguinte
    // na 1º coluna de qualquer linha percorre apenas os valores do 2º array da peça acima da que queremos colocar

    if (row != 0)
    {
        if (column == 0)
        {
            p_up = puzzle[row][0];
            if (!dic[p_up].first.empty())
            {
                for (unsigned long int i = 0; i < dic[p_up].second.size(); i++)
                {
                    // cout << "[ROWS != 0, COLUMN == 0] Checking if ID(" << dic[p_up].second[i].id[0] << "," << dic[p_up].second[i].id[1] << ") has been visited" << endl;
                    if (is_visited[dic[p_up].second[i].id[0]] == false)
                    {
                        // cout << "[ROWS != 0, COLUMN == 0] ID(" << dic[p_up].second[i].id[0] << "," << dic[p_up].second[i].id[1] << ") hasn't been visited yet!" << endl;
                        look_pairs(row, column, dic[p_up].second[i]);
                    }
                }
            }
        }

        // ----------------------------------------------------------------------------------------------- //
        // percorre os valores do 2º array da peça de cima, porque já temos uma peça à esquerda
        // se a peça encaixar tanto de um lado como do outro, é porque podemos usar essa peça no meio daquelas duas

        if (column != 0)
        {
            p_up = puzzle[row - 1][column]; // como já tenho uma peça à esquerda, agora só preciso de comparar com a de cima
            if (!dic[p_up].first.empty())
            {
                for (unsigned long int i = 0; i < dic[p_up].second.size(); i++)
                {
                    // cout << "[ROWS != 0, COLUMN != 0] Checking if ID(" << dic[p_up].second[i].id[0] << "," << dic[p_up].second[i].id[1] << ") has been visited" << endl;
                    if (is_visited[dic[p_up].second[i].id[0]] == false)
                    {
                        // cout << "[ROWS != 0, COLUMN != 0] ID(" << dic[p_up].second[i].id[0] << "," << dic[p_up].second[i].id[1] << ") hasn't been visited yet!" << endl;
                        look_pairs(row, column, dic[p_up].second[i]);
                    }
                }
            }
        }
    }

    // ----------------------------------------------------------------------------------------------- //
    // se a peça não preenche nenhum dos requisitos, é porque não a podemos colocar
    // por isso, voltamos atrás na recursão, voltamos à coluna de trás
    // se já estávamos na coluna 0 e queríamos inserir a peça aí, então voltamos à linha de trás, última coluna

    if (pieces_on_board == N)
        return;

    // cout << endl << "! PIECE ID(" << p2.id[0] << "," << p2.id[1] << ") ISN'T GONNA WORK ! going back..." << endl;

    is_visited[p2.id[0]] = false;
    pieces_on_board--;

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
    for (int j = 0; j < R; j++)
    {
        for (int k = 0; k < C - 1; k++)
            cout << puzzle[j][k].sides[0] << " " << puzzle[j][k].sides[1] << "  ";
        cout << puzzle[j][C - 1].sides[0] << " " << puzzle[j][C - 1].sides[1];
        cout << endl;

        for (int k = 0; k < C - 1; k++)
            cout << puzzle[j][k].sides[3] << " " << puzzle[j][k].sides[2] << "  ";
        cout << puzzle[j][C - 1].sides[3] << " " << puzzle[j][C - 1].sides[2];
        if (j < R - 1)
            cout << endl
                 << endl;
        else
            cout << endl;
    }
}

int main()
{
    int n_test_cases;
    cin >> n_test_cases;

    for (int i = 0; i < n_test_cases; i++)
    {
        personnal_id = 0;
        pieces_on_board = 0;
        is_visited = {false};

        cin >> N >> R >> C;

        for (int j = 0; j < N; j++)
        {

            if (j == 0)
            {
                piece p1;
                cin >> p1.sides[0] >> p1.sides[1] >> p1.sides[2] >> p1.sides[3];

                p1.id[0] = j;
                p1.id[1] = personnal_id++;
                is_visited[p1.id[0]] = true; // 1st piece will never be used again
                dic[p1] = make_pair(vector<piece>(), vector<piece>());

                puzzle[0][0] = p1;
                pieces_on_board++;
            }

            else
            {
                piece p1;
                piece p2;
                piece p3;
                piece p4;
                cin >> p1.sides[0] >> p1.sides[1] >> p1.sides[2] >> p1.sides[3];
                p2.sides[0] = p1.sides[3], p2.sides[1] = p1.sides[0], p2.sides[2] = p1.sides[1], p2.sides[3] = p1.sides[2];
                p3.sides[0] = p2.sides[3], p3.sides[1] = p2.sides[0], p3.sides[2] = p2.sides[1], p3.sides[3] = p2.sides[2];
                p4.sides[0] = p3.sides[3], p4.sides[1] = p3.sides[0], p4.sides[2] = p3.sides[1], p4.sides[3] = p3.sides[2];

                p1.id[0] = j, p2.id[0] = j, p3.id[0] = j, p4.id[0] = j;
                p1.id[1] = personnal_id++, p2.id[1] = personnal_id++, p3.id[1] = personnal_id++, p4.id[1] = personnal_id++;
                is_visited[p1.id[0]] = false;

                dic[p1] = make_pair(vector<piece>(), vector<piece>());
                dic[p2] = make_pair(vector<piece>(), vector<piece>());
                dic[p3] = make_pair(vector<piece>(), vector<piece>());
                dic[p4] = make_pair(vector<piece>(), vector<piece>());
            }
        }

        // 1 --> pre-processing
        add_values();
        // print_dic();

        // 2 --> recursive step
        auto iter = dic.begin();
        for (unsigned long int i = 0; i < iter->second.first.size(); i++)
        {
            look_pairs(0, 1, iter->second.first[i]); // 1st value of 1st array of 1st key
        }

        // 3 --> printing result
        /*if (pieces_on_board != N)
            cout << "impossible puzzle!" << endl;
        else*/
        print_puzzle();

        // 4 --> clear current dictionary
        dic.clear();
    }

    return 0;
}
