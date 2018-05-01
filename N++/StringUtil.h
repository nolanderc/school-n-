#pragma once
#include <string>
#include <sstream>


// Returnerar en ny string med endast små bokstäver
std::string toLower(std::string in);


// Söker efter bokstaven 'start' och returnerar all text därefter till 'end'
bool extractRange(std::stringstream& stream, char start, char end, std::string& result);


// Omvandlar tid (i sekunder) till en fin string
std::string formatTime(double time);