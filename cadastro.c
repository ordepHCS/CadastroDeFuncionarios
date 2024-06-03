#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMERO_MAXIMO_NOME 50
#define NUMERO_MAXIMO_CARGO 50

typedef struct {
    char nome[NUMERO_MAXIMO_NOME];
    char cargo[NUMERO_MAXIMO_CARGO];
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
    printf("Cadastro removido com sucesso!.\n");
}

void modificarCadastro() {
    Funcionario *funcionarios;
    int quantidadeCadastro = lerTodosOsCadastros(&funcionarios);
    if(quantidadeCadastro == 0) {
        printf("Nao ha cadastros para modificar.\n");
        return;
    }
    printf("Cadastros disponiveis:\n");
    for(int indiceCadastro = 0; indiceCadastro < quantidadeCadastro; indiceCadastro++) {
        printf("%d - Nome: %s, Idade: %d, Cargo: %s, Salario: %.2lf\n", indiceCadastro + 1, funcionarios[indiceCadastro].nome, funcionarios[indiceCadastro].idade, funcionarios[indiceCadastro].cargo, funcionarios[indiceCadastro].salario);
    }
    int indiceParaModificar;

    printf("Digite o numero do cadastro que deseja modificar:\n");
    if(scanf("%d", &indiceParaModificar) != 1 || indiceParaModificar < 1 || indiceParaModificar > quantidadeCadastro) {
        printf("Indice invalido.\n");
        free(funcionarios);
        return;
    }
    getchar();
    indiceParaModificar--;

    printf("Modificando cadastro %d:\n", indiceParaModificar + 1);
    
    //Para modificar o nome.
    printf("Digite o novo nome (ou pressione enter para manter o atual: %s):", funcionarios[indiceParaModificar].nome);

    char nome[NUMERO_MAXIMO_NOME];
    
    fgets(nome, sizeof(nome), stdin);
    if(nome[0] != '\n') {
        nome[strcspn(nome, "\n")] = '\0';
        strcpy(funcionarios[indiceParaModificar].nome,nome);
    }
    //Para modificar a idade.
    printf("Digite a nova idade (ou pressione enter para manter a atual idade: %d):", funcionarios[indiceParaModificar].idade);

    char idadeString[10];

    fgets(idadeString, sizeof(idadeString), stdin);
    if(idadeString[0] != '\n') {
        funcionarios[indiceParaModificar].idade = atoi(idadeString);
    }
    //Para modificar o cargo.
    printf("Digite o novo cargo (ou pressione enter para manter o cargo atual: %s):", funcionarios[indiceParaModificar].cargo);

    char cargo[NUMERO_MAXIMO_CARGO];

    fgets(cargo, sizeof(cargo), stdin);
    if(cargo[0] != '\n') {
        cargo[strcspn(cargo, "\n")] = '\0';
        strcpy(funcionarios[indiceParaModificar].cargo, cargo);
    }
    //Para modificar o salario.
    printf("Digite o novo salario (ou pressione enter para manter o atual salario: %.2lf):", funcionarios[indiceParaModificar].salario);

    char salarioString[20];

    fgets(salarioString, sizeof(salarioString), stdin);
    if(salarioString[0] != '\n') {
        funcionarios[indiceParaModificar].salario = atof(salarioString);
    }

    escreverTodosOsCadastros(funcionarios, quantidadeCadastro);
    free(funcionarios);
    printf("Cadastro modificado com sucesso!.\n");
}

int desejaContinuar() {
    int resposta;
    printf("Deseja realizar outra operacao?: (1 = Sim) (2 = Nao).\n");
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
        printf("3 - Modificar Cadastro.\n");
        printf("4 - Remover Cadastro.\n");

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
                    if(strlen(funcionarios[indiceCadastro].nome) == 0) {
                        printf("Nome nao pode ser vazio.\n");
                        free(funcionarios);
                        return 1;
                    }

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
                    if(strlen(funcionarios[indiceCadastro].cargo) == 0) {
                        printf("Cargo nao pode ser vazio.\n");
                        free(funcionarios);
                        return 1;
                    }
                    
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
                modificarCadastro();
                break;
            case 4:
                removerCadastro();
                break;
            default:
                printf("Opcao Invalida.\n");
                break;
        }
    } while (desejaContinuar());

    return 0;
}