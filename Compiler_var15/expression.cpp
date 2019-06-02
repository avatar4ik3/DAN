#include "pch.h"
#include<algorithm>
#include <iostream>
#include "expression.h"
using namespace std;




void expression::set_prior(string line, int &prior, int &assoc,bool prev_is_number)
{
	
}

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

const queue<string> expression::transmute()
{
	stack<string> tokens; // ��� ���� ���������
	queue<string> result; // ��� ��������� � ���� ������� ����� ���� ������� �������� 
	string line; // ������ � ������� ������
	int prior(0), top_prior(0);//���������� �����
	int assoc(0), top_assoc(0);//�������������� �������� 1 = ������ 2 = �����
	bool prev_is_number(false),top_prev_is_number(false); 
	istringstream input(_expression_line);//����� �� ������ ��� �������� ������

	while (line != ";") {
		input >> line;
		if (input.eof() && line != ";")throw exceptions("Token ';' was expected at the end of the expressions at expression::calculate");
		if (line.length() >= 1 && operators.find(line) == operators.end()) {
			try
			{
				stod(line.c_str());
				result.push(line);
				prev_is_number = true;
			}
			catch (const std::exception&)
			{
				throw exceptions("Unexpected token at expression::calculate",input.tellg());
			}
		}
		else {
			//��������� ���������� ������ ��������
			prior = 0;
			assoc = 0;
			this->set_prior(line, prior, assoc, prev_is_number);

			//��� ��������
			//���� ������ - ����� ������ � ���� ( ��� �� ��� �����)
			//���� ������ - ����������� ������� �� ������ � ����(��� ��� ����������� �������)
			//���� ������ - -���������� ������� �� ������ � ����
			if (line == "entier" || line == "frac" || (prior == 6 && !prev_is_number)) {
				tokens.push(line);
			}
			//���� ������ - ����������� ������ �� ������ � ����
			if (line == "(")
			{
				tokens.push(line);
			}
			//���� ������ - ����������� ������ �� ����������� ������ �� ����� ���� �� ������ �� ����������� ������
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
			//���� �������� ��������...
			if (prior) { 
				//TODO ��������� ������ /*tokens.top() == "entier" || tokens.top() == "frac" ||*/ � ���������� ���
				while (!tokens.empty() && (tokens.top() == "entier" || tokens.top() == "frac" || (top_prior == 6 && !top_prev_is_number) || top_prior > prior || (top_prior == prior && top_assoc == 2))) {
					result.push(tokens.top());
					tokens.pop();
					//��������� ���������� ������� �����
					if(!tokens.empty())this->set_prior(tokens.top(), top_prior, top_assoc, top_prev_is_number);
					
				}
				tokens.push(line);
			}
			top_prior = prior;
			top_assoc = assoc;
			top_prev_is_number = prev_is_number;
			prev_is_number = false;

		}
	}
	while (!tokens.empty()) {
		result.push(tokens.top());
		tokens.pop();
	}
	//��������
	while (!result.empty()) {
		cout << result.front() << " ";
		result.pop();
	}
	this->_result_queue = result;
	return _result_queue; // ����� ���������� �����
}

//����� ������
//�.� ����� ���� throw exceptions(string,position) ��� ������� ����������
const string expression::calculate() {
	stack<string> Stack;
	if (_result_queue.empty() == false) throw exceptions("_result_queue is empty");
	else {
		while (_result_queue.empty() != false) {
			string buff;
			buff = _result_queue.front();
			if (buff.length() >= 1 && operators.find(buff) == operators.end()) {
				try
				{
					stod(buff.c_str());
					result.push(line);
					prev_is_number = true;
				}
				catch (const std::exception&)
				{
					throw exceptions("Unexpected token at expression::calculate", input.tellg());
				}
			}
		}
	}
}

