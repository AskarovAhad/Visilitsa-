//библиотеки
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <ctime> 
#include <string>
#include <map>
#include <unordered_map>
#include <filesystem>
//пространства имен
using namespace std;
namespace fs = filesystem;

//глобальные переменные
string DISK = "d:\\visilitsa"; //Путь к папке висилицва
string LOGIN;	//для записи результатов игры
string USERS = "d:\\visilitsa\\USERS\\log.txt";		//путь для хранения и считывания логина и паролей
string PLAYER = "d:\\visilitsa\\PLAYER\\";			//путь для хранения результатов игры
string SYMBOLS;										//для хранения и записи введенных символов во время игры
bool* Status;//статус угаданных букв   line - слово


//класс
class Visilica {
public:
	void setWord(string new_word) { word = new_word; }
	string getWord() { return word; }
	void setHP(int new_hp) { hp = new_hp; }
	int getHP() { return hp; }
	void setAttempts(int new_attempts) { attempts = new_attempts; }
	int getAttempts() { return attempts; }
	double getTimeSession() { return time_session; }
	void setTimeSession(double time) { time_session = time; }
	void FillDefault();

private:
	friend void Print();
	int hp = 0;
	string word = "nAn";
	int attempts = 0;
	double time_session;
};

void SaveInfoToFile(Visilica& game);
bool CheckSym(char c);
void Registration();
void Print(Visilica& game);
bool CheckSymbol(Visilica& game, char symbol);
bool CheckWin(Visilica& game);
void Game(Visilica& game);
void MainMenu();
void AfterGameMenu();
void ReadToMap(unordered_map<string, string>& a, ifstream& tests);
bool Login(ifstream& logs);
void ReadFromFile(Visilica& game);
void EndGameMenu(Visilica& game);

int main() {
	srand(time(NULL));
	string ans;
	ifstream path(USERS);
	Visilica game;
	cout << "===Welcome===\n";
	MainMenu();
	cin >> ans;
	while (ans != "0") {
		if (ans == "1") {
			if (Login(path)) {
				Game(game);
				AfterGameMenu();
				SaveInfoToFile(game);
			}
			else {
				cout << "\nWrong login\n";
				MainMenu();
			}
		}
		if (ans == "2") {
			Registration();
			MainMenu();
		}
		if (ans == "4") {
			Game(game);
			AfterGameMenu();
			SaveInfoToFile(game);
		}
		cin >> ans;
		system("cls");
	}
	return 0;
}
//регистрация нового пользователя
void Registration() {
	ofstream to_login(USERS, ios::app);
	string login;
	string password;
	cout << "Enter login -> "; cin >> login;
	cout << "Enter password -> "; cin >> password;
	to_login << login << "\n";
	to_login << password << "\n";
	string path = DISK + login;
	fs::create_directories(path);
	to_login.close();
	//path += ".txt";
}
//вывод угаданных букв 
void Print(Visilica& game) {
	for (int i = 0; i < game.getWord().size(); i++) {
		if (Status[i] == true) { cout << game.getWord()[i]; }
		else { cout << '-'; }
	}
	cout << endl;
}
//функция по умолчанию для слов и BOOL массива
void Visilica::FillDefault() {
	int r_num = rand() % word.size() - 1;
	Status = new bool[word.size()];
	hp = word.size() + 8; //значения хп = количество букв в слове + 8
	for (int i = 0; i < word.size(); i++) {//присваиваем по умолчанию false
		Status[i] = false;
	}
	for (int i = 0; i < 2;) {//для начала игры определяем 2 рандомные буквы на показ игроку 
		r_num = rand() % word.size() - 1;
		if (Status[r_num] == false) {
			Status[r_num] = true;
			i++;
		}
	}
}
//Функция проверки введенного символа
bool CheckSymbol(Visilica& game, char symbol) {
	for (int i = 0; i < game.getWord().size(); i++) {
		if (symbol == game.getWord()[i]) {
			if (Status[i] == false) { //status == false
				Status[i] = true;
				system("cls");
				cout << "U guessed letter ' " << game.getWord()[i] << " '\n";
				return true;
			}
			else {
				continue;
			}
		}
		if (i + 1 == game.getWord().size()) {
			game.setHP(game.getHP() - 1); game.setAttempts(game.getAttempts() + 1);
			system("cls");
			cout << "U didn't guessed. Ur attempts: " << game.getHP() << endl;

			return false;
		}
	}
}
//функция проверки победы
bool CheckWin(Visilica& game) {
	int count = 0;
	for (int i = 0; i < game.getWord().size(); i++) {
		if (Status[i] == true) { //status == true
			count++;
		}
	}
	if (count == game.getWord().size()) { return false; }
	else { return true; }
}
//сама игра
void Game(Visilica& game) {
	char symbol;
	int start = clock();// засекаем время старта
	ReadFromFile(game);
	cout << "Game starts!\n";
	game.FillDefault();
	while (CheckWin(game) && game.getHP() != 0) {
		Print(game);
		cout << "Enter letter: ";
		cin >> symbol;
		CheckSymbol(game, symbol);
		SYMBOLS.push_back(symbol);
		SYMBOLS += " - ";
	}
	EndGameMenu(game);
	int end = clock();
	double seconds = (double)(end - start) / CLOCKS_PER_SEC;
	game.setTimeSession(seconds);
}

