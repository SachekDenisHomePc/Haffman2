#ifndef BWT_H
#define BWT_H

#include <string>
#include <vector>
#include <forward_list>
#include <fstream>
#include <bitset>

class Binary_File;
using namespace std;

string BWT_encode(string source);
string BWT_decode(string encoding);
string rleText(string t);
string rleTextDecode(string t);

#endif
