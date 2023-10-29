#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Produto {
    char nome[50];
    double preco;
    int ID;
    struct Produto *proximoProduto;
};

typedef struct Produto Produto;
typedef Produto *ProdutoPtr;

struct Admin {
    char login[50];
    char senha[50];
};

typedef struct Admin Admin;
typedef Admin *AdminPtr;

struct Usuarios {
    char nome[50];
    char login[50];
    char senha[50];
    Produto compras[10];
    int ID;
    struct Usuarios *proximoUsuario;
};

typedef struct Usuarios Usuarios;
typedef Usuarios *UsuariosPtr;

void inserirUsuarios(UsuariosPtr *ptrParaInicio);
void inserirDadosParaUsuario(UsuariosPtr *ptrParaInicio, UsuariosPtr ptrNovaEstrutura);
void deletarUsuario(UsuariosPtr *ptrParaInicio);
int verificarLoginUsuario(UsuariosPtr ptrInicioEstrutura);
int verificarLoginDisponivel(UsuariosPtr *ptrInicioEstrutura, UsuariosPtr ptrNovaEstrutura);
void inserirProdutos(ProdutoPtr *ptrParaInicio);
void consultarPedidosUsuario(UsuariosPtr *ptrParaInicio);
void inserirDadosParaProduto(ProdutoPtr ptrNovaEstrutura);
void deletarProduto(ProdutoPtr *ptrParaInicio, AdminPtr estrutura);
void comprarProduto(ProdutoPtr *ptrParaInicioProduto, UsuariosPtr *ptrParaInicioUsuarios);
int verificarSenhaAdmin(AdminPtr estrutura);
void inicializarAdmin(AdminPtr admin);
void imprimirProdutos(ProdutoPtr *ptrParaInicio);
void imprimirDadosUsuario(UsuariosPtr *ptrParaInicio);
void inicializarEcomerce();
void menu();
void instrucoesUsuarios(); 
void intrucoesAdministrador();
void escolhaMenu(UsuariosPtr *ptrParaInicioUsuarios, ProdutoPtr *ptrParaInicioProdutos, AdminPtr admin);
void escolhaMenuAdmin(UsuariosPtr *ptrParaInicioUsuarios, ProdutoPtr *ptrParaInicioProdutos, AdminPtr admin);
void escolhaMenuUsuario(UsuariosPtr *ptrParaInicioUsuarios, ProdutoPtr *ptrParaInicioProdutos, AdminPtr admin);
void limparTela();
void esperarEnter();
void mostrarUsuario(UsuariosPtr novoUsuario);
void mostrarProduto(ProdutoPtr novoProduto);

int main(void) {

    UsuariosPtr inicioUsuarios = NULL;
    Admin admin;
    ProdutoPtr inicioProdutos = NULL; 

    inicializarEcomerce();

    inicializarAdmin(&admin);

    escolhaMenu(&inicioUsuarios, &inicioProdutos, &admin);

}

void inserirUsuarios(UsuariosPtr *ptrParaInicio) {

    UsuariosPtr ptrNovaEstrutura = malloc( sizeof(Usuarios) );
    UsuariosPtr ptrEstruturaAnterior = NULL;
    UsuariosPtr ptrEstruturaAtual;
    
    inserirDadosParaUsuario(ptrParaInicio, ptrNovaEstrutura);

    ptrEstruturaAtual = *ptrParaInicio;

    if( ptrEstruturaAtual == NULL ) {
        *ptrParaInicio = ptrNovaEstrutura;
        ptrNovaEstrutura->proximoUsuario = NULL;
        ptrNovaEstrutura->ID = 1;

        mostrarUsuario(ptrNovaEstrutura);
        esperarEnter();
    } 
    else {
        
        while( ( ptrEstruturaAtual != NULL ) ) {
            ptrEstruturaAnterior = ptrEstruturaAtual;
            ptrEstruturaAtual = ptrEstruturaAtual->proximoUsuario;

            if( ( ptrEstruturaAtual != NULL ) && ( ptrEstruturaAtual->ID - ptrEstruturaAnterior->ID ) != 1 ) {
                ptrNovaEstrutura->ID = ptrEstruturaAnterior->ID + 1;
                break; 
            }
        }

        if( ptrEstruturaAtual == NULL ) {
            ptrNovaEstrutura->ID = ptrEstruturaAnterior->ID + 1;
            ptrEstruturaAnterior->proximoUsuario = ptrNovaEstrutura;
            ptrNovaEstrutura->proximoUsuario = NULL;
        }
        else {
            ptrEstruturaAnterior->proximoUsuario = ptrNovaEstrutura;
            ptrNovaEstrutura->proximoUsuario = ptrEstruturaAtual;       
        }

        mostrarUsuario(ptrNovaEstrutura);
        esperarEnter();
    }
}

