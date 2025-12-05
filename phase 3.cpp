//#include <iostream>
//#include<iomanip>
//#include <string>
//using namespace std;
//
//const int SIZE = 8;
//char board[SIZE][SIZE];
//bool whiteTurn = true;
//bool gameOver = false;
//
//// Function prototypes
//void initializeBoard();
//void printBoard();
//bool isValidMove(int fromR, int fromC, int toR, int toC);
//void makeMove(int fromR, int fromC, int toR, int toC);
//
//// --- Student task: implement this function ---
//// Convert algebraic like "e2" -> returns true and fills row,col.
//// If input is invalid, return false.
//bool algebraicToCoords(const string& alg, int& row, int& col)
//{
//    if (alg.length() != 2) return false;
//
//    char f = tolower(alg[0]);
//    char r = alg[1];
//
//    if (f < 'a' || f>'h') return false;
//    if (r < '1' || r>'8') return false;
//
//    col = f - 'a';
//    row = 8 - (r - '0');
//
//    return true;
//}
//// Helper: check inside board
//bool isInsideBoard(int r, int c)
//{
//    return (r >= 0 && r < 8 && c >= 0 && c < 8);
//}
//
//
//int main() {
//    initializeBoard();
//
//    while (!gameOver) {
//        printBoard();
//        cout << (whiteTurn ? "White" : "Black") << "'s turn (or type exit): ";
//
//        string from, to;
//        if (!(cin >> from >> to)) break; // handle EOF
//
//
//
//        if (from == "exit" || from == "quit" || from == "x" || to == "exit" || to == "quit" || to == "x")
//        {
//            cout << "Game ended by user." << endl;
//            break;
//        }
//
//
//
//
//        int fromR, fromC, toR, toC;
//        bool ok1 = algebraicToCoords(from, fromR, fromC);
//        bool ok2 = algebraicToCoords(to, toR, toC);
//
//        if (!ok1 || !ok2) {
//            cout << "Invalid input format. Use e.g. e2 e4 (columns a-h, rows 1-8)." << endl;
//            continue;
//        }
//
//        if (!isInsideBoard(fromR, fromC) || !isInsideBoard(toR, toC)) {
//            cout << "Coordinates out of range." << endl;
//            continue;
//        }
//
//        if (isValidMove(fromR, fromC, toR, toC)) {
//            makeMove(fromR, fromC, toR, toC);
//            whiteTurn = !whiteTurn;
//        }
//        else {
//            cout << "Invalid move! Pawn can only move 2 squares from starting position." << endl;
//        }
//    }
//
//    cout << "Game ended." << endl;
//    return 0;
//}
//
//// --------------------
//// TODO: Student implements below
//// --------------------
//
//
//
//// (Other functions left as skeletons for students)
//void initializeBoard()
//{
//    // TODO: fill board with starting pieces (uppercase White, lowercase Black),
//    // e.g. 'R','N','B','Q','K','B','N','R' on rank 1 and 'P' pawns on rank 2.
//
//        // set everything to space first
//    for (int rows = 0; rows < 8; rows++)
//        for (int columns = 0; columns < 8; columns++)
//            board[rows][columns] = ' ';
//
//    // white pawns
//    for (int columns = 0; columns < 8; columns++)
//        board[6][columns] = 'P';
//
//    // black pawns
//    for (int columns = 0; columns < 8; columns++)
//        board[1][columns] = 'p';
//}
//
//
//
//void printBoard()
//{
//    // TODO: print board with ranks/files labels (a-h across top, 8-1 along side)
//    const int cellWidth = 3; // each cell will take 3 chars, e.g. " P "
//    // header
//    cout << "    ";
//    for (int c = 0; c < 8; ++c)
//        cout << setw(cellWidth) << char('a' + c);// each col has same width
//    cout << "\n";
//
//    // rows
//    for (int r = 0; r < 8; ++r)
//    {
//        // left rank label
//        cout << setw(2) << (8 - r) << "  ";//alignment of numbers
//
//        // cells
//        for (int c = 0; c < 8; ++c) {
//            char ch = board[r][c];
//            if (ch == ' ')
//                cout << setw(cellWidth) << '.';   // use '.' for empty
//            else
//                cout << setw(cellWidth) << ch;
//        }
//
//        // right rank label
//        cout << "  " << (8 - r) << "\n";
//    }
//
//    // footer (same as header)
//    cout << "    ";
//    for (int c = 0; c < 8; ++c)
//        cout << setw(cellWidth) << char('a' + c);
//    cout << "\n\n";
//
//}
//
//bool isValidMove(int fromR, int fromC, int toR, int toC)
//{
//    // TODO: check piece exists, belongs to player, call piece-specific validators.
//    if (!isInsideBoard(fromR, fromC) || !isInsideBoard(toR, toC)) return false;
//
//    char p = board[fromR][fromC];
//
//    // empty selection
//    if (p == ' ') return false;
//
//    // not player's pawn
//    if (whiteTurn && p != 'P') return false;
//    if (!whiteTurn && p != 'p') return false;
//
//    // ---- white pawn rules ----
//    if (p == 'P') {
//        // forward 1
//        if (toR == fromR - 1 && toC == fromC && board[toR][toC] == ' ')
//            return true;
//
//        // starting double step
//        if (fromR == 6 && toR == 4 && toC == fromC && board[5][fromC] == ' ' && board[4][fromC] == ' ')
//            return true;
//
//        // capture
//        if (toR == fromR - 1 && abs(toC - fromC) == 1 && board[toR][toC] == 'p')
//            return true;
//    }
//
//    // ---- black pawn rules ----
//    if (p == 'p') {
//        if (toR == fromR + 1 && toC == fromC && board[toR][toC] == ' ')
//            return true;
//
//        if (fromR == 1 && toR == 3 && toC == fromC && board[2][fromC] == ' ' && board[3][fromC] == ' ')
//            return true;
//
//        if (toR == fromR + 1 && abs(toC - fromC) == 1 && board[toR][toC] == 'P')
//            return true;
//    }
//
//
//    return false;
//}
//
//void makeMove(int fromR, int fromC, int toR, int toC)
//{
//    // TODO: move piece (capture if necessary), set source to empty.
//    board[toR][toC] = board[fromR][fromC];
//    board[fromR][fromC] = ' ';
//
//}