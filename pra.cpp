#include <bits/stdc++.h>

using namespace std;


struct Individuo {
    string agenda;
    string codigoVoo;
    string codigoAviao;
    string origem;
    string destino;
    string chegada;
    string partida;
    /*
    Individuo (string ag, string cV, string cA, string o, string des, string che, string par){
        ag = agenda;
        chegada = che;
        codigoAviao = cA;
        codigoVoo = cV;
        origem = o;
        destino = dest;
        partida = par;
    }
    */


};

vector<Individuo> individuos;
vector<Individuo> aux;


void iniciaParametros() {
    int numeroGeracoes;
    int tamanhoPopulacao;
    int taxaElitismo;
    int taxaMutacao;
    int cruzamento;

}

void lerArquivo();
void populacaoInicial();
double calculaFitness(int indiceIndividuo);
void selecao();
void Cruzamento(int indiceIndividuo);
void Mutacao(int indiceIndividuo);
void elitismo();
void correcaoIndividuo();
void lerArquivos();
void imprimiIndividuos();
void imprimiIndividuo(Individuo ind);


int main() {
    lerArquivos();
    cout << individuos.size() << endl;


}

void lerArquivos() {
    string line;
    string ag, cA,cV,p, c,o, d;
    ifstream myfile ("VOOS.txt");
    Individuo teste;
    stringstream buffer;
    if (myfile.is_open()) {
        while ( getline (myfile,line) ) {
            buffer.clear();
            buffer.str(line);
            buffer >> teste.agenda;
            buffer >> teste.chegada;
            buffer >> teste.codigoVoo;
            buffer >> teste.origem;
            buffer >> teste.codigoAviao;
            buffer >> teste.destino;
            imprimiIndividuo(teste);
            individuos.push_back(teste);

        }
        myfile.close();
    }

    else cout << "Unable to open file";


}

void imprimiIndividuo(Individuo ind) {
    cout << ind.agenda << endl;
    cout << ind.codigoAviao << endl;
    cout << ind.codigoVoo << endl;
    cout << ind.partida << endl;
    cout << ind.chegada << endl;


}

void imprimiIndividuos() {
    for(int i = 0; i < individuos.size(); i++) {
     imprimiIndividuo(individuos[i]);
    }
}




















