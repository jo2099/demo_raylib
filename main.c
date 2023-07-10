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



// Váriaveis globais
int num_inimigos = 0;

// Calculate the starting position for the map to be centered
int mapStartX = (SCREEN_WIDTH - (SCREEN_WIDTH / MAP_COLS * MAP_COLS)) / 2;
int mapStartY = (SCREEN_HEIGHT - (SCREEN_HEIGHT / MAP_ROWS * MAP_ROWS)) / 2;


typedef struct player
{
    int vidas;
    int pos_x;
    int pos_y;

}JOGADOR;

typedef struct enemy
{
    Vector2 pos_matriz;
    Vector2 pos_tela;
    Texture2D enemy_texture;
    Rectangle enemy_source;
    int can_fire;
    int vivo;

}INIMIGO;

void print_matriz(char matriz[][60],int linhas)
{

    for(int i=0;i<linhas;i++)
    {

        for(int j=0;j<60;j++)
        {

            printf("%c", matriz[i][j]);
        }
        printf("\n");
    }
}

void init_enemies(INIMIGO enemies[MAX_ENEMIES])//inicializa os inimigos
{

    for(int i=0;i<MAX_ENEMIES;i++)
    {
        enemies[i].enemy_texture=LoadTexture("sprites/isaac.png");
        enemies[i].enemy_source=(Rectangle){0,0,enemies[i].enemy_texture.width,enemies[i].enemy_texture.height};
        enemies[i].vivo=0;
        enemies[i].can_fire=0;
    }
}

void movimenta_inimigos(char matriz[30][60], INIMIGO enemies[MAX_ENEMIES]) // movimenta os inimigos pelo mapa
{
    int movimentox = 0;
    int movimentoy = 0;
    srand(time(NULL));
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].vivo)
        {
            // numero aleatorio entre -1 e 1
            movimentox = rand() % 3 - 1;
            movimentoy = rand() % 3 - 1;

            // system("cls");
            // printf("x: %d y: %d\n",movimentox,movimentoy);

            // verifica se o inimigo pode se mover
            switch (movimentox)
            {
            case -1: //se movendo para esquerda
                if (matriz[(int)enemies[i].pos_matriz.x][(int)enemies[i].pos_matriz.y - 1] == CHAR_ESPACO_LIVRE)
                {
                    enemies[i].pos_matriz.y--;
                }
                break;
            case 0: //parado
                break;
            case 1: //se movendo para direita
                if (matriz[(int)enemies[i].pos_matriz.x][(int)enemies[i].pos_matriz.y + 3] == CHAR_ESPACO_LIVRE)
                {
                    enemies[i].pos_matriz.y++;
                }
                break;

            default:
                break;
            }
            switch (movimentoy)
            {
            case -1: //se movendo para cima
                if (matriz[(int)enemies[i].pos_matriz.x - 1][(int)enemies[i].pos_matriz.y] == CHAR_ESPACO_LIVRE)
                {
                    (int)enemies[i].pos_matriz.x--;
                }
                break;
            case 0: //parado
                break;
            case 1: //se movendo para baixo
                if (matriz[(int)enemies[i].pos_matriz.x + 8][(int)enemies[i].pos_matriz.y] == CHAR_ESPACO_LIVRE)
                {
                    (int)enemies[i].pos_matriz.x++;
                }
                break;

            default:
                break;
            }
        }
    }
}

