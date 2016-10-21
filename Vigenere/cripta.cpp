#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;


void imprimeMsg(string txt, ios_base::openmode op, string msg){
    ofstream saida;

    saida.open("cripta.txt", op);
    saida << "*******" << txt << "*******" << endl;

    saida << msg << endl << endl;

    saida.close();
}

void separaInfo(string& msg, string& chave){
    FILE *arq;

    arq = fopen ("decripta.txt","r");

    if (arq){
        char m;
        fseek (arq, 661, SEEK_SET); //criptograma + header

        while ( (m = getc(arq)) && m != '\n'){
            chave += m;
        }

        fseek (arq, 26, SEEK_CUR); //header
        while ( (m = getc(arq)) && m != EOF){
            msg += m;
        }
    }
}


void vigenere(string msg, string chave, string& cript){
    int tamchave = chave.size();
    int tammsg = msg.size();

    for(int i = 0; i < tammsg; i++){
        cript+= msg[i] ^ chave[i % tamchave];
    }
}

int main(){
    string msg, chave, cript;

    separaInfo(msg, chave);

    vigenere(msg, chave, cript);

    imprimeMsg("Mensagem", ofstream::out, msg);
    imprimeMsg("Chave", ofstream::out | ofstream::app, chave);
    imprimeMsg("Criptograma", ofstream::out | ofstream::app, cript);

    return 0;
}

