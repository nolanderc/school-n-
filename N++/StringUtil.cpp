#include "StringUtil.h"


std::string toLower(std::string in)
{
	int charCount = in.size();
	for (int i = 0; i < charCount; ++i)
	{
		char c = in[i];
		if ('A' <= c && c <= 'Z')
		{
			in[i] = c + ('a' - 'A');
		}
	}

	return in;
}

bool extractRange(std::stringstream& stream, char start, char end, std::string& result)
{
	if (stream.ignore(std::numeric_limits<std::streamsize>::max(), start)){
		result = "";
		char c;
		while(stream.get(c)) {
			if (c == end) {
				return true;
			}
			result += c;
		}
	}

	return false;
}
