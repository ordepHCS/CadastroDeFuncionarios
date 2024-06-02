//O arquivo.txt precisa existir na sua maquina.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nome[50];
    char cargo[50];
    int idade;
    double salario;
}Funcionario;

void cadastrarFuncionarios(Funcionario funcionarios[], int quantidadeCadastro) {
    FILE *arquivoCadastro = fopen("dadosDosFuncionarios.txt", "a");
    if(arquivoCadastro == NULL) {
        printf("Nao foi possivel abrir o arquivo.\n");
        return;
    }
    for(int indiceCadastro = 0; indiceCadastro < quantidadeCadastro; indiceCadastro++) {
        fprintf(arquivoCadastro, "Nome: %s\nIdade: %d\nCargo: %s\nSalario: %.2lf\n\n", funcionarios[indiceCadastro].nome, funcionarios[indiceCadastro].idade, funcionarios[indiceCadastro].cargo, funcionarios[indiceCadastro].salario);
    }
    fclose(arquivoCadastro);
    printf("Cadastro(s) realizado(s) com sucesso!.\n");
}

void lerCadastros() {
    FILE *arquivoCadastro = fopen("dadosDosFuncionarios.txt", "r");
    if(arquivoCadastro == NULL) {
        printf("Nao foi possivel abrir o arquivo.\n");
        return;
    }
    char linha[1000];

    while(fgets(linha, sizeof(linha), arquivoCadastro)) {
        printf("%s", linha);
    }
    fclose(arquivoCadastro);
}

int lerTodosOsCadastros(Funcionario **funcionarios) {
    FILE *arquivoCadastro = fopen("dadosDosFuncionarios.txt", "r");
    if(arquivoCadastro == NULL) {
        printf("Nao foi possivel abrir o arquivo.\n");
        return 0;
    }
    Funcionario temporario;
    int quantidadeCadastros = 0;
    *funcionarios = NULL;
    
    while(fscanf(arquivoCadastro, "Nome: %s\nIdade: %d\nCargo: %s\nSalario: %lf\n\n", temporario.nome, &temporario.idade, temporario.cargo, &temporario.salario) == 4) {
        *funcionarios = realloc(*funcionarios, (quantidadeCadastros + 1) * sizeof(Funcionario));
        if(*funcionarios == NULL) {
            printf("Erro de alocacao de memoria.\n");
            fclose(arquivoCadastro);
            return quantidadeCadastros;
        }
        (*funcionarios)[quantidadeCadastros] = temporario;
        quantidadeCadastros++;
    }
    fclose(arquivoCadastro);
    return quantidadeCadastros;
}

void escreverTodosOsCadastros(Funcionario *funcionarios, int quantidadeCadastro) {
    FILE *arquivoCadastro = fopen("dadosDosFuncionarios.txt", "w");
    if(arquivoCadastro == NULL) {
        printf("Nao foi possivel abrir o arquivo.\n");
        return;
    }
    for(int indiceCadastro = 0; indiceCadastro < quantidadeCadastro; indiceCadastro++) {
        fprintf(arquivoCadastro, "Nome: %s\nIdade: %d\nCargo: %s\nSalario: %.2lf\n\n", funcionarios[indiceCadastro].nome, funcionarios[indiceCadastro].idade, funcionarios[indiceCadastro].cargo, funcionarios[indiceCadastro].salario);
    }
    fclose(arquivoCadastro);
}

