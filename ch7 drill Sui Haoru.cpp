/*
Simple calculator by Sui

Grammar:

Statement:
	Expression
	Print
	Quit

Print:
	;

Quit:
	q

Expression:
	Term
	Expression + Term
	Expression – Term

Term:
	Primary
	Term * Primary
	Term / Primary
	Term % Primary

Primary:
	Number
	( Expression )
	– Primary
	+ Primary

Number:
	floating-point-literal

Input comes from cin through the Token_stream called ts.
*/

//——————————————————————————————————————Sui——————————————————————————————————————————————————————————

/*test : 
1 + 2 + 3 * 23 =
1 + 1 
$ % $ % 
#@ 2323;;
;
pow(2,6);
1++2
'234';
'a'
1+a=
1+5*7
2*4;q;
let x=1
exit
... */

//——————————————————————————————————————Sui——————————————————————————————————————————————————————————

#include "std_lib_facilities.h"

//———————————————————————————————————————Sui—————————————————————————————————————————————————————————

constexpr char number = '8';  //t's kind==number
constexpr char quit = 'q'; // t.kind==quit 
constexpr char print = ';';  // t.kind==print 
constexpr char result = '='; // used to indicate that what follows is a resul

constexpr char name = 'a';  // name token
constexpr char let = 'L';  // declaration token
const string pow_function = "pow";
const string sqrt_function = "sqrt";
const string declkey = "#";   // declaration keyword
const string quit_string = "exit";

//————————————————————————————————————————Sui————————————————————————————————————————————————————————

/*the key to both variables and built-in constants is for the calculator program to keep
(name,value) pairs so that we can access the value given the name. */

class Variable 
{
	public:
		string name;
		double value;
};
/*We will use the name member to identify a Variable and the value member to store the value
corresponding to that name.*/

vector<Variable> var_table;  //set a vector to search and store the variables and their values

bool is_declared(string var) //// is var already in var_table?
{
	for (auto v : var_table)
		if (v.name == var) 
			return true;
	return false;
}

double define_name(string var, double val) // add (var,val) to var_table
{
	if(is_declared(var)) 
		error ("declared twice");
	var_table.push_back(Variable{var,val});
	return val;
}

//we can define a set_value() function to give a Variable a new value:
void set_value (string var, double val)   // set the Variable named s, the value is val
{
	for (auto& v : var_table)
		if (v.name == var)
		{
			v.value = val;
			return;
		}
	error("Variable not defined");
}

//We can write a get_value() function that looks for a given name string and returns its corresponding value:
double get_value(string var)    // return the value of the Variable named s
{
	for(auto v : var_table)
		if (v.name == var) 
			return v.value;  //see if its name matches the argument string s. If that is the case, return its value.
	error("Variable not found");
	return 0;
}
/*go through every Variable in var_table (starting with the first element and continuing until the last) 
and see if its name matches the argument string s. If that is the case, return its value.*/

//—————————————————————————————————————————Sui———————————————————————————————————————————————————————

class Token 
{
	public:
		char kind;
		double value;
		string name;
		Token(): kind(0) {}
		Token(char ch): kind(ch) {}
		Token(char ch, double val): kind(ch), value(val) {}
		Token(char ch, string n): kind(ch), name(n) {}
};

class Token_stream 
{
	public:
		Token_stream();
		Token get();
		void putback(Token t);
		void ignore(char c);  // discard characters up to and including a char c
	private:
		bool full {false}; // is there a Token in the buffer?
		Token buffer; // here is where we keep a Token put back using putback()
};	

Token_stream::Token_stream(): full (false), buffer(0) {}

void Token_stream::putback(Token t)
{
	if (full) 
		error("Token_stream buffer is full");
	buffer = t;
	full = true;
}

Token Token_stream::get()  // read characters from cin and compose a Token
{
	if (full)  // check if we already have a Token ready
	{
		full = false;
		return buffer;
	}
	char ch;
	cin >> ch;   // note that >> skips whitespace (space, newline, tab, etc.)

	switch(ch){
		case quit:
		case print:
		case '(':
		case ')':
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '=':
			return Token(ch);  // let each character represent itself
		case '0': 
		case '1': 
		case '2': 
		case '3': 
		case '4':
		case '5':
		case '6': 
		case '7': 
		case '8': 
		case '9': 
		case '.':   // a floating-point-literal can start with a dot
		{
			cin.putback(ch);  //put the number back into the input stream
			double val = 0;
			cin >> val;  // read a floating-point number
			return Token(number, val);
		}
		default:
		{
			if (isalpha(ch)) // isalpha(ch): is ch a letter?
			{
				string s;
				s += ch;
				while(cin.get(ch) && (isalpha(ch) || isdigit(ch)))   // isdigit(ch):Check if character is decimal digit
				/*it reads a character into ch (using cin’s member function get()) and checks if it is a letter or a digit. 
				If so, it adds ch to s and reads again*/
					s += ch;
				cin.putback(ch);
				if ( s == declkey )  
					return Token(let);  // declaration keyword

				 if ( s == quit_string ) 
					return Token(quit);

				if ( s == pow_function ) 
					return sqrt();

				if ( s == sqrt_function ) 
					return pow();

				else if (is_declared(s))
					return Token(number, get_value(s));

				return Token(name, s);
				//a workable specification of a name: a sequence of letters and digits starting with a letter
			}
			error("Bad token");
			return 0;
		}
	}
}

