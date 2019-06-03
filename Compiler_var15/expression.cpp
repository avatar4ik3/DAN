#include "pch.h"
#include<algorithm>
#include <iostream>
#include "expression.h"
using namespace std;




expression::expression():_expression_line(""),map_of_variables(nullptr),_result_queue(nullptr)//,operators({ '^','~',"+","-","*","/",'&','<','>','=','#','!' })
{
	throw exceptions("empty expression");
}

expression::expression(fstream input): map_of_variables(nullptr), _result_queue(nullptr)//:operators({ '^','~',"+","-","*","/",'&','<','>','=','#','!' }) 
{
	getline(input, _expression_line);
	string a(_expression_line); 
	a.erase(std::remove(a.begin(), a.end(), ' '), a.end());
	if (a.empty())throw exceptions("empty expression");
 }

expression::expression(string & str):_expression_line(str), map_of_variables(nullptr), _result_queue(nullptr)//,operators({ '^','~',"+","-","*","/",'&','<','>','=','#','!' })
{
	string a(_expression_line);
	a.erase(std::remove(a.begin(), a.end(), ' '), a.end());
	if (a.empty())throw exceptions("empty expression");
}

expression::expression(const char *str):_expression_line(str), map_of_variables(nullptr), _result_queue(nullptr)//,operators({ '^','~',"+","-","*","/",'&','<','>','=','#','!' })
{
	string a(_expression_line);
	a.erase(std::remove(a.begin(), a.end(), ' '), a.end());
	if (a.empty())throw exceptions("empty expression");
}

expression::expression(const expression & rh):_expression_line(rh._expression_line), map_of_variables(nullptr), _result_queue(nullptr)//,operators({ '^','~',"+","-","*","/",'&','<','>','=','#','!' })
{
	string a(_expression_line);
	a.erase(std::remove(a.begin(), a.end(), ' '), a.end());
	if (a.empty())throw exceptions("empty expression");
}

const string expression::operator*()
{
	return string(this->_expression_line);
}

expression & expression::operator=(const string & line)
{
	string a(line);
	a.erase(std::remove(a.begin(), a.end(), ' '), a.end());
	if (a.empty())throw exceptions("empty expression");
	else _expression_line = line;
}

void expression::set_variables(const map<string, double> & dictionary)
{
	if (dictionary.empty())throw exceptions("attempt to add empty dectionary");
	delete map_of_variables;
	map_of_variables = new map<string,double>(dictionary);
}

void expression::add_variable(const pair<string, double>& var)
{
	map_of_variables->insert(var);
}

const queue<token> expression::transmute()
{
	stack<token> tokens;  // стак токенов
	token tk; //токен для работы 
	istringstream input(_expression_line); // поток из строки для удобства работы
	string line; // строка для считывания
	bool prev_is_number(false);
	//_result_queue.~queue();//очситка очереди
	queue<token> result_queue;


	while (!input.eof()) {
		//если конец файла то заканиваем работу
		if (input.peek() == EOF)break;
		//if (!input.eof())throw exceptions("Expected operand ';' at the end of expression");
		input >> line;
		try
		{
			tk = pair<string, bool>(line, prev_is_number);
		}
		catch (const std::exception&)
		{
			if (map_of_variables == nullptr || map_of_variables->find(line) == map_of_variables->end()) {
				throw exceptions("Unexpected element at expression::transmute", input.tellg());
			}
			else {
				tk = pair<string, bool>(to_string(map_of_variables->find(line)->second), prev_is_number);
			}
		}
		//число отправляется в ответ
		if (tk.is_number()) {
			result_queue.push(tk);
			prev_is_number = true;
		}
		else {
			//если поствикс то в ответ(таких функций нет)
			//если префикс то в стек
			//если открывающая скобка то в стек
			if (tk.is_unary() || tk.is_opened_bracket()) {
				tokens.push(tk);
			}
			//если закрывающая то выталкиваем в ответ до открывающей
			if (tk.is_closed_bracket()) {
				while (!tokens.top().is_opened_bracket()) {
					result_queue.push(tokens.top());
					try
					{
						tokens.pop();
					}
					catch (const std::exception&)
					{
						throw exceptions("Unbalanced expression at expression::transmute", input.tellg());
					}
				}
				tokens.pop();
			}
			//если бинарная операция то выталкиваем топ пока топ префикс или приорететней или левоасоциативный с одинаковым приоритетом
			if (tk.is_binary()) {
				while (!tokens.empty() && (tokens.top().is_unary() || tokens.top() > tk || (tokens.top() == tk && tokens.top().is_left_associative()))) {
					result_queue.push(tokens.top());
					tokens.pop();
				}
				tokens.push(tk);
			}
			prev_is_number = false;
		}
	}
	//пока остались операнды выкидываем их в ответ
	while (!tokens.empty()) {
		result_queue.push(tokens.top());
		tokens.pop();
	}
	//проверяем
	delete _result_queue;
	_result_queue = new queue<token>(result_queue);
	
	return result_queue; // верну нормальное потом
}

