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
	double var_value = 0;
	expression sequence("Default Sequence");
	string var_name = "Default VarName";
	string error("");
	size_t error_position(0);
	string file;// Переменная для чтения из файла
	string saver("");
	unsigned choice = 0;
	while (choice != 0x1B) {
		system("cls");
		cout << "Result:" << saver << endl;
		cout << error << " " << error_position<<endl;
		cout << *sequence << endl;
		cout << "Choose type of reading: F1: Keyboard; F2: File; F3: Add var; F4: del var; F5: Count;  Esc: Exit" << endl;
		cout << "Current variables are:" << endl;
		for(auto var : sequence.get_map_of_variables() )
		{
			cout << var.first << " is " << var.second << endl;
		}
		error = "";
		saver = "";
		error_position = 0;
		choice = _getch();
		switch (choice)
		{
		case 0x3B://f1
		{
			try {
				cout << endl << "type your expression" << endl;
				string name;
				getline(cin, name);
				sequence = name;
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
				sequence = fstream(file+".txt");
			}
			catch (expression::exceptions& ex) {
				error = ex.what();
				error_position = ex.getPosition();
			}
			break;
		}
		case 0x3D://f3
		{
			try {
				cout << "Write yours Var Name" << endl;
				cin >> var_name;
				cout << "Write yours Var Value" << endl;
				cin >> var_value;
				if (cin.fail())rewind(stdin);
				sequence.add_variable(pair<string,double>(var_name, var_value));
			}
			catch (expression::exceptions& ex) {
				error = ex.what();
				error_position = ex.getPosition();
			}
			// Выдает ошибку "Нарушение доступа для чтения"
			break;
		}
		case 0x3E://f4
		{
			cout << "type name of variable to delete" << endl;
			string name;
			cin>>name;
			if (cin.fail()) {
				rewind(stdin);
				break;
			}
			sequence.erase(name);
			break;
		}
		case 0x3F://f5
		{
			try {
				saver = sequence.calculate();
			}
			catch (expression::exceptions& ex) {
				error = ex.what();
				error_position = ex.getPosition();
			}
			break;
		}

		default:
			break;
		}
	}
}
	