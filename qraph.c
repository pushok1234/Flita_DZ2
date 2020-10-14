#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

typedef enum { false, true } bool;

typedef struct{
    uint64_t** qr;
    uint64_t size;
    bool is_weighted;
    bool is_directed;
} qraph;

qraph* create_qr(uint64_t size){
    qraph* new_qr = (qraph*)malloc(sizeof(qraph));
    new_qr->size = size;
    new_qr->is_weighted = false;
    new_qr->qr = (uint64_t**)malloc(size*sizeof(uint64_t*));
    for (uint64_t i = 0; i < size; ++i)
        new_qr->qr[i] = (uint64_t*)calloc(size, sizeof(uint64_t));
    return new_qr;
}

void del_qr(qraph* current_qr){
    for (uint64_t i = 0; i < current_qr->size; ++i)
        free(current_qr->qr[i]);
    free(current_qr->qr);
    free(current_qr);
}

bool check_direction(qraph* current_qr){
    for (uint64_t i = 0; i < current_qr->size; ++i){
        for (uint64_t j = i; j < current_qr->size; ++j){
            if (current_qr->qr[i][j] != current_qr->qr[j][i]){
                return true;
            }
        }
    }
    return false;
}

void input_qr(qraph* current_qr){
    for (uint64_t i = 0; i < current_qr->size; ++i)
        for (uint64_t j = 0; j < current_qr->size; ++j){
            scanf("%lu", &current_qr->qr[i][j]);
            if (current_qr->qr[i][j]>>1)
                current_qr->is_weighted = true;
        }
    current_qr->is_directed = check_direction(current_qr);
}

uint64_t pr_dot(FILE* file, qraph* current_qr){
	fprintf(file, "graph none {\n");
    uint64_t reb = 0;
    for (uint64_t i = 0; i < current_qr->size; ++i){
        uint64_t j = (current_qr->is_directed) ? 0 : i;
        for (; j < current_qr->size; ++j){
            if (current_qr->qr[i][j]){
                fprintf(file, "\t%lu -", i);
                if (current_qr->is_directed){
                    fputc('>', file);
                } else {
                    fputc('-', file);
                }
                fprintf(file,  " %lu", j);
                if(current_qr->is_weighted){
                    fprintf(file, " [label=%lu]", current_qr->qr[i][j]);
                }
                fprintf(file, ";\n");
                ++reb;
            }
        }
    }
    fprintf(file, "}");
    return reb;
}


bool check_conn(qraph* current_qr){
    for (uint64_t i = 0; i < current_qr->size; ++i){
        uint64_t reb = 0;
        for (uint64_t j = 0; j < current_qr->size; ++j){
            if(current_qr->qr[i][j]){
                ++reb;
            }
        }
        if(!reb){
            return false;
        }
    }
    return true;
}

char* terminal(char* filename){
	char* command = (char*)calloc(512, sizeof(char));
    strcat(command, "dot -Tpng ");
    strcat(command, filename);
    strcat(command, " -o ");
    strcat(command, filename);
	strcat(command, ".png");
	printf("%s\n", command);
    system(command);
    free(command);
}

char* filewrite_dot(qraph* qr){
	char* filename = (char*)calloc(256, sizeof(char));
    printf("file:\n");
    scanf("%s",filename);
    FILE* file = fopen(filename, "w");
    pr_dot(file, qr);
    fclose(file);
    return terminal(filename);
}

void pr_charach(qraph* current_qr){
	if (check_conn(current_qr)) {
        printf("qraph is connected\n");//
    } else {
        printf("qraph is not connected\n");
    }
    if (current_qr->is_directed) {
        printf("qraph is directed\n");//
    } else {
        printf("qraph is not directed\n");
    }
     if (current_qr->is_weighted) {
        printf("qraph is weighted\n");//
    } else {
        printf("qraph is not weighted\n");
    }
}

int main(int argc, char *argv[]){
    printf("quantity of points:\n");
    uint64_t quantity;
    scanf("%lu", &quantity);
    qraph* qr = create_qr(quantity);
    printf("matrix:\n");
    input_qr(qr);
    system(filewrite_dot(qr));
    pr_charach(qr);
    del_qr(qr);
    return 0;
}
