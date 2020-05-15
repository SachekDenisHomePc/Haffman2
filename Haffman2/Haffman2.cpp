
#include <fstream>
#include <iostream>
#include <string>
#include "HaffmanArchiver.h"
#include <sys/stat.h>
#include "BWT.h"

using namespace std;

void StartHaffmanLoop();
void StartBWTLoop();
void StartShannonFano();

int main()
{
	setlocale(LC_ALL, "Russian");

	HaffmanArchiver archiver = HaffmanArchiver();

	while (true)
	{
		cout << "1. Хаффман" << endl;
		cout << "2. BWT" << endl;
		cout << "3. Шеннон — Фано" << endl;
		int choice = 0;
		cin >> choice;
		switch (choice)
		{
		case 1:
		{
			StartHaffmanLoop();
		}
		break;
		case 2:
		{
			StartBWTLoop();
		}
		break;
		case 3:
		{
			StartShannonFano();
		}
		break;
		}
	}

	return 0;
}

void StartHaffmanLoop()
{
	system("cls");
	HaffmanArchiver archiver = HaffmanArchiver();

	while (true)
	{
		cout << "1. Архивировать" << endl;
		cout << "2. Разархивировать" << endl;
		int choice = 0;
		cin >> choice;

		switch (choice)
		{
		case 1:
		{

			string input;
			string output;
			int tomeSize;

			cout << "Входной файл" << endl;
			cin >> input;

			cout << "Выходной файл" << endl;
			cin >> output;

			cout << "Размер тома" << endl;
			cin >> tomeSize;

			int outputSize = archiver.ArchiveFile(input, output,tomeSize);

			struct stat inputFileStat;
			stat(input.c_str(), &inputFileStat);
			cout << "Размер входного файла " << inputFileStat.st_size << endl;
			cout << "Размер выходного файла " << outputSize << endl;
			cout << "Степень архивации " << (double)outputSize / inputFileStat.st_size << endl;
			cin.get();
			cin.get();
		}
		break;
		case 2:
		{
			vector<string> inputFiles;
			string output;
			string newItem;

			cout << "Входной файл" << endl;
			while (cin >> newItem && newItem!=".")
			{
				inputFiles.push_back(newItem);
			}

			cout << "Выходной файл" << endl;
			cin >> output;

			archiver.UnarchiveFile(inputFiles, output);
		}
		break;
		}
		system("cls");
	}
}

void StartBWTLoop()
{
	system("cls");

	while (true)
	{
		cout << "1. Архивировать" << endl;
		cout << "2. Разархивировать" << endl;
		int choice = 0;
		cin >> choice;

		switch (choice)
		{
		case 1:
		{
			string input;
			string output;
			int tomeSize;

			cout << "Входной файл" << endl;
			cin >> input;

			cout << "Выходной файл" << endl;
			cin >> output;

			cout << "Размер тома" << endl;
			cin >> tomeSize;

			ifstream fin(input);

			string inputString((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

			auto encoded = BWT_encode(inputString);

			for(int i=0;i<encoded.size();i+=tomeSize)
			{
				ofstream fout(output+to_string(i/tomeSize));
				auto end = 0;
				auto start = i;
				if(i + tomeSize < encoded.size())
					end = i + tomeSize;
				else
					end = encoded.size();
				
				string substring = encoded.substr(start,end-start);


				fout << substring;
				
				fout.close();
			}
				

			struct stat inputFileStat;
			stat(input.c_str(), &inputFileStat);
			cout << "Размер входного файла " << inputFileStat.st_size << endl;
			cout << "Размер выходного файла " << encoded.length() << endl;
			cout << "Степень архивации " << (double)encoded.length() / inputFileStat.st_size << endl;
			cin.get();
			cin.get();
		}
		break;
		case 2:
		{
			vector<string> inputFiles;
			string newItem;
			string output;
			string inputString;

			cout << "Входной файл" << endl;
			while (cin >> newItem && newItem!=".")
			{
				inputFiles.push_back(newItem);
			}

			cout << "Выходной файл" << endl;
			cin >> output;

			ofstream fout(output);

			for(auto input:inputFiles)
			{
				ifstream fin(input);

				inputString+=string((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
			}

			auto decoded = BWT_decode(inputString);
			fout << decoded;

		}
		break;
		}
		system("cls");
	}
}

void StartShannonFano()
{
	system("cls");
	HaffmanArchiver archiver = HaffmanArchiver();

	while (true)
	{
		cout << "1. Архивировать" << endl;
		cout << "2. Разархивировать" << endl;
		int choice = 0;
		cin >> choice;

		switch (choice)
		{
		case 1:
		{

			string input;
			string output;
			string password;
			int tomeSize;

			cout << "Входной файл" << endl;
			cin >> input;

			cout << "Выходной файл" << endl;
			cin >> output;

			cout << "Размер тома" << endl;
			cin >> tomeSize;

			cout << "Пароль" << endl;
			cin >> password;

			int outputSize = archiver.ShannonFanoArchiveFile(input, output,tomeSize, password);

			struct stat inputFileStat;
			stat(input.c_str(), &inputFileStat);
			cout << "Размер входного файла " << inputFileStat.st_size << endl;
			cout << "Размер выходного файла " << outputSize << endl;
			cout << "Степень архивации " << (double)outputSize / inputFileStat.st_size << endl;
			cin.get();
			cin.get();
		}
		break;
		case 2:
		{
			vector<string> inputFiles;
			string output;
			string newItem;
			string password;

			cout << "Входной файл" << endl;
			while (cin >> newItem && newItem!=".")
			{
				inputFiles.push_back(newItem);
			}

			cout << "Выходной файл" << endl;
			cin >> output;

			cout << "Пароль" << endl;
			cin >> password;

			archiver.ShannonFanoUnarchiveFile(inputFiles, output,password);
		}
		break;
		}
		system("cls");
	}
}