#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 100     //���� ��� ������ ���� �迭�� ũ�⸦ 100����
typedef double Element;     //���� ����� �ڷ��� ����

Element data[MAX_SIZE];     //���� ���� ����� �迭
int top;     //���� ������ top

void error(char str[])     //���� ��Ȳ ó���� ���� �Լ�. �޽��� ��� �� ���α׷� ����
{
	printf("%s \n", str);
	exit(1);
}

void init_stack() { top = -1; }     //���� �ֿ� ����
int is_empty() { return top == -1; }
int is_full() { return top == MAX_SIZE - 1; }
int size() { return top + 1; }

void push(Element e)
{
	if (is_full())
		error("���� ��ȭ ����");
	data[++top] = e;
}
Element pop()
{
	if (is_empty())
		error("���� ���� ����[pop]");
	return data[top--];
}
Element peek()
{
	if (is_empty())
		error("���� ���� ����[peek]");
	return data[top];
}

int check_matching(char expr[])     //��ȣ �˻�
{
	int i = 0, prev;
	char ch;

	init_stack();
	while (expr[i] != '\0') {
		ch = expr[i++];
		if (ch == '[' || ch == '(' || ch == '{')
			push(ch);
		else if (ch == ']' || ch == ')' || ch == '}') {
			if (is_empty())
				return 2;	 //���� 2 ����
			
			prev = pop;
			if ((ch == ']' && prev != '[') || (ch == ')' && prev != '(') || (ch == '}' && prev != '{'))
				return 3;	 //���� 3 ����
		}
	}
	if (is_empty() == 0)	return 1;	 //���� 1 ����
	return 0;	 //��ȣ ����
}

int precedence(char op)     //�������� �켱���� ��ȯ(���ڰ� Ŭ���� �켱���� ����)
{
	switch (op) {
	case '[': case ']': return 0;
	case '{': case '}': return 1;
	case '(': case ')': return 2;
	case '+': case '-': return 3;
	case '*': case '/': return 4;
	}
	return -1;
}

char* infix_to_postfix(char expr[], char postexpr[])     //���� ������ ���� ǥ��� ��ȯ
{
	int j = 0;
	char c, top_op;
	int len = strlen(expr);
	int isNum = 0;
	
	init_stack();
	for (int i = 0; i < len; i++) {
		c = expr[i];
		if ((c >= '0' && c <= '9') || (c == '.')) {     //������ ��. �Ҽ����� ���ڷ� �޾Ƶ���
			if (isNum ==0) postexpr[j++] = ' ';     //���� ������ �� �׸� ������
			postexpr[j++] = c;
			isNum = 1;
		}
		else {     //���ڰ� �ƴ� ��
			isNum = 0;
			switch (c) {
			case '+': case '-': case '*': case '/':
				while (!is_empty() && (precedence(c) <= precedence(peek()))) {
					postexpr[j++] = ' ';     //���� ������ �� �׸� ������
					postexpr[j++] = pop();
				}
				push(c);
				break;
			case '(':
				push(c);
				break;
			case ')':
				top_op = pop(); 
				while (top_op != '(') {
					postexpr[j++] = ' ';
					postexpr[j++] = top_op;
					top_op = pop();
				}
				break;
			}
		}
	}
	while (!is_empty()) {    //���ÿ� ����� �����ڵ� ��� 
		postexpr[j++] = ' ';
		postexpr[j++] = pop();
	}
	postexpr[j] = '\0';
	return postexpr;
}

double calc_postfix(char expr[])     //���� ǥ�� ���� ���
{
	char c;
	int i = 0;
	int isNum = 0;
	double val1, val2;
	int j = 0;
	char tmp[MAX_SIZE];     //�Ǽ����� ��� ����

	init_stack();
	while (expr[i] != '\0') {
		c = expr[i++];
		if ((c >= '0' && c <= '9') || (c == '.')) {
			tmp[j++] = c;
			isNum = 1;
		}
		else {
			if (isNum == 1) {
				push(atof(tmp));     //���ڿ��� �Ǽ��� ��ȯ
				isNum = 0;
				memset(tmp, 0, MAX_SIZE);     //���ڿ� ���� �ʱ�ȭ(�� �ϸ� ������ ���� ����)
				j = 0;
			}
			if (c == '+' || c == '-' || c == '*' || c == '/') {
				val2 = pop();
				val1 = pop();
				switch (c) {
				case '+':push(val1 + val2);	break;
				case '-':push(val1 - val2);	break;
				case '*':push(val1 * val2);	break;
				case '/':push(val1 / val2);	break;
				}

			}
		}
	}
	if (isNum) {     //�Ǽ� �ϳ��� �ִ� ������ ��
		return atof(tmp);
	}
	return pop();
}

int main()
{
	char expr[MAX_SIZE];
	char rtnPostexpr[MAX_SIZE];
	
	printf("������ �Է��Ͻÿ� : ");
	//scanf_s("%s", expr, MAX_SIZE); (�� �Լ� ��� ��, ���� ���Ե� ������ ���� ó���� �� ��)
	gets_s(expr, MAX_SIZE);

	infix_to_postfix(expr, rtnPostexpr);
	//printf("postfix: %s\n", rtnPostexpr); (���� ���� Ȯ�ο�)
	printf("= %lf \n", calc_postfix(rtnPostexpr));
}