#include "includes.hpp"

std::string replacer(std::string str, std::string s1, std::string s2)
{
	size_t pos;

	if (s1 == s2)
		return str;
	pos = str.find(s1);
	while (pos != std::string::npos) 
	{
		str.erase(pos, s1.size());
		str.insert(pos, s2);
		pos = str.find(s1);
	}
	return str;
}

std::string str_toupper(std::string &str)
{
    int i = 0;

    while (str[i])
    {
		str[i] = (char)std::toupper(str[i]);
        i++;
    }
    return str;
}

std::vector<std::string> splitString2(std::string str, const std::string delimiter) {
    std::vector<std::string> result;
    std::string::size_type pos = 0, last_pos = 0;

    while ((pos = str.find(delimiter, last_pos)) != std::string::npos)
    {
        if (str.substr(last_pos)[0])
            result.push_back(str.substr(last_pos, pos - last_pos));
        last_pos = pos + delimiter.size();
    }
    if (str.substr(last_pos)[0])
        result.push_back(str.substr(last_pos));

    return result;
}

std::string getToken(std::string line, int &i)
{
    std::string new_str;
    while (line[i] == ' ')
        i++;
    std::string str = line.substr(i);

    std::size_t pos = str.find(' ');

    if (pos != std::string::npos)
    {
        new_str = str.substr(0, pos);
        i += pos + 1;
    }
    else
    {
        new_str = str;
        i = -1;
    }
    return new_str;
}