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

void maxLen(string fileName, int numFields, string maxLen[])
{
	ifstream file(fileName);
	string line, subSTR;
	getline(file, line);
	for (int i = 0; i <= numFields; i++)
		maxLen[i] = "";
	while (getline(file, line))
	{
		int lastPlace;
		int totLen = line.rfind(";") + 1;
		if (totLen > maxLen[numFields].length())
			maxLen[numFields] = line.substr(0, totLen);
		subSTR = line.substr(0, line.find(";"));
		if (subSTR.length() > maxLen[0].length())
			maxLen[0] = subSTR;
		for (int i = 1; i < numFields; i++)
		{
			lastPlace = line.find(";");
			line = line.substr(lastPlace + 1);
			subSTR = line.substr(0, line.find(";"));
			if (subSTR.length() > maxLen[i].length())
				maxLen[i] = subSTR;
		}
	}
	file.close();
}

void searchFieldsNames(string fileName, string fieldsNames[]) {
	string line, subSTR;
	int numFields = countFields(fileName), lastPlace = 0;
	ifstream file(fileName);
	getline(file, line);
	for (int i = 0; i < numFields; i++)
	{
		subSTR = line.substr(0, line.find(";"));
		lastPlace = line.find(";");
		line = line.substr(lastPlace + 1);
		fieldsNames[i] = subSTR;
	}
	file.close();
}

bool checkInputLen(int numFields, string elements[]) {
	string lineToFile = "";
	for (int i = 0; i < numFields; i++)
	{
		if (elements[i].length() == 0)
			return false;
		lineToFile += elements[i] + ";";
	}
	if (numFields == 11)
		if (lineToFile.length() < 256)
			return true;
		else return false;
	else
		if (lineToFile.length() < 251)
			return true;
	return false;
}

bool checkInputChars(int numField, string elements[])
{
	for (int i = 0; i < numField; i++)
	{
		if (elements[i].find(';') != std::string::npos || elements[i].find('\\') != std::string::npos || elements[i].find('#') != std::string::npos)
			return false;
	}
	return true;
}

void addToQueue(string fileName, string fileNameTemp, int numFields, string elements[], int recordLen) {
	ifstream file(fileName);
	ofstream fileTemp(fileNameTemp);
	string line, lineToFile = "";
	while (getline(file, line))
	{
		fileTemp << line << endl;
	}
	for (int i = 0; i < numFields; i++)
	{
		lineToFile += elements[i] + ";";
	}
	while (lineToFile.length() < recordLen - 2)
		lineToFile += " ";
	lineToFile += "##";
	fileTemp << lineToFile << endl;
	file.close();
	fileTemp.close();
	swapFiles(fileName, fileNameTemp);
}

