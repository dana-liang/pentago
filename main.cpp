// CS 141 Program 5 by Dana Liang

#include <iostream>
#include <cctype>     // for toupper()
using namespace std;

//------------------------------------------------------------------------------------
// Display Pentago instructions.
void displayInstructions() {
    cout << "Play the two-player game of Pentago. Be the first to get 5 in a row. \n"
            "                                                                 \n"
            "Pentago is played on a 6 by 6 board, divided into four 3 by 3    \n"
            "quadrants.  There are two players, X and O, who alternate turns. \n"
            "The goal of each player is to get five of their pieces in a row, \n"
            "either horizontally, vertically, or diagonally.                  \n"
            "                                                                 \n"
            "Players take turns placing one of their pieces into an empty     \n"
            "space anywhere on the board, then choosing one of the four       \n"
            "board quadrants to rotate 90 degrees left or right.              \n"
            "                                                                 \n"
            "If both players get five in a row at the same time, or the       \n"
            "last move is played with no five in a row, the game is a tie.    \n"
            "If a player makes five a row by placing a piece, there is no need\n"
            "to rotate a quadrant and the player wins immediately.            \n"
            "                                                                 \n"
            "     Play online at:  https://perfect-pentago.net                \n"
            "     Purchase at:     www.mindtwisterusa.com                     \n"
            "                                                                 \n"
            "For each move provide four inputs:                               \n"
            "   row (A-F), column (1-6), quadrant (1-4), rotation direction (L or R) \n"
            "For instance input of B32R places the next piece at B3 and then  \n"
            "would rotate quadrant 2 (upper-right) to the right (clockwise).  \n"
            "                                                                 \n"
            "At any point enter 'x' to exit the program or 'i' to display instructions.\n" << endl;
} // END displayInstructions()

//------------------------------------------------------------------------------------
// Class that will represent the board.
class Board {
    public:
        Board();    // Default constructor
        void displayBoard(); // display the board
        void makeAMove(char row, char col, char quadrant, char direction); // function to make a move
        bool moveCheck(char row, char col, char quadrant, char direction); // check if move is valid
        bool winCheck(); // check for a win
        bool fullCheck(); // check if board is full
    private:
        char p[6][6]; //2d char array for storing board pieces
        int turnCounter; // Counter for the turn the game is currently at
        void playerPrompt(int num); // display prompt for the player
        void rotate(char quadrant, char direction); // rotate quadrants as needed (part of makeAMove)
        bool xWin(); // Check for an 'X' win
        bool oWin(); // Check for an 'O' win
        bool rowWin(char winningPiece);
        bool colWin(char winningPiece);
        bool diagLRWin(char winningPiece);
        bool diagRLWin(char winningPiece);
};

//CLASS MEMBER FUNCTION DEFINITIONS ---------------------------------------------------
// Default constructor. Set all board pieces to '.'
Board::Board() {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            p[i][j] = '.';
        }
    }
    turnCounter = 1;
}

