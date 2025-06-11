#include "cli.h"
#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>



constexpr char ARROW        {'\x1b'};
constexpr char UP           {'A'};
constexpr char DOWN         {'B'};
constexpr char RIGHT        {'C'};
constexpr char LEFT         {'D'};
constexpr char BACKSPACE    {127};


static std::vector<std::string> history;
static unsigned                 history_index {};
static unsigned                 col {};


// Disable/enable ICANON/ECHO
static void setraw(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else 
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}


// Clear the current line
static void cliClearLine() {
    std::cout << "\r\33[K";
}



static void uarrow(const std::string& prompt, std::string& line) {
    if (history_index != 0) {
        history_index--;
        line = history.at(history_index);
        cliClearLine();
        std::cout << prompt << line << std::flush;
    }
}



static void darrow(const std::string& prompt, std::string& line) {
    if (!history.empty() && history_index != history.size() - 1) {
        history_index++;
        line = history.at(history_index);
        cliClearLine();
        std::cout << prompt << line << std::flush;
    } else {
        cliClearLine();
        line = "";
        std::cout << prompt << std::flush;
    }
}

static void rarrow(std::string& line) {
    if (col < line.size()) {
        std::cout << "\x1b[C" << std::flush;
        col++;
    }
}

static void larrow() {
    if (col > 0)  {
        std::cout << "\x1b[D" << std::flush;
        col--;
    }    
}




static void handlearrow(const std::string& prompt, std::string& line) {
    char seq[2];
    if (read(STDIN_FILENO, &seq[0], 1) == 1 && read(STDIN_FILENO, &seq[1], 1) == 1 && seq[0] == '[') {
        switch(seq[1]) {
            case UP:    
                uarrow(prompt, line); 
                break;
            case DOWN:  
                darrow(prompt, line);
                break;
            case RIGHT:
                rarrow(line);
                break;
            case LEFT:  
                larrow();
                break;
            default:
                std::cerr << "Invalid input for arrow handler";
                std::abort();
        }
    }
}


// Handle backspace
static void handleback(std::string& line) {
    if (col > 0) {
        col--;
        line.pop_back();
        std::cout << "\b \b" << std::flush;
        if (!history.empty())
            history_index = history.size() - 1;
    }
}


// Handle a regular command
static void handlereg(std::string& line, const char c) {
    col++;
    line.push_back(c);
    std::cout << c << std::flush;
}



std::string cliInput(const std::string& prompt) {
    std::string line;
    char c;
    setraw(true);
    std::cout << prompt << std::flush;

    while (read(STDIN_FILENO, &c, 1) == 1 && c != '\n') {
        if (c == ARROW)
            handlearrow(prompt,line);
        else if (c == BACKSPACE)
            handleback(line);
        else
            handlereg(line,c);
    }
    setraw(false);
    std::cout << std::endl;

    if (line.empty())
        return "ignore";

    return line;
}



void cliAddHistory(const std::string& line) {
    if (!line.empty()) {
        history.push_back(line);
        history_index = history.size();
    }
}



void displayHistory() {
    std::cout << "Index: " << history_index << std::endl << "Cmd: ";
    std::copy(history.begin(),history.end(),
              std::ostream_iterator<std::string>(std::cout, " "));
    std::cout << std::endl;
}
