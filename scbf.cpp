#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// funções úteis e ademais ahaha
bool EhNumero(string val)
{
	if(val.at(0) == '0' || val.at(0) == '1' || val.at(0) == '2' || val.at(0) == '3' || val.at(0) == '4' || val.at(0) == '5' || val.at(0) == '6' || val.at(0) == '7' || val.at(0) == '8' || val.at(0) == '9')
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Var(int idx)
{
	return idx*4 + 32;
}

typedef struct
{
	string name;
	vector<string> cd;
}macro;

void CompileError(string msg)
{
	cout << msg << endl;
	exit(1);
}


//------------------------------------
// CLASSE DO COMPILADOR DA CUS2
//------------------------------------
class custwo
{
	private:
		vector<string> code;
		vector<macro> macros;
		string bfcode;
		
	public:
		void ReadCodeFromFile(string filename);
		void Compile();
		void RunBf();
		void DisplayBf();
};

void custwo::ReadCodeFromFile(string filename)
{
	fstream arq(filename);
	string rd;
	while(arq >> rd){code.push_back(rd);}
}

void custwo::Compile()
{
	//variaveis de execução
	vector<string> vars;
	string token = "";
	int pc = 0;
	string optype = "load";
	string out = "";
	string dupe = "[->+>+<<]>>[-<<+>>]<";
	
	for(int i=0 ; i<8 ; i++)
	{
		string varname = "x" + to_string(i);
		vars.push_back(varname);
	}
	
	//E LA VAMOS NÓS x4
	while(pc < code.size())
	{
		//pegar comando a ser executado
		token = code[pc];
		//cout << "Processando token " << token << endl;
		
		//PROCESSAR TOKENS IHAAA
		
		// var
		if(token == "var")
		{
			pc += 1; token = code[pc];
			vars.push_back(token);
		}
		
		// vec
		if(token == "vec")
		{
			pc += 1;
			token = code[pc];
			vars.push_back(token);
			pc += 1;
			int vecsize = stoi(code[pc]);
			
			for(int i=0 ; i<vecsize ; i++)
			{
				vars.push_back(token);
			}
		}
		
		// import
		if(token == "import")
		{
			pc += 1; token = code[pc];
			ReadCodeFromFile(token);
		}
		
		// {}
		else if(token == "{")
		{
			bfcode += "[";
		}
		else if(token == "}")
		{
			bfcode += "]";
		}
		else if(token == "f}")
		{
			bfcode += "[-]]";
		}
		else if(token == "t{")
		{
			bfcode += "[-]+[";
		}
		
		// true e falsee flip
		else if(token == "true")
		{
			bfcode += "[-]+";
		}
		else if(token == "false")
		{
			bfcode += "[-]";
		}
		else if(token == "flip")
		{
			bfcode += ">+<[>[-]<[-]]>[-<+>]<";
		}
		
		// print values ihull
		else if(token.at(0) == '_')
		{
			token = token.substr(1);
			
			// para cada caractere da string a ser printada
			for(int i=0 ; i<token.size() ; i++)
			{
				char chr = token.at(i);
				bfcode += "[-]";
				for(int j=0 ; j<chr ; j++){bfcode += "+";}
				bfcode += ".";
			}
			bfcode += "[-]";
		}
		else if(token == ".")
		{
			bfcode += ">>++++++++++<<[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]>>[-]>>>++++++++++<[->-[>+>>]>[+[-<+>]>+>>]<<<<<]>[-]>>[>++++++[-<++++++++>]<.<<+>+>[-]]<[<[->-<]++++++[->++++++++ <]>.[-]]<<++++++[-<++++++++>]<.[-]<<[-<+>]<";
		}
		else if(token == "..")
		{
			bfcode += ".";
		}
		else if(token == "endl")
		{
			bfcode += "[-]++++++++++.[-]";
		}
		else if(token == "&")
		{
			bfcode += "[-]++++++++++++++++++++++++++++++++.[-]";
		}
		
		// ler valores do terminal
		else if(token == "get_number")
		{
			bfcode += "[-]>[-]+[[-]>[-],[+[-----------[>[-]++++++[<------>-]<--<<[->>++++++++++<<]>>[-<<+>>]<+>]]]<]<";
		}
		else if(token == "get_char")
		{
			bfcode += ",";
		}
		
		// set ops
		else if(token == "+" || token == "-" || token == "*" || token == "/" || token == ">>" || token == "!=" || token == "==" || token == "<" || token == ">" || token == "%" || token == "lda")
		{
			optype = token;
		}
		
		// se bateu em um macro solto, pula
		else if(token == "macro")
		{
			while(token != "endmacro")
			{
				pc += 1;
				token = code[pc];
			}
		}
		
		// se bateu em uma struct solta, pula
		else if(token == "struct")
		{
			while(token != "endstruct")
			{
				pc += 1;
				token = code[pc];
			}
		}
		
		// comentarios (ignorar)
		else if(token.at(0) == '#'){}
		
		// hardcode
		else if(token == "hardcode")
		{
			pc += 1;
			token = code[pc];
			bfcode += token;
		}
		
		// get
		else if(token == "get")
		{
			// definir posição base da variavel
			pc += 1; token = code[pc];
			int varindex = 0;
			for(int i=0 ; i<vars.size() ; i++)
			{
				string vrn = vars[i];
				if(token == vrn){varindex = i;break;}
			}
			varindex = Var(varindex);
			
			//espero que isso funcione
			bfcode += "+";
			bfcode += "[-";
			for(int i=0 ; i<varindex ; i++){bfcode += ">";}
			bfcode += ">+<";
			for(int i=0 ; i<varindex ; i++){bfcode += "<";}
			bfcode += "]";
			for(int i=0 ; i<varindex ; i++){bfcode += ">";}
			bfcode += ">";
			
			bfcode += "[->+>+<<]>>[-<<+>>]<";
			
			bfcode += "[-[->>>>+<<<<]<[->>>>+<<<<]>>>>>]";
			
			bfcode += "<<[->>+>+<<<]>>[-<<+>>]<";
			
			bfcode += "[-[-<<<<+>>>>]>>[-<<<<+>>>>]<<<<<<]";
			
			bfcode += "<[-]>>>[-<<<+>>>]<<<";
			
			for(int i=0 ; i<varindex ; i++){bfcode += "<";}
			
		}
		
		// set
		else if(token == "set")
		{
			// definir posição base da variavel
			pc += 1; token = code[pc];
			int varindex = 0;
			for(int i=0 ; i<vars.size() ; i++)
			{
				string vrn = vars[i];
				if(token == vrn){varindex = i;break;}
			}
			varindex = Var(varindex);
			
			//espero que isso funcione
			bfcode += "+";
			bfcode += "[-";
			for(int i=0 ; i<varindex ; i++){bfcode += ">";}
			bfcode += ">+<";
			for(int i=0 ; i<varindex ; i++){bfcode += "<";}
			bfcode += "]";
			for(int i=0 ; i<varindex ; i++){bfcode += ">";}
			bfcode += ">";
			
			bfcode += dupe;
			
			bfcode += "<<[->>>+<<<]>>";
			
			bfcode += "[-[->>>>+<<<<]<[->>>>+<<<<]>>[->>>>+<<<<]>>>]";
			
			bfcode += "<<[-]>>>[-<<<+>>>]<<";
			
			bfcode += "[-[-<<<<+>>>>]<<<<]<";
			
			for(int i=0 ; i<varindex ; i++){bfcode += "<";}
			
		}
		
		// E EH AGORA EM QUE TUDO EXPLODE
		else
		{
			bool ehvar = false;
			int varindex = 0;
			for(int i=0 ; i<vars.size() ; i++)
			{
				string vrn = vars[i];
				if(token == vrn){ehvar = true;varindex = i;break;}
			}
			varindex = Var(varindex);
			
			bool ehstruct = false;
			int structindex = 0;

			for(int i=1 ; i<code.size() ; i++)
			{
				if(token == code[i] && code[i-1] == "struct") 
				{
					ehstruct = true;
					structindex = i;
					break;
				}
			}
			
			
			// caso 1: eh numero
			if(EhNumero(token))
			{
				if(optype == "+"){for(int i=0 ; i<stoi(token) ; i++){bfcode += "+";}}
				if(optype == "-"){for(int i=0 ; i<stoi(token) ; i++){bfcode += "-";}}
				if(optype == "!=")
				{
					bfcode += ">[-]";
					for(int i=0 ; i<stoi(token) ; i++){bfcode += "+";}
					bfcode += "<>[-<->]<[[-]+>]<";
				}
				if(optype == "==")
				{
					bfcode += ">[-]";
					for(int i=0 ; i<stoi(token) ; i++){bfcode += "+";}
					bfcode += "<>[-<->]<>+<[>[-]<[-]]>[-<+>]<";
				}
				if(optype == "<")
				{
					bfcode += ">[-]";
					for(int i=0 ; i<stoi(token) ; i++){bfcode += "+";}
					bfcode += "<>>[-]>[-]>[-]+>[-]<<<<[>+>+<<-]>[<+>-]<<[>>+<<-]+>>>[>-]>[<<<<->>[-]>>->]<+<<[>-[>-]>[<<<<->>[-]+>>->]<+< <-]>>[-]<[-]<[-]<[-]<";
				}
				if(optype == ">")
				{
					bfcode += ">[-]";
					for(int i=0 ; i<stoi(token) ; i++){bfcode += "+";}
					bfcode += "<>>>[-]>[-]<<[-]<<[>>>+<<[->>[-]>+<<<]>>[-<+>]>[-<<<+>>>]<<<-<-]>[-]>[-<<+>>]<<";
				}
				if(optype == "*")
				{
					bfcode += ">[-]";
					for(int i=0 ; i<stoi(token) ; i++){bfcode += "+";}
					bfcode += "<>>[-]>[-]<<<[>>>+<<<-]>>>[<<[<+>>+<-]>[<+>-]>-][-]<[-]<[-]<";
				}
				if(optype == "/")
				{
					bfcode += ">[-]";
					for(int i=0 ; i<stoi(token) ; i++){bfcode += "+";}
					bfcode += "<>>[-]>[-]>[-]>[-]<<<<<[>>+<<-]>>[<[>>+>+<<<-]>>>[<<<+>>>-]<[>+<<-[>>[-]>+<<<-]>>>[<<<+>>>-]<[<-[<<<->>>[-]]+>-]<-]<<<+>>][-]<[-]<";
				}
				if(optype == "%")
				{
					bfcode += ">[-]";
					for(int i=0 ; i<stoi(token) ; i++){bfcode += "+";}
					bfcode += "<[>->+<[>]>[<+>-]<<[<]>-]>[-]>[-<<+>>]<<";
				}
				if(optype == "lda")
				{
					bfcode += ">[-]";
					for(int i=0 ; i<stoi(token) ; i++){bfcode += "+";}
					bfcode += "<";
				}
				if(optype == "load"){bfcode += "[-]"; for(int i=0 ; i<stoi(token) ; i++){bfcode += "+";}}
				optype = "load";
			}
			
			// caso 1.5: eh char
			else if(token.at(0) == '\'')
			{
				int val = static_cast<int>(token.at(1));
				if(optype == "+"){for(int i=0 ; i<val ; i++){bfcode += "+";}}
				if(optype == "-"){for(int i=0 ; i<val ; i++){bfcode += "-";}}
				if(optype == "!=")
				{
					bfcode += ">[-]";
					for(int i=0 ; i<val ; i++){bfcode += "+";}
					bfcode += "<>[-<->]<[[-]+>]<";
				}
				if(optype == "==")
				{
					bfcode += ">[-]";
					for(int i=0 ; i<val ; i++){bfcode += "+";}
					bfcode += "<>[-<->]<>+<[>[-]<[-]]>[-<+>]<";
				}
				if(optype == "<")
				{
					bfcode += ">[-]";
					for(int i=0 ; i<val ; i++){bfcode += "+";}
					bfcode += "<>>[-]>[-]>[-]+>[-]<<<<[>+>+<<-]>[<+>-]<<[>>+<<-]+>>>[>-]>[<<<<->>[-]>>->]<+<<[>-[>-]>[<<<<->>[-]+>>->]<+< <-]>>[-]<[-]<[-]<[-]<";
				}
				if(optype == ">")
				{
					bfcode += ">[-]";
					for(int i=0 ; i<val ; i++){bfcode += "+";}
					bfcode += "<>>>[-]>[-]<<[-]<<[>>>+<<[->>[-]>+<<<]>>[-<+>]>[-<<<+>>>]<<<-<-]>[-]>[-<<+>>]<<";
				}
				if(optype == "*")
				{
					bfcode += ">[-]";
					for(int i=0 ; i<val ; i++){bfcode += "+";}
					bfcode += "<>>[-]>[-]<<<[>>>+<<<-]>>>[<<[<+>>+<-]>[<+>-]>-][-]<[-]<[-]<";
				}
				if(optype == "/")
				{
					bfcode += ">[-]";
					for(int i=0 ; i<val ; i++){bfcode += "+";}
					bfcode += "<>>[-]>[-]>[-]>[-]<<<<<[>>+<<-]>>[<[>>+>+<<<-]>>>[<<<+>>>-]<[>+<<-[>>[-]>+<<<-]>>>[<<<+>>>-]<[<-[<<<->>>[-]]+>-]<-]<<<+>>][-]<[-]<";
				}
				if(optype == "%")
				{
					bfcode += ">[-]";
					for(int i=0 ; i<val ; i++){bfcode += "+";}
					bfcode += "<[>->+<[>]>[<+>-]<<[<]>-]>[-]>[-<<+>>]<<";
				}
				if(optype == "lda")
				{
					bfcode += ">[-]";
					for(int i=0 ; i<val ; i++){bfcode += "+";}
					bfcode += "<";
				}
				if(optype == "load"){bfcode += "[-]"; for(int i=0 ; i<val ; i++){bfcode += "+";}}
				optype = "load";
			}
			
			// caso 2: eh variavel
			else if(ehvar)
			{
				if(optype == "+")
				{
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += dupe;
					bfcode += "[-";
					for(int i=-1 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "+";
					for(int i=-1 ; i<varindex ; i++){bfcode += ">";}
					bfcode += "]";
					for(int i=-1 ; i<varindex ; i++){bfcode += "<";}
				}
				if(optype == "-")
				{
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += dupe;
					bfcode += "[-";
					for(int i=-1 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "-";
					for(int i=-1 ; i<varindex ; i++){bfcode += ">";}
					bfcode += "]";
					for(int i=-1 ; i<varindex ; i++){bfcode += "<";}
				}
				if(optype == "load")
				{
					bfcode += "[-]";
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += dupe;
					bfcode += "[-";
					for(int i=-1 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "+";
					for(int i=-1 ; i<varindex ; i++){bfcode += ">";}
					bfcode += "]";
					for(int i=-1 ; i<varindex ; i++){bfcode += "<";}
				}
				if(optype == ">>")
				{
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += "[-]";
					for(int i=0 ; i<varindex ; i++){bfcode += "<";}
					
					bfcode += "[-";
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += "+";
					for(int i=0 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "]";
				}
				if(optype == "!=")
				{
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += dupe;
					bfcode += "[-";
					for(int i=0 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "+";
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += "]";
					for(int i=0 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "<>[-<->]<[[-]+>]<";
				}
				if(optype == "==")
				{
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += dupe;
					bfcode += "[-";
					for(int i=0 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "+";
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += "]";
					for(int i=0 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "<>[-<->]<>+<[>[-]<[-]]>[-<+>]<";
				}
				if(optype == "<")
				{
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += dupe;
					bfcode += "[-";
					for(int i=0 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "+";
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += "]";
					for(int i=0 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "<>>[-]>[-]>[-]+>[-]<<<<[>+>+<<-]>[<+>-]<<[>>+<<-]+>>>[>-]>[<<<<->>[-]>>->]<+<<[>-[>-]>[<<<<->>[-]+>>->]<+< <-]>>[-]<[-]<[-]<[-]<";
				}
				if(optype == ">")
				{
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += dupe;
					bfcode += "[-";
					for(int i=0 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "+";
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += "]";
					for(int i=0 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "<>>>[-]>[-]<<[-]<<[>>>+<<[->>[-]>+<<<]>>[-<+>]>[-<<<+>>>]<<<-<-]>[-]>[-<<+>>]<<";
				}
				if(optype == "*")
				{
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += dupe;
					bfcode += "[-";
					for(int i=0 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "+";
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += "]";
					for(int i=0 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "<>>[-]>[-]<<<[>>>+<<<-]>>>[<<[<+>>+<-]>[<+>-]>-][-]<[-]<[-]<";
				}
				if(optype == "/")
				{
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += dupe;
					bfcode += "[-";
					for(int i=0 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "+";
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += "]";
					for(int i=0 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "<>>[-]>[-]>[-]>[-]<<<<<[>>+<<-]>>[<[>>+>+<<<-]>>>[<<<+>>>-]<[>+<<-[>>[-]>+<<<-]>>>[<<<+>>>-]<[<-[<<<->>>[-]]+>-]<-]<<<+>>][-]<[-]<";
				}
				if(optype == "%")
				{
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += dupe;
					bfcode += "[-";
					for(int i=0 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "+";
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += "]";
					for(int i=0 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "<[>->+<[>]>[<+>-]<<[<]>-]>[-]>[-<<+>>]<<";
				}
				if(optype == "lda")
				{
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += dupe;
					bfcode += "[-";
					for(int i=0 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "+";
					for(int i=0 ; i<varindex ; i++){bfcode += ">";}
					bfcode += "]";
					for(int i=0 ; i<varindex ; i++){bfcode += "<";}
					bfcode += "<";
				}
				optype = "load";
			}
			
			// caso 3: eh struct
			else if(ehstruct)
			{
				pc += 1;
				token = code[pc];
				vars.push_back(token);
				
				structindex += 1;
				string decl = code[structindex];
				while(decl != "endstruct")
				{
					string varname = token + "." + decl;
					vars.push_back(varname);
					
					structindex += 1;
					decl = code[structindex];
				}
			}
			
			// caso 4: eh macro
			else
			{
				//achar macro
				int macrostart = 1;
				while(code[macrostart-1] != "macro" || code[macrostart] != token)
				{
					macrostart += 1;
					if(macrostart >= code.size())
					{
						string errormsg = "ERROR: command or macro \"";
						errormsg += token;
						errormsg += "\" does not exist!";
						CompileError(errormsg);
					}
				}
				
				//isolar macro
				macrostart += 1;
				vector<string> mcr;
				while(code[macrostart] != "endmacro")
				{
					mcr.push_back(code[macrostart]);
					macrostart += 1;
				}
				
				//colocar macro no meio do código corrente
				code.erase(code.begin() + pc);
				code.insert(code.begin() + pc, mcr.begin(), mcr.end());
				pc -= 1;
			}
		}
		
		// ACABOU
		//cout << "BF atual:\n\t" << bfcode << endl << endl;
		pc += 1;
	}
}

void custwo::RunBf()
{
	// variaveis de execução
	char mem[4096] = {};
	char op;
	int pc = 0;
	int mc = 0;
	
	// RUN!
	while(pc < bfcode.size())
	{
		op = bfcode[pc];
		
		if(op == '+'){mem[mc] += 1;}
		else if(op == '-'){mem[mc] -= 1;}
		else if(op == '>'){mc += 1;}
		else if(op == '<'){mc -= 1;}
		
		else if(op == '.'){cout << mem[mc];}
		
		else if(op == ','){mem[mc]=(char)cin.get();}
		
		else if(op == ']' && mem[mc] != 0)
		{
			int bal = 1;
			while(bal != 0)
			{
				pc-=1;
				op = bfcode[pc];
				if(op==']'){bal+=1;}
				if(op=='['){bal-=1;}
			}
		}
		else if(op == '[' && mem[mc] == 0)
		{
			int bal = 1;
			while(bal != 0)
			{
				pc+=1;
				op = bfcode[pc];
				if(op=='['){bal+=1;}
				if(op==']'){bal-=1;}
			}
		}
		pc += 1;
	}
	
	// ver memória para debug
	/*
	int ct = 0;
	int pos = 0;
	cout << pos << ": ";
	for(int i=0 ; i<1024 ; i++)
	{
		ct += 1;
		cout << (mem[i]) << " ";
		if(ct == 4){cout << endl << pos << ": "; ct = 0; pos += 1;}
	}
	cout << endl;
	*/
}

void custwo::DisplayBf()
{
	cout << bfcode << endl;
}



int main(int argc, char *argv[])
{
	custwo cust;
	cust.ReadCodeFromFile(argv[1]);
	cust.Compile();
	if(argc > 2)
	{
		cust.DisplayBf();
	}
	else
	{
		cust.RunBf();
	}
}
