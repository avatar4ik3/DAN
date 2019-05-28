#pragma once
#include <string>
#include <set>
using namespace std;

const int right_associative(0);
const int left_associative(1);
const int unary_operand(0);
const int binary_operand(1);
const int punctuation_mark(-1);


//приоритеты операторов ...
const set<string> prefix({ "frac","entier" });//префиксные функции, в т.ч. унарный + и - ( содержатс€ в priority 0)
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
	
	token() = default;
	~token() = default;

	explicit token(const string & operand,const bool & prev_is_number):symbol(operand),priority(set_priority(operand,prev_is_number)),type(set_type(operand,prev_is_number)),associativity(set_associativity(operand,prev_is_number)) {};

	token & operator =(const token & rh) {
		if (*this == rh)throw exception("attempt to selfcopy");
		*this = token(rh);
	}
	token & operator = (const string & rh) {
		if (this->symbol == rh)throw exception("attempt to selfcopy");
		*this = token(rh, 1);
	}
	void set(const string &operand, const bool & prev_is_number) {
		*this = token(operand, prev_is_number);
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

	const bool is_binary() {
		return type == binary_operand;
	}
	const bool is_unary() {
		return type == unary_operand;
	}
	const bool is_right_associative() {
		return associativity == right_associative;
	}
	const bool if_left_associative() {
		return associativity == left_associative;
	}
	const bool is_punctuation_mark() {
		return type == punctuation_mark;
	}

private:

	const int
		type,// его тип , 1 - бинарна€, 0 - унарна€,-1 знак препинани€
		associativity;//ассоциативность 1 - лева€,0 - права€,-1 знак препинани€



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
		throw exception("Unknown operand at token::set_priority");
	}
	int set_type(const string & operand,const bool &prev_is_number) {
		if (prefix.find(operand) == prefix.end()) {
			if (priority0.find(operand) != priority0.end()) {
				return prev_is_number == binary_operand;
			}
			else return punctuation_mark;
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
		throw exception("Unknown operand at token::set_associativity");
	}
};

const bool operand_is_defined(const string & operand) {
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
