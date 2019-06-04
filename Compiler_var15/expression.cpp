#include "pch.h"
#include<algorithm>
#include <iostream>
#include "expression.h"
#include<cmath>
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

expression & expression::operator=(fstream input)
{
	getline(input, _expression_line);
	string a(_expression_line);
	a.erase(std::remove(a.begin(), a.end(), ' '), a.end());
	if (a.empty())throw exceptions("empty expression");
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
		if (!_result_queue->empty() and _result_queue->front().get_operand() == "+" and _result_queue->front().is_unary()) {
			if (Stack.size() > 0) {
				double temp = stod(Stack.top().get_operand());
				Stack.pop();
				Stack.push(token(to_string(temp), true));
				_result_queue->pop();
			}
			else {
				throw exceptions("Stack doesn't contain any numbers to work with unary +\n");
			}
		}
		//если бинарный плюс
		if (!_result_queue->empty() and _result_queue->front().get_operand() == "+" and _result_queue->front().is_binary()) {
			if (Stack.size() >= 2) {
				double sum = stod(Stack.top().get_operand());
				Stack.pop();
				sum += stod(Stack.top().get_operand());
				Stack.pop();
				Stack.push(token(to_string(sum), true));
				_result_queue->pop();
			}
			else {
				throw exceptions("Stack doesn't contain two numbers to work with binary +\n");
			}
		}
		//если унарный минус
		if (!_result_queue->empty() and _result_queue->front().get_operand() == "-" and _result_queue->front().is_unary()) {
			if (Stack.size() > 0) {
				double temp = stod(Stack.top().get_operand());
				Stack.pop();
				temp = temp * (-1);
				Stack.push(token(to_string(temp), true));
				_result_queue->pop();
			}
			else {
				throw exceptions("Stack doesn't contain any numbers to work with unary -\n");
			}
		}

		//если бинарный минус
		if (!_result_queue->empty() and _result_queue->front().get_operand() == "-" and _result_queue->front().is_binary() ) {
			if (Stack.size() >= 2) {
				double min1 = stod(Stack.top().get_operand());
				Stack.pop();
				double min2 = stod(Stack.top().get_operand());
				Stack.pop();
				double min = min2 - min1;
				Stack.push(token(to_string(min), true));
				_result_queue->pop();
			}
			else {
				throw exceptions("Stack doesn't contain two numbers to work with binary -\n");
			}
		}
		// если умножение
		if (!_result_queue->empty() and _result_queue->front().get_operand() == "*" ) {
			if (Stack.size() >= 2) {
				double mul = stod(Stack.top().get_operand());
				Stack.pop();
				mul *= stod(Stack.top().get_operand());
				Stack.pop();
				Stack.push(token(to_string(mul), true));
				_result_queue->pop();
			}
			else {
				throw exceptions("Stack doesn't contain two numbers to work with *\n");
			}
		}

		//елси деление
		if (!_result_queue->empty() and _result_queue->front().get_operand() == "/" ) {
			if (Stack.size() >= 2) {
				double div1 = stod(Stack.top().get_operand());
				Stack.pop();
				if (div1 == 0) { throw exceptions("Div by zero\n"); }
				double div2 = stod(Stack.top().get_operand());
				Stack.pop();
				double div = div2 / div1;
				Stack.push(token(to_string(div), true));
				_result_queue->pop();
			}
			else {
				throw exceptions("Stack doesn't contain two numbers to work with /\n");
			}
		}
		//если возведение в степень
		if (!_result_queue->empty() and _result_queue->front().get_operand() == "^") {
			if (Stack.size() >= 2) {
				double poww1 = stod(Stack.top().get_operand());
				Stack.pop();
				double poww2 = stod(Stack.top().get_operand());
				Stack.pop();
				double poww = pow(poww2, poww1);
				Stack.push(token(to_string(poww), true));
				_result_queue->pop();
			}
			else {
				throw exceptions("Stack doesn't contain two numbers to work with ^\n");
			}
		}

		//елси <
		if (!_result_queue->empty() and _result_queue->front().get_operand() == "<" ) {
			if (Stack.size() >= 2) {
				double com1 = stod(Stack.top().get_operand());
				Stack.pop();
				double com2 = stod(Stack.top().get_operand());
				Stack.pop();
				if (com2 < com1) Stack.push(token("1", true));
				else Stack.push(token("0", true));
				_result_queue->pop();
			}
			else {
				throw exceptions("Stack doesn't contain two numbers to work with <\n");
			}
		}

		//если >
		if (!_result_queue->empty() and _result_queue->front().get_operand() == ">" ) {
			if ( Stack.size() >= 2) {
				double com1 = stod(Stack.top().get_operand());
				Stack.pop();
				double com2 = stod(Stack.top().get_operand());
				Stack.pop();
				if (com2 > com1) Stack.push(token("1", true));
				else Stack.push(token("0", true));
				_result_queue->pop();
			}
			else {
				throw exceptions("Stack doesn't contain two numbers to work with >\n");
			}
		}
		//елси =
		if (!_result_queue->empty() and _result_queue->front().get_operand() == "=") {
			if (Stack.size() >= 2) {
				double com1 = stod(Stack.top().get_operand());
				Stack.pop();
				double com2 = stod(Stack.top().get_operand());
				Stack.pop();
				if (com2 == com1) Stack.push(token("1", true));
				else Stack.push(token("0", true));
				_result_queue->pop();
			}
			else {
				throw exceptions("Stack doesn't contain two numbers to work with =\n");
			}
		}
		//если #
		if (!_result_queue->empty() and _result_queue->front().get_operand() == "#" ) {
			if (Stack.size() >= 2) {
				double com1 = stod(Stack.top().get_operand());
				Stack.pop();
				double com2 = stod(Stack.top().get_operand());
				Stack.pop();
				if (com2 != com1) Stack.push(token("1", true));
				else Stack.push(token("0", true));
				_result_queue->pop();
			}
			else {
				throw exceptions("Stack doesn't contain two numbers to work with #\n");
			}
		}
		//если ~
		if (!_result_queue->empty() and _result_queue->front().get_operand() == "~" and Stack.size() > 0) {
			double neg = stod(Stack.top().get_operand());
			Stack.pop();
			if(neg == 0) Stack.push(token("1", true));
			else Stack.push(token("0", true));
			_result_queue->pop();
		}
		else if (!_result_queue->empty() and _result_queue->front().get_operand() == "~" and Stack.size() <= 0) {
			throw exceptions("Stack doesn't contain two numbers to work with ~\n");
		}

		//если &
		if (!_result_queue->empty() and _result_queue->front().get_operand() == "&" ) {
			if (Stack.size() >= 2) {
				double con1 = stod(Stack.top().get_operand());
				Stack.pop();
				double con2 = stod(Stack.top().get_operand());
				Stack.pop();
				if (con1 != 0 and con2 != 0) Stack.push(token("1", true));
				else Stack.push(token("0", true));
				_result_queue->pop();
			}
			else {
				throw exceptions("Stack doesn't contain two numbers to work with &\n");
			}
		}

		//если !
		if (!_result_queue->empty() and _result_queue->front().get_operand() == "!" ) {
			if (Stack.size() >= 2) {
				double dis1 = stod(Stack.top().get_operand());
				Stack.pop();
				double dis2 = stod(Stack.top().get_operand());
				Stack.pop();
				if (dis1 != 0 or dis2 != 0) Stack.push(token("1", true));
				else Stack.push(token("0", true));
				_result_queue->pop();
			}
			else {
				throw exceptions("Stack doesn't contain two numbers to work with !\n");
			}
		}


	}
	
	if(Stack.size() > 1) throw exceptions("Invalid Data Entered\n");
	else {
		return Stack.top().get_operand();
	}

}

