#pragma once
#include <string>
#include <set>
using namespace std;


//приоритеты операторов ...
const set<string> prefix({ "frac","entier","+","-" });//префиксные функции, в т.ч. унарный + и - ( содержатс€ в priority 0)
const set<string> priority0({ "+","-" });// изменение знака переменной
const set<string> priority1({ "^","~:" }); //возведение в  степень,отрицание, ffjfj
const set<string> priority2({ "*" }); //умножение
const set<string> priority3({ "/","&" }); //деление и конъюнкиц€ 
const set<string> priority4({ "+","-" }); // бинарные сложение и вычитание
const set<string> priority5({ "<",">","=","#" }); // сравнени€ и "не равно"(#)
const set<string> priority6({ "!" }); //дизъюнкци€
const set<string> priority7({ ";","(",")" });//

class token
{
public:
	const string 
		symbol; // сам символ
	const int
		priority; //его приоритет
	const bool 
		type,// его тип , 1 - бинарна€, 0 - унарна€
		associativity;//ассоциативность 1 - лева€,0 - права€

	token() = default;
	explicit token(const string & operand,const bool & prev_is_number):symbol(operand),priority(set_priority(operand,prev_is_number)),type(set_type(operand,prev_is_number)),associativity(set_associativity(operand,prev_is_number)) {};

	token & operator =(const token & rh) {
		if (*this == rh)throw exception("attempt to selfcopy");

	}

	const bool operator ==(const string & rh) {
		return symbol == rh;
	}
	const bool operator ==(const token & rh) {
		return symbol == rh.symbol && priority == rh.priority && type == rh.type && associativity == rh.associativity;
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
	~token() = default;
private:
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
		if()
		return 0;
	}
	bool set_type(const string & operand,const bool &prev_is_number) {
		if(prefix.find(operand)== prefix.end())return prev_is_number;
		return 0;
	}
	bool set_associativity(const string & operand, const bool &prev_is_number) {
		if (priority0.find(operand) != priority0.end() && !prev_is_number) {
			return 1;
		}
		if (priority1.find(operand) != priority1.end()) {
			return 1;
		}
		if (priority2.find(operand) != priority2.end()) {
			return 2;
		}
		if (priority3.find(operand) != priority3.end()) {
			return 1;
		}
		if (priority4.find(operand) != priority4.end() && prev_is_number) {
			return 1;
		}
		if (priority5.find(operand) != priority5.end()) {
			return 1;
		}
		if (priority6.find(operand) != priority6.end()) {
			return 0;
		}
		return 0;
	}
};

