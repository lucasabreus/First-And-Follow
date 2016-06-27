#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <map>
#include <queue>

#define fore(i,ini,fim) for(int i=ini;i<fim;i++)

using namespace std;


char simboloGramatica, simboloVariaveis, simboloTerminais, simboloDerivacoes, variavelInicial;
set <char> VariablesSet, TerminalsSet;
vector <char> ordemVariaveis;
set <char> :: iterator it;
map <char,int> mapaVariaveis;
map <char,int> :: iterator itmapa;

void imprimir () {
	cout << "CONJUNTO DAS VARIÁVEIS: ";
	for(it = VariablesSet.begin(); it != VariablesSet.end(); it++){
		cout << *it << " ";
	} cout << endl;
	
	cout << "CONJUNTO DOS TERMINAIS: ";
	for(it = TerminalsSet.begin(); it != TerminalsSet.end(); it++){
		cout << *it << " ";
	} cout << endl;
}

void cabecalho () {
	// First declaration: G = (N,T,P,S)
	scanf(" %c = ( %c , %c , %c , %c )",&simboloGramatica, &simboloVariaveis, &simboloTerminais,&simboloDerivacoes,&variavelInicial);

	//Check wheter is the variables set or terminals.
	char firstChar, aux;
	fore(i,0,2){
		scanf(" %c = ",&firstChar);

		int pos = 0;
		if ( firstChar == simboloVariaveis){
			while ( scanf(" %c ,",&aux)){
				if ( aux == '}') break;
				if ( aux != ',' && aux != '{') {
					VariablesSet.insert(aux);
					ordemVariaveis.push_back(aux);
					mapaVariaveis.insert(pair<char,int>(aux,pos++));
				}
			}
		}
		if ( firstChar == simboloTerminais){
			while ( scanf(" %c ,",&aux)){
				if ( aux == '}') break;
				if ( aux != ',' && aux != '{') TerminalsSet.insert(aux);
			}
		}
	}
}

void construir ( vector < vector <string> > & derivacoes,char variavel, string valores ){
	//cout << "Variavel " << variavel << ": " << valores << endl;
	stringstream ss;

	ss << valores;

	itmapa = mapaVariaveis.find(variavel);
	if ( itmapa == mapaVariaveis.end() ) { cout << "Variável " << variavel << " não encontrada." << endl; return; }
	int indiceVariavel = itmapa->second;

	string foo,bar;
	while ( ss >> foo ){
		if ( foo.size() == 1 && foo[0] == '|' ){}
		else {
			derivacoes[indiceVariavel].push_back(foo);
		}
	} 
}

set <char> primeiro ( vector < vector <string> > & derivacoes, int indice ){
	queue <string> palavras;
	set <char> first;
	set <char> :: iterator itset;

	fore(i,0,derivacoes[indice].size()){
		string palavra = derivacoes[indice][i];
		palavras.push(palavra);
	}

	while ( !palavras.empty()){
		string palavra = palavras.front();
		palavras.pop();

		fore(i,0,palavra.size()){
			if ( TerminalsSet.find(palavra[i]) != TerminalsSet.end() ){
				first.insert(palavra[i]);
				break; 
			} else if ( VariablesSet.find(palavra[i]) != VariablesSet.end()){
				itmapa = mapaVariaveis.find(palavra[i]);
				int indice = itmapa->second;
				bool vazio = false;
				fore(j,0,derivacoes[indice].size()){
					if ( derivacoes[indice][j][0] == '*' ) {
						vazio = true;
						if ( i == palavra.size() - 1 ) first.insert('*');
					} else {
						palavras.push(derivacoes[indice][j]);
					}
				}
				if ( vazio == false ) break;
			} else {
				first.insert('*');
			}
		}
	}
	/*itset = VariablesSet.begin();
	fore(i,0,indice) itset++;
	char var = *itset;

	cout << "FIRST " << var << ": ";
	for(itset = first.begin(); itset != first.end(); itset++){
		cout << *itset << " ";
	} cout << endl; */
	return first;
}

