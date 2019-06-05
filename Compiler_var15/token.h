#pragma once
#define TOKEN_H
#include <string>
#include <set>
using namespace std;

//константы обозначений 
const string right_associative("right");
const string left_associative("left");
const string unary_operand("unary");
const string binary_operand("binary");
const string punctuation_mark("punctuation");
const string number("number");
const string function("function");


//приоритеты операторов ...
const set<string> prefix({ "frac","entier" });//префиксные функции, в т.ч. унарный + и - ( содержатс€ в priority 0)
const set<string> priority0({ "+","-" });// изменение знака переменной
const set<string> priority1({ "^","~" }); //возведение в  степень,отрицание
const set<string> priority2({ "*" }); //умножение
const set<string> priority3({ "/","&" }); //деление и конъюнкиц€ 
const set<string> priority4({ "+","-" }); // бинарные сложение и вычитание
const set<string> priority5({ "<",">","=","#" }); // сравнени€ и "не равно"(#)
const set<string> priority6({ "!" }); //дизъюнкци€
const set<string> priority7({ ";","(",")" });// пунктуаци€

class token
{
public:
	
	/*поумолчанию создаетс€ токен с "пустым токеном"*/
	token() :symbol("empty token"), priority(0), type(unary_operand), associativity(left_associative) {};
	~token() = default;
	/*конструктор вызова через операнд и "предыдущий аперанд - число?"*/
	explicit token(const string & operand,const bool & prev_is_number):symbol(operand),priority(set_priority(operand,prev_is_number)),type(set_type(operand,prev_is_number)),associativity(set_associativity(operand,prev_is_number)) {};
	/*операторы присваивани€ черен токен, строку и пару из строки и булевской переменной "предыдущий аперанд - число?" */
	/* все операторы присваивани€ ссылаютс€ на первый оператор присваивани€ с токеном,
	поэтому в нЄм нет проверки на самокопирование
	это необходимо учесть если будете писать код типа token a;token b ;a = b;
	хоть токен это статичный класс но все же это может привести к последстви€м которых € не знаю

	*/
	token & operator = (const token &);
	token & operator = (const string &);
	token & operator = (const pair<string, bool> rh);
	/*функци€ присваивани€ аналогично конструктору*/
	void set(const string &operand, const bool & prev_is_number);

	//операторы сравнени€ приоритетов
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
	//проверка на функцию
	const bool is_function();
	//функци€ возврата нашеЄ лексемы
	const string get_operand();
	//функци€ возврата приоритета
	const int get_priority();

private:
	string
		symbol; // сам символ
	int
		priority; //его приоритет
	string
		type,// его тип 
		associativity;//ассоциативность 

	//функции определени€ приоритета типа и асоциативности лексемы необходимые дл€ конструктора
	const int set_priority(const string &, const bool &);
	const string set_type(const string & , const bool &);
	const string set_associativity(const string & , const bool &);

};

