/* Automação em Tempo Real
Exercício Computacional I

Alunas:
Clara Temponi Marigo - 2020077374
Julia Pereira Maia Ribeiro 2021014864
*/ 

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <thread>
#include <chrono>

// Representação do labirinto
char** maze = nullptr;

// Estrutura para representar uma posição no labirinto
struct Position {
    int row;
    int col;
};

// Variáveis globais
int num_rows = 0;
int num_cols = 0;
std::stack<Position> valid_positions;

// Função para carregar o labirinto de um arquivo
Position load_maze(const char* file_name, char*** mazePtr, int& num_rows, int& num_cols) {
    Position initial_pos = {-1, -1}; 
    FILE *arquivo;

    arquivo = fopen(file_name, "r");
    if (arquivo == nullptr) {
        std::cerr << "Não foi possível abrir o arquivo: " << file_name << std::endl;
        return initial_pos;
    }

    if (fscanf(arquivo, "%d %d", &num_rows, &num_cols) != 2) {
        std::cerr << "Erro ao ler as dimensões." << std::endl;
        fclose(arquivo);
        return initial_pos;
    }

    *mazePtr = (char**)malloc(num_rows * sizeof(char*));
    for (int i = 0; i < num_rows; ++i) {
        (*mazePtr)[i] = (char*)malloc(num_cols * sizeof(char));
    }

    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            char c = fgetc(arquivo);
            if (c == '\n') {
                c = fgetc(arquivo);
            }
            if (c == 'e') {
                initial_pos.row = i;
                initial_pos.col = j;
            }
            (*mazePtr)[i][j] = c;
        }
    }

    fclose(arquivo); 
    return initial_pos;
}

// Função para imprimir o labirinto
void print_maze(char** maze, int num_rows, int num_cols) {
    if (maze == nullptr) {
        std::cerr << "O labirinto não foi carregado corretamente." << std::endl;
        return;
    }

    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            std::cout << maze[i][j];
        }
        std::cout << std::endl;
    }
}

// Função para verificar se uma posição é válida
bool is_valid_position(int row, int col) {
    return row >= 0 && row < num_rows && col >= 0 && col < num_cols && (maze[row][col] == 'x' || maze[row][col] == 's');
}

// Função principal para navegar pelo labirinto
bool walk(Position pos) {
    valid_positions.push(pos); 

    while (!valid_positions.empty()) {
        Position current_pos = valid_positions.top();
        valid_positions.pop();

        int i = current_pos.row;
        int j = current_pos.col;

        if (maze[i][j] != 'x' && maze[i][j] != 'e' && maze[i][j] != 's') continue;

        // Verifica se a posição atual é a saída
        if (maze[i][j] == 's') {
            maze[i][j] = 'o'; 
            print_maze(maze, num_rows, num_cols);
            return true;
        }

        // Marca a posição atual como visitada
        if (maze[i][j] == 'x') {
            maze[i][j] = '.';
        } else if (maze[i][j] == 'e') {
            maze[i][j] = '.';
        }
        // Direções: direita, esquerda, baixo, cima
        Position directions[4] = {{i, j + 1}, {i, j - 1}, {i + 1, j}, {i - 1, j}};

        for (auto& dir : directions) {
            if (is_valid_position(dir.row, dir.col)) {
                // Adiciona a posição válida e não visitada à pilha
                valid_positions.push(dir);
            }
        }

        print_maze(maze, num_rows, num_cols);
    }

    return false; 
}

int main(int argc, char* argv[]) {
    Position initial_pos = load_maze(argv[1], &maze, num_rows, num_cols);
    std::cout << "Posição inicial: (" << initial_pos.row << ", " << initial_pos.col << ")" << std::endl;

    bool exit_found = walk(initial_pos);
    if(exit_found) {
        std::cout << "Saída encontrada!" << std::endl;
    } else {
        std::cout << "Saída não encontrada." << std::endl;
    }

    for (int i = 0; i < num_rows; ++i) {
        free(maze[i]);
    }
    free(maze);

    return 0;
}