void mostrarUsuario(UsuariosPtr novoUsuario) {

    limparTela();

    printf("======================================================================\n");
    printf("||                                                                  ||\n");
    printf("||                        DADOS DO NOVO USUARIO                     ||\n");
    printf("||                                                                  ||\n");
    printf("======================================================================\n");
    printf("||                                                                  ||\n");
    printf("|| NOME: %-15s                                            ||\n", novoUsuario->nome);
    printf("|| LOGIN: %-15s                                           ||\n", novoUsuario->login);
    printf("|| ID: %-3d                                                          ||\n", novoUsuario->ID);
    printf("======================================================================\n");

}

void deletarUsuario(UsuariosPtr *ptrParaInicio) {

    limparTela();

    UsuariosPtr atual = *ptrParaInicio;
    UsuariosPtr anterior = NULL;
    UsuariosPtr guardarPtr = NULL;

    if( atual == NULL) {
        printf("\n\n\033[31mATENCAO: Nao ha nenhum usuario cadastrado para ser deletado ainda.\033[0m\n\n");
        esperarEnter();
    }
    else {

        int ID;

        printf("=================================================================\n");
        printf("||                 ANTES DE DELETAR UM USUARIO                 ||\n");
        printf("||                    CONFIRME O SEU LOGIN                     ||\n");
        printf("=================================================================\n");
        printf("||                                                             ||\n");
        
        printf("|| Informe o seu ID: ");
        scanf("%d", &ID);

        while( atual != NULL && atual->ID != ID ) {
            anterior = atual;
            atual = atual->proximoUsuario;
        }

        if( atual == NULL) {
            printf("\n\n\033[31mNao foi possivel encontrar o ID fornecido.\033[0m\n\n");
            esperarEnter();
        }
        else if( atual->ID == ID ) {

            if( verificarLoginUsuario(atual) ) {

                printf("\n\033[32mAcesso permitido. A conta sera deletada.\033[0m\n");

                if(anterior == NULL) {
                    guardarPtr = *ptrParaInicio;
                    *ptrParaInicio = (*ptrParaInicio)->proximoUsuario;
                    free(guardarPtr);
                }
                else {
                    anterior->proximoUsuario = atual->proximoUsuario;
                    free(atual);
                }

                esperarEnter();
            } 
            else {
                printf("\n\n\033[31mVoce nao tem permissao para deletar essa conta.\033[0m\n\n");
                esperarEnter();
            }
        } 
        else {
            printf("\n\n\033[31mNao foi possivel encontrar esse ID na lista.\033[0m\n\n");
            esperarEnter();
        }
    }
}

int verificarLoginUsuario(UsuariosPtr ptrEstrutura) {

    char login[50];
    char senha[50];

    limparTela();

    printf("======================================================================\n");
    printf("||                                                                  ||\n");
    printf("||                 ANTES DE CONTINUAR, REALIZE O SEU LOGIN          ||\n");
    printf("||                                                                  ||\n");
    printf("======================================================================\n");
    printf("||                                                                  ||\n");
    
    printf("|| Digite o login: ");
    scanf(" %[^\n]", login);

    printf("|| Digite a senha: ");
    scanf(" %[^\n]", senha);

    if( strcmp( login, ptrEstrutura->login ) == 0 ) {

        if( strcmp( senha, ptrEstrutura->senha ) == 0 ) {
            return 1;
        }
    }

    return 0;
}