int main()
{
	system("chcp.com 1252");
	string fileName = "ferrari.csv", fileNameTemp = "ferrari_temp.csv";
	int recordLen = 258;
	if (!checkFixedLen(fileName, recordLen))
		createFixedLen(fileName, fileNameTemp, recordLen);
	bool keepGoing = true;
	int numFields;
	string maxLenghts[12], inputs[11], fieldsNames[11];
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
				#pragma region 1 - Aggiunta Miovalore
				case 1:
					if (!checkMyValue(fileName))
						createMyValue(fileName, fileNameTemp, recordLen);
					else
					{
						system("CLS");
						cout << "Nel file sono gi� presenti i campi Miovalore e Cancellazione Logica" << endl;
						cout << "\nPremere un tasto per continuare\n" << endl;
						_getch();
					}
					break;
				#pragma endregion
				#pragma region 2 - Conta Campi
				case 2:
					numFields = countFields(fileName);
					system("CLS");
					cout <<"Numero campi: " << numFields << endl;
					cout << "\nPremere un tasto per continuare\n\n";
					_getch();
					break;
				#pragma endregion
				#pragma region 3 - Calcola Lunghezza Campi/Record
				case 3:
					numFields = countFields(fileName);
					maxLen(fileName, numFields, maxLenghts);
					system("CLS");
					cout << "Il record pi� lungo � " << maxLenghts[numFields] <<" composto da "<< maxLenghts[numFields].length() <<" caratteri\n";
					for (int i = 0; i < numFields; i++)
					{
						cout << "Il campo " << i << " pi� lungo � " << maxLenghts[i] << " composto da "<< maxLenghts[i].length() << " caratteri\n";
					}
					cout << "\nPremere un tasto per continuare\n\n";
					_getch();
					break;
				#pragma endregion
				#pragma region 4 - Aggiunta Record in coda
				case 4:
					numFields = countFields(fileName);
					searchFieldsNames(fileName, fieldsNames);
					for (int i = 0; i < 9; i++)
					{
						system("CLS");
						cout << "Inserire i dati del campo \"" << fieldsNames[i] <<"\" da aggiungere\n\n";
						getline(cin, inputs[i]);
					}
					if (checkMyValue(fileName))
					{
						int ranValue = rand() % (20 - 10 + 1) + 10;
						inputs[9] = to_string(ranValue);
						inputs[10] = "0";
					}
					if (!checkInputLen(numFields, inputs))
					{
						if (!checkMyValue(fileName))
						{
							system("CLS");
							cout << "Errore: uno o pi� campi sono vuoti o troppo lunghi, la lunghezza totale non deve superare i 242 caratteri\n";
							cout << "\nPremere un tasto per continuare\n\n";
							_getch();
						}
						else
						{
							system("CLS");
							cout << "Errore: uno o pi� campi sono vuoti o troppo lunghi, la lunghezza totale non deve superare i 245 caratteri\n";
							cout << "\nPremere un tasto per continuare\n\n";
							_getch();
						}
					}
					else if (!checkInputChars(numFields, inputs))
					{
						system("CLS");
						cout << "Uno o pi� campi contengono caratteri non validi \n\n';', '#', '\\'\n";
						cout << "\nPremere un tasto per continuare\n\n";
						_getch();
					}
					else
					{
						addToQueue(fileName, fileNameTemp, numFields, inputs, recordLen);
					}
					break;
				#pragma endregion
				case 5:
					system("CLS");
					cout << "Inserire numero del primo campo\n");
					string firstFieldS;
					getline(cin, firstFieldS);
					int firstField;
					if (firstFieldS.length() == 1 && firstFieldS[0] > 47 && firstFieldS[0] < 58)
						firstField = stoi(firstFieldS);
					else
					{
						system("CLS");
						cout << "Il campo inserito non � valido\n";
						cout << "\nPremere un tasto per continuare\n\n";
						_getch();
						break;
					}
					system("CLS");
					cout << "Inserire numero del primo campo\n");
					string secondFieldS = Console.ReadLine();
					int secondField;
					if (secondFieldS.Length == 1 && secondFieldS[0] > 47 && secondFieldS[0] < 58)
						secondField = int.Parse(secondFieldS);
					else
					{
						system("CLS");
						Console.ForegroundColor = ConsoleColor.Red;
						cout << "Il campo inserito non � valido");
						Console.ResetColor();
						cout << "\nPremere un tasto per continuare\n");
						Console.ReadKey();
						break;
					}
					system("CLS");
					cout << "Inserire numero del primo campo\n");
					string thirdFieldS = Console.ReadLine();
					int thirdField;
					if (thirdFieldS.Length == 1 && thirdFieldS[0] > 47 && thirdFieldS[0] < 58)
						thirdField = int.Parse(thirdFieldS);
					else
					{
						system("CLS");
						Console.ForegroundColor = ConsoleColor.Red;
						cout << "Il campo inserito non � valido");
						Console.ResetColor();
						cout << "\nPremere un tasto per continuare\n");
						Console.ReadKey();
						break;
					}
					int numFieldsVIS = functions.countFields(fileName);
					int[] checkEd = new int[numFieldsVIS];
					for (int i = 0; i < numFieldsVIS; i++)
						checkEd[i] = 0;
					if (firstField == 0 || secondField == 0 || thirdField == 0) checkEd[0] = 1;
					if (firstField == 1 || secondField == 1 || thirdField == 1) checkEd[1] = 1;
					if (firstField == 2 || secondField == 2 || thirdField == 2) checkEd[2] = 1;
					if (firstField == 3 || secondField == 3 || thirdField == 3) checkEd[3] = 1;
					if (firstField == 4 || secondField == 4 || thirdField == 4) checkEd[4] = 1;
					if (firstField == 5 || secondField == 5 || thirdField == 5) checkEd[5] = 1;
					if (firstField == 6 || secondField == 6 || thirdField == 6) checkEd[6] = 1;
					if (firstField == 7 || secondField == 7 || thirdField == 7) checkEd[7] = 1;
					if (firstField == 8 || secondField == 8 || thirdField == 8) checkEd[8] = 1;
					if (numFieldsVIS > 9)
						if (firstField == 9 || secondField == 9 || thirdField == 9) checkEd[9] = 1;
						else
							if (firstField == 9 || secondField == 9 || thirdField == 9)
							{
								system("CLS");
								Console.ForegroundColor = ConsoleColor.Red;
								cout << "Il campo Miovalore non � presente nel file");
								Console.ResetColor();
								cout << "\nPremere un tasto per continuare\n");
								Console.ReadKey();
								break;
							}
					string[] names = functions.searchFieldsNames(fileName);
					system("CLS");
					using (StreamReader csvReader = File.OpenText(fileName))
					{
						string lineFromFile;
						lineFromFile = csvReader.ReadLine();
						while ((lineFromFile = csvReader.ReadLine()) != null)
						{
							string[] fieldsVIS = lineFromFile.Split(';');
							if (numFieldsVIS == 11)
							{
								if (fieldsVIS[10] == "0")
								{
									cout << );
									for (int i = 0; i < numFieldsVIS; i++)
									{
										if (checkEd[i] == 1)
										{
											Console.Write($"{names[i]}: ");
											Console.ForegroundColor = ConsoleColor.Green;
											Console.Write($"{fieldsVIS[i]} ");
											Console.ResetColor();
										}
									}
								}
							}
							else
							{
								cout << );
								for (int i = 0; i < numFieldsVIS; i++)
								{
									if (checkEd[i] == 1)
									{
										Console.Write($"{names[i]}: ");
										Console.ForegroundColor = ConsoleColor.Green;
										Console.Write($"{fieldsVIS[i]} ");
									}
								}
							}
						}
						csvReader.Close();
						cout << "\n\nPremere un tasto per continuare\n");
						Console.ReadKey();
					}
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
