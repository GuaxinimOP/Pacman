#include <iostream>
#include <stack> // Criar mapa
#include <stdlib.h>
#include <time.h> // Timer
#include <conio.h> // getch
#include <windows.h> // Modificar Cor
#include <fstream> // Salvar mapa
#include "player.h" // Struct do jogador

using namespace std;

char map[20][20];
player jogador;
bool jogando, criado;
int contx;
FILE *mapa;

void clearScreen(){
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}

void modificarCor(unsigned short cor){
    /* Muda a cor da letra com base nessa tabela:

    preto=0,
	azul escuro=1,
	Verde escuro=2,
	Ciano Escuro?=3,
	Vermelho Escuro=4,
	Roxo, Rosa ou Magenta Escuro=5,
	Amarelo Escuro=6,
	Branco Escuro?=7,
	Cinza=8,
	Azul=9,
	Verde=10,
	Ciano=11,
	Vermelho=12,
	Roxo, Rosa ou Magenta=13,
	Amarelo=14,
	Branco=15

    */
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hcon,cor);
}

void gerarMapa(int moedas, int obstaculos){
    int x, y;

    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            map[i][j] = '.';
        }
    }

    // Define a seed p/ gerar os numeros "aleatorios"
    srand(time(0));

    // Gera duas coordenadas aleatorias dentro do vetor p/ identificar o ponto de partida do jogador
    x = (rand() % 19) + 0;
    y = (rand() % 19) + 0;

    map[x][y] = 'J';
    jogador.x = x;
    jogador.y = y;

    // Gera 10 moedas em coordenadas aleatorias do mapa que não sejam ocupadas pelo jogador (Objetivo)
    for(int o = 0; o < moedas; o++){
        // z = x e z2 = y
        int z = (rand() % 19) + 0;
        int z2 = (rand() % 19) + 0;

            if(map[z][z2] != 'J'){
                map[z][z2] = '$';
            }
            else{
                o-=1;
                z = (rand() % 19) + 0;
                z2 = (rand() % 19) + 0;
            }
    }

    // Gera 20 obstaculos em coordenadas aleatorias do mapa que não sejam ocupadas por moedas ou pelo jogador (Paredes)
    for(int p = 0; p < obstaculos; p++){
        int ax = (rand() % 19) + 0;
        int ay = (rand() % 19) + 0;

        if(map[ax][ay] != 'J' && map[ax][ay] != '$'){
            map[ax][ay] = char(219);
            }
        else{
            p-=1;
                ax = (rand() % 19) + 0;
                ay = (rand() % 19) + 0;
        }
    }
}

void gerarGrade(){
    // Gera uma grade 20x20 cheia de "." para a criação de mapas

    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            map[i][j] = '.';
        }
    }

    map[0][0] = 'J';
    jogador.x = 0, jogador.y = 0;
}

void mostrarMapa(){
    // Mostra todas as coordenadas da matriz
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            if(map[i][j] == 'J'){ 
                modificarCor(6); // Muda a cor do jogador pra Laranja
            }
            else if(map[i][j] == '.'){
                modificarCor(8); // Muda a cor do mapa pra cinza
            }
            else if(map[i][j] == '$'){
                modificarCor(15); // Muda a cor das moedas pra branco
            }
            else{
                modificarCor(1); // Muda a cor das paredes pra azul escuro
            }
            if(i <= 19){
                if(j != 19){
                    cout << map[i][j] << " ";
                }
                else{
                    cout << map[i][j] << " " << endl;
                }
            }
        }
    }
}

void cima(){
    int i = jogador.x, j = jogador.y;
    if(map[i][j] == 'J'){
        if(i == 0){ // Caso o jogador esteja na primeira linha da matriz irá parar na ultima linha
            map[i][j] = '.';
            map[i=19][j] = 'J';
            jogador.x = 19;
        }
        else{ // Caso contrario irá uma linha pra cima
            map[i][j] = '.';
            map[i-=1][j] = 'J';
            jogador.x -= 1;
        }
    }
}

