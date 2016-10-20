{§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§
§                       Universidade de Brasília                          §
§                Departamento de Ciência da Computação                    §
§          Segurança de Dados - * - Prof. Pedro A. D. Rezende             §
§                                                                         §
§  Têrmo de isenção: Este programa têm objetivos educacionais. Qualquer   §
§                    utilização do mesmo em ou para outros fins, é de     §
§                    responsabilidade apenas e somente do usuário.        §
§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§
**************************** cripto1.pas **********************************
*   Este programa lê um arquivo contendo texto a ser cifrado, e uma cadeia*
*   de caracteres ASCII do teclado, que funciona como chave de cifração.  *
*   Um byte do criptograma é gerado fazendo-se o XOR do byte seguinte da  *
*   chave com o byte seguinte do arquivo, usando o valor ordinal do código*
*   ASCII de cada caracter e reciclando os bytes da chave até que o final *
*   do arquivo de entrada seja encontrado (cifra polialfabética).         *
*   O programa gera um arquivo texto contendo a representação decimal do  *
*   resultado de cada XOR, separados por NEWLINE.                         *
*   Este arquivo começa com um cabeçalho informando o nome do arquivo de  *
*   onde foi lido o texto pleno, e termina com uma estatística de         *
*   freqüência de caracteres deste texto, onde os caracteres LINEFEED (10)*
*   ou NEWLINE (13) foram ignorados.                                      *
***************************************************************************}


program vigenere_xor;
uses wincrt;

type simbolo = record         {símbolo ASCII: }
             oc: word;        {numero de Ocorrencias no texto e no criptograma }
             ord: byte;       {do caracter cujo valor ASCII = ORD. }
     end;
     stat = array [0..255] of simbolo;
var entra: file of byte;  sai: text;
    arq_entra, arq_sai, key: string;
    a: byte;
    cont: longint;
    b: array [0..7] of byte;
    freqt, freqc: stat;       {estatísticas de ocorrencias de símbolos ASCII}

{-------------------------------------------------------------------------}

procedure inicializa;
    var i: integer;
    procedure init( var freq:stat );
    begin
          for a:= 0 to 255 do begin
              freq[a].oc:= 0;
              freq[a].ord:= a
          end;
    end;
    begin
         writeln( 'Este programa encripta arquivos texto usando Vigenere (XOR)' );
         writeln;
         writeln( 'Digite o nome do arquivo de entrada' );
         readln( arq_entra );
         writeln( 'Digite o nome do arquivo de saida ');
         readln( arq_sai );
         writeln( 'Digite a chave' );
         readln( key );
         writeln('valores ASCII da chave: ');
         for i:= 1 to length(key) do write( ord(key[i]), ' ');
         writeln;
         if (length(key)>0) and (length(arq_entra)>0) and (length(arq_sai)>0)
            then begin
                 assign( entra, arq_entra ); reset( entra );
                 assign(  sai, arq_sai ); rewrite( sai );
                 init( freqc );
                 init( freqt );
                 writeln( sai, 'Criptograma gerado por "Cripto_1.exe" ',
                               'a partir do arquivo "',arq_entra, '"' );
                 for i:=1 to 80 do write( sai, '-' );
                 writeln( sai );
            end
            else begin
                 write( 'Chave ou arquivo inválido' ); halt
            end;
    end;

{------------------------------------------------------------------------}


procedure calcula_freq;
    var i, br: byte;
    procedure ordena( var freq:stat );
        var i,j: byte; aux: simbolo;
    begin
         for i:= 254 downto 0 do
             for j:= i downto 0 do
                 if freq[j].oc<freq[j+1].oc then
                 begin
                    aux:=freq[j];  freq[j]:=freq[j+1];  freq[j+1]:=aux
                 end;
    end;
    begin
         br:= freqt[32].oc;
         writeln( sai,#13,'Distribuição de ocorrencias dos ',
                  cont,' caracteres do texto e criptograma:');
         writeln( sai, '                   TEXTO              ||',
                       '      CRIPTOGRAMA              ');
         ordena( freqt );
         ordena( freqc );
         i:= 0;
         while (freqt[i].oc>0) and (freqc[i].oc>0) do begin
               write( sai,i+1:3,'º- chr(' );
               write( sai,freqt[i].ord:3,') ',#39,chr(freqt[i].ord),#39,
               ': ',freqt[i].oc:3,' (prob=',freqt[i].oc/cont:1:4,')');
               write( sai,' || ');
               write( sai,'chr(',freqc[i].ord:3,') ',
               freqc[i].oc:3,' (prob=',freqc[i].oc/cont:1:4,')');
               writeln( sai );
               i:= i+1;
         end;
         while (freqc[i].oc>0) do begin
               write( sai,i:3,'º-       ' );
               write( sai,'                         ' );
               ; write( sai,' || ');
               write( sai,'chr(',freqc[i].ord:3,') ',
               freqc[i].oc:3,' (prob=',freqc[i].oc/cont:1:4,')');
               writeln( sai );
               i:= i+1;
         end;
     writeln( sai,'Tamanho médio de palavras do texto = ',cont/br-1:1:3,';',
                  #13,'Total de bytes do criptograma:',#13,cont );
    end;

{================================ programa =================================}

begin
   while true do begin
     inicializa; cont:= 0;
     while not eof( entra ) do begin
           read( entra,a );
           if (a<>10) and (a<>13) then begin
              cont:= cont+1;
              freqt[a].oc:= freqt[a].oc+1;
              a:= a xor ord(key[cont mod length(key)+1]);
              freqc[a].oc:= freqc[a].oc+1;
              writeln( sai, a );
           end;
     end;
     calcula_freq;
     close( entra ); close( sai );
     writeln( 'bytes cifrados: ',cont );
     writeln( 'digite espaço para encerrar o programa, qualquer tecla para continuar' );
     if readkey = ' '
        then begin
             donewincrt;
             break
        end;
     end;
end.