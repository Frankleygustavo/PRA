#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

//----Declaração deestruturas usadas no codigo---

struct aviao {
    string prefixo;
    string tipo;
};

struct voo {
    string numero;
    string origem;
    string destino;
    string tipoAviao;
    int saida[3];
    int chegada[3];
};

struct gene {
    aviao aeronave;
    vector<voo> agenda;
};

struct individuo {
    vector<gene> dna;
    float fitness;
};

struct aeroporto {
    string nome;
    vector <aviao> avioesEmSolo;
    vector <voo> voosApartir;
};

struct aviaoXvoo {
    aviao aeronave;
    voo emExecucao;
};
//-----------------------------------------------


//---------Declaração das Listas Usadas----------
vector <aviao> frota,frota_prov;
vector <voo> planos;
vector <aeroporto> bases;
vector <aviaoXvoo> voando;
vector <individuo> populacao;
//-----------------------------------------------



//-----------Declaração de Constantes-----------
const int numeroIndividuos = 100;
const int daLista = 50;// probabilidade de escolher um avião da lista de aeronaves ainda não alocadas
const float alfa = 1.0;// coeficiente de ajuste para numero voos Iuteis
const float beta = 1.0;// Coeficiente de ajuste para o desbalanceamento da divisão de voos
//-----------------------------------------------



//------------Declaração de Variaveis------------
int relogio[3]= {0,0,1};
int tamanhoDaFrota = 0;
int numPousos=0;
int numDecolagem=0;
//-----------------------------------------------


//-------------------CODIGOS---------------------
void atualizaRelogio() {//Usadopara avançar no tempo 1 min por vez
    if(relogio[2]<60) relogio[2]++;
    else if(relogio[1]<24) {
        relogio[1]++;
        relogio[2]=0;
    } else {
        relogio[0]++;
        relogio[1]=0;
        relogio[2]=0;
    }
}

void leFrota() {//Leitura da lista de aviões no arquivo
    int tamanho;
    aviao auxiliar;
    char ch4[4];
    string linha;
    ifstream av("AVIOES1.txt",ifstream::in);
    stringstream buffer;
    if(av.is_open()) {
        while (getline(av, linha)) {
            buffer.clear();
            buffer.str(linha);
            buffer>>auxiliar.prefixo;
            buffer>>auxiliar.tipo;
            frota.push_back(auxiliar);
            tamanhoDaFrota++;
        }
    }
}

string trataString(string a) {//Trata as strings
    string b = "";
    for (int i = 1; i < a.size(); i += 2) {
        b.push_back(a[i]);
    }
    return b;
}

void leVoos() {//Leitura da lista de voos

    string line,dias,saida, chegada;
    int auxiliar,hs,hc, cont=0;
    //string ag, cA,cV,p, c,o, d;
    ifstream myfile("testv14.txt");
    voo teste;

    stringstream buffer;
    if (myfile.is_open()) {
        line.clear();
        while (getline(myfile, line)) {

            buffer.clear();
            //imprimiString(line);
            line = trataString(line);
            //imprimiString(line);
            buffer << line;

            buffer >> dias;
            buffer >> teste.numero;
            buffer >> teste.tipoAviao;
            buffer >> teste.origem;
            buffer >> saida;
            buffer >> teste.destino;
            buffer >> chegada;

            auxiliar=0;
            for(int i=0; i<dias.size(); ++i)if(dias[i]>'0' && dias[i]<'8') {
                    auxiliar+=1<<i;
                }
            //auxiliar=atoi(dias.c_str());
            hs=atoi(saida.c_str());
            hc=atoi(chegada.c_str());
            for(int i=0; i<7; ++i) {
                if(auxiliar&(1<<i)) {
                    teste.saida[0]=i+1;
                    teste.saida[2]=hs%100;
                    teste.saida[1]=(hs-teste.saida[2])/100;

                    if(((hc%100)+teste.saida[2])>59) {
                        teste.chegada[2]=((hc%100)+teste.saida[2])-60;
                        teste.chegada[1]=1;
                    } else {
                        teste.chegada[2]=((hc%100)+teste.saida[2]);
                        teste.chegada[1]=0;
                    }
                    if(((hc-(hc%100))/100+teste.chegada[1]+teste.saida[1])>23) {
                        teste.chegada[1]=((hc-(hc%100))/100+teste.chegada[1]+teste.saida[1])-24;
                        teste.chegada[0]=1;
                    } else {
                        teste.chegada[1]=((hc-(hc%100))/100+teste.chegada[1]+teste.saida[1]);
                        teste.chegada[0]=0;
                    }
                    teste.chegada[0]+=teste.saida[0];

                    planos.push_back(teste);
                    cont++;
                }
                cont=0;
            }
        }
        myfile.close();
    } else cout << "Unable to open file";

}

