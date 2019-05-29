#pragma once
#define TOKEN_H
#include <string>
#include <set>
using namespace std;

//��������� ����������� 
const int right_associative(0);
const int left_associative(1);
const int unary_operand(0);
const int binary_operand(1);
const int punctuation_mark(-1);
const int number(-2);


//���������� ���������� ...
const set<string> prefix({ "frac","entier" });//���������� �������, � �.�. ������� + � - ( ���������� � priority 0)
const set<string> priority0({ "+","-" });// ��������� ����� ����������
const set<string> priority1({ "^","~:" }); //���������� �  �������,���������, ffjfj
const set<string> priority2({ "*" }); //���������
const set<string> priority3({ "/","&" }); //������� � ���������� 
const set<string> priority4({ "+","-" }); // �������� �������� � ���������
const set<string> priority5({ "<",">","=","#" }); // ��������� � "�� �����"(#)
const set<string> priority6({ "!" }); //����������
const set<string> priority7({ ";","(",")" });// ����������

class token
{
public:
	
	/*����������� ��������� ����� � "������ �������"*/
	token() :symbol("empty token"), priority(0), type(-1), associativity(-1) {};
	~token() = default;
	/*����������� ������ ����� ������� � "���������� ������� - �����?"*/
	explicit token(const string & operand,const bool & prev_is_number):symbol(operand),priority(set_priority(operand,prev_is_number)),type(set_type(operand,prev_is_number)),associativity(set_associativity(operand,prev_is_number)) {};
	/*��������� ������������ ����� �����, ������ � ���� �� ������ � ��������� ���������� "���������� ������� - �����?" */
	/*��� ��� ��������� ������������ ��������� �� ������ �������� ������������ � �������,
	������� � �� ��� �������� �� ���������������
	��� ���������� ������ ���� ������ ������ ��� ���� token a;token b ;a = b;
	���� ����� ��� ��������� ����� �� ��� �� ��� ����� �������� � ������������ ������� � �� ����

	*/
	token & operator = (const token &);
	token & operator = (const string &);
	token & operator = (const pair<string, bool> rh);
	/*������� ������������ ���������� ������������*/
	void set(const string &operand, const bool & prev_is_number);

	//��������� ��������� �����������
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
	//�������� �� ��� ��������
	const bool is_binary();
	const bool is_unary();
	//�������� �� ���������������
	const bool is_right_associative();
	const bool is_left_associative();
	//�������� �� ��� ����������
	const bool is_punctuation_mark();
	const bool is_opened_bracket();
	const bool is_closed_bracket();
	const bool is_end_operand();
	//�������� �� �����
	const bool is_number();
	
	//������� �������� ���� �������
	const string get_operand();
	//������� �������� ����������
	const int get_priority();
	/* ������ ��� ������� ������ ���������� ����� �������� �� ��������� ������������ ���� �������� 
	����-���� �� � ���� � ���� �������������� � ��� �� �� ������ �������� �����
	*/

private:
	string
		symbol; // ��� ������
	int
		priority, //��� ���������
		type,// ��� ��� , 1 - ��������, 0 - �������,-1 ���� ����������
		associativity;//��������������� 1 - �����,0 - ������,-1 ���� ����������

	//������� ����������� ���������� ���� � �������������� ������� ����������� ��� ������������
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