//------------------------------------------------------------------------------------
// Display the board using the pieces char array. At the beginning of the game the board looks like:
/*
        1   2   3   4   5   6
      1-----------------------2
    A | .   .   . | .   .   . | A
      |           |           |
    B | .   .   . | .   .   . | B
      |           |           |
    C | .   .   . | .   .   . | C
      |-----------+-----------|
    D | .   .   . | .   .   . | D
      |           |           |
    E | .   .   . | .   .   . | E
      |           |           |
    F | .   .   . | .   .   . | F
      3-----------------------4
        1   2   3   4   5   6
*/
void Board::displayBoard() {
    if (xWin() && oWin()) { // If there's a tie
        cout << "     *** Both X and O have won.  Game is a tie." << endl;
    }

    else if (winCheck()) {
        if ((turnCounter - 1) % 2 == 1) { // If the last turn was odd, it's X's win.
            cout << "    *** X has won the game!" << endl;
        }

        else if ((turnCounter - 1) % 2 == 0) { // If the turn was even, it's O's win.
            cout << "    *** O has won the game!" << endl;
        }
    }

    else if (fullCheck()) { // If the board is full and it's a draw
        cout << "     *** No one has won.  Game is a tie." << endl;
    }

    cout << "    1   2   3   4   5   6   " << endl
         << "  1-----------------------2 " << endl
         << "A | " << p[0][0] << "   " << p[0][1] << "   " << p[0][2] << " | "
         << p[0][3] << "   " << p[0][4] << "   " << p[0][5] << " | A" << endl
         << "  |           |           | " << endl
         << "B | "<< p[1][0] <<"   "<< p[1][1] <<"   "<< p[1][2] <<" | "
         << p[1][3]<<"   "<< p[1][4]<<"   "<< p[1][5]<<" | B"<< endl
         << "  |           |           | " << endl
         << "C | "<< p[2][0] <<"   "<< p[2][1] <<"   "<< p[2][2] <<" | "
         << p[2][3]<<"   "<< p[2][4]<<"   "<< p[2][5]<<" | C"<< endl
         << "  |-----------+-----------| " << endl
         << "D | "<< p[3][0] <<"   "<< p[3][1] <<"   "<< p[3][2] <<" | "
         << p[3][3]<<"   "<< p[3][4]<<"   "<< p[3][5]<<" | D"<< endl
         << "  |           |           | " << endl
         << "E | "<< p[4][0] <<"   "<< p[4][1] <<"   "<< p[4][2] <<" | "
         << p[4][3]<<"   "<< p[4][4]<<"   "<< p[4][5]<<" | E"<< endl
         << "  |           |           | " << endl
         << "F | "<< p[5][0] <<"   "<< p[5][1] <<"   "<< p[5][2] <<" | "
         << p[5][3]<<"   "<< p[5][4]<<"   "<< p[5][5]<<" | F"<< endl
         << "  3-----------------------4 " << endl
         << "    1   2   3   4   5   6   " << endl;

    if (!fullCheck() && !winCheck()) { // prompt the next player if the board is not yet full and no one has won yet
        playerPrompt(turnCounter);
    }
} // END Board::displayBoard()

//------------------------------------------------------------------------------------
// Print instructions for the next turn.
void Board::playerPrompt(int num) {
    if (num % 2 == 1) { // If the turn is odd, it's X's turn.
        cout << num << ". Enter row, column, quadrant, direction for X:";
    }

    else if (num % 2 == 0) { // If the turn is even, it's O's turn.
        cout << num << ". Enter row, column, quadrant, direction for O:";
    }
} // END Board::playerPrompt()

//------------------------------------------------------------------------------------
// Check if move is valid.
bool Board::moveCheck(char row, char col, char quadrant, char direction) {
    bool isValid = true;
    int rowIndex = 0, colIndex = 0;

    // Translate row char input from player to the row index of the 2d pieces array
    if (row == 'a') { rowIndex = 0;}
    else if (row == 'b') { rowIndex = 1;}
    else if (row == 'c') { rowIndex = 2;}
    else if (row == 'd') { rowIndex = 3;}
    else if (row == 'e') { rowIndex = 4;}
    else if (row == 'f') { rowIndex = 5;}

    // Translate column input from player to the column index of the 2d pieces array
    if (col == '1') { colIndex = 0;}
    else if (col == '2') { colIndex = 1;}
    else if (col == '3') { colIndex = 2;}
    else if (col == '4') { colIndex = 3;}
    else if (col == '5') { colIndex = 4;}
    else if (col == '6') { colIndex = 5;}

    if ((row < 'a') || (row > 'f')) { // If the row is anything other than a-f
        cout << "    *** Invalid move row.  Please retry." << endl;
        isValid = false;
        displayBoard();
    }

    else if ((col < '1') || (col > '6')) { // If the column is anything other than 1-6
        cout << "    *** Invalid move column.  Please retry." << endl;
        isValid = false;
        displayBoard();
    }

    else if ((quadrant < '1') || (quadrant > '4')) { // If the quadrant is anything other than 1-4
        cout << "    *** Selected quadrant is invalid.  Please retry. " << endl;
        isValid = false;
        displayBoard();
    }

    else if ((direction != 'r') && (direction != 'l')) { // If the direction is anything other than l or r
        cout << "    *** Quadrant rotation direction is invalid.  Please retry. " << endl;
        isValid = false;
        displayBoard();
    }

    else if (p[rowIndex][colIndex] != '.') { // If the location already has a piece in it
        cout << "   *** That board location is already taken.  Please retry." << endl;
        isValid = false;
        displayBoard();
    }

    return isValid;
} // END Board::moveCheck

