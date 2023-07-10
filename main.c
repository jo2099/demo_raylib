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


typedef struct player
{
    int vidas;
    int pos_x;
    int pos_y;

}JOGADOR;

typedef struct enemy
{
    int pos_x;
    int pos_y;
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

int importa_mapa(char fileName[], char matriz[MAP_ROWS][MAP_COLS], JOGADOR *jogador, INIMIGO inimigos[]){

    int error_code;

    char linha[MAP_COLS + 2];
    FILE *mapaFile = fopen(fileName, "r");

    if (mapaFile == NULL){
        error_code = 1;
    }
    else{
        while (!feof(mapaFile))
        {
            for (int i = 0; i < MAP_ROWS; i++)
            {
                fgets(linha, MAP_COLS+2, mapaFile);
                for (int j = 0; j < MAP_COLS; j++)
                {
                    switch (linha[j]){
                        case CHAR_JOGADOR:
                            jogador->pos_x = i;
                            jogador->pos_y = j;
                            break;
                        case CHAR_INIMIGO:
                            inimigos[num_inimigos].vivo = 1;
                            inimigos[num_inimigos].can_fire=1;
                            inimigos[num_inimigos].pos_x = i;
                            inimigos[num_inimigos].pos_y = i;
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

void calcula_tempo(clock_t init, int *segundos, int *minutos){
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
    int minutos = 0, segundos = 0;
    clock_t init = clock();

    JOGADOR player;
    player.vidas = 3;
    INIMIGO inimigos[MAX_ENEMIES];
    char map[MAP_ROWS][MAP_COLS];
    char nome_arquivo[17] = "mapas/mapa01.txt";
    importa_mapa(nome_arquivo, map, &player, inimigos);

    while (!WindowShouldClose())
    {

        // Calculate the starting position for the map to be centered
        int mapStartX = (SCREEN_WIDTH - (SCREEN_WIDTH / MAP_COLS * MAP_COLS)) / 2;
        int mapStartY = (SCREEN_HEIGHT - (SCREEN_HEIGHT / MAP_ROWS * MAP_ROWS)) / 2;

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



        BeginDrawing();

        ClearBackground(SKYBLUE); // Clear the screen

        // Draw the map on the screen
        for (int i = 0; i < MAP_ROWS; i++)
        {
            for (int j = 0; j < MAP_COLS; j++)
            {
                int x = mapStartX + j * (SCREEN_WIDTH / MAP_COLS);
                int y = mapStartY + i * ((SCREEN_HEIGHT/ MAP_ROWS)-5);

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
                    Rectangle enemy_rec={x,y,20,20};
                    DrawRectangleRec(enemy_rec,RED);
                    break;

                default:
                    DrawRectangle(x, y, (SCREEN_WIDTH) / MAP_COLS, (SCREEN_HEIGHT/ MAP_ROWS)-5, BEIGE);
                    break;
                }
            }
        }

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
