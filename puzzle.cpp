
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

// --------------------------------------------------------------------------------- //

bool check_horizontal_pair(piece p1, piece p2)
{
    int counter = 1;
    int side1 = p2.sides[0], side2 = p2.sides[1], side3 = p2.sides[2], side4 = p2.sides[3];
    // rotates p2
    while (p1.sides[1] != side1 || p1.sides[2] != side4)
    {
        
        if (counter == 4){
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
        if (counter == 4){
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
    if (side == 0){
        dic[p1].first.push_back(p2);
    }
    // combines with upper piece
    if (side == 1){
        dic[p1].second.push_back(p2);
    }
}

void add_values()
{
    for (auto const &[key, val] : dic)
    {
        for (auto const &[key2, val2] : dic){
            if (key.id == key2.id || key2.id == 0) {
                continue;
            }
            if (check_horizontal_pair(key, key2) == true) {
                add_pair(0, key, key2);
            }
            if (check_vertical_pair(key, key2) == true) {
                add_pair(1, key, key2);
            }
        }
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
            p.id = j;
            cin >> p.sides[0] >> p.sides[1] >> p.sides[2] >> p.sides[3];
            dic[p] = make_pair(vector<piece>(), vector<piece>());

            // coloca a primeira peça no puzzle
            if (j == 0)
                puzzle[0][0] = p;
        }

        add_values();

        // print of dictionary
        for (auto const &[key, val] : dic)
        {
            cout << "Key: " << key.sides[0] << key.sides[1] << key.sides[2] << key.sides[3] << endl;
            cout << endl;

            cout << "1st Values: ";
            for (auto p: val.first){
                    cout << " --- ";
                    for (int i = 0; i < 4; i++) cout << p.sides[i] ; 
                }

            cout << endl;
            cout << "2nd Values: ";
            for (auto p: val.second){
                    cout << " --- ";
                    for (int i = 0; i < 4; i++) cout << p.sides[i];
                }
            cout << endl << endl;
        }

        dic.clear();
    }
    return 0;
}