void inserirDadosParaUsuario(UsuariosPtr *ptrParaInicio, UsuariosPtr ptrNovaEstrutura) {

    limparTela();

    printf("======================================================================\n");
    printf("||                                                                  ||\n");
    printf("||                 INSIRA OS DADOS DO NOVO USUARIO                  ||\n");
    printf("||                                                                  ||\n");
    printf("======================================================================\n");
    printf("||                                                                  ||\n");
    printf("|| Digite o seu nome: ");
    scanf(" %[^\n]", ptrNovaEstrutura->nome);

    printf("|| Digite o seu usuario para o login: ");
    scanf(" %[^\n]", ptrNovaEstrutura->login);

    while( verificarLoginDisponivel(ptrParaInicio, ptrNovaEstrutura) == 0 ){

        printf("\n\n\033[31m ATENCAO: Login ja cadastrado. Digite um novo login.\033[0m\n\n");

        printf("|| Login: ");
        scanf(" %[^\n]", ptrNovaEstrutura->login);
    }

    printf("|| Digite a sua senha: ");
    scanf(" %[^\n]", ptrNovaEstrutura->senha);

    for(int i = 0; i < 10; i++) {
        ptrNovaEstrutura->compras[i].ID = 0;
    }
}

int verificarLoginDisponivel(UsuariosPtr *ptrInicioEstrutura, UsuariosPtr ptrNovaEstrutura) {

    UsuariosPtr atual = *ptrInicioEstrutura;

    while( ( atual != NULL ) ) {
        
        if( ( strcmp( atual->login, ptrNovaEstrutura->login ) == 0 ) ) {
            return 0;
        }
        
        atual = atual->proximoUsuario;
    }

    return 1;
}


void inserirProdutos(ProdutoPtr *ptrParaInicio) {

    limparTela();

    char escolha = 'S';

    while ( toupper(escolha) == 'S' ) {

        ProdutoPtr ptrNovaEstrutura = malloc( sizeof(Produto) );
        ProdutoPtr ptrEstruturaAnterior = NULL;
        ProdutoPtr ptrEstruturaAtual;
        
        inserirDadosParaProduto(ptrNovaEstrutura);

        ptrEstruturaAtual = *ptrParaInicio;

        if( ptrEstruturaAtual == NULL) {
            *ptrParaInicio = ptrNovaEstrutura;
            ptrNovaEstrutura->proximoProduto = NULL;
            ptrNovaEstrutura->ID = 1;
        } 
        else {
            
            while( ( ptrEstruturaAtual != NULL ) ) {
                ptrEstruturaAnterior = ptrEstruturaAtual;
                ptrEstruturaAtual = ptrEstruturaAtual->proximoProduto;

                if( ( ptrEstruturaAtual != NULL ) && ( ptrEstruturaAtual->ID - ptrEstruturaAnterior->ID != 1 ) ) {
                    ptrNovaEstrutura->ID = ptrEstruturaAnterior->ID + 1;
                    break;
                }
            }

            if( ptrEstruturaAtual == NULL ) {
                ptrNovaEstrutura->ID = ptrEstruturaAnterior->ID + 1;
                ptrEstruturaAnterior->proximoProduto = ptrNovaEstrutura;
                ptrNovaEstrutura->proximoProduto = NULL;
            }
            else {
                ptrEstruturaAnterior->proximoProduto = ptrNovaEstrutura;
                ptrNovaEstrutura->proximoProduto = ptrEstruturaAtual;        
            }
        }

        mostrarProduto(ptrNovaEstrutura);
        esperarEnter();

        printf("Deseja inserir outro produto? [S/N]: ");
        scanf("%c", &escolha);
    }
   
}

void mostrarProduto(ProdutoPtr novoProduto) {

    limparTela();

    printf("======================================================================\n");
    printf("||                                                                  ||\n");
    printf("||                        DADOS DO NOVO PRODUTO                     ||\n");
    printf("||                                                                  ||\n");
    printf("======================================================================\n");
    printf("||                                                                  ||\n");
    printf("|| NOME: %-15s                                            ||\n", novoProduto->nome);
    printf("|| PRECO: R$%-15.2lf                                         ||\n", novoProduto->preco);
    printf("|| ID: %-3d                                                          ||\n", novoProduto->ID);
    printf("======================================================================\n");
    
    printf("\n\n\033[32mProduto cadastrado!! \033[0m \n\n");

}

void inserirDadosParaProduto(ProdutoPtr ptrNovaEstrutura) {

    limparTela();

    printf("======================================================================\n");
    printf("||                                                                  ||\n");
    printf("||                 INSIRA OS DADOS DO NOVO PRODUTO                  ||\n");
    printf("||                                                                  ||\n");
    printf("======================================================================\n");
    printf("||                                                                  ||\n");
    
    printf("|| Nome: ");
    scanf(" %[^\n]", ptrNovaEstrutura->nome);

    printf("|| Preco: ");
    scanf("%lf", &ptrNovaEstrutura->preco);

    while(ptrNovaEstrutura->preco <= 0) {
        printf("\n\n\033[31m ATENCAO: Preco invalido. Digite um valor maior que zero.\033[0m\n\n");
        
        printf("|| Preco: ");
        scanf("%lf", &ptrNovaEstrutura->preco);
    }

    printf("======================================================================\n");
}