void imprimeVoos() {//Imprime alista de voos
    int tam=planos.size();
    for(int i=0; i<tam; ++i) {
        cout<<planos[i].numero<<" "<<planos[i].origem<<" "<<planos[i].destino<<" "<<planos[i].saida[0]<<"D"<<planos[i].saida[1]<<"H"<<planos[i].saida[2]<<"M   "<<planos[i].chegada[0]<<"D"<<planos[i].chegada[1]<<"H"<<planos[i].chegada[2]<<"M"<<endl;
    }
}

void imprimeFrota() {//Implrime alista de aviões
    int tam=frota.size();
    cout<<"tamanho da frota "<<tamanhoDaFrota<<endl;
    for(int i=0; i<tam; ++i) {
        cout<<i<<" "<<frota[i].prefixo<<" "<<frota[i].tipo<<endl;
    }
}

bool ordenaOrigem(voo v1, voo v2) {//Usado para ordenar os voos pela origem(Usado para criar os aeroportos)
    if(v1.origem.compare(v2.origem)<=0) return true;
    return false;
}

bool ordenaSaida(voo v1, voo v2) {//Usado para ordenar decrecentemente a lista de voos a partir de cada aeroporto
    if(v1.saida[0]>v2.saida[0]) return true;
    else if(v1.saida[0]==v2.saida[0]&&v1.saida[1]>v2.saida[1])return true;
    else if(v1.saida[0]==v2.saida[0]&&v1.saida[1]==v2.saida[1]&&v1.saida[2]>v2.saida[2]) return true;
    return false;
}

void criaAeroportos() {//Cria os aeroportos

    string atual;
    aeroporto aux;

    cout<<"Iniciando"<<endl;
    sort(planos.begin(),planos.end(),ordenaOrigem);//ordena os planos pelo aerodromo de partida
    cout<<"Ordenando"<<endl;

    atual=planos[0].origem;
    aux.nome=planos[0].origem;

    for(int i=0; i<planos.size(); ++i) {
        if(!(atual.compare(planos[i].origem))) {
            cout<<"Comparação de strings "<<atual<<" "<<planos[i].origem<<endl;
            aux.voosApartir.push_back(planos[i]);
        } else {
            sort(aux.voosApartir.begin(),aux.voosApartir.end(),ordenaSaida);//ordena os voos que partem do aeroporto pela hora de saida(a ordem é decrescente para facilitar a retirada dos voos)
            bases.push_back(aux);
            atual=planos[i].origem;
            aux.nome=planos[i].origem;
            aux.voosApartir.clear();
            aux.voosApartir.push_back(planos[i]);
        }
    }
    // reprete-se para incluir o ultimo aeroporto
    sort(aux.voosApartir.begin(),aux.voosApartir.end(),ordenaSaida);//ordena os voos que partem do aeroporto pela hora de saida(a ordem é decrescente para facilitar a retirada dos voos)
    bases.push_back(aux);
}

bool ordenaAleatorio(aviao a1, aviao a2) {//Gera uma ordem aleatoria para possibilitar variabilidade nos aviões que seram escolhidos
    return rand() %2;
}

void mostraAeroportos() {
    for(int i=0; i<bases.size(); ++i) {
        cout<<bases[i].nome<<endl;
        for(int j=0; j<bases[i].voosApartir.size(); ++j) {
            cout<<bases[i].voosApartir[j].numero<<" "<<bases[i].voosApartir[j].tipoAviao<<" "<<bases[i].voosApartir[j].destino<<" "<<bases[i].voosApartir[j].saida[0]<<"D"<<bases[i].voosApartir[j].saida[1]<<"H"<<bases[i].voosApartir[j].saida[2]<<"M"<<endl;
        }
    }
}

