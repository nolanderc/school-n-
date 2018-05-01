#pragma once
#include <string>
#include <sstream>


// Returnerar en ny string med endast sm� bokst�ver
std::string toLower(std::string in);


// S�ker efter bokstaven 'start' och returnerar all text d�refter till 'end'
bool extractRange(std::stringstream& stream, char start, char end, std::string& result);


// Omvandlar tid (i sekunder) till en fin string
std::string formatTime(double time);