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

	token & operator = (const token & rh) {
		//if (*this == rh)throw exception("attempt to selfcopy");
		symbol = rh.symbol;
		type = rh.type;
		associativity = rh.associativity;
		priority = rh.priority;
		return *this;
	}
	token & operator = (const string & rh) {
		if (this->symbol == rh)throw exception("attempt to selfcopy");
		*this = *(new token(rh, 0));
		return *this;
	}
	token & operator = (const pair<string, bool> rh) {
		*this = *(new token(rh.first, rh.second));
		return *this;
	}
	void set(const string &operand, const bool & prev_is_number) {
		*this = token(operand, prev_is_number);
	}
	
	const bool operator ==(const string & rh) {
		return symbol == rh;
	}
	const bool operator ==(const token & rh) {
		return  priority == rh.priority;
	}
	const bool operator ==(const int & rh) {
		return priority == rh;
	}
	const bool operator !=(const string & rh) {
		return symbol != rh;
	}
	const bool operator !=(const token & rh) {
		return symbol != rh.symbol && priority != rh.priority && type != rh.type && associativity != rh.associativity;
	}
	const bool operator !=(const int & rh) {
		return priority != rh;
	}
	const bool operator >(const token & rh) {
		return priority > rh.priority;
	}
	const bool operator <(const token & rh) {
		return priority < rh.priority;
	}
	const bool operator >=(const token & rh) {
		return priority >= rh.priority;
	}
	const bool operator <=(const token & rh) {
		return priority <= rh.priority;
	}
	const bool operator >(const int & rh) {
		return priority > rh;
	}
	const bool operator <(const int & rh) {
		return priority < rh;
	}
	const bool operator >=(const int & rh) {
		return priority >= rh;
	}
	const bool operator <=(const int & rh) {
		return priority <= rh;
	}
	//проверка на тип операнда
	const bool is_binary() {
		return type == binary_operand;
	}
	const bool is_unary() {
		return type == unary_operand;
	}
	//проверка на ассоциативность
	const bool is_right_associative() {
		return associativity == right_associative;
	}
	const bool is_left_associative() {
		return associativity == left_associative;
	}
	//проверка на тип пунктуации
	const bool is_punctuation_mark() {
		return type == punctuation_mark;
	}
	const bool is_opened_bracket() {
		return symbol == "(";
	}
	const bool is_closed_bracket() {
		return symbol == ")";
	}
	const bool is_end_operand() {
		return symbol == ";";
	}
	//проверка на число
	const bool is_number() {
		return type == number;
	}
	

	const string get_operand() {
		return symbol;
	}
	const int get_priority() {
		return priority;
	}

private:
	string
		symbol; // сам символ
	int
		priority, //его приоритет
		type,// его тип , 1 - бинарная, 0 - унарная,-1 знак препинания
		associativity;//ассоциативность 1 - левая,0 - правая,-1 знак препинания


	int set_priority(const string & operand,const bool &prev_is_number) {
		if (priority0.find(operand) != priority0.end() && !prev_is_number) {
			return 6;
		}
		if (priority1.find(operand) != priority1.end()) {
			return 6;
		}
		if (priority2.find(operand) != priority2.end()) {
			return 5;
		}
		if (priority3.find(operand) != priority3.end()) {
			return 4;
		}
		if (priority4.find(operand) != priority4.end() && prev_is_number) {
			return 3;
		}
		if (priority5.find(operand) != priority5.end()) {
			return 2;
		}
		if (priority6.find(operand) != priority6.end()) {
			return 1;
		}
		if (priority7.find(operand) != priority7.end()) {
			return 0;
		}
		try
		{
			stod(operand.c_str());
			return 0;
		}
		catch (const std::exception&)
		{
			throw exception("Unknown operand at token::set_priority");
		}
		
	}
	int set_type(const string & operand,const bool &prev_is_number) {
		if (prefix.find(operand) == prefix.end()) {
			if (priority0.find(operand) != priority0.end()) {
				return prev_is_number == binary_operand;
			}
			else if (priority7.find(operand) != priority7.end()) {
				return punctuation_mark;
			}
			if (priority1.find(operand) == priority1.end() ||
				priority2.find(operand) == priority2.end() ||
				priority3.find(operand) == priority3.end() ||
				priority4.find(operand) == priority4.end() ||
				priority5.find(operand) == priority5.end() ||
				priority6.find(operand) == priority6.end())	{
				return number;
			}
		}
		return binary_operand;
	}
	int set_associativity(const string & operand, const bool &prev_is_number) {
		if (priority0.find(operand) != priority0.end() && !prev_is_number) {
			return left_associative;
		}
		if (priority1.find(operand) != priority1.end()) {
			return left_associative;
		}
		if (priority2.find(operand) != priority2.end()) {
			return right_associative;
		}
		if (priority3.find(operand) != priority3.end()) {
			return left_associative;
		}
		if (priority4.find(operand) != priority4.end() && prev_is_number) {
			return left_associative;
		}
		if (priority5.find(operand) != priority5.end()) {
			return left_associative;
		}
		if (priority6.find(operand) != priority6.end()) {
			return right_associative;
		}
		if (priority7.find(operand) != priority7.end()) {
			return punctuation_mark;
		}
		try
		{
			stod(operand.c_str());
			return number;
		}
		catch (const std::exception&)
		{
			throw exception("Unknown operand at token::set_associativity");
		}
		
	}

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