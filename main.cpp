#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <ctime>
#include <string>

using namespace std;

struct games_counts
{
    int wins;
    int lose;
};

void create_empty_board(char board[10][10]) {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            board[i][j] = '~';
        }
    }
}

void set_color(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void print_boards(const char board1[10][10], const char board2[10][10], bool hide_opponent_ships = false) {
    cout << setw(24) << "Player field 1" << setw(40) << "Player field 2" << endl << endl;
    cout << "   ";
    for (int i = 1; i <= 10; ++i) {
        cout << setw(2) << i << " ";
    }
    cout << setw(9) << " ";
    for (int i = 1; i <= 10; ++i) {
        cout << setw(2) << i << " ";
    }
    cout << endl;

    for (int i = 0; i < 10; ++i) {
        cout << setw(2) << i + 1 << " ";
        for (int j = 0; j < 10; ++j) {
            if (board1[i][j] == '1') {
                set_color(FOREGROUND_RED | FOREGROUND_INTENSITY);
                cout << setw(2) << board1[i][j] << " ";
                set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            else if (board1[i][j] == '0') {
                set_color(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                cout << setw(2) << board1[i][j] << " ";
                set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            else if (board1[i][j] == 'X') {
                set_color(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                cout << setw(2) << board1[i][j] << " ";
                set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            else {
                cout << setw(2) << board1[i][j] << " ";
            }
        }
        cout << setw(6) << " " << setw(2) << i + 1 << " ";
        for (int j = 0; j < 10; ++j) {
            if (hide_opponent_ships && board2[i][j] == '0') {
                cout << setw(2) << '~' << " ";
            }
            else if (board2[i][j] == '1') {
                set_color(FOREGROUND_RED | FOREGROUND_INTENSITY);
                cout << setw(2) << board2[i][j] << " ";
                set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            else if (board2[i][j] == '0') {
                set_color(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                cout << setw(2) << board2[i][j] << " ";
                set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            else if (board2[i][j] == 'X') {
                set_color(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                cout << setw(2) << board2[i][j] << " ";
                set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            else {
                cout << setw(2) << board2[i][j] << " ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

void save_counts_to_file(const games_counts& gamecount, const string& filename) {
    ofstream outFile(filename, ios::app);
    if (outFile.is_open()) {
        outFile << "Wins: " << gamecount.wins << ' ' << "Lose: " << gamecount.lose << endl;
        outFile.close();
    }
}

void load_last_counts_from_file(games_counts& gamecount, const string& filename) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        string line;
        string lastLine;

        while (getline(inFile, line)) {
            lastLine = line;
        }

        inFile.close();

        if (!lastLine.empty()) {
            istringstream iss(lastLine);
            string wins_str, lose_str;
            iss >> wins_str >> gamecount.wins >> lose_str >> gamecount.lose;
        }
        else {
            gamecount.wins = 0;
            gamecount.lose = 0;
        }
    }
    else {
        gamecount.wins = 0;
        gamecount.lose = 0;
    }
}
//
void save_board_to_file(const char board[10][10], const string& filename) {
    ofstream file(filename, ios::app); // Open file in append mode
    if (file.is_open()) {
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                file << board[i][j] << ' ';
            }
            file << '\n';
        }
        file << endl; // Add a newline to separate game states
        file.close();
    }
}
//
void load_board_from_file(char board[10][10], const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        int row = 0;
        while (getline(file, line)) {
            if (row >= 10) break; // Skip lines after the first 10
            for (int col = 0; col < 10; ++col) {
                if (col < line.size()) {
                    board[row][col] = line[col];
                }
                else {
                    board[row][col] = '~';
                }
            }
            row++;
        }
        file.close();
    }
}

bool is_valid_position(char board[10][10], int x, int y, int size, bool horizontal) {
    if (horizontal) {
        if (y + size > 10) return false;
        for (int i = 0; i < size; ++i) {
            if (board[x][y + i] != '~') return false;
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (x + dx >= 0 && x + dx < 10 && y + i + dy >= 0 && y + i + dy < 10) {
                        if (board[x + dx][y + i + dy] != '~') return false;
                    }
                }
            }
        }
    }
    else {
        if (x + size > 10) return false;
        for (int i = 0; i < size; ++i) {
            if (board[x + i][y] != '~') return false;
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (x + i + dx >= 0 && x + i + dx < 10 && y + dy >= 0 && y + dy < 10) {
                        if (board[x + i + dx][y + dy] != '~') return false;
                    }
                }
            }
        }
    }
    return true;
}

void place_ship(char board[10][10], int size, const string& filename) {
    bool placed = false;
    while (!placed) {
        int x = rand() % 10;
        int y = rand() % 10;
        bool horizontal = rand() % 2;

        if (is_valid_position(board, x, y, size, horizontal)) {
            if (horizontal) {
                for (int i = 0; i < size; ++i) {
                    board[x][y + i] = '0';
                }
            }
            else {
                for (int i = 0; i < size; ++i) {
                    board[x + i][y] = '0';
                }
            }
            save_board_to_file(board, filename);
            placed = true;
        }
    }
}

void place_all_ships(char board[10][10], const string& filename) {
    int ships[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
    for (int i = 0; i < 10; ++i) {
        place_ship(board, ships[i], filename);
    }
}

void place_ship_manual(char board[10][10], int size, const string& filename) {
    bool placed = false;
    while (!placed) {
        int x, y;
        char orientation;
        cout << "Enter the coordinates for a ship of size " << size << " (row and column): ";
        cin >> x >> y;
        x--; y--;
        cout << "Orientation (h for horizontal, v for vertical): ";
        cin >> orientation;
        bool horizontal = (orientation == 'h');

        if (is_valid_position(board, x, y, size, horizontal)) {
            if (horizontal) {
                for (int i = 0; i < size; ++i) {
                    board[x][y + i] = '0';
                }
            }
            else {
                for (int i = 0; i < size; ++i) {
                    board[x + i][y] = '0';
                }
            }
            save_board_to_file(board, filename);
            placed = true;
        }
        else {
            cout << "The position is incorrect. Please try again." << endl;
        }
    }
}

void place_all_ships_manual(char board[10][10], const string& filename) {
    int ships[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
    for (int i = 0; i < 10; ++i) {
        print_boards(board, board, true);
        place_ship_manual(board, ships[i], filename);
    }
}

bool is_ship_destroyed(char board[10][10], int x, int y) {
    int start = y;
    while (start > 0 && (board[x][start - 1] == '1' || board[x][start - 1] == '0')) start--;
    int end = y;
    while (end < 9 && (board[x][end + 1] == '1' || board[x][end + 1] == '0')) end++;
    for (int i = start; i <= end; ++i) {
        if (board[x][i] == '0') return false;
    }
    start = x;
    while (start > 0 && (board[start - 1][y] == '1' || board[start - 1][y] == '0')) start--;
    end = x;
    while (end < 9 && (board[end + 1][y] == '1' || board[end + 1][y] == '0')) end++;
    for (int i = start; i <= end; ++i) {
        if (board[i][y] == '0') return false;
    }
    return true;
}

void mark_surroundings(char board[10][10], int x, int y) {
    int start_x = x;
    int start_y = y;
    int end_x = x;
    int end_y = y;

    while (start_y > 0 && (board[x][start_y - 1] == '1' || board[x][start_y - 1] == '0')) start_y--;
    while (end_y < 9 && (board[x][end_y + 1] == '1' || board[x][end_y + 1] == '0')) end_y++;

    while (start_x > 0 && (board[start_x - 1][y] == '1' || board[start_x - 1][y] == '0')) start_x--;
    while (end_x < 9 && (board[end_x + 1][y] == '1' || board[end_x + 1][y] == '0')) end_x++;

    for (int i = start_x; i <= end_x; ++i) {
        for (int j = start_y; j <= end_y; ++j) {
            if (board[i][j] != '1') continue;
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = i + dx;
                    int ny = j + dy;
                    if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10 && board[nx][ny] == '~') {
                        board[nx][ny] = 'X';
                    }
                }
            }
        }
    }
}

bool take_shot(char board[10][10], int x, int y) {
    if (board[x][y] == '0') {
        board[x][y] = '1';
        if (is_ship_destroyed(board, x, y)) {
            mark_surroundings(board, x, y);
        }
        return true;
    }
    else if (board[x][y] == '~') {
        board[x][y] = 'X';
        return false;
    }
    return false;
}

bool player_turn(char opponent_board[10][10]) {
    int x, y;
    bool valid_shot = false;

    while (!valid_shot) {
        cout << "Your move: Enter the coordinates to shoot (row and column): ";
        cin >> x >> y;
        x--; y--;

        if (x < 0 || x >= 10 || y < 0 || y >= 10) {
            cout << "The coordinates are incorrect. Please try again." << endl;
            continue;
        }

        if (opponent_board[x][y] == 'X' || opponent_board[x][y] == '1') {
            cout << "This cell has already been fired upon. Try another square." << endl;
        }
        else {
            valid_shot = true;
        }
    }

    return take_shot(opponent_board, x, y);
}

bool bot_turn(char player_board[10][10]) {
    static bool hit = false;
    static int last_x = -1, last_y = -1;
    static int next_direction = 0;

    int x, y;
    bool valid_shot = false;

    while (!valid_shot) {
        if (hit) {
            if (next_direction == 0 && last_x > 0 && player_board[last_x - 1][last_y] == '~') {
                x = last_x - 1;
                y = last_y;
            }
            else if (next_direction == 1 && last_y < 9 && player_board[last_x][last_y + 1] == '~') {
                x = last_x;
                y = last_y + 1;
            }
            else if (next_direction == 2 && last_x < 9 && player_board[last_x + 1][last_y] == '~') {
                x = last_x + 1;
                y = last_y;
            }
            else if (next_direction == 3 && last_y > 0 && player_board[last_x][last_y - 1] == '~') {
                x = last_x;
                y = last_y - 1;
            }
            else {
                hit = false;
                next_direction = 0;
                continue;
            }
            next_direction++;
        }
        else {
            x = rand() % 10;
            y = rand() % 10;

            if ((x + y) % 2 != 0) continue;

            if (player_board[x][y] == 'X' || player_board[x][y] == '1') {
                continue;
            }
        }

        valid_shot = true;
    }

    cout << "Bot move! The bot is shooting at the coordinates (" << x + 1 << ", " << y + 1 << ")" << endl;

    if (take_shot(player_board, x, y)) {
        hit = true;
        last_x = x;
        last_y = y;
        next_direction = 0;
        return true;
    }
    else {
        hit = false;
        return false;
    }
}

bool check_victory(char board[10][10]) {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (board[i][j] == '0') {
                return false;
            }
        }
    }
    return true;
}

void save_game_state(const char player1_board[10][10], const char player2_board[10][10]) {
    save_board_to_file(player1_board, "player1_board.txt");
    save_board_to_file(player2_board, "player2_board.txt");
}

void load_game_state(char player1_board[10][10], char player2_board[10][10]) {
    load_board_from_file(player1_board, "player1_board.txt");
    load_board_from_file(player2_board, "player2_board.txt");
}

int main() {
    bool gamecontinue = true;

    games_counts gamecount;
    gamecount.wins = 0;
    gamecount.lose = 0;
    string filename = "games_counts.txt";

    load_last_counts_from_file(gamecount, filename);

    while (gamecontinue)
    {
        srand(time(0));

        char player1_board[10][10];
        char player2_board[10][10];

        int choice;
        cout << "Choose the way you want to place your ships: 1 - Random, 2 - Manual: ";
        cin >> choice;

        bool game_loaded = false;
        ifstream player1_file("player1_board.txt");
        ifstream player2_file("player2_board.txt");

        if (player1_file.good() && player2_file.good()) {
            char load_choice;
            cout << "Found saved files. Want to download the game? (y/n): ";
            cin >> load_choice;

            if (load_choice == 'y' || load_choice == 'Y') {
                load_game_state(player1_board, player2_board);
                game_loaded = true;
            }
        }

        player1_file.close();
        player2_file.close();

        if (!game_loaded) {
            create_empty_board(player1_board);
            create_empty_board(player2_board);

            if (choice == 1) {
                place_all_ships(player1_board, "player1_board.txt");
            }
            else {
                place_all_ships_manual(player1_board, "player1_board.txt");
            }

            place_all_ships(player2_board, "player2_board.txt");
        }

        bool player_turn_flag = true;
        bool game_over = false;

        while (!game_over) {
            print_boards(player1_board, player2_board, true);

            if (player_turn_flag) {
                if (player_turn(player2_board)) {
                    cout << "You've made it! Make your next move." << endl;
                }
                else {
                    cout << "You missed the target! Bot move." << endl;
                    player_turn_flag = false;
                }
            }
            else {
                if (bot_turn(player1_board)) {
                    cout << "The bot has been hit! The bot makes its next move." << endl;
                }
                else {
                    cout << "The bot missed! Your move." << endl;
                    player_turn_flag = true;
                }
            }

            if (check_victory(player2_board)) {
                set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                cout << "Congratulations! You have won!" << endl;
                set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                gamecount.wins++;
                game_over = true;
            }
            else if (check_victory(player1_board)) {
                cout << "Unfortunately, the bot won." << endl;
                gamecount.lose++;
                game_over = true;
            }
            save_game_state(player1_board, player2_board);
        }
        save_counts_to_file(gamecount, filename);

        set_color(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "Number of victories: " << gamecount.wins << endl;
        set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        set_color(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Number of defeats: " << gamecount.lose << endl << endl;
        set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        int main_menu_choice;
        cout << "1. Start new game\n2. Exit\nEnter your choice: ";
        cin >> main_menu_choice;

        if (main_menu_choice == 2) {
            gamecontinue = false;
        }
    }

    return 0;
}