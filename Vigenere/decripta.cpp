#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

typedef struct{
    int chr;
    float prob;
}tipoFreq;

void imprimeMsg(string txt, ios_base::openmode op, string msg){
    ofstream saida;

    saida.open("decripta.txt", op);
    saida << "*******" << txt << "*******" << endl;

    saida << msg << endl << endl;

    saida.close();
}

void imprimeMsgTab(string txt, ios_base::openmode op, vector< vector<int> > msgtab){
    ofstream saida;

    saida.open("decripta.txt", op);
    saida << "*******" << txt << "*******" << endl;
    for(vector< vector<int> >::iterator linha = msgtab.begin(); linha != msgtab.end(); linha++){
        for(vector<int>::iterator coluna = linha->begin(); coluna != linha->end(); coluna++){
            //saida << *coluna << '\t';
            if(*coluna != -1)
                saida << (char)*coluna;
        }
        //saida << endl;
    }
    saida << endl << endl;
    saida.close();
}

void separaInfo(vector<int>& msg, vector<tipoFreq>& freq){
    FILE *arq;

    arq = fopen ("c151.txt","r");

    if (arq){
        int m;
        float prob;
        string line;

        fseek (arq, 150, SEEK_SET);

        while ( (fscanf (arq, "%d", &m)) > 0){ //enquanto puder ler inteiro
            msg.push_back (m);
        }

        fseek (arq, 147, SEEK_CUR); //+ 147 caracteres desde o número lido
        do{
            fseek (arq, 9, SEEK_CUR);

            if(fscanf (arq, "%d)", &m) <= 0)
                break;

            fseek (arq, 17, SEEK_CUR);

            fscanf (arq, "%f)", &prob);
            freq.push_back({m, prob});

            fseek (arq, 33, SEEK_CUR);
        }while(1);
        fclose(arq);
    }
    else{
        cout << "Erro ao abrir o arquivo c151.txt. Encerrando..." << endl;
        exit(EXIT_FAILURE);
    }
}

int supoeTamanhoChave(vector<int> msg){
    int chave = 2;
    int maior = 0;

    for(int k = 6; k < 11; k++){
        int msgtam = msg.size();
        int igual = 0;

        for(int i = 0; i < msgtam; i++){
            if(msg[i] == msg[i + k])
                igual++;
        }

        if(igual > maior){
            maior = igual;
            chave = k;
        }
    }

    return chave;
}

void criaTab(vector<int> msg, vector< vector<int> >& msgtab, int chave){
    vector<int>::iterator it;
    vector<int> aux;

    it = msg.begin();
    while(it != msg.end()){
        aux.clear();

        for(int i = 0; i < chave; i++){
            if(it == msg.end())
                aux.push_back(-1);
            else{
                aux.push_back(*it);
                it++;
            }
        }
        msgtab.push_back(aux);

        if(it == msg.end())
            break;
    }
}

int achaChr(vector<tipoFreq>coluna, int palavra){

    int tam = coluna.size();
    for(int i = 0; i < tam; i++){
        if(coluna[i].chr == palavra)
            return i;
    }
    return -1;
}

int analisaFrequenciaColuna(vector< vector<int> > msgtab, int chave, vector<tipoFreq> freq, int shift){
    vector<tipoFreq>coluna;
    int index;
    int tam = msgtab.size();
    int maior = 0;

    for(int i = 0; i < tam; i++){
        index = achaChr(coluna, msgtab[i][chave]);
        if(index == -1){
            coluna.push_back({msgtab[i][chave], 1});
        }
        else{
            coluna[index].prob++;
        }
    }

    tam = coluna.size();
    index = 0;

    maior = coluna[0].prob;
    for(int i = 1; i < tam; i++){
        if(maior < coluna[i].prob){
            maior = coluna[i].prob;
            index = i;
        }
    }

    return coluna[index].chr ^ freq[shift].chr;

}

void cesar(vector< vector<int> >& msgtab, int pos, int k){
    int tam = msgtab.size();

    for(int i = 0; i < tam; i++){
        if(msgtab[i][pos] >= 0)
            msgtab[i][pos] ^= k;
        else
            msgtab[i][pos] = ' ';
    }
}

void vigenere(vector< vector<int> >& msgtab, vector<tipoFreq> freq, int chavetam, string& chave){
    int k;
    int shift;

    for(int i = 0; i < chavetam; i++){
        switch(i){
            case 0:
                shift = 1;
            break;
            case 2:
                shift = 1;
            case 3:
                shift = 1;
            break;
            default:
                shift = 0;
            break;
        }
        k = analisaFrequenciaColuna(msgtab, i, freq, shift);
        chave += (char)k;

        cesar(msgtab, i, k);
    }
}

int main(){
    vector<int> msg;
    vector<tipoFreq> freq;
    vector< vector<int> > msgtab;
    string chave;

    int chavetam;

    separaInfo(msg, freq);

    chavetam = supoeTamanhoChave(msg);

    criaTab(msg, msgtab, chavetam);
    imprimeMsgTab("Criptograma", ofstream::out , msgtab);

    vigenere(msgtab, freq, chavetam, chave);

    imprimeMsg("Chave", ofstream::out | ofstream::app, chave);

    imprimeMsgTab("Mensagem", ofstream::out | ofstream::app, msgtab);

    return 0;
}
