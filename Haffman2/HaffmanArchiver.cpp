#include "HaffmanArchiver.h"

#include <iterator>

void HaffmanArchiver::storeCodes(struct MinHeapNode* root, string str)
{
	if (root == NULL)
		return;
	if (root->data != '$')
		codes[root->data] = str;
	storeCodes(root->left, str + "0");
	storeCodes(root->right, str + "1");
}

int HaffmanArchiver::ArchiveFile(string originalFileName, string archivedFileName, int tomeSize)
{
	string encodedString;

	ifstream fin(originalFileName);

	string str((std::istreambuf_iterator<char>(fin)),
		std::istreambuf_iterator<char>());

	calcFreq(str, str.length());
	HuffmanCodes();

	for (auto i : str)
		encodedString += codes[i];

	auto bytes = ConvertStringToBytes(encodedString);

	WriteToFile(archivedFileName, freq, bytes,tomeSize);

	return freq.size() + bytes.size();
}

void HaffmanArchiver::UnarchiveFile(vector<string> archivedFileName, string unrchivedFileName)
{
	string encodedString = ReadFile(archivedFileName);

	string decodedString = decode_file(minHeap.top(), encodedString);

	ofstream fout(unrchivedFileName);

	fout << decodedString;

	fout.close();
}

int HaffmanArchiver::ShannonFanoArchiveFile(string originalFileName, string archivedFileName, int tomeSize, string password)
{
	hash<string> hFunc;
	auto passHash = hFunc(password);
	string encodedString;

	ifstream fin(originalFileName);

	string str((std::istreambuf_iterator<char>(fin)),
		std::istreambuf_iterator<char>());

	calcFreq(str, str.length());
	ShannonFanoCodes();

	for (auto i : str)
		encodedString += codes[i];

	auto bytes = ConvertStringToBytes(encodedString);

	for (auto i=bytes.begin();i<bytes.end();i++)
		*i += passHash;

	WriteToFile(archivedFileName, freq, bytes,tomeSize);

	return freq.size() + bytes.size();
}

void HaffmanArchiver::ShannonFanoUnarchiveFile(vector<string> archivedFileName, string unrchivedFileName, string password)
{
	hash<string> hFunc;
	auto passHash = hFunc(password);
	
	string encodedString = ReadFileShannonFano(archivedFileName,passHash);

	string decodedString = decode_file(minHeap.top(), encodedString);

	ofstream fout(unrchivedFileName);

	fout << decodedString;

	fout.close();
}

void HaffmanArchiver::HuffmanCodes()
{
	struct MinHeapNode* left, * right, * top;
	for (map<char, int>::iterator v = freq.begin(); v != freq.end(); v++)
		minHeap.push(new MinHeapNode(v->first, v->second));
	while (minHeap.size() != 1)
	{
		left = minHeap.top();
		minHeap.pop();
		right = minHeap.top();
		minHeap.pop();
		top = new MinHeapNode('$', left->freq + right->freq);
		top->left = left;
		top->right = right;
		minHeap.push(top);
	}
	storeCodes(minHeap.top(), "");
}

void HaffmanArchiver::ShannonFanoCodes()
{
	struct MinHeapNode* left, * right, * top;
	vector<MinHeapNode*> freqVector;
	for (map<char, int>::iterator v = freq.begin(); v != freq.end(); v++)
		freqVector.push_back(new MinHeapNode(v->first, v->second));

	stable_sort(freqVector.begin(),freqVector.end(), [](MinHeapNode* left, MinHeapNode* right){return  left->freq > right->freq;});

	top = new MinHeapNode('$', Sum(freqVector,0,freqVector.size()));

	minHeap.push(top);
	
	CreateShannonFanoTree(top,freqVector,0,freqVector.size());
	
	storeCodes(top, "");
}

void HaffmanArchiver::CreateShannonFanoTree(MinHeapNode* root, vector<MinHeapNode*> freqVector, int start, int end)
{
	int mid = (start+end)/2;
	
	if(end-start <= 3)
	{
		root->left = new MinHeapNode(freqVector[start]->data, freqVector[start]->freq);

		//if(end == freqVector.size())
		//	return;

		if(end-start == 3)
		{
			root->right = new MinHeapNode('$', Sum(freqVector,mid,end));
			CreateShannonFanoTree(root->right,freqVector, mid, end);
		}
		else
		{
			root->right = new MinHeapNode(freqVector[end-1]->data, freqVector[end-1]->freq);
		}
		return;
	}


	auto left = new MinHeapNode('$', Sum(freqVector,start,mid));
	auto right = new MinHeapNode('$', Sum(freqVector,mid,end));

	root->left = left;
	root->right = right;
	CreateShannonFanoTree(root->left,freqVector, start, mid);
	CreateShannonFanoTree(root->right,freqVector, mid, end);
	
}

int HaffmanArchiver::Sum(vector<MinHeapNode*> freqVector,int start, int end)
{
	int sum = 0;

	for(auto i=freqVector.begin()+start;i!=freqVector.begin()+end;i++)
	{
		sum+=(*i)->freq;
	}

	return sum;
}

void HaffmanArchiver::calcFreq(string str, int n)
{
	for (int i = 0; i < str.size(); i++)
		freq[str[i]]++;
}

