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

    bool operator==(const piece_t &other) const {return other.id == id && other.rotation == rotation;}
    bool operator<(piece_t other) const {return id < other.id || (id == other.id && rotation < other.rotation);}
} piece;

struct pieceHasher
{
    size_t operator()(const piece_t &p) const
    {
        return hash<int>()((p.id * 7) + p.rotation);
    }
};

array<array<piece, 50>, 50> puzzle;
unordered_map<piece, pair<vector<piece>, vector<piece>>, pieceHasher> dic;
array<int, 1000> num_count;
array<bool, 2500> visited;
int R, C, F;
// --------------------------------------------------------------------------------- //

bool check_horizontal_pair(piece left, piece other)
{
    if (left.sides[1] != other.sides[0] || left.sides[2] != other.sides[3])
        return false;
    else return true;
}

bool check_vertical_pair(piece up, piece other)
{
    if (up.sides[3] != other.sides[0] || up.sides[2] != other.sides[1])
        return false;
    else return true;
}

void add_pair(int side, piece me, piece other)
{
    // combines with left piece
    if (side == 0)
    {
        dic[me].first.push_back(other);
    }
    // combines with upper piece
    if (side == 1)
    {
        dic[me].second.push_back(other);
    }
}

void add_values()
{
    for (auto &[key, val] : dic)
    {
        for (auto &[key2, val2] : dic)
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

        
        sort(val.first.begin(), val.first.end());
        sort(val.second.begin(), val.second.end());
        
    }
}

bool solvePuzzle(int row, int col){
    // base case
    if (row >= R){
        // if all pieces got used then the puzzle is not impossible
        return true;
    }
    
    int next_row = col + 1 >= C ? row + 1 : row;
    int next_col = col + 1 >= C ? 0 : col + 1;


    if (row == 0){
        piece left = puzzle[row][col - 1];
        //vector<piece> match_left =dic[left].first;

        for (piece p : dic[left].first){
            if (!visited[p.id]){
                puzzle[row][col] = p;
                visited[p.id] = true;
                if (solvePuzzle(next_row, next_col)) return true;
                visited[p.id] = false;
            }
        }
    }

    if (col == 0){
        piece up = puzzle[row - 1][col];
        //vector<piece> match_up = dic[up].second;

        for (piece p : dic[up].second){
            if (!visited[p.id]){
                puzzle[row][col] = p;
                visited[p.id] = true;
                if (solvePuzzle(next_row, next_col)) return true;
                visited[p.id] = false;
            }
        }
    }

    if (row > 0 && col > 0){
        piece up = puzzle[row - 1][col];
        piece left = puzzle[row][col - 1];

        //vector<piece> match_left =dic[left].first;
        //vector<piece> match_up = dic[up].second;

        vector <piece> pieces_int = vector <piece>(min(dic[up].second.size(), dic[left].first.size()));
        set_intersection(dic[up].second.begin(), dic[up].second.end(), dic[left].first.begin(), dic[left].first.end(), pieces_int.begin());

        for (piece p : pieces_int){
            if (!visited[p.id]){
                puzzle[row][col] = p;
                visited[p.id] = true;
                if (solvePuzzle(next_row, next_col)) return true;
                visited[p.id] = false;
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
            cout << puzzle[j][k].sides[0] << " " << puzzle[j][k].sides[1] << "  ";
        cout << puzzle[j][C - 1].sides[0] << " " << puzzle[j][C - 1].sides[1];
        cout << endl;

        for (int k = 0; k < C - 1; k++)
            cout << puzzle[j][k].sides[3] << " " << puzzle[j][k].sides[2] << "  ";
        cout << puzzle[j][C - 1].sides[3] << " " << puzzle[j][C - 1].sides[2];

        if (j < R - 1) cout << endl << endl;
        else cout << endl;
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

        // first piece is placed on the board as is
        p.id = 0;
        p.rotation = 0;
        cin >> p.sides[0] >> p.sides[1] >> p.sides[2] >> p.sides[3];
        dic[p] = make_pair(vector<piece>(), vector<piece>());

        puzzle[0][0] = p;
        visited[0] = true;

        num_count[p.sides[0]] ++;
        num_count[p.sides[1]] ++;
        num_count[p.sides[2]] ++;
        num_count[p.sides[3]] ++;


        // add other pieces in all their possible rotations
        for (int j = 1; j < N; j++)
        {
            int side0, side1, side2, side3;

            p.id = j;
            visited[j] = false;
            cin >> side0 >> side1 >> side2 >> side3;
            num_count[side0] ++;
            num_count[side1] ++;
            num_count[side2] ++;
            num_count[side3] ++;

            for (int k = 0; k < 4; k++)
            {
                p.rotation = k;
                p.sides[(0 + k) % 4] = side0;
                p.sides[(1 + k) % 4] = side1;
                p.sides[(2 + k) % 4] = side2;
                p.sides[(3 + k) % 4] = side3;

                dic[p] = make_pair(vector<piece>(), vector<piece>());
            }
        }

        // check if puzzle is even possible to be completed
        int odd_counter = 0;
        for (int num : num_count){
            if (num % 2 != 0) odd_counter++;
            if (odd_counter > 4)
            {
                cout << "impossible puzzle!" << endl;
                flag = 1;
                break;
            }
        }
        
        if (flag == 0){
            // pre-processing
            add_values();

            // recursive call
            if (solvePuzzle(0, 1)){
                print_puzzle();
            }
            else{
                cout << "impossible puzzle!" << endl;
            }

            // clear maps for next test case
        }
        dic.clear();
    }

    return 0;
}
