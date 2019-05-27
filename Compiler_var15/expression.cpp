#include "pch.h"
#include<algorithm>
#include <iostream>
#include "expression.h"
using namespace std;



expression::expression():_expression_line("")//,operators({ '^','~',"+","-","*","/",'&','<','>','=','#','!' })
{
	throw exceptions("empty expression");
}

expression::expression(fstream input)//:operators({ '^','~',"+","-","*","/",'&','<','>','=','#','!' }) 
{
	getline(input, _expression_line);
	string a(_expression_line); 
	a.erase(std::remove(a.begin(), a.end(), ' '), a.end());
	if (a.empty())throw exceptions("empty expression");
 }

expression::expression(string & str):_expression_line(str)//,operators({ '^','~',"+","-","*","/",'&','<','>','=','#','!' })
{
	string a(_expression_line);
	a.erase(std::remove(a.begin(), a.end(), ' '), a.end());
	if (a.empty())throw exceptions("empty expression");
}

expression::expression(const char *str):_expression_line(str)//,operators({ '^','~',"+","-","*","/",'&','<','>','=','#','!' })
{
	string a(_expression_line);
	a.erase(std::remove(a.begin(), a.end(), ' '), a.end());
	if (a.empty())throw exceptions("empty expression");
}

expression::expression(const expression & rh):_expression_line(rh._expression_line)//,operators({ '^','~',"+","-","*","/",'&','<','>','=','#','!' })
{
	string a(_expression_line);
	a.erase(std::remove(a.begin(), a.end(), ' '), a.end());
	if (a.empty())throw exceptions("empty expression");
}

const string expression::transmute()
{
	stack<string> tokens; // это стек операндов
	queue<string> result; // это результат в виде очереди чтобы было удобней работать 
	string line, prev_line; // строка в которую читаем
	int prior(0), top_prior(0);//приоритеты строк
	int assoc(0), top_assoc(0);//ассоциативноть операций 1 = правая 2 = левая
	istringstream input(_expression_line);//поток из строки для удобства работы

	while (!input.eof()) {
		input >> line;
		if (line.length() >= 1 && operators.find(line) == operators.end()) {
			try
			{
				stod(line.c_str());
				result.push(line);
			}
			catch (const std::exception&)
			{
				throw exceptions("Unexpected token at expression::calculate",input.tellg());
			}
		}
		else {
			//установка приоритета текущеё операции
			//TODO добавть учет операторов изменения знака
			prior = 0;
			assoc = 0;
			if (priority1.find(line) != priority1.end()) { 
				prior = 6; assoc = 1; 
			}
			if (priority2.find(line) != priority2.end()) { 
				prior = 5; assoc = 2; 
			}
			if (priority3.find(line) != priority3.end()) {
				prior = 4; assoc = 1;
			}
			if (priority4.find(line) != priority4.end()) {
				prior = 3; assoc = 1;
			}
			if (priority5.find(line) != priority5.end()) {
				prior = 2; assoc = 1;
			}
			if (priority6.find(line) != priority6.end()) {
				prior = 1; assoc = 2;
			}

			//сам алгоритм
			//если строка - число кидаем в стек ( тут он уже кинут)
			//если строка - постфиксная функция то кидаем в стек(тут нет постфиксных функцый)
			//если символ - -префиксная функция то кидаем в стек
			if (line == "entier" || line == "frac") {
				tokens.push(line);
			}
			//если символ - открывающая скобка то кидаем в стек
			if (line == "(")
			{
				tokens.push(line);
			}
			//если символ - закрывающая скобка то выталкиваем токены из стека пока не дойдем до открывающей скобки
			if (line == ")") {
				string tmp(tokens.top());
				try
				{
					tokens.pop();
				}
				catch (const std::exception&)
				{
					throw exceptions("unbalanced expression at expression::calculate", input.tellg());
				}
				while (tmp != "(") {
					result.push(tmp);
					tmp = tokens.top();
					try
					{
						tokens.pop();
					}
					catch (const std::exception&)
					{
						throw exceptions("unbalanced expression at expression::calculate", input.tellg());
					}
				}
			}
			//если бинарная операция
			if (prior) { 
				//TODO перевести строку /*tokens.top() == "entier" || tokens.top() == "frac" ||*/ в приемлимый вид
				while (/*tokens.top() == "entier" || tokens.top() == "frac" ||*/ top_prior > prior || (top_prior == prior && top_assoc == 2)) {
					result.push(tokens.top());
					tokens.pop();
					//изменение приоритета вершины стека
					if (priority1.find(tokens.top()) != priority1.end()) {
						top_prior = 6; top_assoc = 1;
					}
					if (priority2.find(tokens.top()) != priority2.end()) {
						top_prior = 5; top_assoc = 2;
					}
					if (priority3.find(tokens.top()) != priority3.end()) {
						top_prior = 4; top_assoc = 1;
					}
					if (priority4.find(tokens.top()) != priority4.end()) {
						top_prior = 3; top_assoc = 1;
					}
					if (priority5.find(tokens.top()) != priority5.end()) {
						top_prior = 2; top_assoc = 1;
					}
					if (priority6.find(tokens.top()) != priority6.end()) {
						top_prior = 1; top_assoc = 2;
					}
				}
				tokens.push(line);
			}
		}
	}
	while (!tokens.empty()) {
		result.push(tokens.top());
		tokens.pop();
	}
	//проверка
	while (!result.empty()) {
		cout << result.front() << " ";
		result.pop();
	}

	return "1,2,3"; // верну нормальное потом
}

//часть антона
//П.С ТОоха юзай throw exceptions(string,position) для выброса исключений
const string expression::calculate() {

	return "1,2,3";
}

