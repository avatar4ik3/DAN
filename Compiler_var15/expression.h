#pragma once
#define Expression_h
#include <string>
#include <fstream>
#include <sstream>
#include <stack>
#include <set>
using namespace std;

const set<string> operators({ "^","~:","+","-","*","/","&","<",">","=","#","!",";","(",")" });

class expression
{
private:
	string _expression_line;
public:

	class exceptions:public exception {
	private:
		const size_t _position;
	public:
		exceptions(const char *msg,const size_t &position = 0) :exception(msg),_position(position) {};
		size_t getPosition() { return this->_position; }
	};

	expression();
	expression(fstream);
	expression(string &);
	expression(const char*);
	expression(const expression&);


	~expression() {};
	const string transmute();
	const string calculate();
};

