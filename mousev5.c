/* Tenho que fazer um programa em C que explora um labirinto desconhecido ate encontrar o objetivo. Depois de encontrar o objetivo,
  volta para a origem e da origem faco o menor caminho para o objetivo. Ao explorar um labirinto, vou construindo um grafo em que cada
  vertice eh um lugar do labirinto que eu cheguei. Depois de achar o objetivo, uso o algoritmo Dijkstra para achar o menor caminho da 
  origem ate o objetivo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


#define TAM_INICIAL 65
#define MAX_VERTICE (TAM_INICIAL*TAM_INICIAL)
#define MAX_MOV (MAX_VERTICE*2)

int controlePos = 444;

typedef struct no {
    int verticeIdx; 
    struct no* prox;
} No;

typedef struct {
    No* inicio;
    int dir;
    int x;
    int y;
    int parede[4]; //parede[0]: se tem ou n parede na direcao 0
} Lista;

typedef struct {
    int numVertices;
    Lista* listaAdjacencia[MAX_VERTICE];
    char mov[MAX_MOV];
} Grafo;

int andarFrente() {
    int retorno;
    printf("m\n");
    fflush(stdout);
    scanf("%d", &retorno);
    fflush(stdin);
    return retorno;
}
void andarEsquerda() {
    int retorno;
    printf("l\n");
    fflush(stdout);
    scanf("%d", &retorno);
    fflush(stdin);
    controlePos--;
}
void andarDireita() {
    int retorno;
    printf("r\n");
    fflush(stdout);
    scanf("%d", &retorno);
    fflush(stdin);
    controlePos++;
}
int andarCorrendo() {
    int retorno;
    printf("M\n");
    fflush(stdout);
    scanf("%d", &retorno);
    fflush(stdin);
    return retorno;
}
int andarMeiaVolta() {
    int retorno;
    andarDireita();
    andarDireita();
    retorno = andarFrente();
    return retorno;
}

//-------------------------------GRAFO--------------------------------------------
Lista* criarLista() {
    Lista* l = malloc(sizeof(Lista));
    l->inicio = NULL;
    l->dir = -1;
    l->x = INT_MAX;
    l->y = INT_MAX;
    l->parede[0] = 0;
    l->parede[1] = 0;
    l->parede[2] = 0;
    l->parede[3] = 0;
    return l;
}

void inicializarGrafo (Grafo* grafo) {
    for(int i = 0; i < MAX_VERTICE; i++) {
        grafo->listaAdjacencia[i] = criarLista();
    }
    grafo->numVertices = 1;
    strcpy(grafo->mov, "");
}

void adicionarAresta(Grafo* grafo, int vertice1, int vertice2) {
    No* novo = (No*) malloc(sizeof(No));
    novo->verticeIdx = vertice2;
    novo->prox = grafo->listaAdjacencia[vertice1]->inicio;
    grafo->listaAdjacencia[vertice1]->inicio = novo;
    
    No* novo2 = (No*) malloc(sizeof(No));
    novo2->verticeIdx = vertice1;
    novo2->prox = grafo->listaAdjacencia[vertice2]->inicio;
    grafo->listaAdjacencia[vertice2]->inicio = novo2;
}

int acharMenorDist(int dist[], int visitado[], int numVertices) {
    int min = INT_MAX, minIndex;

    for (int v = 0; v < numVertices; v++) {
        if (visitado[v] == 0 && dist[v] <= min) {
            min = dist[v];
            minIndex = v;
        }
    }

    return minIndex;
}

//--------------------------------------------------------------------------------------------------------------------------------

void registraMov(Grafo *grafo, int proximo[], int obj) { //concatena o melhor caminho do inicio (int obj) ate o obj
    if (proximo[obj] == -1) {
        return;
    }
    //recebe a diferenca da direcao do anterior com o atual (anterior eh o que o rato estava antes de descobrir o atual)
    int difMod = grafo->listaAdjacencia[obj]->dir - grafo->listaAdjacencia[proximo[obj]]->dir;
    difMod += 44;
    difMod = difMod % 4;
    switch (difMod) {
    case 0:
        break;
    case 1:
        strcat(grafo->mov, "l");
        break;
    case 2:
        break;
    case 3:
        strcat(grafo->mov, "r");
        break;
    default:
        break;
    }
    strcat(grafo->mov, "m");
    
    registraMov(grafo, proximo, proximo[obj]);
}


void Dijkstra(Grafo* grafo, int origem, int numVertices) {
    int dist[numVertices];
    int visitado[numVertices];
    int proximo[numVertices];

    // Inicializa as distancias como infinito e os vertices como nao visitados
    for (int i = 0; i < numVertices; i++) {
        dist[i] = INT_MAX;
        visitado[i] = 0;
        proximo[i] = -1;
    }

    // A distancia do vertice de origem para ele mesmo eh 0
    dist[origem] = 0;

    for (int i = 0; i < numVertices; i++) {
        // Encontra o vertice com a menor distancia
        int u = acharMenorDist(dist, visitado, numVertices);

        // Marca o vertice atual como visitado
        visitado[u] = 1;

        No* temp = grafo->listaAdjacencia[u]->inicio;
        while (temp) {
            int v = temp->verticeIdx;

            if (visitado[v] == 0 && dist[u] != INT_MAX && dist[u] + 1 < dist[v]) {
                dist[v] = dist[u] + 1;
                proximo[v] = u;
            }

            temp = temp->prox;
        }
    }
    
    registraMov(grafo, proximo, 0);
}

//-------------------------------FIM GRAFO----------------------------------------
void inverterMov(char vet[]){
    int i, fim, tam = strlen(vet);
    int aux;
    for(int i = 0; i < tam; i++) {
        if(vet[i] == 'l') {
            vet[i] = 'r';
        } else if(vet[i] == 'r') {
            vet[i] = 'l';
        }
    }

    fim = tam - 1; // indice do ultimo caracter
    for(i = 0; i < tam/2; i++){
        aux = vet[i];
        vet[i] = vet[fim];
        vet[fim] = aux;
        fim--;
    }
}

void trataStr(char *str) {
    int len = strlen(str);
    int i, j, chec = 1;
    
    while (chec != 0) {
        chec = 0;
        for (i = 0, j = 0; i < len; i++) { //retira l e r consecutivos
            if ((str[i] == 'l' && str[i+1] == 'r') || (str[i] == 'r' && str[i+1] == 'l')) {
                i++;
                chec++;
            } else {
                str[j++] = str[i];
            }
        }
        
        str[j] = '\0';

        for (i = 0, j = 0; i < len; i++, j++) { //converte tres curvas para o mesmo lado para uma curva para o outro lado
            if(str[i] == 'l' && str[i+1] == 'l' && str[i+2] == 'l') {
                str[j] = 'r';
                i += 2;
                chec++;
            } else if(str[i] == 'r' && str[i+1] == 'r' && str[i+2] == 'r') {
                str[j] = 'l';
                i += 2;
                chec++;
            } else {
                str[j] = str[i];
            }
        }
        str[j] = '\0';
        for (i = 0, j = 0; i < len; i++, j++) { //dois m seguidos vira um M
            if(str[i] == 'm' && str[i+1] == 'm') {
                str[j] = 'M';
                i++;
                chec++;
            } else {
                str[j] = str[i];
            }
        }
        str[j] = '\0';
    }
}


void voltaOrigem(Grafo* grafo) {
    int retorno;
    strcat(grafo->mov, "ll");
    inverterMov(grafo->mov);
    trataStr(grafo->mov);
    for(int i = 0; i < strlen(grafo->mov); i++) {
        printf("%c\n", grafo->mov[i]);
        fflush(stdout);
        scanf("%d", &retorno);
        fflush(stdin);
    }
}

void voltaObj(Grafo* grafo) {
    int retorno;
    strcat(grafo->mov, "ll");
    inverterMov(grafo->mov);
    grafo->mov[strlen(grafo->mov)-2] = '\0';
    trataStr(grafo->mov);
    for(int i = 0; i < strlen(grafo->mov); i++) {
        printf("%c\n", grafo->mov[i]);
        fflush(stdout);
        scanf("%d", &retorno);
        fflush(stdin);
    }
}

void calculaCoord(int* x, int* y) { //retorna o idx referente a coordenada atualizada
    int mod = controlePos % 4;
    switch (mod) {
    case 0:
        *x = *x + 1;
        break;
    case 1:
        *y = *y - 1;
        break;
    case 2:
        *x = *x - 1;
        break;
    case 3:
        *y = *y + 1;
        break;
    }
}

void freeNode(No* no) {
    if(no == NULL) {
        return;
    }
    
    freeNode(no->prox);
    free(no);
}

int achaIdxAtual(Grafo grafo, int posX, int posY) {
    for (int i = 0; i < MAX_VERTICE; i++) {
        if(grafo.listaAdjacencia[i]->x == posX && grafo.listaAdjacencia[i]->y == posY) {
            return i; //o vertice i tem essas coord
        } else if(grafo.listaAdjacencia[i]->x == INT_MAX || grafo.listaAdjacencia[i]->y == INT_MAX) {
            return -1; //n existe vertice com essas coord
            break;
        }
    }
    return -1; //n existe vertice com essas coord
}

int checaProx(Grafo grafo, int posX, int posY, int idxAtual) { //0: dir eh novo   1: frente eh novo   2: esq eh novo
    int x, y, aux;
    int novo[4] = {0}; // novo[0] == 0: celula na dir 0 eh conhecida
    int melhor[4] = {0};
    int retorno = -1;
    int controle;
    aux = controlePos;
    controlePos++;
    for(int i = 1; i > -2; i--) { // checa quais celulas a em volta sao novas
        x = posX;
        y = posY;
        calculaCoord(&x, &y);
        if(achaIdxAtual(grafo, x, y) == -1) {
            novo[(aux + (i)) % 4] = 1;
        }
        controlePos--;
    }
    for(int i = 0; i < 4; i++) { // checa quais celulas em volta sao novas e ha possibilidade de ir
        if(novo[i] == 1) {
            if(grafo.listaAdjacencia[idxAtual]->parede[i] == 0) {
                melhor[i] = 1;
            }
        }
    }
    controlePos = aux;
    aux++;
    controle = aux;
    while (1) {
        if(melhor[aux % 4] == 1) {
            retorno = aux % 4;
            break;
        }

        aux--;
        if(controle % 4 == aux % 4) {
            break;
        }
    }

    if(retorno == -1) {
        aux = controlePos;
        aux++;
        for(int i = 0; i < 4; i++) {
            if(grafo.listaAdjacencia[idxAtual]->parede[aux % 4] == 0) {
                retorno = aux % 4;
                break;
            }
            aux--;
        }
    }
    
    return retorno; // qual a melhor direcao para ir
}

int checaProxEsq(Grafo grafo, int posX, int posY, int idxAtual) { //0: esq eh novo   1: frente eh novo   2: dir eh novo
    int x, y, aux;
    int novo[4] = {0}; // novo[0] == 0: celula na dir 0 eh conhecida
    int melhor[4] = {0};
    int retorno = -1;
    int controle;
    aux = controlePos;
    controlePos++;
    for(int i = 1; i > -2; i--) { // checa quais celulas a minha volta sao novas
        x = posX;
        y = posY;
        calculaCoord(&x, &y);
        if(achaIdxAtual(grafo, x, y) == -1) {
            novo[(aux + (i)) % 4] = 1;
        }
        controlePos--;
    }
    for(int i = 0; i < 4; i++) { // checa quais celulas a minha volta sao novas e ha possibilidade de ir
        if(novo[i] == 1) {
            if(grafo.listaAdjacencia[idxAtual]->parede[i] == 0) {
                melhor[i] = 1;
            }
        }
    }
    controlePos = aux;
    aux--;
    controle = aux;
    while (1) {
        if(melhor[aux % 4] == 1) {
            retorno = aux % 4;
            break;
        }

        aux++;
        if(controle % 4 == aux % 4) {
            break;
        }
    }
    
    if(retorno == -1) {
        aux = controlePos;
        aux--;
        for(int i = 0; i < 4; i++) {
            if(grafo.listaAdjacencia[idxAtual]->parede[aux % 4] == 0) {
                retorno = aux % 4;
                break;
            }
            aux++;
        }
    }
    
    return retorno; // qual a melhor direcao para ir
}

void marcaParede(Grafo* grafo, int idxAtual) {
    int dirAtual = controlePos % 4;
    grafo->listaAdjacencia[idxAtual]->parede[dirAtual] = 1;
}

int melhorProxMov(int dirAtual, int dirMelhor) {
    int difMod = dirAtual - dirMelhor;
    difMod += 44;
    difMod = difMod % 4;
    
    return difMod;
}

int main(void) {
    Grafo grafo;
    int retorno = 0, temp;
    int posX = 0, posY = 0, idxAtual = 0, idxProx = 1;
    int modoAtual = 0;// 0: modoDir; 1: modoEsq;
    int modoEsq = 0;
    int modoRandom = 0;
    int proxMov;
    int melhorDir, dirAtual;
    inicializarGrafo(&grafo);
    
    grafo.listaAdjacencia[idxAtual]->dir = 0;
    grafo.listaAdjacencia[idxAtual]->x = 0;
    grafo.listaAdjacencia[idxAtual]->y = 0;
    while(retorno != 4) {
        if(modoAtual == 0) { //modoDir
            melhorDir = checaProx(grafo, posX, posY, idxAtual);
            proxMov = melhorProxMov(controlePos % 4, melhorDir);
            
            dirAtual = controlePos % 4;
            switch (proxMov) {
            case 0: //melhor dir eh a atual
                retorno = andarFrente();
                if(retorno == 1) { //descobri uma parede na frente
                    marcaParede(&grafo, idxAtual);
                    if(grafo.listaAdjacencia[idxAtual]->parede[(dirAtual+1) % 4] == 0) { //acho q n tem parede na direita
                        andarDireita();
                        retorno = andarFrente();
                        if(retorno == 1) { //descobri uma parede na direita
                            marcaParede(&grafo, idxAtual);
                            if(grafo.listaAdjacencia[idxAtual]->parede[(dirAtual-1) % 4] == 0) { // acho q n tem parede na esquerda
                                retorno = andarMeiaVolta();
                                if(retorno == 1) { //descobri uma parede na esquerda
                                    marcaParede(&grafo, idxAtual);
                                    andarEsquerda();
                                    retorno = andarFrente();
                                }
                            } else { //descobri parede na frente e na direita e sei que tem uma parede na esquerda
                                andarDireita();
                                retorno = andarFrente();
                            }
                        }
                    } else if(grafo.listaAdjacencia[idxAtual]->parede[(dirAtual-1) % 4] == 0){ //descobri uma parede na frente e sei que tem uma parede na direita, mas acho que nao tem na esquerda
                        andarEsquerda();
                        retorno = andarFrente();
                        if(retorno == 1) { //sei que tem parede na frente, na direita e na esquerda
                            marcaParede(&grafo, idxAtual);
                            andarEsquerda();
                            retorno = andarFrente();
                        }   
                    } else { //descobri que tem parede na frente e sei que tem parede na direita e na esquerda
                        retorno = andarMeiaVolta();
                    }
                }
                
                break;
            case 1: //melhor direcao eh para a esquerda
                //l
                andarEsquerda();
                retorno = andarFrente();
                if(retorno == 1) { //descobri parede na esquerda
                    marcaParede(&grafo, idxAtual);
                    if(grafo.listaAdjacencia[idxAtual]->parede[(dirAtual+1) % 4] == 0) { //tem parede na esquerda e acho q n tem na direita
                        retorno = andarMeiaVolta();
                        if(retorno == 1) { //tem parede na esquerda e descobri uma parede na direita
                            marcaParede(&grafo, idxAtual);
                            if(grafo.listaAdjacencia[idxAtual]->parede[dirAtual % 4] == 0) { //tem parede na esquerda e na direita, mas acho q n tem na frente
                                andarEsquerda();
                                retorno = andarFrente();
                                if(retorno == 1) { //estou em um beco sem saida
                                    marcaParede(&grafo, idxAtual);
                                    retorno = andarMeiaVolta();
                                }
                            } else { //tem parede na esquerda e na direita e sei que tem na frente tambem
                                andarDireita();
                                retorno = andarFrente();
                            }
                        }
                    } else if(grafo.listaAdjacencia[idxAtual]->parede[dirAtual % 4] == 0) { //tem parede na esquerda e sei que tem parede na direita e acho q n tem na frente
                        andarDireita();
                        retorno = andarFrente();
                        if(retorno == 1) { //descobri uma parede na frente
                            marcaParede(&grafo, idxAtual);
                            retorno = andarMeiaVolta();
                        }    
                    } else { //descobri uma parede na esquerda e sei que tem na direita e na frente
                        andarEsquerda();
                        retorno = andarFrente();
                    }
                }
                break;
            case 2:
                //rr
                andarDireita();
                andarDireita();
                retorno = andarFrente();
                break;
            case 3:
                //r
                andarDireita();
                retorno = andarFrente();
                if(retorno == 1) { //descobri parede na direita
                    marcaParede(&grafo, idxAtual);
                    if(grafo.listaAdjacencia[idxAtual]->parede[dirAtual % 4] == 0) { //tem parede na direita e acho que n tem na frente
                        andarEsquerda();
                        retorno = andarFrente();
                        if(retorno == 1) { //descobri parede na frente
                            marcaParede(&grafo, idxAtual);
                            if(grafo.listaAdjacencia[idxAtual]->parede[(dirAtual-1) % 4] == 0) { // tem parede na direita e na frente e acho q n tem na esquerda
                                andarEsquerda();
                                retorno = andarFrente();
                                if(retorno == 1) { //descobri parede na esquerda
                                    marcaParede(&grafo, idxAtual);
                                    andarEsquerda();
                                    retorno = andarFrente();
                                }
                            } else { //tem parede na direita e na frente e sei que tem na esquerda
                                retorno = andarMeiaVolta();   
                            }
                        }
                    } else if(grafo.listaAdjacencia[idxAtual]->parede[(dirAtual-1) % 4] == 0) { //tem parede na direita e na frente, mas acho que nao tem na esquerda
                        retorno = andarMeiaVolta();   
                        if(retorno == 1) {
                            marcaParede(&grafo, idxAtual);
                            andarEsquerda();
                            retorno = andarFrente();
                        }
                    } else { //tem parede na direita e sei que tem na frente e na esquerda
                        andarDireita();
                        retorno = andarFrente();
                    }
                }
                break;
            }
            
        } else if(modoAtual == 1){ //modoEsq
            melhorDir = checaProxEsq(grafo, posX, posY, idxAtual);
            proxMov = melhorProxMov(controlePos % 4, melhorDir);

            dirAtual = controlePos % 4;

            switch (proxMov) {
            case 0: //direcao atual eh melhor
                retorno = andarFrente();
                if(retorno == 1) { //descobri parede na frente
                    marcaParede(&grafo, idxAtual);
                    if(grafo.listaAdjacencia[idxAtual]->parede[(dirAtual-1) % 4] == 0) { //tem parede na frente mas acho que n tem na esquerda
                        andarEsquerda();
                        retorno = andarFrente();
                        if(retorno == 1) { //tem parede na frente e na esquerda
                            marcaParede(&grafo, idxAtual);
                            if(grafo.listaAdjacencia[idxAtual]->parede[(dirAtual+1) % 4] == 0) { //tem parede na frente e na esquerda, mas acho que nao tem na direita
                                retorno = andarMeiaVolta();
                                if(retorno == 1) { // beco sem saida
                                    marcaParede(&grafo, idxAtual);
                                    andarDireita();
                                    retorno = andarFrente();
                                }
                            } else { //beco sem saida
                                andarEsquerda();
                                retorno = andarFrente();
                            }

                        }
                    } else if(grafo.listaAdjacencia[idxAtual]->parede[(dirAtual+1) % 4] == 0) { //tem parede na frente e na esquerda, mas acho que n tem na direita
                        andarDireita();
                        retorno = andarFrente();
                        if(retorno == 1) { //beco sem saida
                            marcaParede(&grafo, idxAtual);
                            andarDireita();
                            retorno = andarFrente();
                        }   
                    } else { //beco sem saida
                        retorno = andarMeiaVolta();
                    }
                }
                break;
            case 1: //melhor direcao eh para esquerda
                andarEsquerda();
                retorno = andarFrente();
                if(retorno == 1) {
                    marcaParede(&grafo, idxAtual);
                    if(grafo.listaAdjacencia[idxAtual]->parede[dirAtual%4] == 0) { //tem parede na esquerda e acho q n tem na frente
                        andarDireita();
                        retorno = andarFrente();
                        if(retorno == 1) { // tem parede na frente
                            marcaParede(&grafo, idxAtual);
                            if(grafo.listaAdjacencia[idxAtual]->parede[(dirAtual+1) % 4] == 0) { //tem parede na esquerda e na frente, mas acho que nao tem na direita
                                andarDireita();
                                retorno = andarFrente();
                                if(retorno == 1) { //beco sem saida
                                    marcaParede(&grafo, idxAtual);
                                    andarDireita();
                                    retorno = andarFrente();
                                } 
                            } else { //beco sem saida
                                retorno = andarMeiaVolta();
                            }
                        }
                    } else if(grafo.listaAdjacencia[idxAtual]->parede[(dirAtual+1) % 4] == 0) { //tem parede na esquerda e sei que tem na frente, mas acho que nao tem na direita
                        retorno = andarMeiaVolta();
                        if(retorno == 1) { //beco sem saida
                            marcaParede(&grafo, idxAtual);
                            andarDireita();
                            retorno = andarFrente();
                        }
                    } else { //beco sem saida
                        andarEsquerda();
                        retorno = andarFrente();
                    }
                }
                break;
            case 2: //melhor dar meia volta
                retorno = andarMeiaVolta();
                break;
            case 3: //melhor direcao eh para a direita
                andarDireita();
                retorno = andarFrente();
                if(retorno == 1) {
                    marcaParede(&grafo, idxAtual);
                    if(grafo.listaAdjacencia[idxAtual]->parede[(dirAtual-1) % 4] == 0) { //tem parede na direita e acho q n tem na esquerda
                        retorno = andarMeiaVolta();
                        if(retorno == 1) { //descobri q tem parede na esquerda
                            marcaParede(&grafo, idxAtual);
                            if(grafo.listaAdjacencia[idxAtual]->parede[dirAtual % 4] == 0) { //tem parede na direita e na esquerda, mas acho que n tem na frente
                                andarDireita();
                                retorno = andarFrente();
                                if(retorno == 1) { //beco sem saida
                                    marcaParede(&grafo, idxAtual);
                                    retorno = andarMeiaVolta();
                                }
                            } else { //beco sem saida
                                andarEsquerda();
                                retorno = andarFrente();
                            }
                        }
                    } else if(grafo.listaAdjacencia[idxAtual]->parede[dirAtual % 4] == 0) { //tem parede na direita e na esquerda, mas acho que n tem na frente
                        andarEsquerda();
                        retorno = andarFrente();
                        if(retorno == 1) { //beco sem saida
                            marcaParede(&grafo, idxAtual);
                            retorno = andarMeiaVolta();
                        }
                    } else { //beco sem saida
                        andarDireita();
                        retorno = andarFrente();
                    }
                }
                break;
            }
            

        }

        if(retorno == 1) {
            return 1; // fez todos os movimentos possíveis e ainda bateu na parede
        } else if(retorno != 1) {
            calculaCoord(&posX, &posY); // atualiza x e y
            if(achaIdxAtual(grafo, posX, posY) == -1) { //vertice novo
                modoAtual = 0;
                modoEsq = 0;
                modoRandom = 0;
                grafo.listaAdjacencia[idxProx]->dir = controlePos % 4;
                adicionarAresta(&grafo, idxAtual, idxProx);
                idxAtual = idxProx;
                idxProx++;
                grafo.listaAdjacencia[idxAtual]->x = posX;
                grafo.listaAdjacencia[idxAtual]->y = posY;
                grafo.numVertices++;
            } else { //cheguei em um vertice que ja conheco
                idxAtual = achaIdxAtual(grafo, posX, posY); // acha o idx do vertice atual

                if(modoAtual == 0) {
                    modoEsq++;
                } else {
                    modoRandom++;
                }

                if(modoRandom < 20) {
                    if(modoEsq >= 20) {
                        modoAtual = 1;
                    }
                    
                } else {
                    modoAtual = rand() % 2;
                }
                
                
            }
        }
    }
    
    Dijkstra(&grafo, idxAtual, grafo.numVertices);
    
    voltaOrigem(&grafo);
    voltaObj(&grafo);
    for(int i = 0; i < MAX_VERTICE; i++) {
        freeNode(grafo.listaAdjacencia[i]->inicio);
    }
    
}
