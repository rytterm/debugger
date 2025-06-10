#include "cli.h"
#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>



constexpr char ARROW        {'\x1b'};
constexpr char BACKSPACE    {127};


static std::vector<std::string> history;
static unsigned int             history_index {};



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


// Handle up/down
static void handleud(const std::string& prompt, std::string& line) {
    char seq[2];
    if (read(STDIN_FILENO, &seq[0], 1) == 1 && read(STDIN_FILENO, &seq[1], 1) == 1) {
        if (seq[0] == '[' && seq[1] == 'A') {
            // Up arrow
            if (history_index != 0) {
                history_index--;
                line = history.at(history_index);
                cliClearLine();
                std::cout << prompt << line << std::flush;
            }
        } else if (seq[0] == '[' && seq[1] == 'B') {
            // Down arrow 
            if (history_index != history.size() - 1) {
                history_index++;
                line = history.at(history_index);
                cliClearLine();
                std::cout << prompt << line << std::flush;
            } else {
                cliClearLine();
                std::cout << prompt << std::flush;
            }
        }
    }
}


// Handle backspace
static void handleback(std::string& line) {
    if (!line.empty()) {
        line.pop_back();
        std::cout << "\b \b" << std::flush;
        if (!history.empty())
            history_index = history.size() - 1;
    }
}


// Handle a regular command
static void handlereg(std::string& line, const char c) {
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
            handleud(prompt,line);
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
