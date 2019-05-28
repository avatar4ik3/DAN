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

const token_queue  expression::transmute()
{
	stack<token> tokens;  // стак токенов
	token tk; //токен для работы 
	istringstream input(_expression_line); // поток из строки для удобства работы
	string line; // строка для считывания
	bool prev_is_number(false);
	_result_queue.~token_queue();//очситка очереди


	while (!tk.is_end_operand()) {
		if (!input.eof())throw exceptions("Expected operand ';' at the end of expression");
		input >> line;
		try
		{
			tk = pair<string, bool>(line, prev_is_number);
		}
		catch (const std::exception& ex)
		{
			throw("Unexpected element at expression::transmute", input.tellg());
		}
		//число отправляется в ответ
		if (tk.is_number()) {
			_result_queue.push(tk);
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
					_result_queue.push(tokens.top());
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
					_result_queue.push(tokens.top());
					tokens.pop();
				}
				tokens.push(tk);
			}
			prev_is_number = false;
		}
	}
	//пока остались операнды выкидываем их в ответ
	while (!tokens.empty()) {
		_result_queue.push(tokens.top());
		tokens.pop();
	}
	//проверяем
	
	/*while (!_result_queue.empty()) {
		tk = _result_queue.front();
		cout << tk.get_operand() << " ";
		_result_queue.pop();
	}*/
	return _result_queue; // верну нормальное потом
}

//часть антона
//П.С ТОоха юзай throw exceptions(string,position) для выброса исключений
const string expression::calculate() {
	/*stack<string> Stack;
	if (_result_queue.empty() == false) throw exceptions("_result_queue is empty");
	else {
		while (_result_queue.empty() != false) {
			string buff;
			buff = _result_queue.front();
			if (buff != "/") {//

			}
		}
	}*/
	return "1,2,3";// заглушка
}