void deletarProduto(ProdutoPtr *ptrParaInicio, AdminPtr estrutura) {

    limparTela();

    char escolha = 'S';

    while( toupper(escolha) == 'S' ) {

        ProdutoPtr atual = *ptrParaInicio;
        ProdutoPtr anterior = NULL;
        ProdutoPtr guardarPtr = NULL;

        if( atual == NULL ){
            printf("\n\n\033[31mATENCAO: Nao ha nenhum produto cadastrado para ser excluido.\033[0m\n\n");
            esperarEnter();
        }
        else {

            int ID;

            imprimirProdutos(ptrParaInicio);

            printf("Digite o ID do produto para ser deletado: ");
            scanf("%d", &ID);

            while( atual != NULL && atual->ID != ID ) {
                anterior = atual;
                atual = atual->proximoProduto;
            }

            if(atual == NULL){
                printf("\n\n\033[31mID invalido. Nenhum produto encontrado.\033[0m\n\n");
                esperarEnter();
            }
            else if( atual->ID == ID ) {

                if( anterior == NULL ) {
                    guardarPtr = *ptrParaInicio;
                    *ptrParaInicio = (*ptrParaInicio)->proximoProduto;
                    free(guardarPtr);
                }
                else {
                    anterior->proximoProduto = atual->proximoProduto;
                    free(atual);
                }

                printf("\n\n\033[32mProduto deletado com sucesso!!\033[0m\n\n");
                esperarEnter();          
            }
            else {
                printf("\n\n\033[31mO ID nao foi encontrado. \033[0m\n\n");
                esperarEnter();
            }
        }  

        printf("\nGostaria de deletar outro produto? [S/N]: ");
        scanf("%c", &escolha);
    }
}

void comprarProduto( ProdutoPtr *ptrParaInicioProduto, UsuariosPtr *ptrParaInicioUsuarios ) {

    limparTela();

    ProdutoPtr atualProduto = *ptrParaInicioProduto;
    UsuariosPtr atualUsuarios = *ptrParaInicioUsuarios;

    if( atualProduto == NULL || atualUsuarios == NULL ) {
        printf("\n\n\033[31mATENCAO: Operacao invalida. Nao ha produtos ou usuarios definidos.\033[0m\n\n");
        esperarEnter();
    }
    else {

        char continuar = 'S';

        while(  toupper(continuar) == 'S' ) {

            imprimirProdutos(ptrParaInicioProduto);

            int escolha = 0;

            printf("|| Digite o ID do produto que deseja comprar: ");
            scanf("%d", &escolha);

            while( atualProduto != NULL && atualProduto->ID != escolha ) {
                atualProduto = atualProduto->proximoProduto;
            }

            if( atualProduto == NULL ) {
                printf("\n\n\033[31mID nao encontrado na lista de produtos.\033[0m\n\n");
                esperarEnter();
            }
            else {

                int id = 0;

                printf("|| Digite o seu ID de usuario: ");
                scanf("%d", &id);

                while( atualUsuarios != NULL && atualUsuarios->ID != id ) {
                    atualUsuarios = atualUsuarios->proximoUsuario;
                }

                if( atualUsuarios == NULL ) {
                    printf("\n\n\033[31mID de usuario nao encontrado na lista.\033[0m\n\n");
                    esperarEnter();
                }
                else {
                    
                    printf("=========================================================\n");
                    printf("||              ANTES DE REALIZAR A COMPRA             ||\n");
                    printf("||                    FACA O SEU LOGIN                 ||\n");
                    printf("=========================================================\n");
                    printf("||                                                     ||\n");

                    if( verificarLoginUsuario( atualUsuarios ) ) {
                        
                        limparTela();

                        printf("\n\n\033[32mLogin efetuado com sucesso!! Adicionando pedido a conta\033[0m\n\n");

                        for(int i = 0; i < 10; i++) {

                            if( atualUsuarios->compras[i].ID == 0 ){
                                atualUsuarios->compras[i] = *atualProduto;
                                break;
                            }
                        }

                        printf("========================================================================\n");
                        printf("||                           DETALHES DA COMPRA                       ||\n");
                        printf("========================================================================\n");
                        printf("|| Nome: %-20s                                         ||\n", atualUsuarios->nome);
                        printf("|| ID: %-3d                                                            ||\n", atualUsuarios->ID);
                        printf("|| Produto: %-20s                                      ||\n", atualProduto->nome);
                        printf("|| Preco: %-10.2f                                                  ||\n", atualProduto->preco);
                        printf("========================================================================\n");
                        
                        esperarEnter();
                    }
                    else {
                        printf("\n\n\033[31mVoce nao tem permissao para acessar essa conta.\033[0m\n\n");
                        esperarEnter();
                    }
                }
            }

            printf("\nGostaria de comprar outro produto? [S/N]: ");
            scanf("%c", &continuar);
        }        
    }
}

