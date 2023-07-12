#include <raylib.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define MAP_ROWS 30
#define MAP_COLS 60
#define MAX_ENEMIES 10
#define CHAR_ESPACO_LIVRE ' '
#define CHAR_PAREDE '#'
#define CHAR_JOGADOR 'P'
#define CHAR_INIMIGO 'I'
#define BORDA_HUD 130



// Váriaveis globais
int num_inimigos = 0;

// Calculate the starting position for the map to be centered
int mapStartX = (SCREEN_WIDTH - (SCREEN_WIDTH / MAP_COLS * MAP_COLS)) / 2;
int mapStartY = (SCREEN_HEIGHT - (SCREEN_HEIGHT / MAP_ROWS * MAP_ROWS)) / 2;


typedef struct player
{
    int vidas;
    Vector2 pos_matriz_player;
    Vector2 pos_tela_player;
    Texture2D player_texture;
    Rectangle player_source;

} JOGADOR;

typedef struct enemy
{
    Vector2 pos_matriz;
    Vector2 pos_tela;
    Texture2D enemy_texture;
    Rectangle enemy_source;
    int can_fire;
    int vivo;

} INIMIGO;

void print_matriz(char matriz[][60],int linhas)
{

    for(int i=0; i<linhas; i++)
    {

        for(int j=0; j<60; j++)
        {

            printf("%c", matriz[i][j]);
        }
        printf("\n");
    }
}

void init_enemies(INIMIGO enemies[MAX_ENEMIES])//inicializa os inimigos
{

    for(int i=0; i<MAX_ENEMIES; i++)
    {
        enemies[i].enemy_texture=LoadTexture("sprites/isaac.png");
        enemies[i].enemy_source=(Rectangle){0,0,enemies[i].enemy_texture.width,enemies[i].enemy_texture.height};
        enemies[i].vivo=0;
        enemies[i].can_fire=0;
    }
}



int funcao_movimento(char mapa[MAP_ROWS][MAP_COLS],Vector2 pos_matriz,char tecla)
{
    int posx=(int)pos_matriz.x;
    int posy=(int)pos_matriz.y;
    int entidade_livre_jogador;
    int entidade_livre_inimigo;
    switch(tecla)
    {
    case 'w':
        entidade_livre_jogador= (mapa[posx-3][posy+2] !=CHAR_JOGADOR &&mapa[posx-3][posy+1] !=CHAR_JOGADOR &&mapa[posx-3][posy] !=CHAR_JOGADOR &&mapa[posx-3][posy-1] !=CHAR_JOGADOR &&mapa[posx-3][posy-2] !=CHAR_JOGADOR);

        entidade_livre_inimigo=(mapa[posx-3][posy+2] !=CHAR_INIMIGO &&mapa[posx-3][posy+1] !=CHAR_INIMIGO &&mapa[posx-3][posy] !=CHAR_INIMIGO &&mapa[posx-3][posy-1] !=CHAR_INIMIGO &&mapa[posx-3][posy-2] !=CHAR_INIMIGO );

        if(mapa[posx-1][posy]==CHAR_ESPACO_LIVRE && mapa[posx-1][posy+1] == CHAR_ESPACO_LIVRE && mapa[posx-1][posy+2] == CHAR_ESPACO_LIVRE && entidade_livre_jogador &&entidade_livre_inimigo)
        {
            return 1;
        }
        else
        {
            return 0;
        }
        break;
    case 's':
        entidade_livre_jogador=(mapa[posx+3][posy]!=CHAR_JOGADOR &&mapa[posx+3][posy+1] !=CHAR_JOGADOR &&mapa[posx+3][posy+2] !=CHAR_JOGADOR &&mapa[posx+3][posy-1] !=CHAR_JOGADOR &&mapa[posx+3][posy-2] !=CHAR_JOGADOR);


        entidade_livre_inimigo=(mapa[posx+3][posy]!=CHAR_INIMIGO &&mapa[posx+3][posy+1] !=CHAR_INIMIGO &&mapa[posx+3][posy+2] !=CHAR_INIMIGO &&mapa[posx+3][posy-1] !=CHAR_INIMIGO &&mapa[posx+3][posy-2] !=CHAR_INIMIGO);



        if(mapa[posx+3][posy]==CHAR_ESPACO_LIVRE && mapa[posx+3][posy+1] == CHAR_ESPACO_LIVRE && mapa[posx+3][posy+2] == CHAR_ESPACO_LIVRE && entidade_livre_inimigo && entidade_livre_jogador)
        {
            return 1;
        }
        else
        {
            return 0;
        }
        break;
    case 'a':
        entidade_livre_jogador=(mapa[posx][posy-3]!=CHAR_JOGADOR &&mapa[posx+1][posy-3] !=CHAR_JOGADOR &&mapa[posx+2][posy-3] !=CHAR_JOGADOR &&mapa[posx-1][posy-3] !=CHAR_JOGADOR &&mapa[posx-2][posy-3] !=CHAR_JOGADOR);

        entidade_livre_inimigo=(mapa[posx][posy-3]!=CHAR_INIMIGO &&mapa[posx+1][posy-3] !=CHAR_INIMIGO &&mapa[posx+2][posy-3] !=CHAR_INIMIGO &&mapa[posx-1][posy-3] !=CHAR_INIMIGO &&mapa[posx-2][posy-3] !=CHAR_INIMIGO);


        if(mapa[posx][posy-1]==CHAR_ESPACO_LIVRE && mapa[posx+1][posy-1] == CHAR_ESPACO_LIVRE && mapa[posx+2][posy-1] == CHAR_ESPACO_LIVRE && entidade_livre_inimigo && entidade_livre_jogador)
        {

            return 1;
        }
        else
        {
            return 0;
        }
        break;
    case 'd':
        entidade_livre_jogador=(mapa[posx][posy+3]!=CHAR_JOGADOR &&mapa[posx+1][posy+3] !=CHAR_JOGADOR &&mapa[posx+2][posy+3] !=CHAR_JOGADOR &&mapa[posx-1][posy+3] !=CHAR_JOGADOR &&mapa[posx-2][posy+3] !=CHAR_JOGADOR);

        entidade_livre_inimigo=(mapa[posx][posy+3]!=CHAR_INIMIGO &&mapa[posx+1][posy+3] !=CHAR_INIMIGO &&mapa[posx+2][posy+3] !=CHAR_INIMIGO &&mapa[posx-1][posy+3] !=CHAR_INIMIGO &&mapa[posx-2][posy+3] !=CHAR_INIMIGO);

        if(mapa[posx][posy+3]==CHAR_ESPACO_LIVRE && mapa[posx+1][posy+3] == CHAR_ESPACO_LIVRE && mapa[posx+2][posy+3] == CHAR_ESPACO_LIVRE && entidade_livre_inimigo && entidade_livre_jogador)
        {
            return 1;
        }
        else
        {
            return 0;
        }
        break;
    default:
        return 0;
        break;
    }
}

