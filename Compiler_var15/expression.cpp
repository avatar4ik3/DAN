#include "pch.h"
#include<algorithm>
#include <iostream>
#include "expression.h"
using namespace std;




void expression::set_prior(string line, int &prior, int &assoc,bool prev_is_number)
{
	if (priority0.find(line) != priority0.end() && !prev_is_number) {
		prior = 6; assoc = 2;
	}
	if (priority1.find(line) != priority1.end()) {
		prior = 6; assoc = 2;
	}
	if (priority2.find(line) != priority2.end()) {
		prior = 5; assoc = 1;
	}
	if (priority3.find(line) != priority3.end()) {
		prior = 4; assoc = 2;
	}
	if (priority4.find(line) != priority4.end() && prev_is_number) {
		prior = 3; assoc = 2;
	}
	if (priority5.find(line) != priority5.end()) {
		prior = 2; assoc = 2;
	}
	if (priority6.find(line) != priority6.end()) {
		prior = 1; assoc = 1;
	}
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
		if (input.eof() && line != ";")throw exceptions("Token ';' was expecting at the end of the expressions an expression::calculate");
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
			if (line == "entier" || line == "frac") {
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
			//���� �������� ��������
			if (prior) { 
				//TODO ��������� ������ /*tokens.top() == "entier" || tokens.top() == "frac" ||*/ � ���������� ���
				while (/*tokens.top() == "entier" || tokens.top() == "frac" ||*/!tokens.empty() &&( top_prior > prior || (top_prior == prior && top_assoc == 2))) {
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

	return "1,2,3";
}