bool ordenaChegada(aviaoXvoo va1, aviaoXvoo va2){
    if(va1.emExecucao.chegada[0]>va2.emExecucao.chegada[0]) return true;
    else if(va1.emExecucao.chegada[0]==va2.emExecucao.chegada[0] && va1.emExecucao.chegada[1]>va2.emExecucao.chegada[1])return true;
    else if(va1.emExecucao.chegada[0]==va2.emExecucao.chegada[0]&&va1.emExecucao.chegada[1]==va2.emExecucao.chegada[1]&&va1.emExecucao.chegada[2]>va2.emExecucao.chegada[2]) return true;
    return false;
}

int aleatorio(int a) {//Gera um numero aleatório entra 0 e 'a'
    return rand() % a;
}

void decolagem(individuo ind) {//Gera todasas decolagens que devem acontecer em um horário H

    cout<<"Numero de aviões no ar "<<voando.size()<<endl<<endl<<endl;
    if(voando.size()>0) {
            cout<<relogio[0]<<"D"<<relogio[1]<<":"<<relogio[2];
    }

    gene gen;
    aviaoXvoo va;

    for(int i=0; i<bases.size(); ++i) { //varre todos os aeroportos

        for(int j=bases[i].voosApartir.size()-1; j>=0; --j) { //varre todos os voos que sairam de um aeroporto i
            //Se há hora de saida do voo j for igual a do relógio
            if(bases[i].voosApartir[j].saida[0]==relogio[0] && bases[i].voosApartir[j].saida[1]==relogio[1] && bases[i].voosApartir[j].saida[2]==relogio[2]) {
                //deve haver uma decolagem incluindo a decolagem no indivíduo
                cout<<"vai decolar"<<endl;
                //Se houver algum avião não alocado
                if(frota_prov.size()>0) {
                cout<<"Aviões disponivel"<<endl;
                    //inclusão do voo no indivíduo
                    gen.aeronave=frota_prov[frota_prov.size()-1];//adiciona a aeronave no gene
                    gen.agenda.push_back(bases[i].voosApartir[j]);//adiciona o voo na lista dos que são executados pelo aviao
                    ind.dna.push_back(gen);//adiciona o gene ao indivíduo

                    //inclusão na lista de voos em exeução
                    va.aeronave = frota_prov[frota_prov.size()-1];//inclui a aeronave no vooEmExecução
                    va.emExecucao = bases[i].voosApartir[j];//inclui o voo no vooEmExecução
                    voando.push_back(va);//inclui o conjunto aviãoXvoo na lista

                    //Eliminar avião da lista de não atribuidos e o voo da lista de voos a serem executados
                    frota_prov.pop_back();//retira avião da lista de aviões não alocados
                    bases[i].voosApartir.pop_back();//retira voo da lista de voos a serem executados


                    //Se todos os aviões já tiverem sido alocados
                } else {
                    cout<<"Não há Avião Disponível"<<endl;
                    // Há duas opções ou a base que vai ter um voo decolando possui um avião do tipo em solo ou não

                    //Se há aviões em solo
                    if(bases[i].avioesEmSolo.size()>0) {
                        cout<<"Há aviões no aeroporto"<<endl;
                        //procura a aeronave no individuo
                        int k;
                        for(k=0; k<ind.dna.size(); ++k) {
                            if(ind.dna[k].aeronave.prefixo.compare(bases[i].avioesEmSolo[bases[i].avioesEmSolo.size()-1].prefixo))break;
                        }
                        cout<<"Há aviões no aeroporto1"<<endl;

                        //inclusão do voo no indivíduo
                        cout<<bases[i].voosApartir[j].numero<<" voo "<<endl;
                        gen.agenda.push_back(bases[i].voosApartir[j]);
                        cout<<"Há aviões no aeroporto2"<<endl;

                        //inclusão na lista de voos em exeução
                        va.aeronave = bases[i].avioesEmSolo[bases[i].avioesEmSolo.size()-1];//Atribui o avião ao vooEmExecução
                        va.emExecucao = bases[i].voosApartir[j];//Atribui o voo a vooEmExecução
                        voando.push_back(va);//inclui o conjunto aviãoXvoo na lista
                        cout<<"Há aviões no aeroporto3"<<endl;

                         //Eliminar avião da lista de aviões no solo e o voo da lista de voos a serem executados
                         bases[i].voosApartir.pop_back();//Deleta o voo da lista de voo a serem executados
                         bases[i].avioesEmSolo.pop_back();//Deleta o avião da lista de aviões no solo

                         //Não há aviões no solo tem que procurar em uma das outras bases um avião
                    } else {
                        cout<<"Não Há aviões no aeroporto"<<endl;
                        //Escolhe uma base aleatoria que tenha aviões em solo

                        int ale = aleatorio(bases.size());
                        while(bases[ale].avioesEmSolo.size() == 0){
                                int ale = aleatorio(bases.size());
                        }
                        cout<<"Não Há aviões no aeroporto1"<<endl;
                        //REALIZA VOO DE TRANSLADO

                        //Acha o avião que fará o translado no indivíduo
                        int k;

                        cout<<"Não Há aviões no aeroporto2"<<endl;
                        for(k=0; k<ind.dna.size(); ++k) {
                                cout<<"Não Há aviões no aeroporto3"<<endl;
                            if(ind.dna[k].aeronave.prefixo.compare(bases[ale].avioesEmSolo[bases[ale].avioesEmSolo.size()-1].prefixo))break;
                        }

                        //Adiciona voo de translado no indivíduo
                        voo aux;//Cria o voo inutil
                        aux.origem = bases[ale].nome;//Cria o voo inutil
                        aux.destino = bases[i].nome;//Cria o voo inutil
                        aux.numero = "INU0000";//Cria o voo inutil
                        aux.tipoAviao = bases[ale].avioesEmSolo[bases[ale].avioesEmSolo.size()-1].tipo;//Cria o voo inutil
                        for(int l=0;l<3;++i) aux.saida[i]=aux.chegada[i]=relogio[i];//Cria o voo inutil
                        gen.agenda.push_back(aux);//Adiciona o voo inutil no individuo
                        cout<<"Não Há aviões no aeroporto4"<<endl;
                        //Trocar o avião de base
                        bases[i].avioesEmSolo.push_back(bases[ale].avioesEmSolo[bases[ale].avioesEmSolo.size()-1]);//Adiciona o avião na nova base
                        bases[ale].avioesEmSolo.pop_back();//Retira o avião da base anterior

                        cout<<"Não Há aviões no aeroporto5"<<endl;
                        //Gerar voo usando o metodo ja apresentado no IF
                        gen.agenda.push_back(bases[i].voosApartir[j]);
                        cout<<"Não Há aviões no aeroporto6"<<endl;
                        //inclusão na lista de voos em exeução
                        va.aeronave = bases[i].avioesEmSolo[bases[i].avioesEmSolo.size()-1];//Atribui o avião ao vooEmExecução
                        va.emExecucao = bases[i].voosApartir[j];//Atribui o voo a vooEmExecução
                        voando.push_back(va);//inclui o conjunto aviãoXvoo na lista

                        cout<<"Não Há aviões no aeroporto7"<<endl;
                         //Eliminar avião da lista de aviões no solo e o voo da lista de voos a serem executados
                         bases[i].voosApartir.pop_back();//Deleta o voo da lista de voo a serem executados
                         bases[i].avioesEmSolo.pop_back();//Deleta o avião da lista de aviões no solo

                    }
                }

            } else {
                break;
            }
        }
    }
    sort(voando.begin(),voando.end(),ordenaChegada);
}

