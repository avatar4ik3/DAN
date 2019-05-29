#include "pch.h"
#include "token.h"

token & token::token::operator = (const token & rh) {
	//if (*this == rh)throw exception("attempt to selfcopy");
	symbol = rh.symbol;
	type = rh.type;
	associativity = rh.associativity;
	priority = rh.priority;
	return *this;
}
token & token::operator = (const string & rh) {
	if (this->symbol == rh)throw exception("attempt to selfcopy");
	*this = *(new token(rh, 0));
	return *this;
}
token & token::operator = (const pair<string, bool> rh) {
	*this = *(new token(rh.first, rh.second));
	return *this;
}
void token::set(const string &operand, const bool & prev_is_number) {
	*this = token(operand, prev_is_number);
}

const bool token:: token::operator ==(const string & rh) {
	return symbol == rh;
}
const bool token:: token::operator ==(const token & rh) {
	return  priority == rh.priority;
}
const bool token:: token::operator ==(const int & rh) {
	return priority == rh;
}
const bool token:: token::operator !=(const string & rh) {
	return symbol != rh;
}
const bool token:: token::operator !=(const token & rh) {
	return  priority != rh.priority;
}
const bool token:: token::operator !=(const int & rh) {
	return priority != rh;
}
const bool token:: token::operator >(const token & rh) {
	return priority > rh.priority;
}
const bool token:: token::operator <(const token & rh) {
	return priority < rh.priority;
}
const bool token:: token::operator >=(const token & rh) {
	return priority >= rh.priority;
}
const bool token:: token::operator <=(const token & rh) {
	return priority <= rh.priority;
}
const bool token:: token::operator >(const int & rh) {
	return priority > rh;
}
const bool token:: token::operator <(const int & rh) {
	return priority < rh;
}
const bool token:: token::operator >=(const int & rh) {
	return priority >= rh;
}
const bool token:: token::operator <=(const int & rh) {
	return priority <= rh;
}
//проверка на тип операнда
const bool token:: is_binary() {
	return type == binary_operand;
}
const bool token:: is_unary() {
	return type == unary_operand;
}
//проверка на ассоциативность
const bool token:: is_right_associative() {
	return associativity == right_associative;
}
const bool token:: is_left_associative() {
	return associativity == left_associative;
}
//проверка на тип пунктуации
const bool token:: is_punctuation_mark() {
	return type == punctuation_mark;
}
const bool token:: is_opened_bracket() {
	return symbol == "(";
}
const bool token:: is_closed_bracket() {
	return symbol == ")";
}
const bool token:: is_end_operand() {
	return symbol == ";";
}
//проверка на число
const bool token:: is_number() {
	return type == number;
}


const string token::get_operand() {
	return symbol;
}
const int token::get_priority() {
	return priority;
}


const int token::set_priority(const string & operand, const bool &prev_is_number) {
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
const string token::set_type(const string & operand, const bool &prev_is_number) {
	if (prefix.find(operand) == prefix.end()) {
		if (priority0.find(operand) != priority0.end()) {
			if (prev_is_number) {
				return binary_operand;
			}
			return unary_operand;
		}
		else if (priority7.find(operand) != priority7.end()) {
			return punctuation_mark;
		}
		if (priority1.find(operand) == priority1.end() ||
			priority2.find(operand) == priority2.end() ||
			priority3.find(operand) == priority3.end() ||
			priority4.find(operand) == priority4.end() ||
			priority5.find(operand) == priority5.end() ||
			priority6.find(operand) == priority6.end()) {
			return number;
		}
	}
	return binary_operand;
}
const string token::set_associativity(const string & operand, const bool &prev_is_number) {
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