void removerCadastro() {
    Funcionario *funcionarios;
    int quantidadeCadastro = lerTodosOsCadastros(&funcionarios);
    if(quantidadeCadastro == 0) {
        printf("Nao ha cadastros para remover.\n");
        return;
    }
    printf("Cadastros disponiveis:\n");
    for(int indiceCadastro = 0; indiceCadastro < quantidadeCadastro; indiceCadastro++) {
        printf("%d - Nome: %s, Idade: %d, Cargo: %s, Salario: %.2lf\n", indiceCadastro + 1, funcionarios[indiceCadastro].nome, funcionarios[indiceCadastro].idade, funcionarios[indiceCadastro].cargo, funcionarios[indiceCadastro].salario);
    }

    int indiceParaRemover;
    printf("Digite o numero do cadastro que deseja remover:");
    if (scanf("%d", &indiceParaRemover) != 1 || indiceParaRemover < 1 || indiceParaRemover > quantidadeCadastro) {
        printf("Indice invalido.\n");
        free(funcionarios);
        return;
    }
    indiceParaRemover--;

    for(int indiceCadastro = indiceParaRemover; indiceCadastro < quantidadeCadastro - 1; indiceCadastro++) {
        funcionarios[indiceCadastro] = funcionarios[indiceCadastro + 1];
    }
    quantidadeCadastro--;

    escreverTodosOsCadastros(funcionarios, quantidadeCadastro);
    free(funcionarios);
    printf("Cadastro removido com sucesso!\n");
}

int desejaContinuar() {
    int resposta;
    printf("Deseja realizar outra operacao?: (1 = sim) (2 = nao)\n");
    scanf("%d", &resposta);
    
    while(getchar() != '\n');
    if(resposta == 1) {
        return 1;
    }else if(resposta == 2) {
        return 0;
    }else {
        printf("Opcao invalida.\n");
        return desejaContinuar();
    }
}

int main(void) {
    int opcaoCadastro;
    int quantidadeCadastros;

    do {
        printf("Escolha uma opcao:\n");
        printf("1 - Realizar Cadastro.\n");
        printf("2 - Ler Cadastros.\n");
        printf("3 - Remover Cadastro(s).\n");

        if(scanf("%d", &opcaoCadastro) != 1) {
            printf("Opcao Invalida.\n");
            return 1;
        }
        getchar();
        switch(opcaoCadastro) {
            case 1:
                printf("Quantos cadastros deseja fazer?\n");
                if(scanf("%d", &quantidadeCadastros) != 1 || quantidadeCadastros <= 0) {
                    printf("Numero de cadastros invalido.\n");
                    return 1;
                }
                getchar();

                Funcionario *funcionarios = (Funcionario *)malloc(quantidadeCadastros * sizeof(Funcionario));
                if (funcionarios == NULL) {
                    printf("Erro de alocação de memória.\n");
                    return 1;
                }

                for(int indiceCadastro = 0; indiceCadastro < quantidadeCadastros; indiceCadastro++) {
                    printf("Cadastro %d:\n", indiceCadastro + 1);
                    printf("Digite seu nome:\n");
                    fgets(funcionarios[indiceCadastro].nome, sizeof(funcionarios[indiceCadastro].nome), stdin);
                    funcionarios[indiceCadastro].nome[strcspn(funcionarios[indiceCadastro].nome, "\n")] = '\0';

                    printf("Digite sua idade:\n");
                    if (scanf("%d", &funcionarios[indiceCadastro].idade) != 1) {
                        printf("Entrada invalida para idade.\n");
                        free(funcionarios);
                        return 1;
                    }
                    getchar();

                    printf("Digite seu cargo:\n");
                    fgets(funcionarios[indiceCadastro].cargo, sizeof(funcionarios[indiceCadastro].cargo), stdin);
                    funcionarios[indiceCadastro].cargo[strcspn(funcionarios[indiceCadastro].cargo, "\n")] = '\0';

                    printf("Digite seu salario:\n");
                    if (scanf("%lf", &funcionarios[indiceCadastro].salario) != 1) {
                        printf("Entrada invalida para salario.\n");
                        free(funcionarios);
                        return 1;
                    }
                    getchar();
                }
                cadastrarFuncionarios(funcionarios, quantidadeCadastros);
                free(funcionarios);
                break;
            case 2:
                lerCadastros();
                break;
            case 3:
                removerCadastro();
                break;
            default:
                printf("Opcao Invalida.\n");
                break;
        }
    } while (desejaContinuar());

    return 0;
}