int index(string icao) {//Retorna o indice do aeroporto de nome ICAO
    for(int i=0; i<bases.size(); ++i) {
        if(bases[i].nome.compare(icao)) {
            return i;
        }
    }
}

void pouso(){//Executa todos os pousos que devemacontecer em um dado momento de tempo

    for(int i=voando.size()-1;i>=0;--i){
        //verifica se o voo está na hora de pousar
        if(voando[i].emExecucao.chegada[0] == relogio[0] && voando[i].emExecucao.chegada[1] == relogio[1] && voando[i].emExecucao.chegada[2] == relogio[2]){

            cout<< "ouve pouso"<<endl;

            int aux=index(voando[i].emExecucao.destino);//encontra na lista de aeroporto o destino do voa que deve pousar
            bases[aux].avioesEmSolo.push_back(voando[i].aeronave);//insere o avião chegando em um lugar aleatorio na lista de aviões no solo
            //bases[aux].avioesEmSolo.push_back(voando[i].aeronave);//Adiciona o avião na lista de aviões no solo

             cout<< "ouve pouso1"<<endl;

            //caso haja mais de 1 avião no solo, ordena alista de aviões no solo aleatoriamente
            if(bases[aux].avioesEmSolo.size()>1){cout<< "ouve pouso2"<<endl;
            //cout<<"numero de avioes no solo"<<bases[aux].avioesEmSolo.size()<<endl;
            sort(bases[aux].avioesEmSolo.begin(),bases[aux].avioesEmSolo.end(),ordenaAleatorio);}


            voando.pop_back();//retira o voo que pouso da lista de voosEmExecução

             cout<< "ouve pouso2"<<endl;

        }else{
            break;
        }
    }
}

