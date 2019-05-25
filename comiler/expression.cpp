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

const string expression::calculate() {

	string 
		postfix_notation,
		tmp_line1,
		tmp_line2;
	stack<string>
		operands,
		postfix;
	stack<double>
		values;
	istringstream 
		infix_notation(_expression_line);
	double
		tmp_val1(NAN);
	size_t counter(0);
	while (tmp_line2 != ";") {
		infix_notation >> tmp_line1;
		counter++;
		if (tmp_line1.length() >= 1 && operators.find(tmp_line1) == operators.end()) {
			try
			{
				tmp_val1 = stod(tmp_line1.c_str());
				postfix.push(tmp_line1);
			}
			catch (const std::exception&)
			{
				throw exceptions("Unexpected token at expression::calculate", counter);
			}

		}
		else {
			if (tmp_line2.empty())operands.push(tmp_line1);
			if (tmp_line2 == ";") {//конец выражения
				if (tmp_line1 == ";") break;//выражение получено
				if (tmp_line1 == "+")operands.push(tmp_line1);
				if (tmp_line1 == "-")operands.push(tmp_line1);
				if (tmp_line1 == "*")operands.push(tmp_line1);
				if (tmp_line1 == "/")operands.push(tmp_line1);
				if (tmp_line1 == "(")operands.push(tmp_line1);
				if (tmp_line1 == ")") {
					throw exceptions("Unbalanced expression", counter);
				}
			}
			if (tmp_line2 == "+") {//prev +
				if (tmp_line1 == ";") {
					postfix.push(operands.top());
					operands.pop();
				}
				if (tmp_line1 == "+") {
					postfix.push(operands.top());
					operands.pop();
				}
				if (tmp_line1 == "-") {
					postfix.push(operands.top());
					operands.pop();
				}
				if (tmp_line1 == "*")operands.push(tmp_line1);
				if (tmp_line1 == "/")operands.push(tmp_line1);
				if (tmp_line1 == "(")operands.push(tmp_line1);
				if (tmp_line1 == ")") {
					postfix.push(operands.top());
					operands.pop();
				}
			}
			if (tmp_line2 == "-") {//prev -
				if (tmp_line1 == ";") {
					postfix.push(operands.top());
					operands.pop();
				}
				if (tmp_line1 == "+") {
					postfix.push(operands.top());
					operands.pop();
				}
				if (tmp_line1 == "-") {
					postfix.push(operands.top());
					operands.pop();
				}
				if (tmp_line1 == "*")operands.push(tmp_line1);
				if (tmp_line1 == "/")operands.push(tmp_line1);
				if (tmp_line1 == "(")operands.push(tmp_line1);
				if (tmp_line1 == ")") {
					postfix.push(operands.top());
					operands.pop();
				}
			}
			if (tmp_line2 == "*") {//prev *
				if (tmp_line1 == ";") {
					postfix.push(operands.top());
					operands.pop();
				}
				if (tmp_line1 == "+") {
					postfix.push(operands.top());
					operands.pop();
				}
				if (tmp_line1 == "-") {
					postfix.push(operands.top());
					operands.pop();
				}
				if (tmp_line1 == "*") {
					postfix.push(operands.top());
					operands.pop();
				}
				if (tmp_line1 == "/") {
					postfix.push(operands.top());
					operands.pop();
				}
				if (tmp_line1 == "(")operands.push(tmp_line1);
				if (tmp_line1 == ")") {
					postfix.push(operands.top());
					operands.pop();
				}
			}
			if (tmp_line2 == "/") {//prev /
				if (tmp_line1 == ";") {
					postfix.push(operands.top());
					operands.pop();
				}
				if (tmp_line1 == "+") {
					postfix.push(operands.top());
					operands.pop();
				}
				if (tmp_line1 == "-") {
					postfix.push(operands.top());
					operands.pop();
				}
				if (tmp_line1 == "*") {
					postfix.push(operands.top());
					operands.pop();
				}
				if (tmp_line1 == "/") {
					postfix.push(operands.top());
					operands.pop();
				}
				if (tmp_line1 == "(")operands.push(tmp_line1);
				if (tmp_line1 == ")") {
					postfix.push(operands.top());
					operands.pop();
				}
			}
			if (tmp_line2 == "(") {//prev (
				if (tmp_line1 == ";") {
					throw exceptions("Unbalanced expression", counter);
				}
				if (tmp_line1 == "+") {
					operands.push(tmp_line1);
				}
				if (tmp_line1 == "-") {
					operands.push(tmp_line1);
				}
				if (tmp_line1 == "*") {
					operands.push(tmp_line1);
				}
				if (tmp_line1 == "/") {
					operands.push(tmp_line1);
				}
				if (tmp_line1 == "(") { 
					operands.push(tmp_line1); 
				}
				if (tmp_line1 == ")") {
					operands.pop();
				}
			}
			tmp_line2 = tmp_line1;
		}
		
	}

	//test
	while (!postfix.empty()) {
		cout << postfix.top() << " ";
		postfix.pop();
	}
	return "123";
}

