#pragma once
#define Expression_h
#include <string>
#include <fstream>
#include <sstream>
#include <stack>
#include <queue>
#include <set>
//#include "token.h"
using namespace std;
//возможные операторы
const set<string> operators({ "^","~:","+","-","*","/","&","<",">","=","#","!",";","(",")","entier","frac" });

//Денис: я хз почему конъюнкия(&) и дизъюнкция(!) отдельно от всех и настолько далеко и почему отрицание это тильта(и тильт тоже) (╯°□°）╯︵ ┻━┻ 
class expression
{
private:
	string _expression_line;
	queue<string> _result_queue;
	void set_prior(string,int&, int&,bool);
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
	const queue<string> transmute();
	const string calculate();
};