void baixo(){
    int i = jogador.x, j = jogador.y;
    if(map[i][j] == 'J'){
        if(i == 19){ // Caso o jogador esteja na primeira linha da matriz irá parar na ultima linha
            map[i][j] = '.';
            map[i=0][j] = 'J';
            jogador.x = 0;
        }
        else{ // Caso contrario irá uma linha pra cima
            map[i][j] = '.';
            map[i+=1][j] = 'J';
            jogador.x += 1;
        }
    }
}

void esquerda(){
    int i = jogador.x, j = jogador.y;
    if(map[i][j] == 'J'){
        if(j == 0){ // Caso o jogador esteja na primeira coluna da matriz ele irá parar na ultima coluna
            map[i][j] = '.'; 
            map[i][j=19] = 'J';
            jogador.y = 19;
        }
        else{ // Caso contrario irá uma coluna pra esquerda
            map[i][j] = '.';
            map[i][j-=1] = 'J';
            jogador.y -= 1;
        }
    }
}

void direita(){
    int i = jogador.x, j = jogador.y;
    if(map[i][j] == 'J'){
        if(j == 19){ // Caso o jogador esteja na primeira coluna da matriz ele irá parar na ultima coluna
            map[i][j] = '.'; 
            map[i][j=0] = 'J';
            jogador.y = 0;
        }
        else{ // Caso contrario irá uma coluna pra esquerda
            map[i][j] = '.';
            map[i][j+=1] = 'J';
            jogador.y += 1;
        }
    }
}

void movimentacao(char mov){
    int i = jogador.x, j = jogador.y;
    /* Caso o jogador passe pela borda do mapa ele sera redirecionado para o outro lado do mapa
    no mesmo eixo em que estava se movimentando (da esquerda para a direita e de cima para baixo)
    */
    switch(mov){
        case 'w': case 'W': case 72:
            if(map[i][j] == 'J'){ // Encontra a posição do jogador
                if(i != 0 && map[i-=1][j] != char(219)){ // Se a posição acima do jogador estiver livre 
                    if(map[i][j] == '$'){
                        cima();
                        contx += 1; // Aumenta o contador de moedas coletadas
                    }
                    else{
                        cima();
                    }
                }
                else if(i == 0 && map[i=19][j] != char(219)){ // Se a ultima linha da matriz estiver livre
                   if(map[i][j] == '$'){
                        cima();
                        contx += 1;
                    }
                    else{
                        cima();
                    }
                }
                else if(i == 0 && map[19][j] == char(219)){
                    break;
                }
            }
            break;
        case 'a': case 'A': case 75:
            if(map[i][j] == 'J'){
                if(j != 0 && map[i][j-=1] != char(219)){ // Se a posição a esquerda do jogador estiver livre
                    if(map[i][j] == '$'){
                        esquerda();
                        contx += 1;
                    }
                    else{
                        esquerda();
                    }
                }
                else if(j == 0 && map[i][j=19] != char(219)){ // Se a ultima coluna da matriz estiver livre
                    if(map[i][j] == '$'){
                        esquerda();
                        contx += 1;
                    }
                    else{
                        esquerda();
                    }
                }
                else{
                    break;
                }
            }
            break;
        case 's': case 'S': case 80:
            if(map[i][j] == 'J'){
                if(i != 19 && map[i+=1][j] != char(219)){ // Se a posição abaixo do jogador estiver livre
                    if(map[i][j] == '$'){
                        baixo();
                        contx += 1;
                    }
                    else{
                        baixo();
                    }
                }
                else if(i == 19 && map[i=0][j] != char(219)){ // Se a primeira linha da matriz estiver livre
                    if(map[i][j] == '$'){
                        baixo();
                        contx += 1;
                    }
                    else{
                        baixo();
                    }
                }
                else{
                    break;
                }
            }
            break;
        case 'd': case 'D': case 77:
            if(map[i][j] == 'J'){
                if(j != 19 && map[i][j+=1] != char(219)){ // Se a posição a direita do jogador estiver livre
                    if(map[i][j] == '$'){
                        direita();
                        contx += 1;
                    }
                    else{
                        direita();
                    }
                }
                else if(j == 19 && map[i][j=0] != char(219)){ // Se a primeira coluna da matriz estiver livre
                    if(map[i][j] == '$'){
                        direita();
                        contx += 1;
                    }
                    else{
                        direita();
                    }
                }
                else{
                    break;
                }
            }
            break;
    }
}