//------------------------------------------------------------------------------------
// Function to allow player to make a move according to the rules of Pentago
void Board::makeAMove(char row, char col, char quadrant, char direction) {
    char pieceToPlay;
    int rowIndex, colIndex;

    if (turnCounter % 2 == 1) { // If the turn is odd, it's X's turn.
        pieceToPlay = 'X';
    }
    else if (turnCounter % 2 == 0) { // If the turn is even, it's O's turn.
        pieceToPlay = 'O';
    }

    // Translate row char input from player to the row index of the 2d pieces array
    if (row == 'a') { rowIndex = 0;}
    else if (row == 'b') { rowIndex = 1;}
    else if (row == 'c') { rowIndex = 2;}
    else if (row == 'd') { rowIndex = 3;}
    else if (row == 'e') { rowIndex = 4;}
    else if (row == 'f') { rowIndex = 5;}

    // Translate column input from player to the column index of the 2d pieces array
    if (col == '1') { colIndex = 0;}
    else if (col == '2') { colIndex = 1;}
    else if (col == '3') { colIndex = 2;}
    else if (col == '4') { colIndex = 3;}
    else if (col == '5') { colIndex = 4;}
    else if (col == '6') { colIndex = 5;}

    // Put the piece to be played into the requested row and column of the board.
    p[rowIndex][colIndex] = pieceToPlay;

    if (!winCheck()) { // If the spot where the piece is placed DOES NOT win the game, rotate.
        rotate(quadrant, direction);
    }
    turnCounter++;
} // END Board::makeAMove()

//------------------------------------------------------------------------------------
// Rotates the quadrant of the player's choice right or left
void Board::rotate(char quadrant, char direction) {
    char temp1, temp2; //temporary characters to place the last elements to be swapped to prevent data loss
    int row = 0, col = 0; //will hold index of the lower bounds of the rows and cols in quadrants

    if (quadrant == '1') { // Top left of quadrant 1 is p[0][0]
        row = col = 0;
    }
    else if (quadrant == '2') { // Top left of quadrant 2 is p[0][3]
        row = 0;
        col = 3;
    }
    else if (quadrant == '3') { // Top left of quadrant 3 is p[3][0]
        row = 3;
        col = 0;
    }
    else if (quadrant == '4') { // Top left of quadrant 4 is p[3][3]
        row = col = 3;
    }

    temp1 = p[row][col]; // Holds the top left corner piece
    temp2 = p[row][col + 1]; // Holds the middle piece to the right of the top left piece
    if (direction == 'r') { // Rotate the quadrant right. Center piece stays the same.
        // Rotate the corners
        p[row][col] = p[row + 2][col];
        p[row + 2][col] = p[row + 2][col + 2];
        p[row + 2][col + 2] = p[row][col + 2];
        p[row][col + 2] = temp1;

        //Rotate the middle pieces
        p[row][col + 1] = p[row + 1][col];
        p[row + 1][col] = p[row + 2][col + 1];
        p[row + 2][col + 1] = p[row + 1][col + 2];
        p[row + 1][col + 2] = temp2;
    }
    else if (direction == 'l') { // Rotate the quadrant left. Center piece stays the same.
        // Rotate the corners
        p[row][col] = p[row][col + 2];
        p[row][col + 2] = p[row + 2][col + 2];
        p[row + 2][col + 2] = p[row + 2][col];
        p[row + 2][col] = temp1;

        //Rotate the middle pieces
        p[row][col+ 1] = p[row + 1][col + 2];
        p[row + 1][col + 2] = p[row + 2][col + 1];
        p[row + 2][col + 1] = p[row + 1][col];
        p[row + 1][col] = temp2;
    }
} // END Board::rotate()

