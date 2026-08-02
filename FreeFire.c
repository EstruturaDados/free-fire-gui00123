#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ITENS 10

// Estrutura do Item
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
    int prioridade; // Requisito Nível Mestre (1 a 5)
} Item;

// Enumeração para critérios de ordenação
typedef enum {
    ORDEM_NENHUMA,
    ORDEM_NOME,
    ORDEM_TIPO,
    ORDEM_PRIORIDADE
} CriterioOrdenacao;

// --- PROTÓTIPOS DAS FUNÇÕES ---
void adicionarItem(Item mochila[], int *total);
void removerItem(Item mochila[], int *total, CriterioOrdenacao *estadoOrdenacao);
void listarItens(const Item mochila[], int total);
void ordenarMochila(Item mochila[], int total, CriterioOrdenacao criterio, CriterioOrdenacao *estadoOrdenacao);
void buscarBinaria(const Item mochila[], int total, CriterioOrdenacao estadoOrdenacao);

int main() {
    Item mochila[MAX_ITENS];
    int totalItens = 0;
    CriterioOrdenacao estadoOrdenacao = ORDEM_NENHUMA;
    int opcao;

    do {
        printf("\n========================================\n");
        printf("    🎒 MOCHILA FREE FIRE - NÍVEL MESTRE  \n");
        printf("========================================\n");
        printf("1. Adicionar Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens\n");
        printf("4. Ordenar Mochila (Insertion Sort)\n");
        printf("5. Buscar Item por Nome (Busca Binaria)\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionarItem(mochila, &totalItens);
                // Invalida a ordenação prévia ao inserir novo item no final
                estadoOrdenacao = ORDEM_NENHUMA;
                break;

            case 2:
                removerItem(mochila, &totalItens, &estadoOrdenacao);
                break;

            case 3:
                listarItens(mochila, totalItens);
                break;

            case 4: {
                if (totalItens < 2) {
                    printf("\n[AVISO] Necessario pelo menos 2 itens para ordenar.\n");
                    break;
                }
                int opCriterio;
                printf("\n--- MENU DE ORDENACAO ---\n");
                printf("1. Por Nome\n");
                printf("2. Por Tipo\n");
                printf("3. Por Prioridade (Maior -> Menor)\n");
                printf("Escolha o criterio: ");
                scanf("%d", &opCriterio);

                if (opCriterio == 1) {
                    ordenarMochila(mochila, totalItens, ORDEM_NOME, &estadoOrdenacao);
                } else if (opCriterio == 2) {
                    ordenarMochila(mochila, totalItens, ORDEM_TIPO, &estadoOrdenacao);
                } else if (opCriterio == 3) {
                    ordenarMochila(mochila, totalItens, ORDEM_PRIORIDADE, &estadoOrdenacao);
                } else {
                    printf("\n[ERRO] Criterio invalido.\n");
                }
                break;
            }

            case 5:
                buscarBinaria(mochila, totalItens, estadoOrdenacao);
                break;

            case 0:
                printf("\nFechando mochila. Boa sorte no Booyah!\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}

/**
 * Cadastra um novo item com validação de prioridade (1 a 5).
 */
void adicionarItem(Item mochila[], int *total) {
    if (*total >= MAX_ITENS) {
        printf("\n[ERRO] Mochila cheia! Capacidade maxima de 10 itens atingida.\n");
        return;
    }

    printf("\n--- NOVO ITEM ---\n");
    printf("Nome do item: ");
    scanf(" %[^\n]", mochila[*total].nome);

    printf("Tipo (Arma, Cura, Municao, Colete...): ");
    scanf(" %[^\n]", mochila[*total].tipo);

    printf("Quantidade: ");
    scanf("%d", &mochila[*total].quantidade);

    do {
        printf("Prioridade (1 a 5 - sendo 5 a maior): ");
        scanf("%d", &mochila[*total].prioridade);
        if (mochila[*total].prioridade < 1 || mochila[*total].prioridade > 5) {
            printf("Valor invalido! A prioridade deve ser entre 1 e 5.\n");
        }
    } while (mochila[*total].prioridade < 1 || mochila[*total].prioridade > 5);

    (*total)++;
    printf("Item adicionado com sucesso!\n");
}

/**
 * Remove um item deslocando os elementos e atualiza o estado de ordenação se necessário.
 */
void removerItem(Item mochila[], int *total, CriterioOrdenacao *estadoOrdenacao) {
    if (*total == 0) {
        printf("\n[AVISO] Mochila vazia!\n");
        return;
    }

    char nomeRemover[30];
    bool encontrado = false;

    printf("\nDigite o nome do item a remover: ");
    scanf(" %[^\n]", nomeRemover);

    for (int i = 0; i < *total; i++) {
        if (strcmp(mochila[i].nome, nomeRemover) == 0) {
            encontrado = true;
            for (int j = i; j < *total - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            (*total)--;
            printf("Item '%s' removido com sucesso!\n", nomeRemover);
            break;
        }
    }

    if (!encontrado) {
        printf("\n[ERRO] Item '%s' nao encontrado.\n", nomeRemover);
    }
}

/**
 * Exibe os itens cadastrados em formato de tabela.
 */
void listarItens(const Item mochila[], int total) {
    printf("\n=================================================================\n");
    printf("                       INVENTARIO DA MOCHILA                     \n");
    printf("=================================================================\n");
    if (total == 0) {
        printf("Mochila vazia.\n");
        return;
    }

    printf("%-20s | %-15s | %-10s | %-10s\n", "NOME", "TIPO", "QTD", "PRIORIDADE");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < total; i++) {
        printf("%-20s | %-15s | %-10d | %-10d\n",
               mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
    }
}

/**
 * Ordenação por Insertion Sort com contador de comparações.
 */
void ordenarMochila(Item mochila[], int total, CriterioOrdenacao criterio, CriterioOrdenacao *estadoOrdenacao) {
    int comparacoes = 0;

    for (int i = 1; i < total; i++) {
        Item chave = mochila[i];
        int j = i - 1;
        bool trocar = false;

        while (j >= 0) {
            comparacoes++;
            trocar = false;

            if (criterio == ORDEM_NOME) {
                if (strcmp(mochila[j].nome, chave.nome) > 0) trocar = true;
            } else if (criterio == ORDEM_TIPO) {
                if (strcmp(mochila[j].tipo, chave.tipo) > 0) trocar = true;
            } else if (criterio == ORDEM_PRIORIDADE) {
                // Ordenação decrescente de prioridade (da maior para a menor)
                if (mochila[j].prioridade < chave.prioridade) trocar = true;
            }

            if (trocar) {
                mochila[j + 1] = mochila[j];
                j--;
            } else {
                break;
            }
        }
        mochila[j + 1] = chave;
    }

    *estadoOrdenacao = criterio;

    printf("\n----------------------------------------\n");
    printf("Mochila ordenada com sucesso!\n");
    printf("Desempenho: %d comparacoes realizadas.\n", comparacoes);
    printf("----------------------------------------\n");
}

/**
 * Busca Binária por Nome com validação de pré-requisito de ordenação.
 */
void buscarBinaria(const Item mochila[], int total, CriterioOrdenacao estadoOrdenacao) {
    if (total == 0) {
        printf("\n[AVISO] Mochila vazia!\n");
        return;
    }

    // Validação do pré-requisito para busca binária
    if (estadoOrdenacao != ORDEM_NOME) {
        printf("\n[ERRO] A busca binaria exige que a mochila esteja ordenada POR NOME.\n");
        printf("Por favor, acesse a opcao 4 do menu e ordene a mochila por Nome primeiro.\n");
        return;
    }

    char nomeBusca[30];
    printf("\n--- BUSCA BINÁRIA --- \n");
    printf("Digite o nome do item que procura: ");
    scanf(" %[^\n]", nomeBusca);

    int inicio = 0;
    int fim = total - 1;
    int comparacoesBusca = 0;
    bool encontrado = false;

    while (inicio <= fim) {
        comparacoesBusca++;
        int meio = inicio + (fim - inicio) / 2;
        int res = strcmp(mochila[meio].nome, nomeBusca);

        if (res == 0) {
            printf("\n========================================\n");
            printf("🎯 ITEM ENCONTRADO (em %d comparacoes)\n", comparacoesBusca);
            printf("========================================\n");
            printf("Nome:       %s\n", mochila[meio].nome);
            printf("Tipo:       %s\n", mochila[meio].tipo);
            printf("Quantidade: %d\n", mochila[meio].quantidade);
            printf("Prioridade: %d\n", mochila[meio].prioridade);
            printf("========================================\n");
            encontrado = true;
            break;
        }

        if (res < 0) {
            inicio = meio + 1; // Busca na metade direita
        } else {
            fim = meio - 1;    // Busca na metade esquerda
        }
    }

    if (!encontrado) {
        printf("\n[ERRO] Item '%s' nao encontrado (Comparações feitas: %d).\n", nomeBusca, comparacoesBusca);
    }
}



// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.
    // Menu principal com opções:
    // 1. Adicionar um item
    // 2. Remover um item
    // 3. Listar todos os itens
    // 4. Ordenar os itens por critério (nome, tipo, prioridade)
    // 5. Realizar busca binária por nome
    // 0. Sair

    // A estrutura switch trata cada opção chamando a função correspondente.
    // A ordenação e busca binária exigem que os dados estejam bem organizados.

// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).

// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
// Exibe a quantidade de comparações feitas (análise de desempenho).

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
