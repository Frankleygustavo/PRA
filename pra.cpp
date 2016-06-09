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
struct aviao{
char prefixo[5];
char modelo[4];};

struct voo{
char origem[4];
char destino[4];
int partida[3];
int chegada[3];
char modelo[5];};

struct aeroporto{
char nome[4];
vector<aviao> AvioesEmSolo;
vector<voo> VoosSaindo;};

struct emVoo{
vector<aviao> noAr;
int numAvioesNoAr;};



int rel[3]={0,0,0};

vector<aeroporto> bases;
vector<Individuo> individuos;
vector<Individuo> aux;
vector<aviao> naoAlocado;
emVoo decolados;

bool vooSaiAgora(voo v){
    if(v.partida[0]==rel[0]&&v.partida[1]==rel[1]&&v.partida[2]==rel[2]) return true;
    return false;
}

bool aviaoDoModeloAAlocar(char m[4]){
    for(int i=0;i<naoAlocado.size();++i)if(strcmp(naoAlocado[i].modelo,m)==0)return true;
    return false;
}

aviao AlocarAviaoDisponivel(char m[4]){
    aviao res;
    for(int i=0;i<naoAlocado.size();++i){
            if(strcmp(naoAlocado[i].modelo,m)==0){
                res= naoAlocado[i];
                if(rand()%2){
                        naoAlocado.erase(naoAlocado.begin()+i);
                        return res;
                }
            }
    }
    return res;
}

void alocarAviaoAleatorio(aeroporto a ,voo v){

    if(!aviaoDoModeloAAlocar(v.modelo)){
        int numAv=a.AvioesEmSolo.size(), aleat;
        aleat=rand()%numAv;
        if(a.AvioesEmSolo[aleat].modelo==v.modelo){
            decolados.noAr.push_back(a.AvioesEmSolo[aleat]);
            decolados.numAvioesNoAr++;
            a.AvioesEmSolo.erase(a.AvioesEmSolo.begin()+aleat);
        }
    }
    else{
        aviao av=AlocarAviaoDisponivel(v.modelo);
           decolados.noAr.push_back(av);
           decolados.numAvioesNoAr++;
        }
    }


void cria_individuo(){
    while(rel[0]<7&&rel[1]<24&&rel[2]<60){
            for(int i=0;i<bases.size();++i){
                for(int j=0;j<bases[i].VoosSaindo.size();++j){
                    if(vooSaiAgora(bases[i].VoosSaindo[j])){
                        alocarAviaoAleatorio(bases[i], bases[i].VoosSaindo[j]);

                        // atualizar individuo com o voo bases[i].VoosSaindo[j] e com o avião decolados.noAr.end();

                        //receber pouso;

                        //incrementar o relogio;
                    }
                }
            }

    }
}





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




















