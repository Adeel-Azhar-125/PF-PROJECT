#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
using namespace sf;

const int SIZE = 8;
const int TILE_SIZE = 100;

char board[SIZE][SIZE];
int selectedRow = -1, selectedCol = -1;
bool pieceSelected = false;
bool whiteTurn = true;
bool gameOver = false;


Color lightColor(240, 217, 181);
Color darkColor(181, 136, 99);


// Textures & Sprites
Texture wpawn, wrook, wknight, wbishop, wqueen, wking;
Texture bpawn, brook, bknight, bbishop, bqueen, bking;


// Highlight arrays
int highlightMoves[SIZE][SIZE];



// -------------------- FUNCTIONS --------------------
int absInt(int x)
{
    if (x < 0)
    {
        x = -x;
    }
    return x;
}
bool white(char p)
{
    bool flag = false;
    if (p >= 'A' && p <= 'Z')
    {
        flag = true;
    }
    return flag;
}


bool black(char p)
{
    bool flag = false;
    if (p >= 'a' && p <= 'z')
    {
        flag = true;
    }
    return flag;
}

// ------------------- Board Initialization -------------------

void initializeBoard()
{
    for (int r = 0; r < SIZE; r++) 
    {
        for (int c = 0; c < SIZE; c++)
        {
            board[r][c] = '.';
        }
    }

    char blackRow[8] = { 'r','n','b','q','k','b','n','r' };
    char whiteRow[8] = { 'R','N','B','Q','K','B','N','R' };
    for (int c = 0; c < SIZE; c++)
    {
        board[0][c] = blackRow[c];
        board[1][c] = 'p';
        board[6][c] = 'P';
        board[7][c] = whiteRow[c];
    }
}
bool clearStraightPath(int fromR, int fromC, int toR, int toC)
{
    if (fromR == toR)
    {
        int step;
        if (toC > fromC)
        {
            step = 1;
        }
        else
        {
            step = -1;
        }
        for (int c = fromC + step; c != toC; c += step)
        {
            if (board[fromR][c] != '.')
            {
                return false;
            }
        }
        return true;
    }
    else if (fromC == toC)
    {
        int step;
        if (toR > fromR)
        {
            step = 1;
        }
        else
        {
            step = -1;
        }
        for (int r = fromR + step; r != toR; r += step)
        {
            if (board[r][fromC] != '.')
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool diagonalpath(int fromR, int fromC, int toR, int toC)
{
    int rowDiff = toR - fromR;
    int colDiff = toC - fromC;
    if (absInt(rowDiff) != absInt(colDiff)) return false;

    int stepR, stepC;
    if (toR > fromR)
    {
        stepR = 1;
    }
    else
    {
        stepR = -1;
    }
    if (toC > fromC)
    {
        stepC = 1;
    }
    else
    {
        stepC = -1;
    }
    int r = fromR + stepR, c = fromC + stepC;
    while (r != toR && c != toC) 
    {
        if (board[r][c] != '.')
        {
            return false;
        }
        r += stepR; c += stepC;
    }
    return true;
}

bool isInsideBoard(int r, int c)
{
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}



// ------------------- Move Validation -------------------
bool isvalidtoRookMove(int fromR, int fromC, int toR, int toC)
{
    char piece = board[fromR][fromC], Target = board[toR][toC];

    if ((white(piece) && white(Target)) || (black(piece) && black(Target)))
    {
        return false;
    }
    if (!(fromR == toR || fromC == toC))
    {
        return false;
    }
    if (!clearStraightPath(fromR, fromC, toR, toC))
    {
        return false;
    }
    return true;
}

bool isValidBishopMove(int fromR, int fromC, int toR, int toC)
{
    char piece = board[fromR][fromC], Target = board[toR][toC];
    if ((white(piece) && white(Target)) || (black(piece) && black(Target)))
    {
        return false;
    }
    if (absInt(toR - fromR) != absInt(toC - fromC))
    {
        return false;
    }
    if (!diagonalpath(fromR, fromC, toR, toC))
    {
        return false;
    }
    return true;
}

bool isValidKnightMove(int fromR, int fromC, int toR, int toC)
{
    char piece = board[fromR][fromC], Target = board[toR][toC];
    if ((white(piece) && white(Target)) || (black(piece) && black(Target)))
    {
        return false;
    }
    int rD = absInt(toR - fromR), cD = absInt(toC - fromC);
    {
        return (rD == 2 && cD == 1) || (rD == 1 && cD == 2);
    }
}

bool isValidQueenMove(int fromR, int fromC, int toR, int toC)
{
    bool flag = false;

    if (isvalidtoRookMove(fromR, fromC, toR, toC) || isValidBishopMove(fromR, fromC, toR, toC))
    {
        flag = true;
    }

    return flag;
}

bool isValidKingMove(int fromR, int fromC, int toR, int toC)
{
    char piece = board[fromR][fromC], Target = board[toR][toC];
    if ((white(piece) && white(Target)) || (black(piece) && black(Target)))
    {
        return false;
    }
    int rD = absInt(toR - fromR), cD = absInt(toC - fromC);
    return (rD <= 1 && cD <= 1);
}

bool isValidPawnMove(int fromR, int fromC, int toR, int toC)
{
    char piece = board[fromR][fromC], Target = board[toR][toC];
    if (piece == 'P')
    {
        if (toC == fromC && toR == fromR - 1 && Target == '.')
        {
            return true;
        }
        if (toC == fromC && fromR == 6 && toR == 4 && board[5][fromC] == '.' && Target == '.')
        {
            return true;
        }
        if ((toC == fromC - 1 || toC == fromC + 1) && toR == fromR - 1 && black(Target))
        {
            return true;
        }
    }
    else if (piece == 'p')
    {
        if (toC == fromC && toR == fromR + 1 && Target == '.')
        {
            return true;
        }
        if (toC == fromC && fromR == 1 && toR == 3 && board[2][fromC] == '.' && Target == '.')
        {
            return true;
        }
        if ((toC == fromC - 1 || toC == fromC + 1) && toR == fromR + 1 && white(Target)) 
        {
            return true;
        }
    }
    return false;
}

bool isValidMove(int fromR, int fromC, int toR, int toC, bool checkSafety = true)
{
    char piece = board[fromR][fromC];
    switch (piece) 
    {
    case 'R':
    case 'r':
    { 
        return isvalidtoRookMove(fromR, fromC, toR, toC); 
    }
    case 'B':
    case 'b':
    {
        return isValidBishopMove(fromR, fromC, toR, toC); 
    }
    case 'N':
    case 'n':
    {
        return isValidKnightMove(fromR, fromC, toR, toC); 
    }
    case 'Q':
    case 'q':
    { 
        return isValidQueenMove(fromR, fromC, toR, toC); 
    }
    case 'K': 
    case 'k':
    {
        return isValidKingMove(fromR, fromC, toR, toC);
    }
    case 'P':
    case 'p':
    {
        return isValidPawnMove(fromR, fromC, toR, toC);
    }
    default:
    { 
        return false; 
    }
    }
    return false;
}
bool isKingInCheck(bool whiteKing)
{
    char kingChar;
    if (whiteKing)
    {
        kingChar = 'K';
    }
    else
    {
        kingChar = 'k';
    }
    int kr = -1, kc = -1;

    for (int r = 0; r < SIZE ; r++) 
    {
        for (int c = 0; c < SIZE; c++)
        {
            if (board[r][c] == kingChar)
            {
                kr = r;
                kc = c;
                break; // exit inner loop
            }
        }
    }

    if (kr == -1)
    {
        return true;
    }// king not found, consider in check

    // Check if any opponent piece attacks the king
    for (int r = 0; r < SIZE; r++)
    {
        for (int c = 0; c < SIZE; c++) 
        {
            char p = board[r][c];
            if (p == '.')
            {
                continue;
            }

            if ((whiteKing && black(p)) || (!whiteKing && white(p))) 
            {
                if (isValidMove(r, c, kr, kc))
                {
                    return true; // king is under attack
                }
            }
        }
    }

    return false; // king is safe
}


// ------------------- Check if a move is safe for the king -------------------
bool isMoveSafe(int fromR, int fromC, int toR, int toC, bool whiteTurn)
{
    char fromPiece = board[fromR][fromC];
    char toPiece = board[toR][toC];

    // Simulate the move on the original board
    board[toR][toC] = fromPiece;
    board[fromR][fromC] = '.';

    // Check if king is in check
    bool safe = !isKingInCheck(whiteTurn);

    // Undo the move
    board[fromR][fromC] = fromPiece;
    board[toR][toC] = toPiece;

    return safe;
}
// ------------------- Compute legal moves -------------------

void computeLegalMovesForSelection(bool moves[SIZE][SIZE],int selectedRow, int selectedCol)
{
    for (int r = 0; r < SIZE; r++)
    {
        for (int c = 0; c < SIZE; c++)
        {
            moves[r][c] = false;
        }
    }

    if (!pieceSelected)
    {
        return;
    }

    for (int r = 0; r < SIZE; r++)
    {
        for (int c = 0; c < SIZE; c++)
        {
            if (!(r == selectedRow && c == selectedCol))
            {
                if (isValidMove(selectedRow, selectedCol, r, c) && (isMoveSafe(selectedRow, selectedCol, r, c, whiteTurn)))
                {
                    moves[r][c] = true;
                }
            }
        }
    }
}
bool hasAnyLegalMove(bool forWhite)
{
    for (int fromR = 0; fromR < SIZE; fromR++)
    {
        for (int fromC = 0; fromC < SIZE; fromC++)
        {
            char p = board[fromR][fromC];
            if (p == '.')
            {
                continue;
            }
            if (forWhite && !isupper(p))
            {
                continue;
            }
            if (!forWhite && !islower(p))
            {
                continue;
            }


            for (int tr = 0; tr < SIZE; tr++)
            {
                for (int tc = 0; tc < SIZE; tc++)

                {
                    if (!(fromR == tr && fromC == tc))
                    {
                        if (isValidMove(fromR, fromC, tr, tc))
                        {
                            if (isMoveSafe(fromR, fromC, tr, tc, forWhite))
                            {
                                return true; // legal move exists
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

Texture& textureForChar(char p)
{
    if (p == 'P')
    {
        return wpawn;
    }
    if (p == 'R')
    {
        return wrook;
    }
    if (p == 'N')
    {
        return wknight;
    }
    if (p == 'B')
    {
        return wbishop;
    }
    if (p == 'Q')
    {
        return wqueen;
    }
    if (p == 'K')
    {
        return wking;
    }
    if (p == 'p')
    {
        return bpawn;
    }
    if (p == 'r')
    {
        return brook;
    }
    if (p == 'n')
    {
        return bknight;
    }
    if (p == 'b')
    {
        return bbishop;
    }
    if (p == 'q')
    {
        return bqueen;
    }
    if (p == 'k')
    {
        return bking;
    }
    return wpawn; // fallback
}

// ------------------- Draw board, pieces, highlights -------------------
void drawBoard(RenderWindow& window)
{
    Font font;
    font.loadFromFile("images/text.otf"); // make sure it exists

    for (int r = 0; r < SIZE; r++)
    {
        for (int c = 0; c < SIZE; c++)
        {
            RectangleShape rect(Vector2f(TILE_SIZE, TILE_SIZE));
            rect.setPosition(c * TILE_SIZE, r * TILE_SIZE);
            rect.setFillColor((r + c) % 2 == 0 ? Color(240, 230, 140) : Color(34, 139, 34));
            window.draw(rect);
        }
    }

    /*  rank numbers*/
    for (int r = 0; r < SIZE; r++)
    {
        Text rank(to_string(SIZE - r), font, 20);
        rank.setFillColor(Color::Black);
        rank.setPosition(2, r * TILE_SIZE + 5);
        window.draw(rank);
    }

    // file letters
    for (int c = 0; c < SIZE; c++)
    {
        char letter = 'a' + c;
        Text file(string(1, letter), font, 20);
        file.setFillColor(Color::Black);
        file.setPosition(c * TILE_SIZE + TILE_SIZE - 20, SIZE * TILE_SIZE - 20);
        window.draw(file);
    }
}

void drawPieces(RenderWindow& window)
{
    for (int r = 0; r < SIZE; r++)
    {
        for (int c = 0; c < SIZE; c++)
        {
            char p = board[r][c];
            if (p == '.')
            {
                continue;
            }
            Sprite spr;
            spr.setTexture(textureForChar(p));
            spr.setPosition(c * TILE_SIZE + 12, r * TILE_SIZE + 12);
            spr.setScale(0.6f, 0.6f);
            window.draw(spr);
        }
    }
}

void drawHighlights(RenderWindow& window, bool moves[SIZE][SIZE],int selectedRow,int selectedCol)
{
    if (pieceSelected)
    {
        

        RectangleShape sel(Vector2f(TILE_SIZE - 4, TILE_SIZE - 4));
        sel.setPosition(selectedCol * TILE_SIZE + 2, selectedRow * TILE_SIZE + 2);
        sel.setFillColor(Color(0, 0, 0, 0));
        sel.setOutlineThickness(3);
        sel.setOutlineColor(Color::Yellow);
        window.draw(sel);

        for (int r = 0; r < SIZE; r++)
        {
            for (int c = 0; c < SIZE; c++)
            {
                if (moves[r][c])
                {
                    RectangleShape highlight(Vector2f(TILE_SIZE - 4, TILE_SIZE - 4));
                    highlight.setPosition(c * TILE_SIZE + 2, r * TILE_SIZE + 2);
                    highlight.setFillColor(Color(255, 0, 0, 100));
                    window.draw(highlight);
                }
            }
        }
    }
}
// Function to handle pawn promotion GUI
char showPromotionMenu(RenderWindow& window, bool isWhite)
{
    // Draw semi-transparent overlay
    RectangleShape overlay(Vector2f(TILE_SIZE * 4, TILE_SIZE));
    overlay.setPosition(TILE_SIZE * 2, TILE_SIZE * 3);
    overlay.setFillColor(Color(50, 50, 50, 200));
    window.draw(overlay);

    // Load textures
    Texture q, r, b, n;
    if (isWhite) 
    {
        q.loadFromFile("images/white-queen.png");
        r.loadFromFile("images/white-rook.png");
        b.loadFromFile("images/white-bishop.png");
        n.loadFromFile("images/white-knight.png");
    }
    else 
    {
        q.loadFromFile("images/black-queen.png");
        r.loadFromFile("images/black-rook.png");
        b.loadFromFile("images/black-bishop.png");
        n.loadFromFile("images/black-knight.png");
    }

    Sprite sq(q), sr(r), sb(b), sn(n);
    sq.setPosition(TILE_SIZE * 2, TILE_SIZE * 3); sq.setScale(0.6f, 0.6f);
    sr.setPosition(TILE_SIZE * 3, TILE_SIZE * 3); sr.setScale(0.6f, 0.6f);
    sb.setPosition(TILE_SIZE * 4, TILE_SIZE * 3); sb.setScale(0.6f, 0.6f);
    sn.setPosition(TILE_SIZE * 5, TILE_SIZE * 3); sn.setScale(0.6f, 0.6f);

    window.draw(sq);
    window.draw(sr);
    window.draw(sb);
    window.draw(sn);

    window.display();

    // Wait for player click
    Event event;
    while (true)
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                if (y >= TILE_SIZE * 3 && y < TILE_SIZE * 4)
                {
                    if (x >= TILE_SIZE * 2 && x < TILE_SIZE * 3) return isWhite ? 'Q' : 'q';
                    else if (x >= TILE_SIZE * 3 && x < TILE_SIZE * 4) return isWhite ? 'R' : 'r';
                    else if (x >= TILE_SIZE * 4 && x < TILE_SIZE * 5) return isWhite ? 'B' : 'b';
                    else if (x >= TILE_SIZE * 5 && x < TILE_SIZE * 6) return isWhite ? 'N' : 'n';
                }
            }
        }
    }
}


// ------------------- Score Saving -------------------
void saveScore(const string& winner)
{
    fstream file;
    file.open("chess_scores.txt", ios::out | ios::app);
    if (!file.is_open())
    {
        cout << "Error opening score file!\n"; 
        return; 
    }
    if (winner == "White")
    {
        file << "White wins\n";
    }
    else if (winner == "Black")
    {
        file << "Black wins\n";
    }
    else if (winner == "Draw")
    {
         file << "Draw\n";
    }
    file.close();
}

void displayScores() 
{
    ifstream file("chess_scores.txt");
    if (!file.is_open())
    {
        cout << "No scores yet.\n";
        return; 
    }
    string line; 
    int whiteWins = 0, blackWins = 0, draws = 0;
    while (getline(file, line)) 
    {
        if (line == "White wins")
        {
            whiteWins++;
        }
        else if (line == "Black wins")
        {
            blackWins++;
        }
        else if (line == "Draw")
        {
            draws++;
        }
    }
    file.close();
    cout << "---- Scoreboard ----\n";
    cout << "White wins: " << whiteWins << "\nBlack wins: " << blackWins << "\nDraws: " << draws << "\n";
    cout << "-------------------\n";
}

int main()
{
    initializeBoard();

    // Load textures
    if (!wpawn.loadFromFile("images/white-pawn.png") || !wrook.loadFromFile("images/white-rook.png") ||
        !wknight.loadFromFile("images/white-knight.png") || !wbishop.loadFromFile("images/white-bishop.png") ||
        !wqueen.loadFromFile("images/white-queen.png") || !wking.loadFromFile("images/white-king.png") ||
        !bpawn.loadFromFile("images/black-pawn.png") || !brook.loadFromFile("images/black-rook.png") ||
        !bknight.loadFromFile("images/black-knight.png") || !bbishop.loadFromFile("images/black-bishop.png") ||
        !bqueen.loadFromFile("images/black-queen.png") || !bking.loadFromFile("images/black-king.png"))
    {
        cout << "Error loading textures!\n";
        return 0;
    }
    // Load move sound
    SoundBuffer moveBuffer;
    if (!moveBuffer.loadFromFile("images/music.mp3")) cout << "Error loading move sound!\n";
    Sound moveSound(moveBuffer);

    RenderWindow window(VideoMode(TILE_SIZE * SIZE, TILE_SIZE * SIZE), "SFML Chess by ADEEL AZHAR", Style::Titlebar | Style::Close);
    
    bool moves[SIZE][SIZE] = { false };
    bool waitingPromotion = false; 
    int promotionRow = -1, promotionCol = -1;
    char promotionPiece = 'Q';

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            // --- Graphical Promotion ---
            if (waitingPromotion && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                int x = event.mouseButton.x, y = event.mouseButton.y;
                int squareSize = TILE_SIZE;
                if (y >= 0 && y < squareSize)
                { 
                    if (x < squareSize)
                    {
                        promotionPiece = 'Q';
                    }
                    else if (x < 2 * squareSize)
                    {
                        promotionPiece = 'R';
                    }
                    else if (x < 3 * squareSize)
                    {
                        promotionPiece = 'B';
                    }
                    else
                    {
                        promotionPiece = 'N';
                    }
                }
                else if(y >= 4 * squareSize && y < 5 * squareSize) 
                { 
                    if (x < squareSize)
                    {
                        promotionPiece = 'q';
                    }
                    else if (x < 2 * squareSize)
                    {
                        promotionPiece = 'r';
                    }
                    else if (x < 3 * squareSize)
                    {
                        promotionPiece = 'b';
                    }
                    else
                    {
                        promotionPiece = 'n';
                    }
                }
                board[promotionRow][promotionCol] = promotionPiece;
                waitingPromotion = false; whiteTurn = !whiteTurn; promotionRow = promotionCol = -1;
            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && !waitingPromotion)

            {
                int row = max(0, min(7, event.mouseButton.y / TILE_SIZE));
                int col = max(0, min(7, event.mouseButton.x / TILE_SIZE));
                char clicked = board[row][col];

                if (!pieceSelected)
                {
                    if ((whiteTurn && white(clicked)) || (!whiteTurn && black(clicked)))
                    {
                        selectedRow = row;
                        selectedCol = col;
                        pieceSelected = true;
                        computeLegalMovesForSelection(moves, selectedRow, selectedCol);
                        cout << "Selected " << clicked << " at (" << SIZE - selectedRow << "," << selectedCol + 1 << ")\n";
                    }
                }
                else
                {
                    if (row == selectedRow && col == selectedCol)
                    {
                        pieceSelected = false;
                        selectedRow = selectedCol = -1;
                        memset(moves, 0, sizeof(moves));
                        cout << "Deselected.\n";
                    }
                    else if (moves[row][col])
                    {
                        char fromPiece = board[selectedRow][selectedCol];

                        // Make the move
                        board[row][col] = fromPiece;
                        board[selectedRow][selectedCol] = '.';
                        // Play move sound
                        moveSound.play();
                        if (fromPiece == 'K' && selectedRow == 7 && selectedCol == 4 && row == 7 && (col == 6 || col == 2))
                        {
                            if (col == 6) // King-side
                            {
                                board[7][5] = board[7][7]; board[7][7] = '.';
                            }
                            else if (col == 2) // Queen-side
                            {
                                board[7][3] = board[7][0]; board[7][0] = '.';
                            }
                        }
                        if (fromPiece == 'k' && selectedRow == 0 && selectedCol == 4 && row == 0 && (col == 6 || col == 2))
                        {
                            if (col == 6) board[0][5] = board[0][7], board[0][7] = '.';
                            else if (col == 2) board[0][3] = board[0][0], board[0][0] = '.';
                        }

                        // Pawn promotion
                        if ((fromPiece == 'P' && row == 0) || (fromPiece == 'p' && row == 7))
                        {
                            bool isWhitePawn = (fromPiece == 'P');
                            char promotionPiece = showPromotionMenu(window, isWhitePawn);
                            board[row][col] = promotionPiece;
                        }

                        pieceSelected = false;
                        selectedRow = selectedCol = -1;
                        memset(moves, 0, sizeof(moves));

                        // Switch turn
                        whiteTurn = !whiteTurn;

                        // Check opponent status
                        bool opponentKingInCheck = isKingInCheck(whiteTurn);
                        bool opponentHasMoves = hasAnyLegalMove(whiteTurn);

                        if (opponentKingInCheck && !opponentHasMoves)
                        {
                            if (whiteTurn)
                            {
                                cout << "White is in CHECKMATE!\n";
                                cout << "GAME ENDED! Winner: Black\n";
                                saveScore("Black");
                            }
                            else
                            {
                                cout << "Black is in CHECKMATE!\n";
                                cout << "GAME ENDED! Winner: White\n";
                                saveScore("White");
                            }
                            gameOver = true; // optional, if you want to stop further moves
                        }
                        else if (!opponentKingInCheck && !opponentHasMoves)
                        {
                            cout << (whiteTurn ? "White" : "Black") << " is in STALEMATE!\n";
                            cout << "Game Ended! No One Wins(Game Draw)\n";
                            gameOver = true;
                            saveScore("Draw");
                        }
                        else if (opponentKingInCheck)
                        {
                            if (whiteTurn)
                            {
                                cout << "White KING IS IN CHECK!\n";
                            }
                            else
                            {
                                cout << "Black KING IS IN CHECK!\n";
                            }
                        }
                        else
                        {
                            if (whiteTurn)
                            {
                                cout << "White's turn.\n";
                            }
                            else
                            {
                                cout << "Black's turn.\n";
                            }
                        }


                    }
                    else
                    {
                        // select a different piece of same color
                        if ((whiteTurn && white(clicked)) || (!whiteTurn && black(clicked)))
                        {
                            selectedRow = row;
                            selectedCol = col;
                            pieceSelected = true;
                            computeLegalMovesForSelection(moves, selectedRow, selectedCol);
                            cout << "Selected " << clicked << " at (" << SIZE - selectedRow << "," << selectedCol + 1 << ")\n";
                        }
                        else
                        {
                            cout << "Not a legal move.\n";
                        }
                    }
                    if (gameOver)
                    {
                        pieceSelected = false;
                        memset(moves, 0, sizeof(moves));
                    }
                }
            }
        }
        // to draw board
        window.clear();
        drawBoard(window);
        drawHighlights(window, moves,selectedRow,selectedCol);
        drawPieces(window);

        window.display();
    }

    return 0;
}