void consultarPedidosUsuario(UsuariosPtr *ptrParaInicio) {

    limparTela();

    UsuariosPtr atual = *ptrParaInicio;
    int id = 0;

    if(  atual == NULL ) {
        printf("\n\n\033[31mATENCAO: Ainda nao ha nenhum usuario definido.\033[0m\n\n");
        esperarEnter();
    }
    else {

        printf("====================================================================\n");
        printf("||              INFORME O SEU ID PARA CONTINUAR                   ||\n");
        printf("====================================================================\n");
        printf("||                                                                ||\n");
        
        printf("|| Digite o seu ID: ");
        scanf("%d", &id);

        while( atual != NULL && atual->ID != id) {
            atual = atual->proximoUsuario;
        }

        if( atual == NULL ) {
            printf("\n\n\033[31mID nao encontrado dentro da lista de usuarios.\033[0m\n\n");
            esperarEnter();
        } 
        else {

            if( verificarLoginUsuario( atual ) ) {

                printf("\n\033[32mLogin efetuado com sucesso!\033[0m\n\nProdutos comprados:\n\n");

                if( atual->compras[0].ID == 0 ){
                    printf("\n\n\033[31mNenhum produto foi comprado ainda.\033[0m\n\n");
                    esperarEnter();
                }
                else {

                    for(int i = 0; atual->compras[i].ID != 0; i++) {

                        printf("=========================================================\n");
                        printf("||                       PRODUTO %d                     ||\n", (i + 1));
                        printf("=========================================================\n");
                        printf("|| Nome: %-20s                          ||\n", atual->compras[i].nome);
                        printf("|| Preco: %-10.2f                                   ||\n", atual->compras[i].preco);
                        printf("|| ID: %-3d                                             ||\n", atual->compras[i].ID);
                        printf("=========================================================\n");
                    }
                    esperarEnter();
                }
            }
            else {
                printf("\n\n\033[31mVoce nao tem permissao para acessar essa conta.\033[0m\n\n");
                esperarEnter();
            }
        }
    }
}

int verificarSenhaAdmin(AdminPtr estrutura) {

    char login[50];
    char senha[50];

    printf("===========================================\n");
    printf("||                                       ||\n");
    printf("||       INSIRA O SEU LOGIN DE ADMIN     ||\n");
    printf("||                                       ||\n");
    printf("===========================================\n");
    
    printf("|| Login: ");
    scanf(" %[^\n]", login );

    printf("|| Senha: ");
    scanf(" %[^\n]", senha);

    printf("===========================================\n");

    if ( strcmp( login, estrutura->login ) == 0) {

        if( strcmp( senha, estrutura->senha ) == 0) {
            printf("\n\n\033[32mAcesso permitido!!\033[0m\n\n");
            esperarEnter();
            return 1;
        }
    }

    return 0;
}

void inicializarEcomerce() {

    limparTela();

    printf("=====================================================\n");
    printf("||                     E-COMMERCE                  ||\n");
    printf("=====================================================\n");

    printf("\n\033[32mPara inicializar o aplicativo, crie a sua conta de admin.\033[0m\n");
}

