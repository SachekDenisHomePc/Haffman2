#include "BWT.h"

#include <algorithm>
#include <bitset>
#include <string>
#include <iterator>
#include <iostream>
#include <forward_list>
#include <vector>

using namespace std;

const char EOT_CHAR = '$';

string cyclic_shift(string source, int distance) {
	return source.substr(distance, string::npos) + source.substr(0, distance);
}

string BWT_encode(string source) {
	source.append(&EOT_CHAR);

	vector<string> shifts;
	
	for (unsigned int i = 0; i < source.length(); i++) {
		string shift = cyclic_shift(source, i);

		shifts.push_back(shift);
	}

	sort(shifts.begin(), shifts.end());

	string encoding = "";

	for (vector<string>::iterator it = shifts.begin(); it != shifts.end(); it++) {
		encoding = encoding + it->at(it->length() - 1);
	}

	return rleText(encoding);
}

string rleText(string t)
{
	string s2 = t;
	string s;
	int c = 1;
	int i = 1;
	
	
	while(i <= s2.size())
	{
		while(s2[i-1] == s2[i] && i < s2.size())
		{
			c++;
			i++;
		}
		if(c >= 10)
		{
			while(c > 9)
			{
				s = s + to_string(9);
				s.push_back(0);
				s.push_back(s2[i-1]);
				c-=9;
			}
			
			s = s + to_string(c);
			s.push_back(0);
			s.push_back(s2[i-1]);
		}
		else if(c > 2)
		{
			s = s + to_string(c);
			s.push_back(0);
			s.push_back(s2[i-1]);
		}
		else
		{
			if(c == 2)
			{
				s.push_back(s2[i-2]);
			}
			s.push_back(s2[i-1]);
		}
		c=1;
		i++;
	}
	
	return s;
}

bool comp_tuples(pair<unsigned char, int> a, pair<unsigned char, int> b) {
	return (a.first < b.first);
}

string rleTextDecode(string t)
{
	string s = t;
	string ret = "";
	int i = 0;
	int c = 0;
	
	while(i < s.size())
	{
		if(isdigit(s.at(i)))
		{
			c = s.at(i) - 0x30;
			
			if((i+1 < s.size()) && (i+2 < s.size()) && s.at(i+1) == 0)
			{
				for(int a = 0; a < c; a++)
				{
					ret.push_back(s.at(i+2));
				}
				
				i+=2;
			}
			else
			{
				ret.push_back(s.at(i));
			}
			c=0;
		}
		else
		{
			ret.push_back(s.at(i));
		}
		
		i++;
	}
	return ret;
}


string BWT_decode(string encoding) {
	encoding = rleTextDecode(encoding);
	if (encoding.empty()) {
		return "";
	}

	int j;

	vector<pair<unsigned char, int>> coding_tuples;

	for (unsigned int i = 0; i < encoding.length(); i++) {
		char coded_char = encoding.at(i);

		if (coded_char == EOT_CHAR) {
			j = i;
		}

		coding_tuples.push_back(pair<unsigned char, int>(encoding.at(i), i));
	}

	stable_sort(coding_tuples.begin(), coding_tuples.end(), comp_tuples);

	string decoding = "";

	do {
		decoding = decoding + (char)coding_tuples.at(j).first;
		j = coding_tuples.at(j).second;
	} while (coding_tuples.at(j).first != EOT_CHAR);

	return decoding;
}