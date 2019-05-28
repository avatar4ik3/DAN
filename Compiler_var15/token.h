#pragma once
#define TOKEN_H
#include <string>
#include <set>
using namespace std;

//константы обозначений 
const int right_associative(0);
const int left_associative(1);
const int unary_operand(0);
const int binary_operand(1);
const int punctuation_mark(-1);
const int number(-2);


//приоритеты операторов ...
const set<string> prefix({ "frac","entier" });//префиксные функции, в т.ч. унарный + и - ( содержатся в priority 0)
const set<string> priority0({ "+","-" });// изменение знака переменной
const set<string> priority1({ "^","~:" }); //возведение в  степень,отрицание, ffjfj
const set<string> priority2({ "*" }); //умножение
const set<string> priority3({ "/","&" }); //деление и конъюнкиця 
const set<string> priority4({ "+","-" }); // бинарные сложение и вычитание
const set<string> priority5({ "<",">","=","#" }); // сравнения и "не равно"(#)
const set<string> priority6({ "!" }); //дизъюнкция
const set<string> priority7({ ";","(",")" });// пунктуация

class token
{
public:
	
	
	token() :symbol("empty token"), priority(0), type(-1), associativity(-1) {};
	~token() = default;

	explicit token(const string & operand,const bool & prev_is_number):symbol(operand),priority(set_priority(operand,prev_is_number)),type(set_type(operand,prev_is_number)),associativity(set_associativity(operand,prev_is_number)) {};

	token & operator = (const token &);
	token & operator = (const string &);
	token & operator = (const pair<string, bool> rh);
	void set(const string &operand, const bool & prev_is_number);
	const bool operator ==(const string &);
	const bool operator ==(const token &);
	const bool operator ==(const int &);
	const bool operator !=(const string &);
	const bool operator !=(const token &);
	const bool operator !=(const int &);
	const bool operator >(const token &);
	const bool operator <(const token &);
	const bool operator >=(const token &);
	const bool operator <=(const token &);
	const bool operator >(const int &);
	const bool operator <(const int &);
	const bool operator >=(const int &);
	const bool operator <=(const int &);
	//проверка на тип операнда
	const bool is_binary();
	const bool is_unary();
	//проверка на ассоциативность
	const bool is_right_associative();
	const bool is_left_associative();
	//проверка на тип пунктуации
	const bool is_punctuation_mark();
	const bool is_opened_bracket();
	const bool is_closed_bracket();
	const bool is_end_operand();
	//проверка на число
	const bool is_number();
	

	const string get_operand();
	const int get_priority();

private:
	string
		symbol; // сам символ
	int
		priority, //его приоритет
		type,// его тип , 1 - бинарная, 0 - унарная,-1 знак препинания
		associativity;//ассоциативность 1 - левая,0 - правая,-1 знак препинания


	int set_priority(const string &, const bool &);
	int set_type(const string & , const bool &);
	int set_associativity(const string & , const bool &);

};

/*const bool operand_is_defined(const string & operand) {
	return 
		prefix.find(operand) != prefix.end() ||
		priority0.find(operand) != priority0.end() ||
		priority1.find(operand) != priority1.end() ||
		priority2.find(operand) != priority2.end() ||
		priority3.find(operand) != priority3.end() ||
		priority4.find(operand) != priority4.end() ||
		priority5.find(operand) != priority5.end() ||
		priority6.find(operand) != priority6.end() ||
		priority7.find(operand) != priority7.end();
}
*/