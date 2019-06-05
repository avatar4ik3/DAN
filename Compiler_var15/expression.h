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
#include "token.h"
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
	//словарь для возможности работы с переменными
	map<string, double> *map_of_variables;

	/*
		функция перевода в ОПЗ
	*/
	const queue<token>  transmute();
public:
	//класс ислючений с возможностью указать позицию исключения
	class exceptions:public exception {
	private:
		const size_t _position;
	public:
		exceptions(const char *msg,const size_t &position = 0) :exception(msg),_position(position) {};
		const size_t getPosition() { return size_t(_position); }
	};
	//конструкторы
	expression();
	expression(fstream);
	expression(string &);
	expression(const char*);
	expression(const expression&);
	//деструктор ( •_•)>⌐■-■ (￣y▽,￣)╭ 
	~expression() = default;

	//разыменователь для доступа к строке
	const string operator*();
	//оператор присваивания для строки
	expression & operator=(const string & line);
	//оператор присваивание для файлов
	expression & operator=(fstream);
	/*
		функция запоминания переменных с помощью словаря
	*/
	void set_variables(const map<string,double> &);
	/*
		функция запоминания переменых с помощью пары
	*/
	void add_variable(const pair<string, double>&);
	/*
	функция возвращающая копию на словарь переменных
	*/
	const map<string, double>  get_map_of_variables();
	/*
		функция удаляющая элемент словаря
	*/
	void erase(const string & name);
	/*
		САЛУ
	*/
	const string calculate();

	
};