void ReadToMap(unordered_map<string, string>& a, ifstream& tests) {
	string que;
	while (getline(tests, que)) {
		string ans;
		while (getline(tests, ans)) {
			a.insert(pair<string, string>(que, ans));
			break;
		}
	}
	tests.close();
}
//система логина
bool Login(ifstream& logs) {
	system("cls");
	string log; string pas;
	cout << "Enter login -> "; cin >> log;
	cout << "Enter password -> "; cin >> pas;
	unordered_map<string, string> login;
	ReadToMap(login, logs);
	for (auto x : login) {
		if (x.first == log && x.second == pas) {
			LOGIN = x.first;
			logs.close();
			return true;
		}
	}
	logs.close();
	return false;
}
//меню
void MainMenu() {
	cout << "1 - sign in \n";
	cout << "2 - Sign up\n";
	cout << "0 - Exit\n";
	cout << "-->";
}
void AfterGameMenu() {
	cout << "\n4 - Play one more time\n";
	cout << "0 - exit\n";
}
//Функция для сохранения игровой информации в файл
void SaveInfoToFile(Visilica& game) {
	string path = PLAYER + LOGIN + "info.txt";
	ofstream put(path, ios::app);
	time_t seconds = time(NULL);
	tm* timeinfo = localtime(&seconds);
	for (int i = 0; i < 1; i++) {
		put << "/*--------------------------------*/" << endl;
		put << "Game session : " << asctime(timeinfo);
		put << "Bad attempts: " << game.getAttempts() << endl;
		put << "HP: " << game.getHP() << endl;
		put << "Word: " << game.getWord() << endl;
		put << "Played time: " << game.getTimeSession() << " seconds " << endl;
		put << "Entered letters: " << SYMBOLS << endl;
		put << "/*--------------------------------*/" << endl << endl;
	}
	put.close();
}
//функция вывода результатов игры
void EndGameMenu(Visilica& game) {
	if (!CheckWin(game)) {
		system("cls");
		cout << "You guessed word: " << game.getWord() << endl;
		cout << "Congratulations with won!" << endl;
		cout << "You may watch your statistics in file" << endl;
	}
	if (game.getHP() == 0) {
		system("cls");
		cout << "You loosed :(" << endl;
		cout << "You didn't guessed " << game.getWord() << " word" << endl;
		cout << "Better luck next time" << endl;
	}
}

//функция рандомного выбора и расшифровывания слова
void ReadFromFile(Visilica& game) {
	string s; int index = 0;
	vector <string> cword;			//считываетм слово и потоком записываем в вектор
	ifstream stream("d:\\file.txt");
	while (getline(stream, s)) {
		cword.push_back(s);
		stream.ignore(1);
	}
	int num = rand() % cword.size();	//тут рандомно определяем слово
	s = cword[num];
	for (int i = 0; i < s.size(); i++) {		//тут слово расшифровывается
		index = s[i];
		index = index - 1;
		s[i] = char(index);
	}
	stream.close();
	game.setWord(s);
}