string HaffmanArchiver::decode_file(struct MinHeapNode* root, string s)
{
	string ans = "";
	struct MinHeapNode* curr = root;
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == '0')
			curr = curr->left;
		else
			curr = curr->right;

		if (curr->left == NULL and curr->right == NULL)
		{
			ans += curr->data;
			curr = root;
		}
	}
	return ans;
}

vector<unsigned char> HaffmanArchiver::ConvertStringToBytes(string encodedString)
{
	auto bytes = vector<unsigned char>();

	unsigned char tailSize = 0;

	for (int i = 0; i < encodedString.length(); i += 8)
	{
		if (encodedString.length() - i < 8)
		{
			string tail = encodedString.substr(i, encodedString.length() - i);

			tailSize = tail.size();

			while (tail.size() != 8)
				tail += "0";

			bytes.push_back(stoi(tail, 0, 2));

		}
		else
		{
			bytes.push_back(stoi(encodedString.substr(i, 8), 0, 2));
		}
	}

	bytes.push_back(8 - tailSize);

	return bytes;
}

string HaffmanArchiver::ConvertBytesToString(vector<unsigned char> bytes)
{
	string encodedString;
	unsigned char tailLength = bytes.back();

	int i = 0;
	for (auto byte = bytes.begin(); byte != bytes.end() && i < bytes.size() - 1; byte++)
	{
		if (i != bytes.size() - 2 || (i == bytes.size() - 2 && tailLength == 8))
		{
			encodedString += std::bitset<8>(*byte).to_string();
		}
		else
		{
			encodedString += std::bitset<8>(*byte).to_string().substr(0, 8 - tailLength);
			bytes.pop_back();
		}
		i++;
	}

	return encodedString;
}

void HaffmanArchiver::WriteToFile(string outputFileName, map<char, int> freq, vector<unsigned char> bytes, int tomeSize)
{
	vector<unsigned char> resultVector;

	for (auto node : freq)
	{
		auto castedInt = reinterpret_cast<unsigned char*>(&node.second);
		resultVector.push_back(node.first);
		resultVector.insert(resultVector.end(),castedInt,castedInt+4);
	}

	resultVector.push_back((unsigned char)0);

	for (auto byte : bytes)
	{
		resultVector.push_back(byte);
	}

	for(int i=0;i<resultVector.size();i+=tomeSize)
	{
		ofstream fout(outputFileName+to_string(i/tomeSize), ios::binary);
		auto end = resultVector.begin();
		auto start = resultVector.begin() +i;
		if(i + tomeSize < resultVector.size())
			end = resultVector.begin() + i + tomeSize;
		else
			end = resultVector.begin() + resultVector.size();
		vector<unsigned char> fileVector(start,end);

		for(auto element : fileVector)
			fout << element;
		
		fout.close();
	}

}

string HaffmanArchiver::ReadFile(vector<string> inputFileNames)
{
	vector<unsigned char> inputBytes;
	vector<unsigned char> bytes;
	freq = map<char, int>();
	for(auto inputFile:inputFileNames)
	{
		ifstream fin(inputFile, ios::binary);
		fin.unsetf(ios_base::skipws);
		
		fin.seekg(0, std::ios_base::end);
		int length = fin.tellg();
		fin.seekg(0, std::ios_base::beg);
		
		inputBytes.insert(inputBytes.end(),
               std::istream_iterator<unsigned char>(fin),
               std::istream_iterator<unsigned char>());
	}
	int i =0;

	while (inputBytes[i] != 0)
	{
		char first;
		first = inputBytes[i];
		i++;
		freq[first] = *reinterpret_cast<int*>(vector<unsigned char>(inputBytes.begin()+i,inputBytes.begin()+i+4).data());
		i+=4;
	}

	minHeap = priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare>();
	HuffmanCodes();

	i++;

	unsigned char byte;
	while (i<inputBytes.size())
	{
		byte = inputBytes[i];
		bytes.push_back(byte);
		i++;
	}

	return ConvertBytesToString(bytes);
}

string HaffmanArchiver::ReadFileShannonFano(vector<string> inputFileNames,size_t passHash)
{
	vector<unsigned char> inputBytes;
	vector<unsigned char> bytes;
	freq = map<char, int>();
	for(auto inputFile:inputFileNames)
	{
		ifstream fin(inputFile, ios::binary);
		fin.unsetf(ios_base::skipws);
		
		fin.seekg(0, std::ios_base::end);
		int length = fin.tellg();
		fin.seekg(0, std::ios_base::beg);
		
		inputBytes.insert(inputBytes.end(),
               std::istream_iterator<unsigned char>(fin),
               std::istream_iterator<unsigned char>());
	}
	int i =0;

	while (inputBytes[i] != 0)
	{
		char first;
		first = inputBytes[i];
		i++;
		freq[first] = *reinterpret_cast<int*>(vector<unsigned char>(inputBytes.begin()+i,inputBytes.begin()+i+4).data());
		i+=4;
	}

	ShannonFanoCodes();

	i++;

	unsigned char byte;
	while (i<inputBytes.size())
	{
		byte = inputBytes[i];
		bytes.push_back(byte);
		i++;
	}

	for (auto i=bytes.begin();i<bytes.end();i++)
		*i -= passHash;

	return ConvertBytesToString(bytes);
}