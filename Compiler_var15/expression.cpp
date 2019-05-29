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

const queue<token> * expression::transmute()
{
	stack<token> tokens;  // ���� �������
	token tk; //����� ��� ������ 
	istringstream input(_expression_line); // ����� �� ������ ��� �������� ������
	string line; // ������ ��� ����������
	bool prev_is_number(false);
	//_result_queue.~queue();//������� �������
	queue<token> result_queue;


	while (!input.eof()) {
		//���� ����� ����� �� ���������� ������
		if (input.peek() == EOF)break;
		//if (!input.eof())throw exceptions("Expected operand ';' at the end of expression");
		input >> line;
		try
		{
			tk = pair<string, bool>(line, prev_is_number);
			prev_is_number = true;
		}
		catch (const std::exception&)
		{
			if (map_of_variables == nullptr || !map_of_variables->find(line)->second) {
				throw exception("Unexpected element at expression::transmute", input.tellg());
			}
			else {
				tk = pair<string, bool>(to_string(map_of_variables->find(line)->second), prev_is_number);
				prev_is_number = true;
			}
		}
		//����� ������������ � �����
		if (tk.is_number()) {
			result_queue.push(tk);
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
			//���� �������� �������� �� ����������� ��� ���� ��� ������� ��� ������������ ��� ���������������� � ���������� �����������
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
	//���� �������� �������� ���������� �� � �����
	while (!tokens.empty()) {
		result_queue.push(tokens.top());
		tokens.pop();
	}
	//���������
	delete _result_queue;
	_result_queue = new queue<token>(result_queue);
	while (!result_queue.empty()) {
		tk = result_queue.front();
		cout << tk.get_operand() << " ";
		result_queue.pop();
	}
	
	return _result_queue; // ����� ���������� �����
}

//����� ������
//�.� ����� ���� throw exceptions(string,position) ��� ������� ����������
//P.P.S ����� ��� �� ������ ��� � ������ ���� ��� variable (����������) � ����� ����� ������ � ������ ��
//�������� ���� ���� 2 a + �� ����� ����� a + 2
//������ ������� ��������� ������ � �� �����
//���� ���������� ��� �� ������� ������ ���� �����
//�� ���� ����� ����� �������� �� �������� ������� �����,�� ������ ����� �� ������� � ��������������� �������� ������ �����
//P.P.P.S � ��� ������� � ��� ����� ����� � ������������� ������� ������ �������� ���� ��������� � ������� �� ������ ���� �� ��� �� ������� �������
//���� ����� ������� ������ �������� ���������� ����� transmute
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

