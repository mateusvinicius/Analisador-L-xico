#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <locale.h> 


using namespace std;
enum lexTipo
{
	palavra,
	numero,
	Tipo_string,
	separador
};
struct lexLista
{
	enum lexTipo tipo;
	char *value;
	int pos;
	lexLista *next;
};

const char *enum_to_str(enum lexTipo type)
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

class Lexico
{
	enum estados
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
	enum estados condicao;
	char buf;
	int err_pos;
	lexLista *current, *list;
	bool IsSimbolo(const char c);
	bool IsLetra(const char c);
	bool Numero(const char c);
	void AddSimboloALista(char c);
	void CriarNovalista(char c);
	void BuffStep();
	void ProximoDaLista(enum estados state, char c);

public:
	bool AskForState();
	int AskForLine();
	Lexico();
	~Lexico() {}
	void Analise(char c);
	void Print();
	lexLista *GetList() { return list; }
};

struct Error
{
	int pos;
	char *value;
};

int Lexico::AskForLine()
{
	return counter;
}
bool Lexico::AskForState()
{
	if (condicao == H || condicao == CL || condicao == CB || condicao == CBE)
		return true;
	else
		return false;
}

bool Lexico::IsSimbolo(const char c)
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
bool Lexico::IsLetra(const char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
	{
		return true;
	}
	return false;
}
bool Lexico::Numero(const char c)
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
void Lexico::ProximoDaLista(enum estados state, char c)
{
	condicao = state;
	CriarNovalista(c);
}
Lexico::Lexico()
{
	condicao = H;
	counter = 1;
	buf = '\0';
	list = NULL;
	current = list;
}
void Lexico::BuffStep()
{
	if (buf != '\0')
	{
		char tmp = buf;
		buf = '\0';
		Analise(tmp);
	}
}
void Lexico::AddSimboloALista(char c)
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
void Lexico::CriarNovalista(char c)
{
	lexLista *elem = new lexLista;
	elem->next = NULL;
	elem->value = new char[2];
	elem->value[0] = c;
	elem->value[1] = '\0';
	elem->pos = counter;
	switch (condicao)
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

void Lexico::Analise(char c)
{
	switch (condicao)
	{
	case H:
		if (c == '/')
			condicao = C;
		else if (c == '\n')
			counter++;
		else if (Numero(c))
			ProximoDaLista(N, c);
		else if (IsLetra(c))
			ProximoDaLista(K, c);
		else if ((c == '=' || c == '>' || c == '<'))
			ProximoDaLista(A, c);
		else if (c == '"')
			ProximoDaLista(S, '\0');
		else if ((c == ' ') || (c == '\t'))
		{
			;
		}
		else
			ProximoDaLista(H, c);
		break;
	case N:
		if (Numero(c))
		{
			AddSimboloALista(c);
		}
		else if (IsSimbolo(c))
		{
			condicao = H;
			buf = c;
		}
		else
		{
			//printf("'%c'\n", c);
			condicao = E;
			err_pos = counter;
		}
		break;
	case K:
		if (IsLetra(c))
			AddSimboloALista(c);
		else
		{
			condicao = H;
			buf = c;
		}
		break;
	case S:
		if (c != '"')
			AddSimboloALista(c);
		else
		{
			condicao = H;
		}
		break;
	case A:
		if (c == '=')
			AddSimboloALista(c);
		else
		{
			current->tipo = separador;
			condicao = H;
			buf = c;
		}
		break;
	case C:
		if (c == '*')
		{
			condicao = CB;
			break;
		}
		if (c == '/')
			condicao = CL;
		else
		{
			buf = c;
			condicao = H;
		}
		break;
	case CB:
		if (c == '*')
			condicao = CBE;
		break;
	case CBE:
		if (c == '/')
		{
			condicao = H;
		}
		else
			condicao = CB;
		break;
	case CL:
		if (c == '\n')
		{
			condicao = H;
			buf = c;
		}
		break;
	case E:
		condicao = E;
	}
	BuffStep();
}

void Lexico::Print()
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
	Lexico Lexico;
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
		Lexico.Analise(c);
	}
	fclose(fd);

	if (!Lexico.AskForState())
	{
		printf("ERRO:Erro to tipo Lexico na linha %d\n", Lexico.AskForLine());
		//exit(1);
	}else{
		printf("-Analise léxica valida ! \n");
	}

	Sintatica sin(Lexico.GetList());

	sin.analise();

	if (!sin.erros_list.empty())
	{
		for (string &erro : sin.erros_list)
			cout << erro;
	}else{
		cout<<"-Analise sintatica valida ! \n";
	}
}