void Token_stream::ignore(char c)
{
	if (full && c == buffer.kind)  // first look in buffer:
	{
		full = false;
		return;
	}
	full = false;

	char ch = 0;  // now search input:
	while (cin >> ch)
		if (ch == c) 
			return;
}
/*This code first looks at the buffer. If there is a c there, we are finished after discarding that c;
otherwise, we need to read characters from cin until we find a c.*/

//————————————————————————————————————————Sui————————————————————————————————————————————————————————

Token_stream ts;

//————————————————————————————————————————Sui————————————————————————————————————————————————————————

void clean_up_mess()
{
	ts.ignore(print);
}

//——————————————————————————————————————————Sui——————————————————————————————————————————————————————

double expression();

//——————————————————————————————————————Sui——————————————————————————————————————————————————————————

/*Grammar:  /by Sui

Calculation:  
	Statement
	Print
	Quit
	Calculation Statement
Calculation is the new top production (rule) of the grammar. 
It expresses the loop (in calculate()) that allows us to do several calculations in a run of the calculator program.

Statement:
	Declaration
	Expression

Declaration:
	"let" Name "=" Expression*/

double declaration()
// assume we have seen "let”
// handle: name = expression
// declare a variable called "name” with the initial value "expression”
{
	Token t = ts.get();
	if(t.kind != name) 
		error("variable name expected");
	string var_name = t.name;

	Token t2 = ts.get();
	if (t2.kind != '=') 
		error ("= expected in declaration");

	double d = expression();
	define_name(var_name, d);
	return d;
}

double statement()
{
	Token t = ts.get();
	switch(t.kind)
	{
		case let:
			return declaration();
		default:
			ts.putback(t);
			return expression();
	}
}

//——————————————————————————————————————Sui——————————————————————————————————————————————————————————

double sqrt ()
{
	char ch;
    cin >> ch;
	if (ch != '(')
		error("'(' expected!");
	cin.putback(ch);
	double d = expression();
	if (d < 0) 
		error("The number in sqrt root <0");
	return sqrt(d);
	}

int power (double x,int i)
{
	int n=0 , val=1;
	for ( ; n < i; n++)
		val *= x;
	return val;
}

double pow ()
{
	Token t1 = ts.get();
	if (t1.kind != '(')
		error("'(' expected!");

	Token t2 = ts.get();
	double x = t2.value;

	Token t3 = ts.get();
	if (t3.kind != ',')
		error("',' expected!");

	Token t4 = ts.get();
	int i = t4.value;

	return power(x, i);
}
//——————————————————————————————————————Sui——————————————————————————————————————————————————————————

void calculate()  //make the calculation loop into a separate function calculate() from main
{
	while(cin)
	try 
	{
		Token t = ts.get();
		while(t.kind == print) 
			t = ts.get();  // first discard all “prints”
		if (t.kind == quit) 
			return;
		ts.putback(t);
		cout << result << statement() << endl;
		
	} 
	catch (exception& e)  //we have to catch exceptions and continue after we havecleaned up any mess
	{
		cerr << e.what() << endl;  // write error message
		clean_up_mess();
		//we need to do is to ensure that we don’t have tokens related to the aborted calculation sitting around to confuse the next calculation.
	}

}

//—————————————————————————————————————————Sui———————————————————————————————————————————————————————

double expression()
{
	double left = term();
	Token t = ts.get();
	while(true)
	{	
		switch(t.kind){
			case '+':
				left += term();
				t = ts.get();
				break;
			case '-':
				left -= term();
				t = ts.get();
				break;
			default:
				ts.putback(t);
				return left;
		}
	}
}

double term()
{
	double left = primary();
	Token t = ts.get();
	while(true)
	{
		switch(t.kind)
		{
			case '*':
				left *= primary();
				t = ts.get();
				break;
			case '/':
				left /= primary(); // primary == 0?
				t = ts.get();
				break;
			case '%':
			{
				/*
				double d = primary();
				if (d==0) error("Zero divider in %");
				left = fmod(left, d);
				t = ts.get();
				break;
				*/
				int i1 = narrow_cast<int>(left);
				int i2 = narrow_cast<int>(primary());
				if (i2 == 0) 
					error("Zero divider in %");
				left = i1 % i2;
				t = ts.get();
				break;
			}
			default:
				ts.putback(t);
				return left;
		}
	}
}

double primary()
{
	Token t = ts.get();
	switch(t.kind)
	{
		case '(':
		{
			double d = expression();
			Token t = ts.get();
		
			if (t.kind != ')') 
				error(") expected!");
			return d;
		}
		case number:
			return t.value;  //return this number
		case '-':
			return - primary();
		case '+':
			return primary();
		default:
			error("primary expected");
			return 0;
	}
}
//——————————————————————————————————————Sui——————————————————————————————————————————————————————————

int main()
try
 {
	define_name("pi", 3.1415926535);
	define_name("k", 1000);

	calculate();	

	keep_window_open();
	return 0;
} 
catch (exception& e) 
{
	cerr << e.what() << endl;
	keep_window_open("~~");
	return 1;
} 
catch (...) 
{
	cerr << "Some error\n";
	keep_window_open("~~");
	return 2;
}