//часть антона
//П.С ТОоха юзай throw exceptions(string,position) для выброса исключений
//P.P.S нужно так же учесть что в токене есть тип variable (переменная) и ответ нужно давать с учетем их
//например если было 2 a + то ответ будет a + 2
//именно поэтому возращаем строку а не ответ
//если переменных нет то ответом должно быть число
//то есть нужно знать возможно ли получить внятный ответ,но вообще можно не ебаться и воспользоваться методами класса токен
//P.P.P.S я еще поебусь с тем чтобы потом у пользователей просить ввести значения этих переменых и пускать на второй круг но уже со взятыми данными
//либо сразу просить ввести значения переменных после transmute
const string expression::calculate() { 
	stack<token> Stack;
	if (_result_queue == nullptr) {
		throw exceptions("No pointer\n");
	}

	while (!_result_queue->empty())
	{

		//если число
		if(_result_queue->front().is_number()) {
			Stack.push(_result_queue->front());
			_result_queue->pop();
		}

		//если унарный плюс
		if (_result_queue->front().get_operand() == "+" and _result_queue->front().is_unary() and Stack.size() > 0) {

		}
		else if (_result_queue->front().get_operand() == "+" and _result_queue->front().is_unary() and Stack.size() <= 0) {

		}

		//если бинарный плюс
		if (_result_queue->front().get_operand() == "+" and _result_queue->front().is_binary() and Stack.size() >= 2) {

		}
		else if (_result_queue->front().get_operand() == "+" and _result_queue->front().is_binary() and Stack.size() < 2) {

		}

		//если унарный минус
		if (_result_queue->front().get_operand() == "-" and _result_queue->front().is_unary() and Stack.size() > 0) {

		}
		else if (_result_queue->front().get_operand() == "-" and _result_queue->front().is_unary() and Stack.size() <= 0) {

		}

		//если бинарный минус
		if (_result_queue->front().get_operand() == "-" and _result_queue->front().is_binary() and Stack.size() >= 2) {

		}
		else if (_result_queue->front().get_operand() == "-" and _result_queue->front().is_binary() and Stack.size() < 2) {

		}

		// если умножение
		if (_result_queue->front().get_operand() == "*" and Stack.size() >= 2) {

		}
		else if (_result_queue->front().get_operand() == "*" and Stack.size() < 2) {

		}

		//елси деление
		if (_result_queue->front().get_operand() == "/" and Stack.size() >= 2) {

		}
		else if (_result_queue->front().get_operand() == "/" and Stack.size() < 2) {

		}

		//если возведение в степень
		if (_result_queue->front().get_operand() == "^" and Stack.size() >= 2) {

		}
		else if(_result_queue->front().get_operand() == "^" and Stack.size() < 2) {

		}

		//елси <
		
	}
	


	return "1,2,3";// заглушка
}