//Calcula o First para todas as variáveis
vector < set <char> > first ( vector < vector <string> > & derivacoes ){
	vector < set<char> > conjuntoFirst;

	fore(i,0,VariablesSet.size()){
		conjuntoFirst.push_back(primeiro(derivacoes,i));
	}
	return conjuntoFirst;
}

set <char> follow ( vector < vector <string> > & derivacoes, vector < set<char> > & conjuntoFirst, vector <bool> & visitado, int indice, vector < set<char> > & conjuntoFollow ){
	if ( visitado[indice] == 1 ) return conjuntoFollow[indice];

	visitado[indice] = 1;

	fore(i,0,derivacoes.size()){
		fore(j,0,derivacoes[i].size()){
			string palavra = derivacoes[i][j];
			fore(k,0,palavra.size()){
				if ( mapaVariaveis.find(palavra[k]) != mapaVariaveis.end() && mapaVariaveis.find(palavra[k])->second == indice ){
					if ( k == palavra.size() - 1 ){
						set <char> conjunto = follow(derivacoes,conjuntoFirst,visitado,i,conjuntoFollow);
						conjuntoFollow[indice].insert(conjunto.begin(),conjunto.end());
					} else {
						if ( mapaVariaveis.find(palavra[k+1]) != mapaVariaveis.end() ){
							int n = k+1;
							bool fim = false;
							while ( fim == false ){
								int i3 = mapaVariaveis.find(palavra[n])->second;
								if ( mapaVariaveis.find(palavra[n]) == mapaVariaveis.end() ){
									conjuntoFollow[indice].insert(palavra[n]);
									fim = true;
								} else {
									//Verificar se no conjunto first da variável seguinte tem o vazio.
									if ( conjuntoFirst[i3].find('*') != conjuntoFirst[i3].end() ){
										set <char> conjunto = conjuntoFirst[i3];
										conjunto.erase('*');
										conjuntoFollow[indice].insert(conjunto.begin(),conjunto.end()); 
									} else {
										conjuntoFollow[indice].insert(conjuntoFirst[i3].begin(),conjuntoFirst[i3].end());
										fim = true;
									}
								}
								if ( n == palavra.size()-1) {
									fim = true;
									conjuntoFollow[indice].insert('$');
								} 
								n++; 
							}
						} else {
							conjuntoFollow[indice].insert(palavra[k+1]);
						}
					}
				}
			}
		}
	}

	return conjuntoFollow[indice];
}

void imprimeConjunto ( vector < set<char> > & conjunto, string tag ){
	set <char> :: iterator itset;
	fore(i,0,ordemVariaveis.size()){
		cout << tag << " " << ordemVariaveis[i] << ": ";
		for(itset = conjunto[i].begin();itset != conjunto[i].end();itset++){
			cout << *itset << " ";
		} cout << endl;
	}
}

//Responsável por separar as variáveis em um vetor e para cada variável
//colocar em um vetor de strings suas derivações.
void gramatica (){
	char simboloG;
	scanf(" %c = {", &simboloG);

	//Conjunto First
	vector < set<char> > conjuntoFirst;

	//Conjunto Follow
	vector < set<char> > conjuntoFollow(mapaVariaveis.size());

	if ( simboloG != simboloDerivacoes) { cout << "Gramática não identificada no cabeçalho." << endl; return ; }

	vector < vector <string> > derivacoes(VariablesSet.size());

	char simbol;
	string aux;
	while ( cin >> simbol){
		if ( simbol == '}' ) break;
		scanf(" -> ");
		getline(cin, aux);
		construir(derivacoes,simbol,aux);
	}

	conjuntoFirst = first(derivacoes);
	imprimeConjunto(conjuntoFirst,"First");

	cout << endl;

	vector <bool> visitado(derivacoes.size(),0);

	int indiceInicial = mapaVariaveis.find(variavelInicial)->second;
	conjuntoFollow[indiceInicial].insert('$');

	fore(i,0,derivacoes.size()){
		conjuntoFollow[i] = follow(derivacoes,conjuntoFirst,visitado,i,conjuntoFollow);
		if ( conjuntoFollow[i].size() == 0 ) conjuntoFollow[i].insert('$');
	}
	imprimeConjunto(conjuntoFollow,"Follow");

}

int main () {
	cabecalho();
	imprimir();
	gramatica();
}