void movimenta_inimigo_debug(INIMIGO* enemy,char mapa[MAP_ROWS][MAP_COLS])
{
    if(IsKeyDown(KEY_UP) && funcao_movimento(mapa,enemy->pos_matriz,'w'))
    {
        mapa[(int)enemy->pos_matriz.x][(int)enemy->pos_matriz.y]=CHAR_ESPACO_LIVRE;
        enemy->pos_matriz.x-=1;
        mapa[(int)enemy->pos_matriz.x][(int)enemy->pos_matriz.y]=CHAR_INIMIGO;

    }
    if(IsKeyDown(KEY_DOWN) && funcao_movimento(mapa,enemy->pos_matriz,'s'))
    {
        mapa[(int)enemy->pos_matriz.x][(int)enemy->pos_matriz.y]=CHAR_ESPACO_LIVRE;
        enemy->pos_matriz.x+=1;
        mapa[(int)enemy->pos_matriz.x][(int)enemy->pos_matriz.y]=CHAR_INIMIGO;
    }
    if(IsKeyDown(KEY_LEFT) && funcao_movimento(mapa,enemy->pos_matriz,'a'))
    {
        mapa[(int)enemy->pos_matriz.x][(int)enemy->pos_matriz.y]=CHAR_ESPACO_LIVRE;
        enemy->pos_matriz.y-=1;
        mapa[(int)enemy->pos_matriz.x][(int)enemy->pos_matriz.y]=CHAR_INIMIGO;
    }
    if(IsKeyDown(KEY_RIGHT) && funcao_movimento(mapa,enemy->pos_matriz,'d'))
    {
        mapa[(int)enemy->pos_matriz.x][(int)enemy->pos_matriz.y]=CHAR_ESPACO_LIVRE;
        enemy->pos_matriz.y+=1;
        mapa[(int)enemy->pos_matriz.x][(int)enemy->pos_matriz.y]=CHAR_INIMIGO;
    }
}

