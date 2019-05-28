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
	stack<token> tokens;  // ���� �������
	token tk; //����� ��� ������ 
	istringstream input(_expression_line); // ����� �� ������ ��� �������� ������
	string line; // ������ ��� ����������
	bool prev_is_number(false);
	_result_queue.~token_queue();//������� �������


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
		//����� ������������ � �����
		if (tk.is_number()) {
			_result_queue.push(tk);
			prev_is_number = true;
		}
		else {
			//���� �������� �� � �����(����� ������� ���)
			//���� ������� �� � ����
			//���� ����������� ������ �� � ����
			if (tk.is_unary() || tk.is_opened_bracket()) {
				tokens.push(tk);
			}
			//���� ����������� �� ����������� � ����� �� �����������
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
			//���� �������� �������� �� ����������� ��� ���� ��� ������� ��� ������������ ��� ���������������� � ���������� �����������
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
	//���� �������� �������� ���������� �� � �����
	while (!tokens.empty()) {
		_result_queue.push(tokens.top());
		tokens.pop();
	}
	//���������
	
	/*while (!_result_queue.empty()) {
		tk = _result_queue.front();
		cout << tk.get_operand() << " ";
		_result_queue.pop();
	}*/
	return _result_queue; // ����� ���������� �����
}

//����� ������
//�.� ����� ���� throw exceptions(string,position) ��� ������� ����������
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
	return "1,2,3";// ��������
}

