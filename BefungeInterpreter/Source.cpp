#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;

template <class T>
class Stack {
public:
	void push(T val) {
		values.push_back(val);
	}
	T pop() {
		if (values.empty()) {
			return 0;
		} else {
			T tmp = values.back();
			values.pop_back();
			return tmp;
		}
	}
private:
	vector<T> values;
};


class Interpreter {
public:
	enum {LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3};
	Interpreter(): x(0), y(0), direction(RIGHT), pushAlpha(false) {}
	char nextChar();
	void getCode(string);
	char getChar();
	bool interpretCommand();
	int popStack();
	void firstCmd();

private:
	int direction;            //Current direction of travel
	int x;                    //Current x position
	int y;                    //Current y position
	char curr;                //Current character
	vector<string> codeLines; //Each line of code
	Stack<int> theStack;	  //Stack for the program
	bool pushAlpha;           //Flag for whether to push value as ASCII
};

int Interpreter::popStack() {
	return theStack.pop();
}

void Interpreter::firstCmd() {
	curr = codeLines[y][x];
}


void Interpreter::getCode(string fileName) {
	ifstream inFile(fileName);
	string temp;
	while (getline(inFile, temp)) {
		codeLines.push_back(temp);
	}
	inFile.close();
}

char Interpreter::nextChar() {
	switch (direction) {
	case RIGHT:
		if (x == codeLines[y].length() - 1) x = 0;
		else x++;
		break;
	case LEFT:
		if (x == 0) x = codeLines[y].length() - 1;
		else x--;
		break;
	case DOWN:
		if (y == codeLines.size() - 1) y = 0;
		else y++;
		break;
	case UP:
		if (y == 0) y = codeLines.size() - 1;
		else y--;
		break;
	default:
		cout << "Error! Direction is a value which it shouldn't be!" << endl;
		break;
	}

    if (codeLines[y].length() >= x) {
	    curr = codeLines[y][x];
    } else {
        curr = ' ';
    }

	return curr;
}

char Interpreter::getChar() {
	return curr;
}

bool Interpreter::interpretCommand() {
	int tmp1 = 0;
	int tmp2 = 0;
	int tmp3 = 0;
	char ctmp;
	if (curr == '"') {           //Switch to/from string mode
		pushAlpha = !pushAlpha;
	} else if (pushAlpha) {      //If " has been encountered
		theStack.push(curr);
	} else if (isdigit(curr)) {  //push digit
		theStack.push(curr - '0');
	} else {                     //Everything else
		switch (curr) {
		case '<':                //Movement
			direction = LEFT;
			break;
		case '>':
			direction = RIGHT;
			break;
		case 'v':
		case 'V':
			direction = DOWN;
			break;
		case '^':
			direction = UP;
			break;
		case '?':
			direction = rand() % 4; //Random direction
			//cout << "Direction: " << direction << endl;
            //system("pause");
			break;
		case '_':
			tmp1 = theStack.pop();
			if (tmp1) direction = LEFT;
			else direction = RIGHT;
			break;
		case '|':
			tmp1 = theStack.pop();
			if (tmp1) direction = UP;
			else direction = DOWN;
			break;

		case '+':                //Arithmetic
			theStack.push(theStack.pop() + theStack.pop());
			break;
		case '-':
			tmp1 = theStack.pop();
			tmp2 = theStack.pop();
			theStack.push(tmp2 - tmp1);
			break;
		case '*':
			theStack.push(theStack.pop() * theStack.pop());
			break;
		case '/':
			tmp1 = theStack.pop();
			tmp2 = theStack.pop();
			theStack.push(tmp2 / tmp1);
			break;
		case '%':
			tmp1 = theStack.pop();
			tmp2 = theStack.pop();
			theStack.push(tmp2 % tmp1);
			break;
		case '!':
			tmp1 = theStack.pop();
			if (tmp1) theStack.push(0);
			else theStack.push(1);
			break;
		case '`':
			tmp1 = theStack.pop();
			tmp2 = theStack.pop();
			if (tmp2 > tmp1) theStack.push(1);
			else theStack.push(0);
			break;

		case ':':
			tmp1 = theStack.pop();
			theStack.push(tmp1);
			theStack.push(tmp1);
			break;
		case '\\':
			tmp1 = theStack.pop();
			tmp2 = theStack.pop();
			theStack.push(tmp1);
			theStack.push(tmp2);
			break;
		case '$':
			theStack.pop();
			break;
		case '.':
			cout << theStack.pop() << ' ';
			break;
		case ',':
			cout << (char)theStack.pop();
			break;
		case '#':
			do { nextChar(); } while (curr == ' ');
			break;

		case '&':
			cin >> tmp1;
			theStack.push(tmp1);
			break;
		case '~':
			cin >> ctmp;
			theStack.push((int)ctmp);
			break;
		case 'p':
			tmp1 = theStack.pop();//y
			tmp2 = theStack.pop();//x
			tmp3 = theStack.pop();//val
			codeLines[tmp1][tmp2] = tmp3;
			break;
		case 'g':
			tmp1 = theStack.pop();//y
			tmp2 = theStack.pop();//x
			theStack.push(codeLines[tmp1][tmp2]);
			break;
		case '@':                //End the program
			return false;

		default:
			break;
		}
	}
	return true;
}

int main() {
	string temp;

	Interpreter befunge;

	befunge.getCode("test2.txt");
	befunge.firstCmd();

	while (befunge.interpretCommand()) {
		befunge.nextChar();
	}

	cout << endl << endl;

	system("pause");
	return 0;
}
