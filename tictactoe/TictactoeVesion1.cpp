/*
filename : TictactoeVersion1.cpp
playable 3x3 tic tac toe with X goes first
1. Ai engine
    - class Move
        + row, col
        + Print()
    - class State
        + state representation : vector<vector<char>> (3x3)
        + render (text screen)
        + check if game is final : isFinal()
        + char getNextPlayer() : find the next turn
        + getScoreFinal() : calculate Score if game is final
			* return 1 if computer victories
            * return -1 if computer defeats
        + getScore(char computer, char human) : calculate score of the state
		    if (checkFinal(s)) return score
            char nextplayer = getNextPlayer()
		    get Next States
		    if (nextplayer == computer) return max(score, getScore(computer, human))
		    if (nextplayer == human) // return min(score, getScore(computer, human))
		+ findMove()
		    get Next States
		    calculate the highest score of the next states -> save move
		    return move
        + play()
        + acionIfGameIsEnd()
2. User interface
    - Asking X or O : X goes first
	- Choose the number corresponding to the box to play
	    -------------
 	    | 1 | 2 | 3 |
	    -------------
 	    | 4 | 5 | 6 |
 	    -------------
 	    | 7 | 8 | 9 |
 	    -------------
	- Exception handling when input is invalid
	- Notify when the game is over
	- print current game
	- print computer's move
*/
#include <iostream>
#include <vector>
using namespace std;

typedef vector<vector<char>> Board;

const Board start = {{'1','2','3'},{'4','5','6'},{'7','8','9'}};
const Board empty = {{' ',' ',' '},{' ',' ',' '},{' ',' ',' '}};

class Move
{
public:
    int row, col;
    Move(int _row = 0, int _col = 0) {
        row = _row;
        col = _col;
    }
    void Print() {
        cout << row << " " << col << "\n";
    }
};

// convert chr to move
// input : chr from '1' to '9'
// output : turn corresponding
Move setMove(char chr) {
    int stt = chr - '0';
    int row = (stt - 1)/3;
    int col = (stt - 3*row - 1);
    Move res(row, col);
    return res;
}

class State
{
public:
    Board s; // state representation
    State(Board state) {
        s = state;
    }
    // print gamestate to text screen
    void render() {
        cout << " -------------" << endl;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cout << " | " << s[i][j];
            }
            cout << " |" << endl << " -------------" << endl;
        }
    }
    // get next player
    char getNextPlayer() {
        int countX = 0, countO = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (s[i][j] == 'X') countX++;
                else if (s[i][j] == 'O') countO++;
            }
        }
        if (countX == countO) return 'X';
        return 'O';
    }
    // check if game state current is end
    bool isFinal() {
        for (int i = 0; i < 3; i++) {
	        //check rows
		    if (s[i][0] != ' ' && s[i][0] == s[i][1] && s[i][1] == s[i][2]) return true;
	        // check cols
		    if (s[0][i] != ' ' && s[0][i] == s[1][i] && s[1][i] == s[2][i]) return true;
	    }
	    // check diagonals
	    if (s[0][0] != ' ' && s[0][0] == s[1][1] && s[1][1] == s[2][2]) return true;
	    if (s[0][2] != ' ' && s[0][2] == s[1][1] && s[1][1] == s[2][0]) return true;
	    // check full
	    for (int i = 0; i < 3; i++)
		    for (int j = 0; j < 3; j++) if (s[i][j] == ' ') return false;
	    return true;
    }
    // calculate Score if game is final
    // input : char computer, char human
    // output : 1 if X wins, -1 if X loses
    int getScoreFinal(char computer) {
	    for (int i = 0; i < 3; i++) {
		    if (s[i][0] != ' ' && s[i][0] == s[i][1] && s[i][1] == s[i][2]) if (s[i][0] == computer) return 1; else return -1;
		    if (s[0][i] != ' ' && s[0][i] == s[1][i] && s[1][i] == s[2][i]) if (s[0][i] == computer) return 1; else return -1;
	    }
	    if (s[0][0] != ' ' && s[0][0] == s[1][1] && s[1][1] == s[2][2]) if (s[0][0] == computer) return 1; else return -1;
	    if (s[0][2] != ' ' && s[0][2] == s[1][1] && s[1][1] == s[2][0]) if (s[0][2] == computer) return 1; else return -1;
	    return 0;
    }
    // check next turn is final ?
    // input : int row, col
    // ouput : true or false
    // bool nextStateIsFinal(int nextR, int nextC) {
    //     s[nextR][nextC] = getNextPlayer();
    //     return isFinal();
    // }
    // get score of the state
    int getScore(char computer, char human) {
        if (isFinal()) return getScoreFinal(computer);
        char nextPlayer = getNextPlayer();
        if (nextPlayer == computer) {
            int best = -10;
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    if (s[i][j] == ' ') {
                        s[i][j] = computer;
                        best = max(best, getScore(computer, human));
                        s[i][j] = ' ';
                    }
            return best;
        }
        else {
            int best = 10;
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    if (s[i][j] == ' ') {
                        s[i][j] = human;
                        best = min(best, getScore(computer, human));
                        s[i][j] = ' ';
                }
            return best;
        }
    }
    // return
    Move findMove(char computer, char human) {
        int bestScore = -10;
	    Move bestMove(-1,-1);
	    for (int i = 0; i < 3; i++) {
		    for (int j = 0; j < 3; j++) {
			    if (s[i][j] == ' ') {
				    s[i][j] = computer;
				    int Score = getScore(computer, human);
				    if (Score > bestScore) {
					    bestScore = Score;
					    bestMove.row = i;
					    bestMove.col = j;
				    }
				    s[i][j] = ' ';
			    }
		    }
	    }
	    return bestMove;
    }
    // play X or O into board
    void play(Move mov, char val) {
        s[mov.row][mov.col] = val;
    }
    // handle when the game is end
    void actionIfGameEnd(char computer) {
        cout << "END GAME.\n";
        int score = getScoreFinal(computer);
        if (score == 1) cout << "Computer wins!" << endl;
		else if (score == -1) cout << "You wins!" << endl;
		else cout << "Draw!" << endl;
    }
};


int main()
{
    State board1(start);
    char human, computer; bool running = true;
    board1.render();
    State board(empty);
    cout << "You choose the number corresponding to the box to play !!!" << "\n";
	cout << "Choose your player (X or O) : ";
    cin >> human;
    while(human != 'X' && human != 'O') {
        system("CLS");
        cout << "Error! You can choose 'X' or 'O'.\n";
        cout << "Choose again : ";  cin >> human;
    }
    computer = (human ^ 'X' ^ 'O');
    char chr; // dai dien cho nuoc di (from '1' to '9')
    while(running) {
        char nextTurn = board.getNextPlayer();
        if (nextTurn == human) {
            cout << "Enter your choice : ";
            while(true) {
                cin >> chr;
                if (chr < '1' || chr > '9') {
                    system("CLS"); board.render();
                    cout << "Error! Choose again : "; continue;
                }  else if (board.s[setMove(chr).row][setMove(chr).col] != ' ') {
                    system("CLS"); board.render();
                    cout << "Error! Choose again : "; continue;
                }
                break;
            }
            board.play(setMove(chr), nextTurn);
            system("CLS");
        } else {
            system("CLS");
            cout << "AI choose : ";
            Move mov = board.findMove(computer, human);
            cout << (3*mov.row + mov.col + 1) << "\n";
            board.play(mov, nextTurn);
        }
        board.render();
        if (board.isFinal()) {
            board.actionIfGameEnd(computer);
            return 0;
        }
    }
    return 0;
}