void inicializarAdmin(AdminPtr admin) {

    printf("\033[32mPressione enter para realizar o cadastro\033[0m\n");
    getchar();

    printf("======================================================\n");
    printf("||   DIGITE O SEU LOGIN DE ADMINISTRADOR ABAIXO     ||\n");
    printf("======================================================\n");
    printf("||                                                  ||\n");
    
    printf("|| Login: ");
    scanf(" %[^\n]", admin->login);

    printf("|| Senha: ");
    scanf(" %[^\n]", admin->senha);

    printf("======================================================\n");

    printf("\n\n\033[32mConta criada com sucesso!\033[0m\n\n");
    esperarEnter();
}

void intrucoesAdministrador() {

    limparTela();

    printf("=================================================================\n");
    printf("||                                                             ||\n");
    printf("||                ESCOLHA UMA DAS OPCOES ABAIXO                ||\n");
    printf("||                                                             ||\n");
    printf("=================================================================\n");
    printf("||                                                             ||\n");
    printf("||  [1] Cadastrar produto                                      ||\n");
    printf("||  [2] Remover produto                                        ||\n");
    printf("||  [3] Consultar produtos                                     ||\n");
    printf("||  [0] Sair da conta                                          ||\n");
    printf("=================================================================\n");
}

void menu() {

    limparTela();

    printf("==========================================================\n");
    printf("||                                                      ||\n");
    printf("||                 ESCOLHA A FORMA DE LOGIN             ||\n");
    printf("||                                                      ||\n");
    printf("==========================================================\n");
    printf("||                                                      ||\n");
    printf("||  [1] Administrador                                   ||\n");
    printf("||  [2] Cliente                                         ||\n");
    printf("||  [0] Encerrar o programa                             ||\n");
    printf("==========================================================\n");
}

void escolhaMenu(UsuariosPtr *ptrParaInicioUsuarios, ProdutoPtr *ptrParaInicioProdutos, AdminPtr admin) {

    int escolha = 1;

    while( escolha != 0 ) {
        
        menu();
        printf("\nDigite a sua escolha: ");
        scanf("%d", &escolha);

        switch( escolha ) {
            case 1:
                escolhaMenuAdmin(ptrParaInicioUsuarios, ptrParaInicioProdutos, admin);
                break;
            case 2:
                escolhaMenuUsuario(ptrParaInicioUsuarios, ptrParaInicioProdutos, admin);
                break;
            case 0:
                printf("\n\n\033[31mEncerrando o programa...\033[0m\n\n");
                exit(0);
                break;
            default:
                printf("\n\n\033[31mValor invalido. Insira uma opcao do menu.\033[0m\n\n");
                esperarEnter();
                break;
        }
    }
}

void escolhaMenuAdmin(UsuariosPtr *ptrParaInicioUsuarios, ProdutoPtr *ptrParaInicioProdutos, AdminPtr admin) {

    limparTela();

    int escolha = 1;

    if( verificarSenhaAdmin(admin) == 1 ) {
        
        while( escolha != 0 ) {
            
            intrucoesAdministrador();

            printf("Digite a sua escolha: ");
            scanf("%d", &escolha);

            switch( escolha ) {
                case 1:
                    inserirProdutos(ptrParaInicioProdutos);
                    break;
                case 2:
                    deletarProduto(ptrParaInicioProdutos, admin);
                    break;
                case 3:
                    imprimirProdutos(ptrParaInicioProdutos);
                    break;
                case 0:
                    escolhaMenu(ptrParaInicioUsuarios, ptrParaInicioProdutos, admin);
                    break;
                default:
                    printf("\n\n\033[31mValor invalido. Insira uma opcao do menu.\033[0m\n\n");
                    esperarEnter();
                    break;
            }
        }
    }
    else {
        printf("\n\n\033[31mVoce nao tem permissao para acessar essas funcionalidades.\033[0m\n\n");
        esperarEnter();
    }
}

void escolhaMenuUsuario(UsuariosPtr *ptrParaInicioUsuarios, ProdutoPtr *ptrParaInicioProdutos, AdminPtr admin) {

    int escolha = 1;

    while( escolha != 0 ) {
        
        instrucoesUsuarios();
        printf("Digite a sua escolha: ");
        scanf("%d", &escolha);

        switch( escolha ) {
            case 1:
                imprimirProdutos(ptrParaInicioProdutos);
                break;
            case 2:
                inserirUsuarios(ptrParaInicioUsuarios);
                break;
            case 3:
                deletarUsuario(ptrParaInicioUsuarios);
                break;
            case 4:
                comprarProduto(ptrParaInicioProdutos, ptrParaInicioUsuarios);
                break;
            case 5:
                consultarPedidosUsuario(ptrParaInicioUsuarios);
                break;
            case 6:              
                imprimirDadosUsuario(ptrParaInicioUsuarios);
                break;
            case 0:
                escolhaMenu(ptrParaInicioUsuarios, ptrParaInicioProdutos, admin);
                break;
            default:
                printf("\n\n\033[31mValor invalido. Insira uma opcao do menu.\033[0m\n\n");
                esperarEnter();
                break;
        }
    }
}