//------------------------------------------------------------------------------------
// Check if the board is full.
bool Board::fullCheck() {
    bool full = true;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (p[i][j] == '.') {
                full = false;
                break;
            }
        }
    }

    return full;
} // END Board::fullCheck()

//------------------------------------------------------------------------------------
// Check for a win (5 like pieces in any line on the board: rows, columns, or diagonal).
bool Board::winCheck() {
    return xWin() || oWin();
} // END Board::winCheck()

//CHECKING FOR WINS-------------------------------------------------------------------
// ROW win check in all 6 rows
bool Board::rowWin(char winningPiece) {
    int counter = 0;
    for (int i = 0; i < 6; i++) {
        counter = 0; // Reset count at the beginning of every row
        for (int j = 0; j < 6; j++) {
            if ((p[i][j] == p[i][j+1]) && (p[i][j] == winningPiece)) {
                counter++; // Increment counter if consecutive pairs are found in a row
            }
            if (counter == 4) {
                return true;
            }
        }
    }
    return false;
} // END Board::rowWin()

//------------------------------------------------------------------------------------
// COL win check in all 6 cols
bool Board::colWin(char winningPiece) {
    int counter = 0;
    for (int j = 0; j < 6; j++) {
        counter = 0; // Reset count at the beginning of every col
        for (int i = 0; i < 6; i++) {
            if ((p[i][j] == p[i+1][j]) && (p[i][j] == winningPiece)) {
                counter++;      // Increment counter if consecutive pairs are found in a col
            }
            if (counter == 4) {
                return true;
            }
        }
    }
    return false;
} // END Board::colWin()

//------------------------------------------------------------------------------------
// DIAGONAL win check in diagonals from top left to bottom right
bool Board::diagLRWin(char winningPiece) {
    int counter = 0;

    // Checking for DIAGONAL wins, top left corner to bottom right corner.
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 1; j++) {
            if ((p[i][i] == p[i+1][i+1]) && (p[i][i] == winningPiece)) {
                counter++;     // Increment counter if consecutive pairs are found in this diagonal
            }
            if (counter == 4) {
                return true;
            }
        }
    }

    // Checking for DIAGONAL win from p[0][1] to p[4][5]
    counter = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 1; j++) {
            if ((p[i][i+1] == p[i+1][i+2]) && (p[i][i+1] == winningPiece)) {
                counter++;     // Increment counter if consecutive pairs are found in this diagonal
            }
            if (counter == 4) {
                return true;
            }
        }
    }

    // Checking for DIAGONAL win from p[1][0] to p[5][4]
    counter = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 1; j++) {
            if ((p[i+1][i] == p[i+2][i+1]) && (p[i+1][i] == winningPiece)) {
                counter++;     // Increment counter if consecutive pairs are found in this diagonal
            }
            if (counter == 4) {
                return true;
            }
        }
    }

    return false;
} // END Board::diagWin()

