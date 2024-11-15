#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 50
#define MAX_GENRE 20
#define MAX_NAME 50
#define MAX_FAVORITES 10

typedef struct {
    int id;
    char title[MAX_TITLE];
    char genre[MAX_GENRE];
} Video;

typedef struct {
    int id;
    char name[MAX_NAME];
    int favorited_videos[MAX_FAVORITES];
    int num_favorites;
} Usuario;

void adicionar_video(FILE *video_file);
void adicionar_usuario(FILE *user_file);
void listar_videos(FILE *video_file);
void listar_usuarios(FILE *user_file, FILE *video_file);

int main() {
    FILE *video_file, *user_file;

    video_file = fopen("videos.bin", "rb+");
    if (!video_file) video_file = fopen("videos.bin", "wb+");

    user_file = fopen("usuarios.bin", "rb+");
    if (!user_file) user_file = fopen("usuarios.bin", "wb+");

    if (!video_file || !user_file) {
        printf("Erro ao abrir os arquivos binários.\n");
        return 1;
    }

    int opcao;
    do {
        printf("\n=== MENU ===\n");
        printf("1. Adicionar Vídeo\n");
        printf("2. Adicionar Usuário\n");
        printf("3. Listar Vídeos\n");
        printf("4. Listar Usuários\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionar_video(video_file);
                break;
            case 2:
                adicionar_usuario(user_file);
                break;
            case 3:
                listar_videos(video_file);
                break;
            case 4:
                listar_usuarios(user_file, video_file);
                break;
            case 5:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 5);

    // Fechando os arquivos
    fclose(video_file);
    fclose(user_file);

    return 0;
}

void adicionar_video(FILE *video_file) {
    Video video;
    printf("\n=== Adicionar Vídeo ===\n");
    printf("ID: ");
    scanf("%d", &video.id);
    printf("Título: ");
    getchar();
    fgets(video.title, MAX_TITLE, stdin);
    strtok(video.title, "\n");
    printf("Gênero: ");
    fgets(video.genre, MAX_GENRE, stdin);
    strtok(video.genre, "\n");

    fseek(video_file, 0, SEEK_END);
    fwrite(&video, sizeof(Video), 1, video_file);

    printf("Vídeo adicionado com sucesso!\n");
}

void adicionar_usuario(FILE *user_file) {
    Usuario user;
    printf("\n=== Adicionar Usuário ===\n");
    printf("ID: ");
    scanf("%d", &user.id);
    printf("Nome: ");
    getchar(); 
    fgets(user.name, MAX_NAME, stdin);
    strtok(user.name, "\n");

    printf("Quantos vídeos favoritados? ");
    scanf("%d", &user.num_favorites);

    for (int i = 0; i < user.num_favorites; i++) {
        printf("ID do vídeo favorito %d: ", i + 1);
        scanf("%d", &user.favorited_videos[i]);
    }

    fseek(user_file, 0, SEEK_END);
    fwrite(&user, sizeof(Usuario), 1, user_file);

    printf("Usuário adicionado com sucesso!\n");
}

void listar_videos(FILE *video_file) {
    Video video;
    printf("\n=== Lista de Vídeos ===\n");

    fseek(video_file, 0, SEEK_SET);
    while (fread(&video, sizeof(Video), 1, video_file)) {
        printf("ID: %d\n", video.id);
        printf("Título: %s\n", video.title);
        printf("Gênero: %s\n", video.genre);
        printf("-----------------------\n");
    }
}

void listar_usuarios(FILE *user_file, FILE *video_file) {
    Usuario user;
    Video video;

    printf("\n=== Lista de Usuários ===\n");
    fseek(user_file, 0, SEEK_SET);

    while (fread(&user, sizeof(Usuario), 1, user_file)) {
        printf("ID: %d\n", user.id);
        printf("Nome: %s\n", user.name);
        printf("Vídeos Favoritos:\n");

        for (int i = 0; i < user.num_favorites; i++) {
            fseek(video_file, 0, SEEK_SET);
            while (fread(&video, sizeof(Video), 1, video_file)) {
                if (video.id == user.favorited_videos[i]) {
                    printf("  - %s\n", video.title);
                    break;
                }
            }
        }
        printf("-----------------------\n");
    }
}