float calculaFitness(individuo ind) {//Calcula o valor do fitness de um indivíduo
    float fitness;
    int numVoosInuteis=0,maxVoos,minVoos;
    maxVoos=minVoos=ind.dna[0].agenda.size();
    for(int i=0; i<ind.dna.size(); ++i) {
        for(int j=0; j<ind.dna[i].agenda.size(); ++j) {
            if(ind.dna[i].agenda[j].numero.compare("INU0001")==0) {
                numVoosInuteis++;
            }
        }
        if(ind.dna.size()>maxVoos)maxVoos=ind.dna.size();
        else if(ind.dna.size()<minVoos) minVoos=ind.dna.size();
    }
    fitness=alfa*(float)numVoosInuteis+beta*(float)(maxVoos-minVoos);
    return fitness;
}

void mostraIndividuo(individuo ind) {//Mostra um indivíduo
    for(int i=0; i<ind.dna.size(); ++i) {
        for(int j=0; j<ind.dna[i].agenda.size(); ++j) {
            cout<<ind.dna[i].agenda[j].numero<<" "<<ind.dna[i].agenda[j].origem<<" "<<ind.dna[i].agenda[j].destino<<" "<<ind.dna[i].agenda[j].saida[0]<<"D"<<ind.dna[i].agenda[j].saida[1]<<"H"<<ind.dna[i].agenda[j].saida[2]<<"M "<<ind.dna[i].agenda[j].chegada[0]<<"D"<<ind.dna[i].agenda[j].chegada[1]<<"H"<<ind.dna[i].agenda[j].chegada[2]<<"M"<<endl<<endl;
        }
    }
}

void populacaoInicial() {//Gera a população Inicial
    vector<aviao> equipamentos;
    individuo ind;
    // for(int i=0; i<numeroIndividuos; ++i) {
    relogio[0]=relogio[1]=relogio[2]=0;
    frota_prov=frota;
    sort(equipamentos.begin(),equipamentos.end(),ordenaAleatorio);
    while(relogio[0]<9) {
        atualizaRelogio();
        decolagem(ind);
        if(voando.size()>0) {
            pouso();
        }
    }
    mostraIndividuo(ind);
    ind.fitness=calculaFitness(ind);
    populacao.push_back(ind);

}

int main() {
    srand (time(NULL));
    leFrota();
    imprimeFrota();
    leVoos();
    imprimeVoos();
    cout<<"voos"<<endl;
    criaAeroportos();
    cout<<"C_Aeroporto"<<endl;
    mostraAeroportos();
    cout<<"C_Pop"<<endl;
    populacaoInicial();
    cout<<"PopInicial"<<endl;
    system("pause");
    return 0;
}
