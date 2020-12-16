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
            scanf("%llu", &current_qr->qr[i][j]);
            if (current_qr->qr[i][j]>>1)
                current_qr->is_weighted = true;
        }
    current_qr->is_directed = check_direction(current_qr);
}

uint64_t pr_dot(FILE* file, qraph* current_qr){
    if (current_qr->is_directed) fprintf(file, "di");
	fprintf(file, "graph none {\n");
    uint64_t reb = 0;
    for (uint64_t i = 0; i < current_qr->size; ++i){
        uint64_t j = (current_qr->is_directed) ? 0 : i;
        for (; j < current_qr->size; ++j){
            if (current_qr->qr[i][j]){
                fprintf(file, "\t%llu -", i);
                if (current_qr->is_directed){
                    fputc('>', file);
                } else {
                    fputc('-', file);
                }
                fprintf(file,  " %llu", j);
                if(current_qr->is_weighted){
                    fprintf(file, " [label=%llu]", current_qr->qr[i][j]);
                }
                fprintf(file, ";\n");
                ++reb;
            }
        }
    }
    fprintf(file, "}");
    return reb;
}

uint64_t dfs(qraph* current_qraph, bool* color,uint64_t pos){
    uint64_t quantity_of_lines = 1;
    color[pos] = true;
    for (uint64_t i = 0; i < current_qraph->size; ++i) {
        if (current_qraph->qr[pos][i] && !color[i]){
            quantity_of_lines += dfs(current_qraph, color, i);
        }
    }
    return quantity_of_lines;
}

bool check_conn(qraph* current_qr){
    bool* color = (bool*)calloc(current_qr->size, sizeof(bool));
    uint64_t quantity_of_lines = dfs(current_qr,color,0);
    free(color);
    if (quantity_of_lines < current_qr->size)
        return false;
    else 
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
	return command;
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

void search_degree(uint64_t degree_val ,qraph* qr){
	uint64_t qr_size = qr->size;
	uint64_t sum = 0;
	uint64_t* degree_arr = (uint64_t*) calloc(qr_size, sizeof(uint64_t));
	for (uint64_t i = 0; i < qr_size; ++i){
		for (uint64_t j = 0; j < qr_size; ++j){
			sum += qr->qr[i][j];
		}
		degree_arr[i] = sum;
		sum = 0;
	}

	printf("points with degree value %llu\n", degree_val);
	for (uint64_t i = 0; i < qr_size; ++i){
		if (degree_arr[i] == degree_val) printf("num of point: %llu\n", i);
	}
}

int main(void){
    printf("quantity of points:\n");
    uint64_t quantity;
    scanf("%llu", &quantity);
    qraph* qr = create_qr(quantity);
    printf("matrix:\n");
    input_qr(qr);
    system(filewrite_dot(qr));
    pr_charach(qr);
    printf("degree value:\n");
    uint64_t degree_val;
    scanf("%llu", &degree_val);
    search_degree(degree_val, qr);
    del_qr(qr);
    return 0;
}