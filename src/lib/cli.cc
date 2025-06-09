#include "cli.h"
#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <vector>


static std::vector<std::string> history;


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


/* Handle command-line input character by character */
std::string cliInput(const std::string& prompt) {
    std::string line;

    char c;
    std::cout << prompt << std::flush;
    setraw(true);

    while (read(STDIN_FILENO, &c, 1) == 1 && c != '\n') {
        if (c == 127 || c == 8) {
            if (!line.empty()) {
                line.pop_back();
                std::cout << "\b \b" << std::flush;
            }
        }
        else {  
            line.push_back(c);
            std::cout << c << std::flush;
        } 
    }
    setraw(false);
    std::cout << std::endl;

    return line;
}


void cliAddHistory(const std::string& line) {
    if (!line.empty())
        history.push_back(line);

}



