// Processador de imagens PNM
// Processador de imagens no formato PNM (P2 = tons de cinza, P3 = colorida), em ASCII.
// Opcoes: 1 Escurecer | 2 Clarear | 3 Negativo | 4 Espelhar horizontal |
//         5 Espelhar vertical | 6 Desfocagem | 7 Sobel | 8 Tons de cinza |
//         9 Pixelizar (item extra) | 0 Salvar e sair

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

const int MAX_ALTURA  = 2000;
const int MAX_LARGURA = 2000;
const int MAX_CANAIS  = 3;

// Matrizes tridimensionais [linha][coluna][canal]
unsigned char imagem[MAX_ALTURA][MAX_LARGURA][MAX_CANAIS];
unsigned char copia[MAX_ALTURA][MAX_LARGURA][MAX_CANAIS];

int altura = 0, largura = 0, canais = 1;
string tipo; // "P2" (tons de cinza) ou "P3" (colorida)

// Le o proximo inteiro do arquivo, ignorando comentarios (# ...)
bool proximoInt(ifstream &arq, int &v) {
    while (true) {
        arq >> ws;
        if (arq.peek() == '#') {
            string lixo;
            getline(arq, lixo);
        } else break;
    }
    return (bool)(arq >> v);
}

bool carregarImagem(const string &nome) {
    ifstream arq(nome.c_str());
    if (!arq.is_open()) {
        cout << "Erro: nao foi possivel abrir o arquivo " << nome << "\n";
        return false;
    }
    arq >> tipo;
    if (tipo == "P2") canais = 1;
    else if (tipo == "P3") canais = 3;
    else {
        cout << "Erro: formato nao suportado (use uma imagem PNM P2 ou P3 em ASCII).\n";
        return false;
    }
    int maxval;
    if (!proximoInt(arq, largura) || !proximoInt(arq, altura) || !proximoInt(arq, maxval)) {
        cout << "Erro: cabecalho invalido.\n";
        return false;
    }
    if (altura <= 0 || largura <= 0 || maxval <= 0) {
        cout << "Erro: dimensoes invalidas.\n";
        return false;
    }
    if (altura > MAX_ALTURA || largura > MAX_LARGURA) {
        cout << "Erro: imagem maior que o limite (" << MAX_LARGURA << "x" << MAX_ALTURA
             << "). Aumente MAX_LARGURA/MAX_ALTURA no codigo.\n";
        return false;
    }
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            for (int k = 0; k < canais; k++) {
                int v;
                if (!proximoInt(arq, v)) {
                    cout << "Erro: arquivo terminou antes do esperado.\n";
                    return false;
                }
                if (maxval != 255) v = v * 255 / maxval; // normaliza para 0-255
                if (v < 0) v = 0;
                if (v > 255) v = 255;
                imagem[i][j][k] = (unsigned char)v;
            }
        }
    }
    return true;
}

bool salvarImagem(const string &nome) {
    ofstream arq(nome.c_str());
    if (!arq.is_open()) {
        cout << "Erro: nao foi possivel criar o arquivo " << nome << "\n";
        return false;
    }
    arq << tipo << "\n" << largura << " " << altura << "\n255\n";
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            for (int k = 0; k < canais; k++) {
                arq << (int)imagem[i][j][k];
                arq << (k == canais - 1 ? "\n" : " ");
            }
        }
    }
    return true;
}

void mostrarMenu() {
    cout << "\n===== PROCESSADOR DE IMAGENS =====\n";
    cout << "1 - Escurecer\n";
    cout << "2 - Clarear\n";
    cout << "3 - Inverter cor (negativo)\n";
    cout << "4 - Espelhar na horizontal\n";
    cout << "5 - Espelhar na vertical\n";
    cout << "6 - Desfocagem\n";
    cout << "7 - Filtro Sobel (detectar bordas)\n";
    cout << "8 - Converter para tons de cinza\n";
    cout << "9 - Pixelizar (efeito retro)\n";
    cout << "0 - Salvar e sair\n";
    cout << "Escolha uma opcao: ";
}