int importa_mapa(char fileName[], char matriz[MAP_ROWS][MAP_COLS], JOGADOR *jogador, INIMIGO inimigos[]){ // Importa o mapa do arquivo para a matriz

    int error_code;

    char linha[MAP_COLS + 2];
    FILE *mapaFile = fopen(fileName, "r");

    if (mapaFile == NULL){
        error_code = 1;
    }
    else{
        while (!feof(mapaFile)) // Enquanto não chegar no final do arquivo
        {
            for (int i = 0; i < MAP_ROWS; i++)
            {
                fgets(linha, MAP_COLS+2, mapaFile); // Lê uma linha do arquivo
                for (int j = 0; j < MAP_COLS; j++) // Percorre a linha lida
                {
                    switch (linha[j]){ // Verifica o caractere lido
                        case CHAR_JOGADOR: // Se for o jogador, adiciona ele na struct
                            jogador->pos_x = i;
                            jogador->pos_y = j;
                            break;
                        case CHAR_INIMIGO: // Se for um inimigo, adiciona ele ao vetor de inimigos
                            inimigos[num_inimigos].vivo = 1;
                            inimigos[num_inimigos].can_fire=1;
                            inimigos[num_inimigos].pos_matriz.x = i;
                            inimigos[num_inimigos].pos_matriz.y = j;
                            num_inimigos++;
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

void calcula_posTela_enemies(INIMIGO inimigos[MAX_ENEMIES])
{
    for(int i=0;i<MAX_ENEMIES;i++)
    {
        inimigos[i].pos_tela.x= (inimigos[i].pos_matriz.y * (SCREEN_WIDTH / MAP_COLS));
        inimigos[i].pos_tela.y= (inimigos[i].pos_matriz.x * (SCREEN_HEIGHT / MAP_ROWS));
    }
}

void desenha_inimigos(INIMIGO inimigos[MAX_ENEMIES])
{
    for(int i=0;i<MAX_ENEMIES;i++)
    {
        if(inimigos[i].vivo)
        {
            DrawTextureRec(inimigos[i].enemy_texture,inimigos[i].enemy_source,inimigos[i].pos_tela,WHITE);
        }
    }
}

void printa_inimigos(INIMIGO enemies[MAX_ENEMIES])//printa os inimigos na tela
{
    for(int i=0;i<MAX_ENEMIES;i++)
    {
        if(enemies[i].vivo)
        {
            printf("inimigo numero %d-> x: %d y: %d\n",i+1, (int)enemies[i].pos_matriz.x,(int) enemies[i].pos_matriz.y);
        }
    }
}

void calcula_tempo(clock_t init, int *segundos, int *minutos){ // Calcula o tempo de jogo
    *segundos = ((double) clock() -  init)/ CLOCKS_PER_SEC - (60 * (*minutos));
    if(*segundos >= 60){
        *minutos = *minutos + 1;
    }
}


int main(void)
{
    // Inicializações da Raylib
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Binding of Isaac");
    SetTargetFPS(60); // Set the desired frame rate

    // Incializações do sprite do isaac
    Texture2D isaac = LoadTexture("sprites/isaac.png");
    Rectangle isaac_source = {0, 0, isaac.width, isaac.height};
    Vector2 isaac_pos;

    // Variáveis e outras inicalizações
    int contador_fase=1;
    int generico=0;
    int minutos = 0, segundos = 0;
    clock_t init = clock();

    JOGADOR player;
    player.vidas = 3;
    INIMIGO inimigos[MAX_ENEMIES];
    char map[MAP_ROWS][MAP_COLS];
    char nome_arquivo[17] = "mapas/mapa01.txt";
    init_enemies(inimigos);
    importa_mapa(nome_arquivo, map, &player, inimigos);
    printa_inimigos(inimigos);
    print_matriz(map,MAP_ROWS);
    // system("pause");


    // Main game loop
    while (!WindowShouldClose())
    {



        // Movimentação do personagem
        if(IsKeyDown(KEY_A)){
            // Se move para a esquerda
            if(map[player.pos_x][player.pos_y-1] != CHAR_PAREDE && map[player.pos_x][player.pos_y-1] == CHAR_ESPACO_LIVRE) {
                map[player.pos_x][player.pos_y]=CHAR_ESPACO_LIVRE;
                player.pos_y-=1;
                map[player.pos_x][player.pos_y]=CHAR_JOGADOR;
            }
        }
        if(IsKeyDown(KEY_D)){
            // Se move para a direita
            if(map[player.pos_x][player.pos_y+3] != CHAR_PAREDE && map[player.pos_x][player.pos_y+3] == CHAR_ESPACO_LIVRE) {
                map[player.pos_x][player.pos_y]=CHAR_ESPACO_LIVRE;
                player.pos_y+=1;
                map[player.pos_x][player.pos_y]=CHAR_JOGADOR;
            }
        }
        if(IsKeyDown(KEY_W)){
            // Se move para cima
            if(map[player.pos_x-1][player.pos_y] != CHAR_PAREDE && map[player.pos_x-1][player.pos_y] == CHAR_ESPACO_LIVRE) {
                map[player.pos_x][player.pos_y]=CHAR_ESPACO_LIVRE;
                player.pos_x-=1;
                map[player.pos_x][player.pos_y]=CHAR_JOGADOR;
            }
        }
        if(IsKeyDown(KEY_S)){
            if(map[player.pos_x+3][player.pos_y] != CHAR_PAREDE && map[player.pos_x+3][player.pos_y] == CHAR_ESPACO_LIVRE) {
                map[player.pos_x][player.pos_y]=CHAR_ESPACO_LIVRE;
                player.pos_x+=1;
                map[player.pos_x][player.pos_y]=CHAR_JOGADOR;
            }
        }

        movimenta_inimigos(map, inimigos);
        calcula_posTela_enemies(inimigos);


        BeginDrawing();

        ClearBackground(SKYBLUE); // Clear the screen

        // Draw the map on the screen
        for (int i = 0; i < MAP_ROWS; i++)
        {
            for (int j = 0; j < MAP_COLS; j++)
            {
                int x =  j * (SCREEN_WIDTH / MAP_COLS);
                int y =   i * ((SCREEN_HEIGHT/ MAP_ROWS)-5);

                // Draw each character of the map
                switch (map[i][j])
                {
                case CHAR_PAREDE:
                    DrawRectangle(x, y, SCREEN_WIDTH / MAP_COLS, (SCREEN_HEIGHT / MAP_ROWS) -5, DARKBROWN);
                    break;

                case CHAR_JOGADOR:
                    isaac_pos.x = x;
                    isaac_pos.y = y;
                    DrawRectangle(x, y, (SCREEN_WIDTH) / MAP_COLS, (SCREEN_HEIGHT/ MAP_ROWS)-5, BEIGE);
                    break;

                case CHAR_INIMIGO:
                    // Rectangle enemy_rec={x,y,20,20};
                    // DrawRectangleRec(enemy_rec,RED);

                    // inimigos[generico].pos_tela.x = x;
                    // inimigos[generico].pos_tela.y = y;
                    // DrawTextureRec(inimigos[generico].enemy_texture, inimigos[generico].enemy_source, inimigos[generico].pos_tela, WHITE);
                    // generico++;
                    DrawRectangle(x, y, (SCREEN_WIDTH) / MAP_COLS, (SCREEN_HEIGHT/ MAP_ROWS)-5, BEIGE);
                    break;
                case CHAR_ESPACO_LIVRE:
                    DrawRectangle(x, y, (SCREEN_WIDTH) / MAP_COLS, (SCREEN_HEIGHT/ MAP_ROWS)-5, BEIGE);
                    break;

                default:
                    DrawRectangle(x, y, (SCREEN_WIDTH) / MAP_COLS, (SCREEN_HEIGHT/ MAP_ROWS)-5, BEIGE);
                    break;
                }
            }
        }
        generico=0;
        desenha_inimigos(inimigos);

        // Desenha o isaac
        DrawTextureRec(isaac, isaac_source, isaac_pos, WHITE);

        // Impressao dos textos
        calcula_tempo(init, &segundos, &minutos);
        DrawText(TextFormat("Timer: %02dmin e %02ds", minutos, segundos) , 10, SCREEN_HEIGHT-140, 35,BLACK);
        DrawText(TextFormat("Vidas: %1d", player.vidas), 10, SCREEN_HEIGHT - 45, 35, BLACK);
        DrawText(TextFormat("Barra de especial"), SCREEN_WIDTH-750, SCREEN_HEIGHT - 120, 35, RED);
        Rectangle especial_rec={SCREEN_WIDTH-750,SCREEN_HEIGHT-80,310,35};
        DrawRectangleLinesEx(especial_rec, 5 ,RED);
        DrawText(TextFormat("Fase: %02d", contador_fase), SCREEN_WIDTH-150, SCREEN_HEIGHT-140, 35,BLACK);
        DrawText(TextFormat("Inimigos: %02d", num_inimigos), SCREEN_WIDTH-200, SCREEN_HEIGHT - 45, 35, BLACK);


        EndDrawing();
    }

    // Clean up resources and close the window
    UnloadTexture(isaac);
    CloseWindow();

    return 0;
}