//------------------------------------------------------------------------------------
// DIAGONAL win check in diagonals from top right to bottom left
bool Board::diagRLWin(char winningPiece) {
    int counter = 0;

    // Checking for DIAGONAL wins, top right corner to bottom left corner
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 1; j++) {
            if ((p[i][5-i] == p[i+1][5-(i+1)]) && (p[i][5-i] == winningPiece)) {
                counter++;     // Increment counter if consecutive pairs are found in this diagonal
            }
            if (counter == 4) {
                return true;
            }
        }
    }

    // Checking for DIAGONAL win from p[0][4] to p[4][0]
    counter = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 1; j++) {
            if ((p[i][4-i] == p[i+1][4-(i+1)]) && (p[i][4-i] == winningPiece)) {
                counter++;     // Increment counter if consecutive pairs are found in this diagonal
            }
            if (counter == 4) {
                return true;
            }
        }
    }

    // Checking for DIAGONAL win from p[1][5] to p[5][1]
    counter = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 1; j++) {
            if ((p[i+1][5-i] == p[i+2][5-(i+1)]) && (p[i+1][5-i] == winningPiece)) {
                counter++;     // Increment counter if consecutive pairs are found in this diagonal
            }
            if (counter == 4) {
                return true;
            }
        }
    }

    return false;
} // END Board::diagRLwin()

//------------------------------------------------------------------------------------
// Check for an 'X' win. (5 'X' pieces in any line on the board: rows, columns, or diagonal).
bool Board::xWin() {
    char winningPiece = 'X';

    if (rowWin(winningPiece)) { return true;} // Checking for ROW wins.

    if (colWin(winningPiece)) { return true;} // Checking for COL wins.

    if (diagLRWin(winningPiece)) { return true;} // Checking for DIAGONAL wins, top L to bottom R

    if (diagRLWin(winningPiece)) { return true;} // Checking for DIAGONAL wins, top R to bottom L

    return false;
} // END Board::xWin()

//------------------------------------------------------------------------------------
// Check for an 'O' win. (5 'O' pieces in any line on the board: rows, columns, or diagonal).
bool Board::oWin() {
    char winningPiece = 'O';

    if (rowWin(winningPiece)) { return true;} // Checking for ROW wins.

    if (colWin(winningPiece)) { return true;} // Checking for COL wins.

    if (diagLRWin(winningPiece)) { return true;} // Checking for DIAGONAL wins, top L to bottom R

    if (diagRLWin(winningPiece)) { return true;} // Checking for DIAGONAL wins, top R to bottom L

    return false;
} // END Board::oWin()

//BEGIN MAIN()------------------------------------------------------------------------------------
int main() {
    Board game; // Create a Board object named game. Will represent the Pentago board.
    char playerChoice, row, col, quadrant, direction;

    cout << "Welcome to Pentago, where you try to get 5 of your pieces in a line.\n"
            "At any point enter 'x' to exit or 'i' for instructions. \n" << endl;
    game.displayBoard();
    cin >> playerChoice;
    playerChoice = tolower(playerChoice);

    while (playerChoice != 'x' && !game.fullCheck()) {
        if (playerChoice == 'i') { // Display instructions if player enters 'i'.
            displayInstructions();
            game.displayBoard();

            cin >> playerChoice;
            playerChoice = tolower(playerChoice);
            if (playerChoice == 'x') { // Break loop and exit if player enters 'x'.
                break;
            }
            else if (playerChoice == 'i') { // Display instructions again if player enters 'i'.
                continue;
            }
        }

        // Read in all required inputs: row, col, quadrant, direction.
        row = playerChoice; // create a copy of playerChoice for clarity
        cin >> col;
        cin >> quadrant;
        cin >> direction;
        direction = tolower(direction);
        cout << endl;

        if (game.moveCheck(row, col, quadrant, direction)) { // if the move is valid, make the move.
            game.makeAMove(row, col, quadrant, direction);
            game.displayBoard();
        }

        if (game.winCheck() || game.fullCheck()) { // if someone won or if the game is full
            cout << "Thanks for playing.  Exiting... ";
            break;
        }

        if (!game.fullCheck()) { // read in input again if the game is still not full
            cin >> playerChoice;
            playerChoice = tolower(playerChoice);
        }
    }

    if (!game.winCheck() && !game.fullCheck()) {
        cout << "Exiting program...";
    }
    return 0;
}