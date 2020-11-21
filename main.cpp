#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <locale.h> 


using namespace std;
enum lextype
{
	palavra,
	numero,
	Tipo_string,
	separador
};
struct lexLista
{
	enum lextype tipo;
	char *value;
	int pos;
	lexLista *next;
};

const char *enum_to_str(enum lextype type)
{
	switch (type)
	{
	case palavra:
		return "palavra";
	case numero:
		return "numero";
	case Tipo_string:
		return "string";
	case separador:
		return "separador";
	}
	return "error";
}

int str_len(const char *str)
{
	int i = 0;
	while (str[i] != '\0')
		i++;
	return i;
}

class Lexic
{
	enum states
	{
		N,
		H,
		K,
		A,
		S,
		E,
		C,
		CL,
		CB,
		CBE
	};
	int counter;
	enum states condition;
	char buf;
	int err_pos;
	lexLista *current, *list;
	bool IsSep(const char c);
	bool IsLetter(const char c);
	bool IsNum(const char c);
	void AddSymToList(char c);
	void MakeNewList(char c);
	void BuffStep();
	void NextList(enum states state, char c);

public:
	bool AskForState();
	int AskForLine();
	Lexic();
	~Lexic() {}
	void Analyse(char c);
	void Print();
	lexLista *GetList() { return list; }
};

struct Error
{
	int pos;
	char *value;
};

int Lexic::AskForLine()
{
	return counter;
}
bool Lexic::AskForState()
{
	if (condition == H || condition == CL || condition == CB || condition == CBE)
		return true;
	else
		return false;
}

bool Lexic::IsSep(const char c)
{
	char mas[] = " \t\n+=-*/%,:><{}()";
	int i = 0;
	while (mas[i] != '\0')
	{
		if (mas[i] == c)
			return true;
		i++;
	}
	return false;
}
bool Lexic::IsLetter(const char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
	{
		return true;
	}
	return false;
}
bool Lexic::IsNum(const char c)
{
	if (c >= '0' && c <= '9')
		return true;
	return false;
}
int str_size(const char *str)
{
	int i = 0;
	while (str[i] != '\0')
		i++;
	return i;
}
void Lexic::NextList(enum states state, char c)
{
	condition = state;
	MakeNewList(c);
}
Lexic::Lexic()
{
	condition = H;
	counter = 1;
	buf = '\0';
	list = NULL;
	current = list;
}
void Lexic::BuffStep()
{
	if (buf != '\0')
	{
		char tmp = buf;
		buf = '\0';
		Analyse(tmp);
	}
}
void Lexic::AddSymToList(char c)
{
	char *str, *newstr;
	str = current->value;
	int n = str_len(str);
	newstr = new char[n + 2];
	for (int i = 0; i < n; i++)
		newstr[i] = str[i];
	newstr[n] = c;
	newstr[n + 1] = '\0';
	current->value = newstr;
}
void Lexic::MakeNewList(char c)
{
	lexLista *elem = new lexLista;
	elem->next = NULL;
	elem->value = new char[2];
	elem->value[0] = c;
	elem->value[1] = '\0';
	elem->pos = counter;
	switch (condition)
	{
	case N:
		elem->tipo = numero;
		break;
	case K:
		elem->tipo = palavra;
		break;
	case S:
		elem->tipo = Tipo_string;
		break;
	default:
		elem->tipo = separador;
	}
	if (list == NULL)
	{
		list = elem;
		current = list;
		return;
	}
	current->next = elem;
	current = current->next;
}

