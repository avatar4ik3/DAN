#pragma once
#define Expression_h
#include <string>
#include <fstream>
#include <sstream>
#include <stack>
#include <queue>
#include <set>
#include <list>
#include <map>
#include "token_queue.h"
//#include "token.h"
using namespace std;
//возможные операторы
const set<string> operators({ "^","~:","+","-","*","/","&","<",">","=","#","!",";","(",")","entier","frac" });

//Денис: я хз почему конъюнкия(&) и дизъюнкция(!) отдельно от всех и настолько далеко и почему отрицание это тильта(и тильт тоже) (╯°□°）╯︵ ┻━┻ 
class expression
{
private:
	/*переменные необходимые для работы компилятора*/
	//само выражение
	string _expression_line;
	//выражение в опз
	queue<token> *_result_queue;
	//словарь для возможности работы с переменными
	map<string, double> *map_of_variables;
public:
	//класс ислючений с возможностью указать позицию исключения
	class exceptions:public exception {
	private:
		const size_t _position;
	public:
		exceptions(const char *msg,const size_t &position = 0) :exception(msg),_position(position) {};
		size_t getPosition() { return this->_position; }
	};
	//конструкторы
	expression();
	expression(fstream);
	expression(string &);
	expression(const char*);
	expression(const expression&);
	//деструктор ( •_•)>⌐■-■
	~expression() = default;
	/*
		функция запоминания переменных с помощью словаря
	*/
	void set_variables(const map<string,double> &);
	/*
		функция запоминания переменых с помозью пары
	*/
	void add_variable(const pair<string, double>&);

	/*
		функция перевода в ОПЗ
	*/
	const queue<token>*  transmute();
	/*
		САЛУ
	*/
	const string calculate();
};

