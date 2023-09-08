#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
using namespace std;

void swapFiles(string fileName, string fileNameTemp) {
	remove(fileName.c_str());
	rename(fileNameTemp.c_str(), fileName.c_str());
}

bool checkFixedLen(string fileName, int recordLen)
{
	ifstream file(fileName);
	string line;
	getline(file, line);
	file.close();
	if (line.length() != recordLen)
		return false;
	return true;
}

void createFixedLen(string fileName, string fileNameTemp, int recordLen)
{
	ifstream file(fileName);
	ofstream fileTemp(fileNameTemp);
	string line;
	while (getline(file, line))
	{
		line += ";";
		while (line.length() < recordLen - 2)
			line += " ";
		line += "##";
		fileTemp << line << endl;
	}
	file.close();
	fileTemp.close();
	swapFiles(fileName, fileNameTemp);
}

void subChar(string fileName, string fileNameTemp)
{
	ifstream file(fileName);
	ofstream fileTemp(fileNameTemp);
	string line;
	while (getline(file, line))
	{
		for (int i = 0; i < line.length(); i++)
			if (line[i] == '\uFFFD')
				line[i] = '\'';
		fileTemp << line << endl;
	}
	file.close();
	fileTemp.close();
	swapFiles(fileName, fileNameTemp);
}

bool checkConversionStrToInt(string checkSTR) {
	bool valid = true;
	for (int i = 0; i < checkSTR.length(); i++) {
		if (checkSTR[i] < 48 || checkSTR[i] > 57)
			valid = false;
	}
	return valid;
};

bool checkMyValue(string fileName)
{
	ifstream file(fileName);
	string line;
	getline(file, line);
	file.close();
	if (line.find("Miovalore") != string::npos && line.find("Cancellazione Logica") != string::npos)
		return true;
	return false;
}

void createMyValue(string fileName, string fileNameTemp, int recordLen) {
	ifstream file(fileName);
	ofstream fileTemp(fileNameTemp);
	string line;
	getline(file, line);
	string subSTR = line.substr(0, line.rfind(";") + 1);
	subSTR += "Miovalore;Cancellazione Logica;";
	while (subSTR.length() < recordLen - 2)
		subSTR += " ";
	subSTR += "##";
	fileTemp << subSTR << endl;
	while (getline(file, line))
	{
		subSTR = line.substr(0, line.rfind(";") + 1);
		int randNum = rand() % (20 - 10 + 1) + 10;
		subSTR += to_string(randNum) + ";0;";
		while (subSTR.length() < recordLen - 2)
			subSTR += " ";
		subSTR += "##";
		fileTemp << subSTR << endl;
	}
	file.close();
	fileTemp.close();
	swapFiles(fileName, fileNameTemp);
}

int countFields(string fileName)
{
	ifstream file(fileName);
	string line;
	getline(file, line);
	file.close();
	int count = 0;
	for (int i = 0; i < line.length(); i++)
		if (line[i] == ';')
			count++;
	return count;
}

int main()
{
	string fileName = "ferrari.csv", fileNameTemp = "ferrari_temp.csv";
	int recordLen = 258;
	if (!checkFixedLen(fileName, recordLen))
		createFixedLen(fileName, fileNameTemp, recordLen);
	//subChar(fileName, fileNameTemp);
	bool keepGoing = true;
	int numFields;
	string basicOptions = "Opzioni:\n1 - Aggiunta Miovalore\n2 - Conta Campi\n3 - Calcola Lunghezza Campi/Record\n4 - Aggiunta Record in coda\n5 - Visualizza Campi\n6 - Ricerca Record\n7 - Modifica Record\n8 - Cancella Record\n0 - Chiusura Programma\n\n";
	do {
		system("CLS");
		cout << basicOptions;
		string optionsSTR;
		getline(cin, optionsSTR);
		cin.clear();
		if (!checkConversionStrToInt(optionsSTR)) {
			system("CLS");
			cout << "Inserire un numero valido\n" << endl;
			_getch();
		}
		else {
			int optionsINT = stoi(optionsSTR);
			switch (optionsINT) {
				case 1:
					if (!checkMyValue(fileName))
						createMyValue(fileName, fileNameTemp, recordLen);
					else
					{
						system("CLS");
						cout << "Nel file sono presenti i campi Miovalore e Cancellazione Logica" << endl;
						cout << "\nPremere un tasto per continuare\n" << endl;
						_getch();
					}
					break;
				case 2:
					numFields = countFields(fileName);
					system("CLS");
					cout <<"Numero campi: " << numFields << endl;
					cout << "\nPremere un tasto per continuare\n\n";
					_getch();
					break;
				case 3:
					break;
				case 4:
					break;
				case 5:
					break;
				case 6:
					break;
				case 7:
					break;
				case 8:
					break;
				#pragma region 0 - Uscita
				case 0:
					bool validEXIT;
					do
					{
						system("CLS");
						cout << "Chiudere il programma?\n1 - Si\n0 - No\n\n";
						getline(cin, optionsSTR);
						cin.clear();
						if (!checkConversionStrToInt(optionsSTR)) {
							system("CLS");
							cout << "Inserire un numero valido\n" << endl;
							_getch();
						}
						else {
							optionsINT = stoi(optionsSTR);
							switch (optionsINT)
							{
							case 1:
								validEXIT = true;
								keepGoing = false;
								break;
							case 0:
								validEXIT = true;
								keepGoing = true;
								break;
							default:
								validEXIT = false;
								system("CLS");
								cout << "Inserire un numero valido" << endl;
								_getch();
								break;
							}
						}
					} while (!validEXIT);
					break;
				#pragma endregion
				#pragma region Default
				default:
					system("CLS");
					cout << "Inserire un numero valido" << endl;
					_getch();
					break;
				#pragma endregion
			}
		}
	} while (keepGoing);
}
