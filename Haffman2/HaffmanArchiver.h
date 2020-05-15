#pragma once
#include <map>
#include <queue>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>
#include <algorithm>
#include <numeric>

using namespace std;

class HaffmanArchiver
{
public:
	int ArchiveFile(string originalFileName, string archivedFileName, int tomeSize);
	void UnarchiveFile(vector<string> archivedFileName, string unrchivedFileName);
	int ShannonFanoArchiveFile(string originalFileName, string archivedFileName, int tomeSize,string password);
	void ShannonFanoUnarchiveFile(vector<string> archivedFileName, string unrchivedFileName, string password);
private:
	struct MinHeapNode
	{
		char data;
		int freq;
		MinHeapNode* left, * right;

		MinHeapNode(char data, int freq)
		{
			left = right = NULL;
			this->data = data;
			this->freq = freq;
		}
	};

	struct compare
	{
		bool operator()(MinHeapNode* l, MinHeapNode* r)
		{
			return (l->freq > r->freq);
		}
	};
	
	map<char, string> codes;

	map<char, int> freq;

	priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;

	void storeCodes(struct MinHeapNode* root, string str);

	void HuffmanCodes();

	void ShannonFanoCodes();

	void CreateShannonFanoTree(MinHeapNode* root, vector<MinHeapNode*> freqVector, int start, int end);

	int Sum(vector<MinHeapNode*> freqVector,int start, int end);

	void calcFreq(string str, int n);

	string decode_file(struct MinHeapNode* root, string s);

	vector<unsigned char> ConvertStringToBytes(string encodedString);

	string ConvertBytesToString(vector<unsigned char> bytes);

	void WriteToFile(string outputFileName ,map<char, int> freq, vector<unsigned char> bytes, int tomeSize);

	string ReadFile(vector<string> inputFileNames);
	
	string ReadFileShannonFano(vector<string> inputFileNames,size_t passHash);

};