void movimenta_inimigos(char matriz[30][60], INIMIGO enemies[MAX_ENEMIES]) // movimenta os inimigos pelo mapa
{
    int movimentox = 0;
    int movimentoy = 0;
    srand(time(NULL));

    //movimento aleatorio entre -1 e 1
    for(int i=0; i<MAX_ENEMIES; i++)
    {
        movimentox=rand()%3-1;
        movimentoy=rand()%3-1;
        if(enemies[i].vivo)
        {

            switch(movimentox)
            {
            case 1:
                if(funcao_movimento(matriz,enemies[i].pos_matriz,'d'))
                {
                    matriz[(int)enemies[i].pos_matriz.x][(int)enemies[i].pos_matriz.y]=CHAR_ESPACO_LIVRE;
                    enemies[i].pos_matriz.y+=1;
                    matriz[(int)enemies[i].pos_matriz.x][(int)enemies[i].pos_matriz.y]=CHAR_INIMIGO;
                }
                break;
            case -1:
                if(funcao_movimento(matriz,enemies[i].pos_matriz,'a'))
                {
                    matriz[(int)enemies[i].pos_matriz.x][(int)enemies[i].pos_matriz.y]=CHAR_ESPACO_LIVRE;
                    enemies[i].pos_matriz.y-=1;
                    matriz[(int)enemies[i].pos_matriz.x][(int)enemies[i].pos_matriz.y]=CHAR_INIMIGO;
                }
                break;
            default:
                break;
            }
            switch(movimentoy)
            {
            case 1:
                if(funcao_movimento(matriz,enemies[i].pos_matriz,'s'))
                {
                    matriz[(int)enemies[i].pos_matriz.x][(int)enemies[i].pos_matriz.y]=CHAR_ESPACO_LIVRE;
                    enemies[i].pos_matriz.x+=1;
                    matriz[(int)enemies[i].pos_matriz.x][(int)enemies[i].pos_matriz.y]=CHAR_INIMIGO;
                }
                break;
            case -1:
                if(funcao_movimento(matriz,enemies[i].pos_matriz,'w'))
                {
                    matriz[(int)enemies[i].pos_matriz.x][(int)enemies[i].pos_matriz.y]=CHAR_ESPACO_LIVRE;
                    enemies[i].pos_matriz.x-=1;
                    matriz[(int)enemies[i].pos_matriz.x][(int)enemies[i].pos_matriz.y]=CHAR_INIMIGO;
                }
                break;
            default:
                break;
            }
        }
    }


}

int importa_mapa(char fileName[], char matriz[MAP_ROWS][MAP_COLS], JOGADOR *jogador, INIMIGO inimigos[MAX_ENEMIES])  // Importa o mapa do arquivo para a matriz
{

    int error_code;

    char linha[MAP_COLS + 2];
    FILE *mapaFile = fopen(fileName, "r");

    if (mapaFile == NULL)
    {
        error_code = 1;
    }
    else
    {
        while (!feof(mapaFile)) // Enquanto não chegar no final do arquivo
        {
            for (int i = 0; i < MAP_ROWS; i++)
            {
                fgets(linha, MAP_COLS+2, mapaFile); // Lê uma linha do arquivo
                for (int j = 0; j < MAP_COLS; j++) // Percorre a linha lida
                {
                    switch (linha[j])  // Verifica o caractere lido
                    {
                    case CHAR_JOGADOR: // Se for o jogador, adiciona ele na struct
                        jogador->pos_matriz_player.x = i;
                        jogador->pos_matriz_player.y = j;
                        break;
                    case CHAR_INIMIGO: // Se for um inimigo, adiciona ele ao vetor de inimigos
                        inimigos[num_inimigos].vivo = 1;
                        inimigos[num_inimigos].can_fire=1;
                        inimigos[num_inimigos].pos_matriz.x = i;
                        inimigos[num_inimigos].pos_matriz.y = j;
                        num_inimigos++;

                        // inimigos[num_inimigos_vivos(inimigos)].vivo=1;


                        break;
                    default:
                        break;
                    }
                    matriz[i][j] = linha[j];
                }
            }
        }
        error_code = 0;
    }
    fclose(mapaFile);
    return error_code;
}

Vector2 calcula_posTela(int pos_x_matriz,int pos_y_matriz) //recebe dois inteiros e devolve um vector2 com a posicao na tela
{
    Vector2 pos_tela;
    pos_tela.x=pos_y_matriz*(SCREEN_WIDTH/MAP_COLS);
    pos_tela.y=pos_x_matriz*((SCREEN_HEIGHT-BORDA_HUD)/MAP_ROWS);
    return pos_tela;
}


