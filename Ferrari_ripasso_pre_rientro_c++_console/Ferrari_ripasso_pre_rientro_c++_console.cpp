#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <tuple>
using namespace std;

void lineSplit(string line, int numFields, string fields[]) {
	int lastPlace;
	for (int i = 0; i < numFields; i++)
	{
		lastPlace = line.find(";");
		fields[i] = line.substr(0, lastPlace);
		line = line.substr(lastPlace + 1);
	}
}

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
	string line, lineToFile = "";
	for (int i = 0; i < numFields; i++)
	{
		lineToFile += elements[i] + ";";
	}
	while (lineToFile.length() < recordLen - 2)
		lineToFile += " ";
	lineToFile += "##";
	fstream file(fileName);
	file.seekg(0, ios::end);
	file << lineToFile << endl;
	file.close();
	}

tuple<string, int> searchPosition(string fileName, string identifier, bool deleted, string fields[])
{
	tuple<string, int> RecordAndPosition;
	ifstream file(fileName);
	string lineFromFile;
	int position = 0;
	if (countFields(fileName) == 11)
	{
		while (getline(file, lineFromFile))
		{
			lineSplit(lineFromFile, countFields(fileName), fields);
			if (fields[6] == identifier)
			{
				if (((fields[10] == to_string(0) && !deleted) || (fields[10] == to_string(1) && deleted == true)) && position != 0)
				{
					file.close();
					RecordAndPosition = make_tuple(lineFromFile, position);
					return RecordAndPosition;
				}
			}
			position++;
		}
	}
	else
	{
		while (getline(file, lineFromFile))
		{
			lineSplit(lineFromFile, countFields(fileName), fields);
			if (fields[6] == identifier && position != 0)
			{
				file.close();
				RecordAndPosition = make_tuple(lineFromFile, position);
				return RecordAndPosition;
			}
			position++;
		}
	}
	file.close();
	return make_tuple("", -1);
}

void modifyRecord(string fileName, string fileNameTemp, int numFields, string elements[], int position, int recordLen)
{
	int lineNum = 0;
	string line, lineToFile = "";
	for (int i = 0; i < numFields; i++)
		lineToFile += elements[i] + ";";
	while (lineToFile.length() < recordLen - 2)
		lineToFile += " ";
	lineToFile += "##";
	fstream file(fileName);
	file.seekg((recordLen + 2) * position, ios::beg);
	file << lineToFile << endl;
	file.close();
}

void deleteRecord (string fileName, string fileNameTemp, int numFields, string elementsLine, int position, int recordLen, string elements[]) {
	lineSplit(elementsLine, numFields, elements);
	int lineNum = 0;
	string line, lineToFile = "";
	for (int i = 0; i < numFields - 1; i++)
		lineToFile += elements[i] + ";";
	lineToFile += "1;";
	while (lineToFile.length() < recordLen - 2)
		lineToFile += " ";
	lineToFile += "##";
	fstream file(fileName);
	file.seekg((recordLen + 2) * position, ios::beg);
	file << lineToFile << endl;
	file.close();
}

void recoverRecord(string fileName, string fileNameTemp, int numFields, string elementsLine, int position, int recordLen, string elements[]) {
	lineSplit(elementsLine, numFields, elements);
	int lineNum = 0;
	string line, lineToFile = "";
	for (int i = 0; i < numFields - 1; i++)
		lineToFile += elements[i] + ";";
	lineToFile += "0;";
	while (lineToFile.length() < recordLen - 2)
		lineToFile += " ";
	lineToFile += "##";
	fstream file(fileName);
	file.seekg((recordLen + 2) * position, ios::beg);
	file << lineToFile << endl;
	file.close();
}