bool verificarMapa(int moedas){
    // Conta quantas moedas faltam pra coletar
    if(contx == moedas){
        return true;
    }
    else{
        return false;
    }
}

void carregarMapa(){
    ifstream mapa; // Ler arquivo de texto
    mapa.open("mapa.txt"); // Abre o arquivo
    char linha[20];
    int i = 0;
    while(mapa.getline(linha,20)){ // Copia os caracteres do arquivo .txt para a matriz map[][]
        strcpy(map[i],linha); 
        i++;
    }
    mapa.close(); // Fecha o arquivo
    system("cls");
    jogando = true; // Inicia o jogo
    criado = false; // Reseta a criação do mapa
}

void criarMapa(int moedas, int obstaculos){
    /* A criação de mapa nem eu entendi, mas basicamente eu criei uma pilha e coloquei todos os objetos dentro dela
    com o push antes de começar a criação.
    
    depois de colocar tudo na pilha, ele entra em um loop pro jogador se mover e colocar as paredes e moedas onde
    quiser, se ele apertar ESC o loop vai encerrar e vai voltar pro menu inicial do jogo.

    Caso ele termine de colocar todos os objetos o loop vai encerrar e vai esperar ele apertar alguma tecla pra
    começar a partida no mapa que ele criou
    */
    stack<char> p;
    stack<int> acaoX, acaoY; // Pilha pra armazenar as posições dos objetos
    bool criando = true; // Vai determinar se o jogo está pronto pra começar
    char movc; // Movimentação do jogador
    int i = jogador.x, j = jogador.y;
    int contM=0, contO=0; // Contador de quantos objetos restam pra colocar
    int axc, ayc; // Variaveis pra identificar a posição do ultimo objeto colocado no mapa
    char op;

    // Coloca a quantidade de moedas e obstaculos na pilha antes de iniciar a criação do mapa
    for(int i = 0; i < moedas; i++){
        p.push('$');
        contM += 1;
    }
    for(int i = 0; i < obstaculos; i++){
        p.push(char(219));
        contO += 1;
    }

    // Começa a criação do mapa
    while(true){ 
        mostrarMapa();
        cout << "[Enter] - Coloca objeto\n[CTRL+Z] - Desfaz acao" << endl;
        cout << "Obstaculos restantes: "<< contO << "\nMoedas Restantes: " << contM << endl;
        char movc = getch();
        if(movc == 13){
            if(map[i][j] == 'J' && (i != 0 && i != 19) && (j != 0 && j != 19)){ // 
                map[i][j] = p.top();
                axc = i; // Armazena a posição do objeto na variavel
                ayc = j; // ''
                acaoX.push(axc); // Armazena a posição do objeto na pilha
                acaoY.push(ayc); // ''
                if(p.top() == char(219)){
                    contO -= 1;
                }
                else if(p.top() == '$'){
                    contM -= 1;
                }
                p.pop();
                map[0][0] = 'J';
                jogador.x = 0, jogador.y = 0;
                i = jogador.x, j = jogador.y;
            }
        }
        else if(movc == 26 && (!acaoX.empty() || !acaoY.empty())){ // Se o jogador pressionar a ctrl+z    
            if(map[acaoX.top()][acaoY.top()] == char(219)){ // Caso o ultimo objeto colocado seja uma parede
                map[acaoX.top()][acaoY.top()] = '.'; // Ele voltará a ser um espaço "vazio"
                acaoX.pop(); // Remove a posição do objeto deletado da pilha
                acaoY.pop(); // ''

                p.push(char(219)); // O Objeto voltará para a pilha
                contO += 1; // O Contador de objetos restantes aumenta
            }
            else if(map[acaoX.top()][acaoY.top()] == '$'){ // Mesma coisa só que com as moedas
                    map[acaoX.top()][acaoY.top()] = '.';
                    acaoX.pop(); // Remove a posição do objeto deletado da pilha
                    acaoY.pop(); // ''
                p.push('$');
                contM += 1;
            }
        }
        else if(movc == 27){ // Se o jogador pressionar a tecla ESC o jogo volta pro menu inicial
            break;
        }
        else{
            i = jogador.x, j = jogador.y;
            switch(movc){
                case 'w': case 'W': case 72:
                    if(i != 0 && map[i-=1][j] != '$'){
                        movimentacao(movc);
                    }
                    else if(i == 0 && map[i=19][j] != '$'){
                        movimentacao(movc);
                    }
                    else{
                        break;
                    }
                    break;
                case 'a': case 'A': case 75:
                    if(j != 0 && map[i][j-=1] != '$'){
                            movimentacao(movc);
                    }
                    else if(j == 0 && map[i][j=19] != '$'){
                        movimentacao(movc);
                    }
                    else{
                        break;
                    }
                    break;
                case 's': case 'S': case 80:
                    if(i != 19 && map[i+=1][j] != '$'){
                        movimentacao(movc);
                    }
                    else if(i == 19 && map[i=0][j] != '$'){
                        movimentacao(movc);
                    }
                    else{
                        break;
                    }
                    break;
                case 'd': case 'D': case 77:
                    if(j != 19 && map[i][j+=1] != '$'){
                        movimentacao(movc);
                    }
                    else if(j == 19 && map[i][j=0] != '$'){
                        movimentacao(movc);
                    }
                    else{
                        break;
                    }
                    break;
            }
        }
        clearScreen();
        if(p.empty()){ // Quando a pilha estiver vazia a criação de mapa vai parar e encerrar o loop
            criando = false;
            break;
        }
    }
    if(criando == false){ // Quando a criação de mapa acabar o jogador voltará a posição (0,0) e o jogo vai começar
        for(int i = 0; i < 20; i++){
            for(int j = 0; j < 20; j++){
                if(map[i][j] == 'J'){
                    map[i][j] = '.';
                    map[0][0] = 'J';
                    jogador.x = 0, jogador.y = 0;
                    i = jogador.x, j = jogador.y;
                }
            }
        }
        system("cls");
        cout << "Deseja salvar o mapa? Y/N" << endl;
        cin >> op;
        if(op == 'Y'){
            ofstream mapa;         // Modificar arquivo de texto
            mapa.open("mapa.txt"); // Abre o mapa
            for(int i = 0; i < 20; i++){  
                for(int j = 0; j < 20; j++){ // Escreve o mapa no arquivo .txt
                    if(j != 19){
                        mapa << map[i][j]; 
                    }
                    else{
                        mapa << map[i][j] << endl;
                    }
                }
            }
            mapa.close(); // Fecha o arquivo
            cout << "Mapa salvo em mapa.txt" << endl;
            criado = true;
            system("pause");             
        }
        system("cls");
        if(criado == true){
            carregarMapa();
        }
    }
}

