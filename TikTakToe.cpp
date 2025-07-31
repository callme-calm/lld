#include <iostream>
#include <vector>
#include <memory>

using namespace std;

enum class Symbol
{
    X,
    O,
    EMPTY
};

string symbolToStr(Symbol s)
{
    switch (s)
    {
    case Symbol::X:
        return "X";
    case Symbol::O:
        return "O";
    case Symbol::EMPTY:
        return ".";
    }
    return "?";
}

class Player
{
    string name;
    Symbol symbol;

public:
    Player(string name, Symbol s) : name(name), symbol(s) {}

    Symbol getSymbol() const
    {
        return symbol;
    }

    string getName() const
    {
        return name;
    }
};

class Cell
{
    Symbol symbol;

public:
    Cell() : symbol(Symbol::EMPTY) {}

    Symbol getSymbol() const
    {
        return symbol;
    }

    void assign(Symbol s)
    {
        symbol = s;
    }

    bool isEmpty() const
    {
        return symbol == Symbol::EMPTY;
    }
};

class Board
{
    vector<vector<Cell>> board;
    int sz;

public:
    Board(int n) : sz(n), board(n, vector<Cell>(n)) {}

    int getSize() const
    {
        return sz;
    }

    Cell &getCell(int x, int y)
    {
        return board[x][y];
    }

    void display() const
    {
        for (int i = 0; i < sz; ++i)
        {
            for (int j = 0; j < sz; ++j)
            {
                cout << symbolToStr(board[i][j].getSymbol()) << " ";
            }
            cout << endl;
        }
    }

    bool checkWin(Symbol symbol) const
    {
        // Check rows
        for (int i = 0; i < sz; ++i)
        {
            bool win = true;
            for (int j = 0; j < sz; ++j)
            {
                if (board[i][j].getSymbol() != symbol)
                {
                    win = false;
                    break;
                }
            }
            if (win)
                return true;
        }

        // Check columns
        for (int j = 0; j < sz; ++j)
        {
            bool win = true;
            for (int i = 0; i < sz; ++i)
            {
                if (board[i][j].getSymbol() != symbol)
                {
                    win = false;
                    break;
                }
            }
            if (win)
                return true;
        }

        // Check main diagonal
        bool win = true;
        for (int i = 0; i < sz; ++i)
        {
            if (board[i][i].getSymbol() != symbol)
            {
                win = false;
                break;
            }
        }
        if (win)
            return true;

        // Check anti-diagonal
        win = true;
        for (int i = 0; i < sz; ++i)
        {
            if (board[i][sz - i - 1].getSymbol() != symbol)
            {
                win = false;
                break;
            }
        }

        return win;
    }

    bool isFull() const
    {
        for (int i = 0; i < sz; ++i)
        {
            for (int j = 0; j < sz; ++j)
            {
                if (board[i][j].isEmpty())
                    return false;
            }
        }
        return true;
    }
};

class Game
{
    vector<shared_ptr<Player>> players;
    int currentPlayer = 0;
    Board board;

public:
    Game(int size) : board(size) {}

    void addPlayer(shared_ptr<Player> p)
    {
        players.push_back(p);
    }

    bool makeMove(int x, int y)
    {
        if (!isValidMove(x, y))
        {
            cout << "Invalid move!" << endl;
            return false;
        }

        Symbol symbol = players[currentPlayer]->getSymbol();
        board.getCell(x, y).assign(symbol);

        board.display();

        if (board.checkWin(symbol))
        {
            cout << players[currentPlayer]->getName() << " won the match!" << endl;
            return true;
        }

        if (board.isFull())
        {
            cout << "It's a draw!" << endl;
            return true;
        }

        currentPlayer = (currentPlayer + 1) % players.size();
        return false;
    }

    void start()
    {
        cout << "Starting Tic-Tac-Toe...\n";
        board.display();

        while (true)
        {
            auto current = players[currentPlayer];
            cout << current->getName() << "'s turn (" << symbolToStr(current->getSymbol()) << "):" << endl;
            int x, y;
            cout << "Enter row and column (0-indexed): ";
            cin >> x >> y;

            if (makeMove(x, y))
                break;
        }
    }

private:
    bool isValidMove(int x, int y)
    {
        if (x < 0 || y < 0 || x >= board.getSize() || y >= board.getSize())
            return false;
        return board.getCell(x, y).isEmpty();
    }
};

int main()
{
    Game game(3); // 3x3 board

    auto p1 = make_shared<Player>("Player 1", Symbol::X);
    auto p2 = make_shared<Player>("Player 2", Symbol::O);

    game.addPlayer(p1);
    game.addPlayer(p2);

    game.start();
    return 0;
}