void permDelete(string fileName, string fileNameTemp, string elements[]) {
	ifstream file(fileName);
	ofstream fileTemp(fileNameTemp);
	string line;
	int numFields = countFields(fileName);
	getline(file, line);
	fileTemp << line << endl;
	while (getline(file, line))
	{
		lineSplit(line, numFields, elements);
		if (elements[10] == "0")
			fileTemp << line << endl;
	}
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
	int numFields, checkEd[11], firstField, secondField, thirdField;
	string maxLenghts[12], inputs[11], fieldsNames[11], fields[11], firstFieldS, secondFieldS, thirdFieldS, lineFromFile;
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
						cout << "Nel file sono già presenti i campi Miovalore e Cancellazione Logica" << endl;
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
					cout << "Il record più lungo è " << maxLenghts[numFields] <<" composto da "<< maxLenghts[numFields].length() <<" caratteri\n";
					for (int i = 0; i < numFields; i++)
					{
						cout << "Il campo " << i << " più lungo è " << maxLenghts[i] << " composto da "<< maxLenghts[i].length() << " caratteri\n";
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
							cout << "Errore: uno o più campi sono vuoti o troppo lunghi, la lunghezza totale non deve superare i 242 caratteri\n";
							cout << "\nPremere un tasto per continuare\n\n";
							_getch();
						}
						else
						{
							system("CLS");
							cout << "Errore: uno o più campi sono vuoti o troppo lunghi, la lunghezza totale non deve superare i 245 caratteri\n";
							cout << "\nPremere un tasto per continuare\n\n";
							_getch();
						}
					}
					else if (!checkInputChars(numFields, inputs))
					{
						system("CLS");
						cout << "Uno o più campi contengono caratteri non validi \n\n';', '#', '\\'\n";
						cout << "\nPremere un tasto per continuare\n\n";
						_getch();
					}
					else
					{
						addToQueue(fileName, fileNameTemp, numFields, inputs, recordLen);
					}
					break;
				#pragma endregion
				#pragma region 5 - Visualizza Campi
				case 5:
					system("CLS");
					cout << "Inserire numero del primo campo\n\n";
					getline(cin, firstFieldS);
					if (firstFieldS.length() == 1 && firstFieldS[0] > 47 && firstFieldS[0] < 58)
						firstField = stoi(firstFieldS);
					else
					{
						system("CLS");
						cout << "Il campo inserito non è valido\n";
						cout << "\nPremere un tasto per continuare\n\n";
						_getch();
						break;
					}
					system("CLS");
					cout << "Inserire numero del secondo campo\n\n";
					getline(cin, secondFieldS);
					int secondField;
					if (secondFieldS.length() == 1 && secondFieldS[0] > 47 && secondFieldS[0] < 58)
						secondField = stoi(secondFieldS);
					else
					{
						system("CLS");
						cout << "Il campo inserito non è valido\n";
						cout << "\nPremere un tasto per continuare\n\n";
						_getch();
						break;
					}
					system("CLS");
					cout << "Inserire numero del terzo campo\n\n";
					getline(cin, thirdFieldS);
					int thirdField;
					if (thirdFieldS.length() == 1 && thirdFieldS[0] > 47 && thirdFieldS[0] < 58)
						thirdField = stoi(thirdFieldS);
					else
					{
						system("CLS");
						cout << "Il campo inserito non è valido\n";
						cout << "\nPremere un tasto per continuare\n\n";
						_getch();
						break;
					}
					numFields = countFields(fileName);
					for (int i = 0; i < numFields; i++)
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
					if (numFields > 9)
						if (firstField == 9 || secondField == 9 || thirdField == 9) checkEd[9] = 1;
						else
							if (firstField == 9 || secondField == 9 || thirdField == 9)
							{
								system("CLS");
								cout << "Il campo Miovalore non è presente nel file";
								cout << "\nPremere un tasto per continuare\n";
								_getch();
								break;
							}
					searchFieldsNames(fileName, fieldsNames);
					system("CLS");
					{
						ifstream file(fileName);
						getline(file, lineFromFile);
						while (getline(file, lineFromFile))
						{
							lineSplit(lineFromFile, numFields, fields);
							if (numFields == 11)
							{
								if (fields[10] == "0")
								{
									cout << endl;
									for (int i = 0; i < numFields; i++)
									{
										if (checkEd[i] == 1)
										{
											cout << fieldsNames[i] << ": " << fields[i] << " ";
										}
									}
								}
							}
							else
							{
								cout << endl;
								for (int i = 0; i < numFields; i++)
								{
									if (checkEd[i] == 1)
									{
										cout << fieldsNames[i] << ": " << fields[i] << " ";
									}
								}
							}
						}
						file.close();
					}
					cout << "\n\nPremere un tasto per continuare\n";
					_getch();
					break;
				#pragma endregion
				#pragma region 6 - Ricerca Record
				case 6:
					system("CLS");
					cout << "Inserire Identificatore in OpenStreetMap\n\n";
					{
						string idOSM;
						getline(cin, idOSM);
						tuple<string, int> RecordAndPosition = searchPosition(fileName, idOSM, false, fields);
						if (get<1>(RecordAndPosition) == -1)
						{
							system("CLS");
							cout << "Il record cercato non è presente" << endl << endl;
							system("pause");
						}
						else
						{
							system("CLS");
							cout << "Il record " << get<0>(RecordAndPosition) << " è presente in posizione "<< get<1>(RecordAndPosition) << endl << endl;
							system("pause");
						}
					}
					break;
					#pragma endregion
				#pragma region 7 - Modifica Record
				case 7:
					system("CLS");
					cout << "Inserire Identificatore in OpenStreetMap\n\n";
					{
						string idOSM;
						getline(cin, idOSM);
						tuple<string, int> RecordAndPosition = searchPosition(fileName, idOSM, false, fields);
						if (get<1>(RecordAndPosition) == -1)
						{
							system("CLS");
							cout << "Il record cercato non è presente\n";
							cout << "\nPremere un tasto per continuare\n\n";
							_getch();
						}
						else
						{
							numFields = countFields(fileName);
							searchFieldsNames(fileName, fieldsNames);
							for (int i = 0; i < 9; i++)
							{
								system("CLS");
								cout << "Inserire i dati del campo \"" << fieldsNames[i] << "\" da aggiungere\n\n";
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
									cout << "Errore: uno o più campi sono vuoti o troppo lunghi, la lunghezza totale non deve superare i 242 caratteri\n";
									cout << "\nPremere un tasto per continuare\n\n";
									_getch();
								}
								else
								{
									system("CLS");
									cout << "Errore: uno o più campi sono vuoti o troppo lunghi, la lunghezza totale non deve superare i 245 caratteri\n";
									cout << "\nPremere un tasto per continuare\n\n";
									_getch();
								}
							}
							else if (!checkInputChars(numFields, inputs))
							{
								system("CLS");
								cout << "Uno o più campi contengono caratteri non validi \n\n';', '#', '\\'\n";
								cout << "\nPremere un tasto per continuare\n\n";
								_getch();
							}
							else
							{
								modifyRecord(fileName, fileNameTemp, numFields, inputs, get<1>(RecordAndPosition), recordLen);
							}
						}
					}
					break;
				#pragma endregion
				#pragma region 8 - Cancella Record
				case 8:
					bool valid;
					do
					{
						system("CLS");
						cout <<"Opzioni cancellazione\n1 - Cancella record\n2 - Recupera record\n3 - Ricompatta database\n\n";
						getline(cin, optionsSTR);
						cin.clear();
						if (!checkConversionStrToInt(optionsSTR)) {
							system("CLS");
							cout << "Inserire un numero valido\n" << endl;
							_getch();
						}
						else
						{
							optionsINT = stoi(optionsSTR);
							switch (optionsINT)
							{
							case 1:
								valid = true;
								if (!checkMyValue(fileName))
								{
									system("CLS");
									cout << "Nel file non sono presenti i campi Miovalore e Cancellazione Logica";
									cout << "\nPremere un tasto per continuare\n";
									_getch();
								}
								else
								{
									system("CLS");
									cout << "Inserire Identificatore in OpenStreetMap\n\n";
									string idOSM;
									getline(cin, idOSM);
									tuple<string, int> RecordAndPosition = searchPosition(fileName, idOSM, false, fields);
									if (get<1>(RecordAndPosition) == -1)
									{
										system("CLS");
										cout << "Il record cercato non è presente\n";
										cout << "\nPremere un tasto per continuare\n\n";
										_getch();
									}
									else
									{
										numFields = countFields(fileName);
										deleteRecord(fileName, fileNameTemp, numFields, get<0>(RecordAndPosition), get<1>(RecordAndPosition), recordLen, fields);
									}
								}
								break;
							case 2:
								valid = true;
								if (!checkMyValue(fileName))
								{
									system("CLS");
									cout << "Nel file non sono presenti i campi Miovalore e Cancellazione Logica\n";
									cout << "\nPremere un tasto per continuare\n";
									_getch();
								}
								else
								{
									system("CLS");
									cout << "Inserire Identificatore in OpenStreetMap\n\n";
									string idOSMDEL;
									getline(cin, idOSMDEL);
									tuple<string, int> RecordAndPosition = searchPosition(fileName, idOSMDEL, true, fields);
									if (get<1>(RecordAndPosition) == -1)
									{
										system("CLS");
										cout << "Il record cercato non è presente\n";
										cout << "\nPremere un tasto per continuare\n\n";
										_getch();
									}
									else
									{
										numFields = countFields(fileName);
										recoverRecord(fileName, fileNameTemp, numFields, get<0>(RecordAndPosition), get<1>(RecordAndPosition), recordLen, fields);
									}
								}
								break;
							case 3:
								valid = true;
								if (!checkMyValue(fileName))
								{
									system("CLS");
									cout << "Nel file non sono presenti i campi Miovalore e Cancellazione Logica\n";
									cout << "\nPremere un tasto per continuare\n\n";
									_getch();
								}
								else
								{
									bool validCOMP;
									do
									{
										system("CLS");
										cout << "Cancellare definitivamente i record eliminati\n1 - Si\n0 - No\n\n";
										getline(cin, optionsSTR);
										cin.clear();
										if (!checkConversionStrToInt(optionsSTR)) {
											system("CLS");
											cout << "Inserire un numero valido\n" << endl;
											_getch();
											break;
										}
										else
										{
											optionsINT = stoi(optionsSTR);
											switch (optionsINT)
											{
											case 1:
												validCOMP = true;
												permDelete(fileName, fileNameTemp, fields);
												break;
											case 0:
												validCOMP = true;
												break;
											default:
												validCOMP = false;
												system("CLS");
												cout << "Opzione non valida\n\nPremere un tasto per continuare\n\n";
												_getch();
												break;
											}
										}
									} while (!validCOMP);
								}
								break;
							default:
								valid = false;
								system("CLS");
								cout << "Opzione non valida\n\nPremere un tasto per continuare\n\n";
								_getch();
								break;
							}
						}
					} while (!valid);
					break;
				#pragma endregion
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