int main(){
    // system("mode con:cols=66 lines=25"); // Define o tamanho do console
    int jogo = 0, fim = 0, valor = 0, flag = 0; 
    int op;
    /* 
    jogo = Loop principal 
    fim = determina o fim da partida
    valor = Armazena temporariamente a quantidade de obstaculos e Moedas nas configurações 
    */
    int moedas=10, obstaculos=10; // Valores padrão de moedas e obstaculos

    while(jogo == 0){
        int pontuacao = 0; // Reseta a pontuação
        system("cls");
        modificarCor(15); // Define a cor dessa frase VVVV pra branco
        cout << "\n                       Feito por: GuaxinimOP\n\n" << endl;

        modificarCor(6); // Define a cor do menu pra Amarelo Escuro
        cout << "                            PACMAN              " << endl;
        cout << "                                                " << endl;
        cout << "                      [1] - Iniciar o jogo      " << endl;
        cout << "                      [2] - Criar Mapa          " << endl;
        cout << "                      [3] - Tutorial            " << endl;
        cout << "                      [4] - Configuracoes       " << endl;
        cout << "                      [5] - Sair                " << endl;
        cout << "\n\n                      Insira a opcao: ";
        cin >> op;

        // Define o timer do jogo
        clock_t timer;

        // Switch do menu inicial
        switch(op){
            case 1:
                system("cls");
                gerarMapa(moedas, obstaculos);
                jogando = true; 
                fim = 0;
                contx = 0;
                flag = 0;
                break;
            case 2:
                system("cls");
                gerarGrade();
                jogando = false;
                fim = 0;
                flag = 0;
                criarMapa(moedas, obstaculos);
                break;   
            case 3:
                system("cls");
                cout << "                      Tutorial Basico\n" << endl;
                cout << "\nIdentificacao dos icones: \n[J] - Jogador\n[$] - Moedas\n[/] - Obstaculos\n" << endl;
                cout << "Para vencer o jogo basta coletar todas as moedas ($) no mapa" << endl;
                cout << "A movimentacao e feita atraves das teclas w,a,s,d ou as setas" << endl;

                cout << "\n\nTutorial da Criacao de Mapa\n" << endl;
                cout << "\nO jogador pode colocar os objetos na parte interna do mapa" << endl;
                cout << "A ordem de distribuicao dos objetos e: Obstaculos > Moedas\n" << endl;
                system("pause");
                continue;
            case 4:
                system("cls");
                cout << "Configurar a quantidade de moedas e obstaculos no mapa\n" << endl;
                cout << "Insira a quantidade de moedas no mapa (Minimo 10 e Maximo 50) \n[$] - Moedas: ";
                cin >> valor;
                if(valor >= 1 && valor <= 50){ // Checa o valor minimo e maximo de moedas
                    moedas = valor;
                }
                else{
                    cout << "\nValor invalido" << endl;
                    system("pause");
                    continue;
                }
                system("cls");
                cout << "Insira a quantidade de obstaculos no mapa (Min 10 e Max 40) \n[/] - Obstaculos: ";
                cin >> valor;
                if(valor >= 1 && valor <= 40){ // Checa o valor minimo e maximo de obstaculos
                    obstaculos = valor;
                    cout << "Configuracao realizada" << endl;
                    system("pause");
                    continue;
                }
                else{
                    cout << "\nValor invalido" << endl;
                    system("pause");
                    continue;
                }
            case 5:
                exit(0); // Sai do jogo
        }

        // Loop para movimentação do jogador
        while(jogando == true){
            char mov;
            if(flag == 0){
                timer =  clock();
                contx = 0;
                flag = 1;
            }
            mostrarMapa();
            
            cout << "Moedas coletadas: ";
            mov = getch();
            if(mov == 27){
                fim = 0;
                break;
            }
            movimentacao(mov);
            verificarMapa(moedas);
            cout << contx << endl;

            if(verificarMapa(moedas)){
                fim = 1;
                modificarCor(6);
                system("cls");
                break;
            }
            clearScreen();
        }

        if(fim == 1){
            timer = clock() - timer;
            // Define a pontuação do jogador com base no tempo e quantidade de moedas coletadas
            pontuacao += (moedas * 50) + 10000 / ((float)timer/CLOCKS_PER_SEC); 
            cout << "Parabens, voce coletou todas as moedas" << endl;
            cout << "Pontuacao final: " << pontuacao << endl;
            cout << "Tempo total: " << ((float)timer)/CLOCKS_PER_SEC << " Segundos" << endl; // Mostra o tempo em segundos
            cout << "Deseja jogar novamente?\n[1] Sim\n[2] Nao" << endl; // Menu p/ continuar jogando
            cin >> op;
            if(op == 1){
                continue;
                fim = 0;
            }
            else if(op == 2){
                exit(0);
            }
        }
    }
}