void desenha_inimigos(INIMIGO inimigos[MAX_ENEMIES])
{
    for(int i=0; i<MAX_ENEMIES; i++)
    {
        if(inimigos[i].vivo)
        {
            inimigos[i].pos_tela=calcula_posTela((int)inimigos[i].pos_matriz.x,(int)inimigos[i].pos_matriz.y);
            //DrawRectangle(inimigos[i].pos_tela.x,inimigos[i].pos_tela.y,SCREEN_WIDTH/MAP_COLS,(SCREEN_HEIGHT-BORDA_HUD)/MAP_ROWS,RED);
            DrawTextureRec(inimigos[i].enemy_texture,inimigos[i].enemy_source, inimigos[i].pos_tela, WHITE);
        }
    }
}

void printa_inimigos(INIMIGO enemies[MAX_ENEMIES])//printa os inimigos na tela
{
    for(int i=0; i<MAX_ENEMIES; i++)
    {
        if(enemies[i].vivo)
        {
            printf("inimigo numero %d-> x: %d y: %d\n",i+1, (int)enemies[i].pos_matriz.x,(int) enemies[i].pos_matriz.y);
        }
    }
}

void calcula_tempo(clock_t init, int *segundos, int *minutos)  // Calcula o tempo de jogo
{
    *segundos = ((double) clock() -  init)/ CLOCKS_PER_SEC - (60 * (*minutos));
    if(*segundos >= 60)
    {
        *minutos = *minutos + 1;
    }
}

void desenha_mapa(char mapa[MAP_ROWS][MAP_COLS])
{
    int x,y;
    for(int i=0; i<MAP_ROWS; i++)
    {
        for(int j=0; j<MAP_COLS; j++)
        {
            if(mapa[i][j]==CHAR_PAREDE)
            {
                x=j*(SCREEN_WIDTH/MAP_COLS);
                y=i*((SCREEN_HEIGHT-BORDA_HUD)/MAP_ROWS);
                DrawRectangle(x,y,(SCREEN_WIDTH/MAP_COLS),(SCREEN_HEIGHT-BORDA_HUD)/MAP_ROWS,BLACK); //drawrectangle(x,y,width,height,color)
            }
            else if(mapa[i][j]==CHAR_ESPACO_LIVRE)
            {
                x=j*(SCREEN_WIDTH/MAP_COLS);
                y=i*((SCREEN_HEIGHT-BORDA_HUD)/MAP_ROWS);
                DrawRectangleLines(x,y,SCREEN_WIDTH/MAP_COLS,(SCREEN_HEIGHT-BORDA_HUD)/MAP_ROWS,RED);
            }
        }
    }
}

void init_player(JOGADOR *player)
{
    player->pos_matriz_player.x=0;
    player->vidas=3;
    player->pos_matriz_player.y=0;
    player->player_texture=LoadTexture("sprites/isaac.png");
    player->player_source=(Rectangle)
    {
        0,0,player->player_texture.width,player->player_texture.height
    };
}

void desenha_jogador(JOGADOR *player)
{
    player->pos_tela_player=calcula_posTela((int)player->pos_matriz_player.x,(int)player->pos_matriz_player.y);
    // printf("x tela: %d y tela: %d\n",(int)player->pos_tela_player.x,(int)player->pos_tela_player.y);
    // printf("x matriz: %d y matriz: %d\n",player->pos_x_matriz,player->pos_y_matriz);
    // system("pause");
    DrawTextureRec(player->player_texture,player->player_source,player->pos_tela_player,WHITE);
}

void movimenta_jogador(char map[MAP_ROWS][MAP_COLS],JOGADOR* player)
{
    if(IsKeyDown(KEY_A))
    {
        if(funcao_movimento(map,player->pos_matriz_player,(char)'a'))
        {
            map[(int)player->pos_matriz_player.x][(int)player->pos_matriz_player.y] = CHAR_ESPACO_LIVRE;
            player->pos_matriz_player.y -= 1;
            map[(int)player->pos_matriz_player.x][(int)player->pos_matriz_player.y] = CHAR_JOGADOR;
        }
    }
    if(IsKeyDown(KEY_D))
    {
        if(funcao_movimento(map, player->pos_matriz_player, 'd'))
        {
            map[(int)player->pos_matriz_player.x][(int)player->pos_matriz_player.y] = CHAR_ESPACO_LIVRE;
            player->pos_matriz_player.y += 1;
            map[(int)player->pos_matriz_player.x][(int)player->pos_matriz_player.y] = CHAR_JOGADOR;
        }
    }
    if(IsKeyDown(KEY_W))
    {
        if(funcao_movimento(map, player->pos_matriz_player, 'w'))
        {
            map[(int)player->pos_matriz_player.x][(int)player->pos_matriz_player.y] = CHAR_ESPACO_LIVRE;
            player->pos_matriz_player.x -= 1;
            map[(int)player->pos_matriz_player.x][(int)player->pos_matriz_player.y] = CHAR_JOGADOR;
        }
    }
    if(IsKeyDown(KEY_S))
    {
        if(funcao_movimento(map, player->pos_matriz_player, 's'))
        {
            map[(int)player->pos_matriz_player.x][(int)player->pos_matriz_player.y] = CHAR_ESPACO_LIVRE;
            player->pos_matriz_player.x += 1;
            map[(int)player->pos_matriz_player.x][(int)player->pos_matriz_player.y] = CHAR_JOGADOR;
        }
    }
}

