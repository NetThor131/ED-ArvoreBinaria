#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char nome[50];
    char cdg[50];
    char tmnh[50];
    int nmr;
} Arquivo;

struct NO
{
    Arquivo info;
    struct NO *esq;
    struct NO *dir;
};

typedef struct NO *ArvBin;

ArvBin *cria_ArvBin()
{
    ArvBin *raiz = (ArvBin *)malloc(sizeof(ArvBin));
    if (raiz != NULL)
        *raiz = NULL;
    return raiz;
}

void libera_NO(struct NO *no)
{
    if (no == NULL)
        return;
    libera_NO(no->esq);
    libera_NO(no->dir);
    free(no);
    no = NULL;
}

void libera_ArvBin(ArvBin *raiz)
{
    if (raiz == NULL)
        return;
    libera_NO(*raiz);
    free(raiz);
}

void PED(ArvBin *raiz, FILE **output)
{
    if (raiz == NULL)
        return;
    if (*raiz != NULL)
    {
        if (strcmp("1", (*raiz)->info.tmnh) == 0)
        {
            fprintf(*output, "%i %s %s %s byte\n", (*raiz)->info.nmr, (*raiz)->info.nome, (*raiz)->info.cdg, (*raiz)->info.tmnh);
            PED(&((*raiz)->esq), output);
            PED(&((*raiz)->dir), output);
        }
        else
        {
            fprintf(*output, "%i %s %s %s bytes\n", (*raiz)->info.nmr, (*raiz)->info.nome, (*raiz)->info.cdg, (*raiz)->info.tmnh);
            PED(&((*raiz)->esq), output);
            PED(&((*raiz)->dir), output);
        }
    }
}

void EPD(ArvBin *raiz, FILE **output)
{
    if (raiz == NULL)
        return;
    if (*raiz != NULL)
    {
        if (strcmp("1", (*raiz)->info.tmnh) == 0)
        {
            EPD(&((*raiz)->esq), output);
            fprintf(*output, "%i %s %s %s byte\n", (*raiz)->info.nmr, (*raiz)->info.nome, (*raiz)->info.cdg, (*raiz)->info.tmnh);
            EPD(&((*raiz)->dir), output);
        }
        else
        {
            EPD(&((*raiz)->esq), output);
            fprintf(*output, "%i %s %s %s bytes\n", (*raiz)->info.nmr, (*raiz)->info.nome, (*raiz)->info.cdg, (*raiz)->info.tmnh);
            EPD(&((*raiz)->dir), output);
        }
    }
}

void EDP(ArvBin *raiz, FILE **output)
{
    if (raiz == NULL)
        return;
    if (*raiz != NULL)
    {
        if (strcmp("1", (*raiz)->info.tmnh) == 0)
        {
            EDP(&((*raiz)->esq), output);
            EDP(&((*raiz)->dir), output);
            fprintf(*output, "%i %s %s %s byte\n", (*raiz)->info.nmr, (*raiz)->info.nome, (*raiz)->info.cdg, (*raiz)->info.tmnh);
        }
        else
        {
            EDP(&((*raiz)->esq), output);
            EDP(&((*raiz)->dir), output);
            fprintf(*output, "%i %s %s %s bytes\n", (*raiz)->info.nmr, (*raiz)->info.nome, (*raiz)->info.cdg, (*raiz)->info.tmnh);
        }
    }
}

int insere_ArvBin(ArvBin *raiz, Arquivo arquivo)
{
    if (raiz == NULL)
        return 0;
    struct NO *novo;
    novo = (struct NO *)malloc(sizeof(struct NO));
    if (novo == NULL)
        return 0;
    novo->info = arquivo;
    novo->dir = NULL;
    novo->esq = NULL;
    if (*raiz == NULL)
        *raiz = novo;
    else
    {
        struct NO *atual = *raiz;
        struct NO *ant = NULL;
        while (atual != NULL)
        {
            ant = atual;
            if (strcmp(arquivo.nome, atual->info.nome) == 0)
            {
                if (strcmp(atual->info.cdg, "ro") == 0)
                {
                    free(novo);
                    return 0;
                }
                else
                {
                    atual->info = arquivo;
                    return 1;
                }
            }
            if (strcmp(arquivo.nome, atual->info.nome) > 0)
                atual = atual->dir;
            else
                atual = atual->esq;
        }
        if (strcmp(arquivo.nome, ant->info.nome) > 0)
            ant->dir = novo;
        else
            ant->esq = novo;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    int cntd;
    char nome[50], cdg[50], tmnh[50];
    ArvBin *raiz = cria_ArvBin();
    fscanf(input, "%i", &cntd);
    Arquivo arquivo;
    for (int i = 0; i < cntd; i++)
    {
        fscanf(input, "%s %s %s", nome, cdg, tmnh);
        strcpy(arquivo.nome, nome);
        strcpy(arquivo.cdg, cdg);
        strcpy(arquivo.tmnh, tmnh);
        arquivo.nmr = i;
        insere_ArvBin(raiz, arquivo);
    }

    fprintf(output, "EPD:\n");
    EPD(raiz, &output);
    fprintf(output, "PED:\n");
    PED(raiz, &output);
    fprintf(output, "EDP:\n");
    EDP(raiz, &output);

    fclose(input);
    fclose(output);
}