#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <algorithm>

using namespace std;

// VARIABLES ---------------------------------------------------------------------- //

typedef struct piece_t
{
    int id;
    int rotation;
    array<int, 4> sides;
    pair<vector<int>, vector<int>> combination;
    bool operator==(const piece_t &other) const { return other.id == id && other.rotation == rotation; }
    bool operator<(const piece_t &other) const { return id < other.id || (id == other.id && rotation < other.rotation); }
} piece;

vector<piece> pieces;
array<array<int, 50>, 50> puzzle;
array<int, 1000> num_count;
array<bool, 2500> visited;
int R, C, F;

// --------------------------------------------------------------------------------- //

bool check_horizontal_pair(piece *left, piece *other)
{
    if (left->sides[1] != other->sides[0] || left->sides[2] != other->sides[3])
        return false;
    else
        return true;
}

bool check_vertical_pair(piece *up, piece *other)
{
    if (up->sides[3] != other->sides[0] || up->sides[2] != other->sides[1])
        return false;
    else
        return true;
}

void add_pair(int side, int i, int j)
{
    // combines with left piece
    if (side == 0)
    {
        pieces[i].combination.first.push_back(j);
    }
    // combines with upper piece
    if (side == 1)
    {
        pieces[i].combination.second.push_back(j);
    }
}

void add_values()
{

    for (int i = 0; i < (int)pieces.size(); i++)
    {
        for (int j = 0; j < (int)pieces.size(); j++)
        {
            if (pieces[i].id == pieces[j].id || pieces[j].id == 0)
                continue;
            if (check_horizontal_pair(&pieces[i], &pieces[j]) == true)
            {
                
                add_pair(0, i, j);
            }
            if (check_vertical_pair(&pieces[i], &pieces[j]) == true)
            {
                add_pair(1, i, j);
            }
        }
    }
}

bool solvePuzzle(int row, int col)
{
    // base case
    if (row >= R)
    {
        // if all pieces got used then the puzzle is not impossible
        return true;
    }

    int next_row = col + 1 >= C ? row + 1 : row;
    int next_col = col + 1 >= C ? 0 : col + 1;

    if (row == 0)
    {
        piece *left = &pieces[puzzle[row][col - 1]];
        // vector<piece> match_left =dic[left].first;

        for (int p : left->combination.first)
        {
            if (!visited[pieces[p].id])
            {
                
                puzzle[row][col] = p;
                visited[pieces[p].id] = true;
                if (solvePuzzle(next_row, next_col))
                    return true;
                visited[pieces[p].id] = false;
            }
        }
    }

    if (col == 0)
    {
        piece *up = &pieces[puzzle[row - 1][col]];
        // vector<piece> match_up = dic[up].second;

        for (int p : up->combination.second)
        {
            if (!visited[pieces[p].id])
            {
                puzzle[row][col] = p;
                visited[pieces[p].id] = true;
                if (solvePuzzle(next_row, next_col))
                    return true;
                visited[pieces[p].id] = false;
            }
        }
    }

    if (row > 0 && col > 0)
    {
        piece *up = &pieces[puzzle[row - 1][col]];
        piece *left = &pieces[puzzle[row][col - 1]];

        // vector<piece> match_left =dic[left].first;
        // vector<piece> match_up = dic[up].second;

        vector <int> pieces_int = vector <int>(min(up->combination.second.size(), left->combination.first.size()));
        set_intersection(up->combination.second.begin(), up->combination.second.end(), left->combination.first.begin(), left->combination.first.end(), pieces_int.begin());

        for (int p : pieces_int){
            if (!visited[pieces[p].id]){
                puzzle[row][col] = p;
                visited[pieces[p].id] = true;
                if (solvePuzzle(next_row, next_col)) return true;
                visited[pieces[p].id] = false;
            }
        }
    }

    return false;
}

void print_puzzle()
{
    for (int j = 0; j < R; j++)
    {
        for (int k = 0; k < C - 1; k++)
            cout << pieces[puzzle[j][k]].sides[0] << " " << pieces[puzzle[j][k]].sides[1] << "  ";
        cout << pieces[puzzle[j][C - 1]].sides[0] << " " << pieces[puzzle[j][C - 1]].sides[1];
        cout << endl;

        for (int k = 0; k < C - 1; k++)
            cout << pieces[puzzle[j][k]].sides[3] << " " << pieces[puzzle[j][k]].sides[2] << "  ";
        cout << pieces[puzzle[j][C - 1]].sides[3] << " " << pieces[puzzle[j][C - 1]].sides[2];

        if (j < R - 1)
            cout << endl
                 << endl;
        else
            cout << endl;
    }
}

int main()
{
    int n_test_cases, N;
    piece p;
    

    ios_base::sync_with_stdio(0);
    cin.tie(0);

    cin >> n_test_cases;

    for (int i = 0; i < n_test_cases; i++)
    {
        cin >> N >> R >> C;
        int flag = 0;
        num_count = {0};
        pieces = vector<piece>();

        // first piece is placed on the board as is
        p.id = 0;
        p.rotation = 0;
        cin >> p.sides[0] >> p.sides[1] >> p.sides[2] >> p.sides[3];

        num_count[p.sides[0]]++;
        num_count[p.sides[1]]++;
        num_count[p.sides[2]]++;
        num_count[p.sides[3]]++;

        pieces.push_back(p);

        // add other pieces in all their possible rotations
        for (int j = 1; j < N; j++)
        {
            int side0, side1, side2, side3;

            p.id = j;
            visited[j] = false;
            cin >> side0 >> side1 >> side2 >> side3;
            num_count[side0]++;
            num_count[side1]++;
            num_count[side2]++;
            num_count[side3]++;

            for (int k = 0; k < 4; k++)
            {
                p.rotation = k;
                p.sides[(0 + k) % 4] = side0;
                p.sides[(1 + k) % 4] = side1;
                p.sides[(2 + k) % 4] = side2;
                p.sides[(3 + k) % 4] = side3;
                pieces.push_back(p);
            }

        }

        // check if puzzle is even possible to be completed
        int odd_counter = 0;
        for (int num : num_count)
        {
            if (num % 2 != 0)
                odd_counter++;
            if (odd_counter > 4)
            {
                cout << "impossible puzzle!" << endl;
                flag = 1;
                break;
            }
        }

        if (flag == 0)
        {
            // pre-processing

            add_values();
            puzzle[0][0] = 0;
            visited[0] = true;

            // recursive call
            if (solvePuzzle(0, 1))
            {
                print_puzzle();
            }
            else
            {
                cout << "impossible puzzle!" << endl;
            }

            // clear maps for next test case
        }
    }

    return 0;
}