int num_inimigos_vivos(INIMIGO inimigos[MAX_ENEMIES])
{
    int contador=0;
    for(int i=0; i<MAX_ENEMIES; i++)
    {
        if(inimigos[i].vivo)
        {
            contador++;
        }
    }
    return contador;
}

int main(void)
{
    // Inicializações da Raylib
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Binding of Isaac");
    SetTargetFPS(60); // Set the desired frame rate

    // Variáveis e outras inicalizações
    int contador_fase=1;
    int minutos = 0, segundos = 0;
    clock_t init = clock();

    JOGADOR player;
    INIMIGO inimigos[MAX_ENEMIES];
    char map[MAP_ROWS][MAP_COLS];
    char nome_arquivo[17] = "mapas/mapa01.txt";
    init_enemies(inimigos);
    init_player(&player);
    importa_mapa(nome_arquivo, map, &player, inimigos);

    // printa_inimigos(inimigos);
    // print_matriz(map,MAP_ROWS);

    // printf("%d",num_inimigos_vivos(inimigos));
    // printf("%d %d\n",(int)inimigos[0].pos_matriz.x,(int)inimigos[0].pos_matriz.y);
    // printf("%d %d\n",(int)inimigos[1].pos_matriz.x,(int)inimigos[1].pos_matriz.y);
    // printf("%d\n",inimigos[0].vivo);
    // printf("%c\n",map[(int)inimigos[0].pos_matriz.x][(int)inimigos[0].pos_matriz.y]);



    // Main game loop
    while (!WindowShouldClose())
    {

        // Update
        movimenta_jogador(map, &player);
        movimenta_inimigos(map, inimigos);
        // printf("%d",funcao_movimento(map,inimigos[0].pos_matriz,'w'));
        // system("cls");

        movimenta_inimigo_debug(&inimigos[0],map);


        // Draw
        BeginDrawing();

        ClearBackground(SKYBLUE);
        desenha_mapa(map);
        desenha_jogador(&player);
        desenha_inimigos(inimigos);


        // Impressao dos textos
        calcula_tempo(init, &segundos, &minutos);
        DrawText(TextFormat("Timer: %02dmin e %02ds", minutos, segundos), 10, SCREEN_HEIGHT-140, 35,BLACK);
        DrawText(TextFormat("Vidas: %1d", player.vidas), 10, SCREEN_HEIGHT - 45, 35, BLACK);
        DrawText(TextFormat("Barra de especial"), SCREEN_WIDTH-750, SCREEN_HEIGHT - 120, 35, RED);
        Rectangle especial_rec= {SCREEN_WIDTH-750,SCREEN_HEIGHT-80,310,35};
        DrawRectangleLinesEx(especial_rec, 5,RED);
        DrawText(TextFormat("Fase: %02d", contador_fase), SCREEN_WIDTH-150, SCREEN_HEIGHT-140, 35,BLACK);
        DrawText(TextFormat("Inimigos: %02d", num_inimigos), SCREEN_WIDTH-200, SCREEN_HEIGHT - 45, 35, BLACK);

        //debug
        Vector2 teste1=calcula_posTela((int)inimigos[0].pos_matriz.x-1,(int)inimigos[0].pos_matriz.y);
        DrawRectangle(teste1.x,teste1.y,SCREEN_WIDTH/MAP_COLS,(SCREEN_HEIGHT-BORDA_HUD)/MAP_ROWS,GREEN);
        EndDrawing();
    }

    // Clean up resources and close the window
    UnloadTexture(player.player_texture);
    CloseWindow();

    return 0;
}
