#include <iostream>
#include <vector>
#include <limits>

class Game{
  public:
    bool AI_first_move = false;
    char temp_mark;
    std::vector<std::vector<char>> board;

    void AskFirstMove(){
        char choice;
        std::cout << "Do you want AI go first? (y/n): ";
        std::cin >> choice;
        if(choice == 'y' || choice == 'Y'){
            AI_first_move = true;
        } else {
            AI_first_move = false;
        }
    }

    void AunounceWinner(){
        std::cout << "Player " << temp_mark << " wins!\n";
    }

    void CreateBoard(){
        board.resize(3, std::vector<char>(3, ' '));
    }

    void PrintBoard(){
        std::cout << "-------------\n";
        for(const auto& row : board){
            for(const auto& cell : row){
                std::cout << "|" << cell;
            }
            std::cout << "|\n";
        }
        std::cout << "-------------\n";
    }

    void ResetBoard(){
        for(auto& row : board){
            std::fill(row.begin(), row.end(), ' ');
        }
    }

    void PlaceMark(int row, int col, char mark){
        temp_mark = mark;
        if(board[row][col] != ' '){
            std::cout << "Cell already occupied!\n";
            return;
        }

        if(row >= 0 && row < 3 && col >= 0 && col < 3){
            board[row][col] = mark;
            return;
        }

        else {
            std::cout << "Invalid position!\n";
            return;
        }
    }

    bool CheckWin(char mark){
        for(int i = 0; i < 3; ++i){
            if(board[i][0] == mark && board[i][1] == mark && board[i][2] == mark)
                return true;
            if(board[0][i] == mark && board[1][i] == mark && board[2][i] == mark)
                return true;
        }

        if(board[0][0] == mark && board[1][1] == mark && board[2][2] == mark)
            return true;
        if(board[0][2] == mark && board[1][1] == mark && board[2][0] == mark)
            return true;

        return false;
    }

    bool IsFull(){
        for(const auto& row : board){
            for(const auto& cell : row){
                if(cell == ' ')
                    return false;
            }
        }
        return true;
    }

    bool IsOver(){
        return CheckWin('x') || CheckWin('o') || IsFull();
    }

};

class Player{
  public:
    int row;
    int col;
    char mark;

    Player(char m) : mark(m) {}

    void TurnToValidIndex(int &input){
        input -= 1;
    }

    void GetMark(std::vector<std::vector<char>>& board){
        int temp_col;
        int temp_row;
        while(1) {
            std::cout << "Enter your mark position (row and column)(1~3): ";

            std::cin >> temp_row >> temp_col;

            TurnToValidIndex(temp_row);
            TurnToValidIndex(temp_col);

            if (temp_row < 0 || temp_row >= 3 || temp_col < 0 || temp_col >= 3) {
                std::cout << "Invalid input. Please enter values between 1 and 3.\n";
                continue;
            }

            if (board[temp_row][temp_col] != ' ') {
                std::cout << "Cell already occupied! Choose another position.\n";
                continue;
            }

            else break;

        }
        row = temp_row;
        col = temp_col;
    }

};

class AI_Player : public Player{
  //AI's mark default is 'x'

  public:
    AI_Player(char m) : Player(m) {}

    void Go_first (){
        row = 1;
        col = 1;
    }

    int minimax(Game &temp_game, char currentPlayer){
        if (temp_game.CheckWin(mark)) {  // AI wins
            return 1;
        }

        else if (temp_game.CheckWin(mark == 'X' ? 'O' : 'X')) {  // Opponent wins
            return -1;
        }

        else if (!temp_game.CheckWin(mark) && !temp_game.CheckWin(mark == 'X' ? 'O' : 'X') && temp_game.IsFull()) {  // Draw
            return 0;
        }

        if (currentPlayer == mark) {  // default mark of Ai is 'x'
            int value = std::numeric_limits<int>::min();
            //chess_board temp_board = board;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (temp_game.board[i][j] == ' ') {
                        temp_game.board[i][j] = currentPlayer;
                        value = std::max(value, minimax(temp_game, (currentPlayer == 'X' ? 'O' : 'X')));
                        temp_game.board[i][j] = ' ';
                    }
                }
            }
            return value;

        } 
        
        else {
            int value = std::numeric_limits<int>::max();
            //chess_board temp_board = board;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (temp_game.board[i][j] == ' ') {
                        temp_game.board[i][j] = currentPlayer;
                        value = std::min(value, minimax(temp_game, (currentPlayer == 'X' ? 'O' : 'X')));
                        temp_game.board[i][j] = ' ';
                    }
                }
            }
            return value;
        
        }

    }
  
    
    void NextMove(Game main_game) {
        int best_value = std::numeric_limits<int>::min();
        int best_row = -1;
        int best_col = -1;
        Game temp_game;
        temp_game.board = main_game.board;

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (temp_game.board[i][j] == ' ') {
                    temp_game.board[i][j] = mark;
                    int next_move_value = minimax(temp_game, (mark == 'X' ? 'O' : 'X'));
                    temp_game.board[i][j] = ' ';

                    if (next_move_value > best_value) {
                        best_row = i;
                        best_col = j;
                        best_value = next_move_value;
                    }
                }
            }
        }

        row = best_row;
        col = best_col;
    }


};

int main(){
    Game game;
    AI_Player playerX('X');
    AI_Player playerO('O');
    game.AskFirstMove();
    game.CreateBoard();
    if(game.AI_first_move) {
        playerX.Go_first();
        game.PlaceMark(playerX.row, playerX.col, playerX.mark);
    }

    game.PrintBoard();
    while(1) {

        playerO.NextMove(game);
        game.PlaceMark(playerO.row, playerO.col, playerO.mark);
        game.PrintBoard();
        if(game.IsOver()) {
            break;
        }

        //playerX.GetMark(game.board);
        playerX.NextMove(game);
        game.PlaceMark(playerX.row, playerX.col, playerX.mark);
        game.PrintBoard();
        if(game.IsOver()) {
            break;
        }

    }


    std::cout << "Game Over!\n";

    if(!game.CheckWin('X') && !game.CheckWin('O')) {
        std::cout << "It's a draw!\n";
    }

    else {
        game.AunounceWinner();
    }
    game.PrintBoard();
    return 0;

}