void Lexic::Analyse(char c)
{
	switch (condition)
	{
	case H:
		if (c == '/')
			condition = C;
		else if (c == '\n')
			counter++;
		else if (IsNum(c))
			NextList(N, c);
		else if (IsLetter(c))
			NextList(K, c);
		else if ((c == '=' || c == '>' || c == '<'))
			NextList(A, c);
		else if (c == '"')
			NextList(S, '\0');
		else if ((c == ' ') || (c == '\t'))
		{
			;
		}
		else
			NextList(H, c);
		break;
	case N:
		if (IsNum(c))
		{
			AddSymToList(c);
		}
		else if (IsSep(c))
		{
			condition = H;
			buf = c;
		}
		else
		{
			//printf("'%c'\n", c);
			condition = E;
			err_pos = counter;
		}
		break;
	case K:
		if (IsLetter(c))
			AddSymToList(c);
		else
		{
			condition = H;
			buf = c;
		}
		break;
	case S:
		if (c != '"')
			AddSymToList(c);
		else
		{
			condition = H;
		}
		break;
	case A:
		if (c == '=')
			AddSymToList(c);
		else
		{
			current->tipo = separador;
			condition = H;
			buf = c;
		}
		break;
	case C:
		if (c == '*')
		{
			condition = CB;
			break;
		}
		if (c == '/')
			condition = CL;
		else
		{
			buf = c;
			condition = H;
		}
		break;
	case CB:
		if (c == '*')
			condition = CBE;
		break;
	case CBE:
		if (c == '/')
		{
			condition = H;
		}
		else
			condition = CB;
		break;
	case CL:
		if (c == '\n')
		{
			condition = H;
			buf = c;
		}
		break;
	case E:
		condition = E;
	}
	BuffStep();
}

void Lexic::Print()
{
	lexLista *cur = list;
	while (cur)
	{
		printf("%s  \'%s\'  %d\n",
			   cur->value, enum_to_str(cur->tipo), cur->pos);
		cur = cur->next;
	}
}

class Sintatica
{
	lexLista *lista;
	lexLista *temp_lista;
	vector<string> palavras_reservadas{"for", "step", "step", "sqrt", "if", "else", "return", "print", "read", "to", "num", "string", "break"};
	vector<string> lista_auxiliar;
	void GetLex();
	void VARIAVEL();
	void PALAVRA();

public:
	vector<string> erros_list;
	Sintatica(lexLista *l)
	{
		lista = l;
		temp_lista = l;
	}
	void analise();
};

void Sintatica::GetLex()
{
	if (temp_lista->next)
		temp_lista = temp_lista->next;
}
void Sintatica::VARIAVEL()
{

	if (temp_lista->tipo == palavra)
	{
		if (temp_lista->pos == 1)
		{
			if (!(find(palavras_reservadas.begin(), palavras_reservadas.end(), temp_lista->value) != palavras_reservadas.end()))
				lista_auxiliar.push_back(temp_lista->value);
		}
	}
}

void Sintatica::PALAVRA()
{
	if (temp_lista->tipo == palavra)
	{
		if (temp_lista->pos != 1)
		{
			if (!(find(palavras_reservadas.begin(), palavras_reservadas.end(), temp_lista->value) != palavras_reservadas.end()))
				if (!(find(lista_auxiliar.begin(), lista_auxiliar.end(), temp_lista->value) != lista_auxiliar.end()))
				{
					string value{temp_lista->value};
					string position = to_string(temp_lista->pos);
					string err = "-Error Sintatico-linha:" + position + ": A palavra " + value + " não é uma palavra reservada e nem uma variavel. \n";
					erros_list.push_back(err);
				}
		}
	}
}

void Sintatica::analise()
{
	//Verifica variavel
	while (temp_lista->next != NULL)
	{
		VARIAVEL();
		GetLex();
	}
	temp_lista = lista;
	//Verficar palavra
	while (temp_lista->next != NULL)
	{
		PALAVRA();
		GetLex();
	}
}

int main(int argc, char **argv)
{
	FILE *fd;
	Lexic lexic;
	char c;
	setlocale(LC_ALL, "Portuguese");
	if (argc != 2)
	{
		printf("Erro: é necessario executar a aplicacão com um argumento! ");
		exit(1);
	}
	fd = fopen(argv[1], "r");
	if (!fd)
	{
		perror("fopen");
		exit(1);
	}

	while ((c = fgetc(fd)) != EOF)
	{
		lexic.Analyse(c);
	}
	fclose(fd);

	if (!lexic.AskForState())
	{
		printf("ERRO:Erro to tipo lexico na linha %d\n", lexic.AskForLine());
		//exit(1);
	}else{
		printf("-Analise léxica valida ! \n");
	}

	Sintatica sin(lexic.GetList());

	sin.analise();

	if (!sin.erros_list.empty())
	{
		for (string &erro : sin.erros_list)
			cout << erro;
	}else{
		cout<<"-Analise sintatica valida ! \n";
	}
}
