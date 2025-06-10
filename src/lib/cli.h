#pragma once

#include <string>


/* Handle command-line input character by character */
std::string cliInput(const std::string&);

// Add a command-line to the history, used for up/down arrow.
void cliAddHistory(const std::string&);

// Print out the entire command-line history as well as the current index,
// Useful for debugging.
void displayHistory();
