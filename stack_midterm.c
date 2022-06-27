#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 100     //스택 요소 저장을 위한 배열의 크기를 100으로
typedef double Element;     //스택 요소의 자료형 정의

Element data[MAX_SIZE];     //실제 스택 요소의 배열
int top;     //실제 스택의 top

void error(char str[])     //오류 상황 처리를 위한 함수. 메시지 출력 후 프로그램 종료
{
	printf("%s \n", str);
	exit(1);
}

void init_stack() { top = -1; }     //스택 주요 연산
int is_empty() { return top == -1; }
int is_full() { return top == MAX_SIZE - 1; }
int size() { return top + 1; }

void push(Element e)
{
	if (is_full())
		error("스택 포화 에러");
	data[++top] = e;
}
Element pop()
{
	if (is_empty())
		error("스택 공백 에러[pop]");
	return data[top--];
}
Element peek()
{
	if (is_empty())
		error("스택 공백 에러[peek]");
	return data[top];
}

int check_matching(char expr[])     //괄호 검사
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
				return 2;	 //조건 2 위반
			
			prev = pop;
			if ((ch == ']' && prev != '[') || (ch == ')' && prev != '(') || (ch == '}' && prev != '{'))
				return 3;	 //조건 3 위반
		}
	}
	if (is_empty() == 0)	return 1;	 //조건 1 위반
	return 0;	 //괄호 정상
}

int precedence(char op)     //연산자의 우선순위 반환(숫자가 클수록 우선순위 높음)
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

char* infix_to_postfix(char expr[], char postexpr[])     //중위 수식을 후위 표기로 변환
{
	int j = 0;
	char c, top_op;
	int len = strlen(expr);
	int isNum = 0;
	
	init_stack();
	for (int i = 0; i < len; i++) {
		c = expr[i];
		if ((c >= '0' && c <= '9') || (c == '.')) {     //숫자일 때. 소수점도 문자로 받아들임
			if (isNum ==0) postexpr[j++] = ' ';     //후위 수식의 각 항목 구분자
			postexpr[j++] = c;
			isNum = 1;
		}
		else {     //숫자가 아닐 때
			isNum = 0;
			switch (c) {
			case '+': case '-': case '*': case '/':
				while (!is_empty() && (precedence(c) <= precedence(peek()))) {
					postexpr[j++] = ' ';     //후위 수식의 각 항목 구분자
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
	while (!is_empty()) {    //스택에 저장된 연산자들 출력 
		postexpr[j++] = ' ';
		postexpr[j++] = pop();
	}
	postexpr[j] = '\0';
	return postexpr;
}

double calc_postfix(char expr[])     //후위 표기 수식 계산
{
	char c;
	int i = 0;
	int isNum = 0;
	double val1, val2;
	int j = 0;
	char tmp[MAX_SIZE];     //실수값을 담는 변수

	init_stack();
	while (expr[i] != '\0') {
		c = expr[i++];
		if ((c >= '0' && c <= '9') || (c == '.')) {
			tmp[j++] = c;
			isNum = 1;
		}
		else {
			if (isNum == 1) {
				push(atof(tmp));     //문자열을 실수로 변환
				isNum = 0;
				memset(tmp, 0, MAX_SIZE);     //문자열 변수 초기화(안 하면 엉뚱한 값이 나옴)
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
	if (isNum) {     //실수 하나만 있는 수식일 때
		return atof(tmp);
	}
	return pop();
}

int main()
{
	char expr[MAX_SIZE];
	char rtnPostexpr[MAX_SIZE];
	
	printf("수식을 입력하시오 : ");
	//scanf_s("%s", expr, MAX_SIZE); (이 함수 사용 시, 공백 포함된 수식은 정상 처리가 안 됨)
	gets_s(expr, MAX_SIZE);

	infix_to_postfix(expr, rtnPostexpr);
	//printf("postfix: %s\n", rtnPostexpr); (후위 수식 확인용)
	printf("= %lf \n", calc_postfix(rtnPostexpr));
}