void instrucoesUsuarios() {

    limparTela();

    printf("=================================================================\n");
    printf("||                                                             ||\n");
    printf("||               ESCOLHA UMA DAS OPCOES ABAIXO                 ||\n");
    printf("||                                                             ||\n");
    printf("=================================================================\n");
    printf("||                                                             ||\n");
    printf("||  [1] Consultar produtos                                     ||\n");
    printf("||  [2] Cadastrar usuario                                      ||\n");
    printf("||  [3] Remover usuario                                        ||\n");
    printf("||  [4] Comprar produto                                        ||\n");
    printf("||  [5] Consultar pedidos                                      ||\n");
    printf("||  [6] Consultar dados de um cliente                          ||\n");
    printf("||  [0] Sair da conta                                          ||\n");
    printf("=================================================================\n");
}

void imprimirProdutos(ProdutoPtr *ptrParaInicio) {

    limparTela();

    ProdutoPtr atual = *ptrParaInicio;

    int i = 1;

    if( atual == NULL ) {
        printf("\n\n\033[31mATENCAO: Nao ha nenhum produto para ser exibido.\033[0m\n\n");
        esperarEnter();
    } 
    else {

        while( atual != NULL ) {
            
            printf("====================================================================\n");
            printf("||                            PRODUTO %-3d                         ||\n", i);
            printf("====================================================================\n");
            printf("|| Nome: %-20s                                     ||\n", atual->nome);
            printf("|| Preco: R$%-10.2lf                                            ||\n", atual->preco);
            printf("|| ID: %d                                                          ||\n", atual->ID);
            printf("||                                                                ||\n");
            printf("====================================================================\n");

            printf("\n\n");

            atual = atual->proximoProduto;
            i++;
        }

        esperarEnter();
    }
}

void imprimirDadosUsuario(UsuariosPtr *ptrParaInicio) {

    limparTela();

    UsuariosPtr atual = *ptrParaInicio;

    if( atual == NULL ) {
        printf("\n\n\033[31mATENCAO: Nao ha ainda nenhum usuario para ser consultado. \033[0m\n\n");
        esperarEnter();
    }
    else {
        
        int ID = 0;

        printf("=========================================================\n");
        printf("||              ANTES DE CONSULTAR OS DADOS            ||\n");
        printf("||                    FACA O SEU LOGIN                 ||\n");
        printf("=========================================================\n");
        printf("||                                                     ||\n");

        printf("|| Digite o seu ID: ");
        scanf("%d", &ID);

        while( atual != NULL &&  atual->ID != ID) {
            atual = atual->proximoUsuario;
        }

        if( atual != NULL && atual->ID == ID ) {

            if( verificarLoginUsuario(atual) ) {
                
                limparTela();

                printf("\n\n\033[32mAcesso permitido.\033[0m\n\n");
        
                printf("=================================================\n");
                printf("||                                             ||\n");
                printf("||                DADOS DA CONTA               ||\n");
                printf("||                                             ||\n");
                printf("=================================================\n");
                printf("||                                             ||\n");
                printf("|| Nome: %-20s                  ||\n", atual->nome);
                printf("|| Login: %-20s                 ||\n", atual->login);
                printf("|| Senha: %-20s                 ||\n", atual->senha);
                printf("|| ID: %-3d                                     ||\n", atual->ID);
                printf("=================================================\n");
                esperarEnter();
            }
            else {
                printf("\n\n\033[31mVoce nao ter permissao para acessar essa conta.\033[0m\n\n");
                esperarEnter();
            }
        }
        else {            
            printf("\n\n\033[31mNao foi possivel encontrar o ID informado. Por favor, tente novamente.\033[0m\n\n");     
            esperarEnter();
        }
    }
}

void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void esperarEnter() {
    printf("\n\033[32mPressione enter para continuar.\033[0m\n");
    getchar();
    getchar();
}