int main() {
    string arquivoEntrada, arquivoSaida;
    int opcao;
    int i, j, k, valor;

    cout << "Nome do arquivo de entrada (.pnm): ";
    cin >> arquivoEntrada;
    if (!carregarImagem(arquivoEntrada)) return 1;
    cout << "Imagem " << tipo << " carregada: " << largura << "x" << altura << "\n";

    do {
        mostrarMenu();
        cin >> opcao;

        // Opção 1: Operação de Escurecimento da Imagem
        if (opcao == 1) {
            int fator;
            cout << "Qual o fator de escurecimento (0-255)? ";
            cin >> fator;
            // Percorre todas as linhas, colunas e canais de cores disponíveis
            for (i = 0; i < altura; i++) {
                for (j = 0; j < largura; j++) {
                    for (k = 0; k < canais; k++) {
                        // Subtrai o fator do valor original do pixel
                        valor = (int)imagem[i][j][k] - fator;
                        // Garante a consistência do limite inferior (Truncamento em 0)
                        if (valor < 0) valor = 0;
                        imagem[i][j][k] = (unsigned char)valor;
                    }
                }
            }
            cout << "Efeito de escurecimento aplicado!\n";
        }
        // Opção 2: Operação de Clareamento da Imagem
        else if (opcao == 2) {
            int fator;
            cout << "Qual o fator de clareamento (0-255)? ";
            cin >> fator;
            // Percorre a matriz tridimensional da imagem
            for (i = 0; i < altura; i++) {
                for (j = 0; j < largura; j++) {
                    for (k = 0; k < canais; k++) {
                        // Adiciona o fator ao valor original do pixel
                        valor = (int)imagem[i][j][k] + fator;
                        // Garante a consistência do limite superior (Truncamento em 255)
                        if (valor > 255) valor = 255;
                        imagem[i][j][k] = (unsigned char)valor;
                    }
                }
            }
            cout << "Efeito de clareamento aplicado!\n";
        }
        // Opção 3: Inversão de Cores (Efeito Negativo)
        else if (opcao == 3) {
            for (i = 0; i < altura; i++) {
                for (j = 0; j < largura; j++) {
                    for (k = 0; k < canais; k++) {
                        // Calcula o complemento do valor do pixel em relação ao valor máximo (255)
                        imagem[i][j][k] = (unsigned char)(255 - (int)imagem[i][j][k]);
                    }
                }
            }
            cout << "Efeito negativo aplicado!\n";
        }
        // Opção 4: Espelhamento no Eixo Vertical (Inversão Horizontal)
        else if (opcao == 4) {
            for (i = 0; i < altura; i++) {
                // Percorre apenas até a metade da largura para realizar a troca simétrica
                for (j = 0; j < largura / 2; j++) {
                    for (k = 0; k < canais; k++) {
                        // Swap utilizando variável auxiliar
                        unsigned char swap = imagem[i][j][k];
                        imagem[i][j][k] = imagem[i][largura - 1 - j][k];
                        imagem[i][largura - 1 - j][k] = swap;
                    }
                }
            }
            cout << "Espelhamento horizontal aplicado!\n";
        }
        // Opção 5: Espelhamento no Eixo Horizontal (Inversão Vertical)
        else if (opcao == 5) {
            // Percorre apenas até a metade da altura para realizar a troca simétrica
            for (i = 0; i < altura / 2; i++) {
                for (j = 0; j < largura; j++) {
                    for (k = 0; k < canais; k++) {
                        // Swap utilizando variável auxiliar
                        unsigned char swap = imagem[i][j][k];
                        imagem[i][j][k] = imagem[altura - 1 - i][j][k];
                        imagem[altura - 1 - i][j][k] = swap;
                    }
                }
            }
            cout << "Espelhamento vertical aplicado!\n";
        }
        // Opção 6: Filtro de Desfocagem
        else if (opcao == 6) {
            // Ignora as bordas externas de 1 pixel para evitar leitura fora dos limites da matriz
            for (i = 1; i < altura - 1; i++) {
                for (j = 1; j < largura - 1; j++) {
                    for (k = 0; k < canais; k++) {
                        int some = 0;
                        // Coleta e soma o valor dos 9 vizinhos da janela de convolução
                        for (int dy = -1; dy <= 1; dy++) {
                            for (int dx = -1; dx <= 1; dx++) {
                                some += (int)imagem[i + dy][j + dx][k];
                            }
                        }
                        // Atribui a média dos vizinhos à matriz auxiliar
                        copia[i][j][k] = (unsigned char)(some / 9);
                    }
                }
            }
            // Transfere o resultado armazenado na matriz cópia de volta para a matriz principal
            for (i = 1; i < altura - 1; i++) {
                for (j = 1; j < largura - 1; j++) {
                    for (k = 0; k < canais; k++) {
                        imagem[i][j][k] = copia[i][j][k];
                    }
                }
            }
            cout << "Filtro de desfocagem aplicado!\n";
        }
        // Opção 7: Filtro Sobel (Detecção de Contornos / Bordas)
        else if (opcao == 7) {
            // Processamento interno limitando-se às coordenadas seguras da matriz (internas às bordas)
            for (i = 1; i < altura - 1; i++) {
                for (j = 1; j < largura - 1; j++) {
                    for (k = 0; k < canais; k++) {
                        // Convolução com a máscara horizontal de Sobel (Gx)
                        int gx = (imagem[i-1][j+1][k] + 2*imagem[i][j+1][k] + imagem[i+1][j+1][k]) -
                                 (imagem[i-1][j-1][k] + 2*imagem[i][j-1][k] + imagem[i+1][j-1][k]);
                        // Convolução com a máscara vertical de Sobel (Gy)
                        int gy = (imagem[i+1][j-1][k] + 2*imagem[i+1][j][k] + imagem[i+1][j+1][k]) -
                                 (imagem[i-1][j-1][k] + 2*imagem[i-1][j][k] + imagem[i-1][j+1][k]);
                        // Magnitude aproximada do gradiente pela soma dos valores absolutos
                        int res = abs(gx) + abs(gy);
                        // Truncamento técnico para não estourar o limite de 8 bits (255)
                        if (res > 255) res = 255;
                        copia[i][j][k] = (unsigned char)res;
                    }
                }
            }
            // Sobrescreve a imagem original com as bordas computadas na matriz cópia
            for (i = 1; i < altura - 1; i++) {
                for (j = 1; j < largura - 1; j++) {
                    for (k = 0; k < canais; k++) {
                        imagem[i][j][k] = copia[i][j][k];
                    }
                }
            }
            cout << "Filtro Sobel aplicado!\n";
        }
        // Opção 8: Conversão de Imagem Colorida para Tons de Cinza
        else if (opcao == 8) {
            // Executa a conversão estritamente se a imagem de entrada for do tipo colorida
            if (canais == 3) {
                for (i = 0; i < altura; i++) {
                    for (j = 0; j < largura; j++) {
                        // Calcula a média aritmética simples das intensidades de Vermelho, Verde e Azul
                        int media = ((int)imagem[i][j][0] + (int)imagem[i][j][1] + (int)imagem[i][j][2]) / 3;
                        // Iguala todos os 3 canais de cor ao valor médio obtido para gerar a escala de cinza
                        imagem[i][j][0] = imagem[i][j][1] = imagem[i][j][2] = (unsigned char)media;
                    }
                }
            }
            cout << "Filtro de tons de cinza aplicado!\n";
        }
        // Opção 9: Filtro do Item Extra (Pixelizar/Retrô)
        else if (opcao == 9) {
            int tamanhoBloco;
            cout << "Digite o tamanho do bloco de pixelizacao (ex: 8, 12, 16): ";
            cin >> tamanhoBloco;
            // Salta as linhas e colunas com base no passo definido pelo tamanho do bloco
            for (i = 0; i < altura; i += tamanhoBloco) {
                for (j = 0; j < largura; j += tamanhoBloco) {
                    // Validação preventiva de consistência de limites geométricos
                    if (i < altura && j < largura) {
                        unsigned char corBloco[3];
                        // Captura e armazena a cor de referência obtida no primeiro pixel (canto superior esquerdo) do sub-bloco
                        for (k = 0; k < canais; k++) {
                            corBloco[k] = imagem[i][j][k];
                        }
                        // Preenche todos os pixels internos deste bloco quadrado com a cor capturada
                        for (int bi = 0; bi < tamanhoBloco; bi++) {
                            for (int bj = 0; bj < tamanhoBloco; bj++) {
                                // Impede a inserção fora das dimensões limite da imagem para blocos periféricos
                                if ((i + bi) < altura && (j + bj) < largura) {
                                    for (k = 0; k < canais; k++) {
                                        imagem[i + bi][j + bj][k] = corBloco[k];
                                    }
                                }
                            }
                        }
                    }
                }
            }
            cout << "Efeito de pixelizacao retro aplicado!\n";
        }
        else if (opcao != 0) {
            cout << "Opcao invalida!\n";
        }
    } while (opcao != 0 && cin);

    cout << "Nome do arquivo de saida: ";
    cin >> arquivoSaida;
    if (salvarImagem(arquivoSaida)) cout << "Imagem salva em " << arquivoSaida << "\n";
    return 0;
}
