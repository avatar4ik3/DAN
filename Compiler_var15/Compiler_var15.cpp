// Compiler_var15.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include<fstream>
#include <iostream>
#include <conio.h>
#include "expression.h"


using namespace std;
/*
	 в мейне:
	 создание / считывание строки
	 добавление переменных ( любое количество)
	 вывод ОПЗ на экран
	 Вывод ответа из САЛУ на экран

	 и оформить все красиво чтобы бахта не доебалась
*/

/*
	по поводу try catch
	метод getPosition() не работает если в условии catch стоит const
	я хз почему так но это нужно учитывать и мне пока впадлу этим заниматься
*/
int main()
{
	int var;
	expression sequence("default sequence");
	string error;
	size_t error_position;
	string file;// Переменная для чтения из файла
	
	unsigned choice = 0;
	while (choice != 0x1B) {
		cout << error << " " << error_position<<endl;
		cout << "Choose type of reading: F1: Keyboard; F2: File; F3: Add var; F4: Write OPZ; F5: Count;  Esc: Exit" << endl;
		cout << *sequence;
		error = "";
		error_position = -1;
		choice = _getch();
		switch (choice)
		{
		case 0x3B://f1
		{
			try {
				string name;
				getline(cin, name);
				expression sequence(name);
			}
			catch (expression::exceptions& ex) {
				error = ex.what();
				error_position = ex.getPosition();
			}
			break;
		}
		case 0x3C://f2
		{
			cout << "Write file name: ";
			cin >> file;
			try {
				expression sequence(fstream(file+".txt"));
			}
			catch (expression::exceptions& ex) {
				cout << ex.what() << ex.getPosition();
			}
			break;
		}
		case 0x3D://f3
		{
			cin >> var;
			//Тут я вообще хз чо писать тока это
			// add_variable(var);
			// set_variable(var)
			break;
		}
		case 0x3E://f4
		{
			try {
				sequence.transmute();
			}
			catch (expression::exceptions& ex) {
				cout << ex.what() << ex.getPosition();
			}
			break;
		}
		case 0x3F://f5
		{
			try {
				sequence.calculate();
			}
			catch (expression::exceptions& ex) {
				cout << ex.what() << ex.getPosition();
			}
			break;
		}
		default:
			break;
		}
	}
	/*try
	{
		map<string, double> dict;
		dict.insert(pair<string, double>("a", 1.5));
		expression a("#");
		a.set_variables(dict);
		a.transmute();
	}
	catch (expression::exceptions& ex)
	{
		cout << ex.what() << ex.getPosition();
	}*/
}
	