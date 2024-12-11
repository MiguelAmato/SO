#include "def.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <dirent.h>
#include <string.h>

char* loadstr(FILE* students);
student_t* parse_records(char* records[], int* nr_records);
void dump_entries(student_t* entries, int nr_entries, FILE* students);
student_t* read_student_file(FILE* students, int* nr_entries);
void show_students(student_t* students, int nr_entries);
int find_duplicate(student_t* students,int nr_entries,int id);
student_t* append_records(FILE* file, student_t* recordSt,int* fileEntries, int* recordEntries);
void find_id(student_t* students, char* id_list[], int st_size, int arg_size);
void find_NIF(student_t* students, char* NIF_list[], int st_size, int arg_size);

student_t* parse_records(char* records[], int* nr_records) {
    student_t* ret = malloc(sizeof(student_t) * (*nr_records));
    student_t* cur_student;
    int i;
    char* aux;
    int cont = 0;
    for (int i = 0; i < *nr_records; ++i) {
        aux =  strsep(&records[i], ":");
        if(find_duplicate(ret,cont,atoi(aux)) == 0){
            cur_student = &ret[cont];
            cur_student->student_id = atoi(aux);
            aux =  strsep(&records[i], ":");
            strcpy(cur_student->NIF, aux);
            aux = strsep(&records[i], ":");
            cur_student->first_name = strdup(aux);
            aux = strsep(&records[i], ":");
            cur_student->last_name = strdup(aux);
            cont++;
        }
    }
    *nr_records = cont;
    // free(aux);
    return ret;
}

void dump_entries(student_t* entries, int nr_entries, FILE* students) {
    char* aux = '\0';
    int i;
    for (i = 0; i < nr_entries; ++i) {
        fwrite(&entries[i].student_id, sizeof(int), 1, students); 
        fwrite(&entries[i].NIF, sizeof(char) * strlen(entries[i].NIF), 1, students);
        fwrite(&aux, sizeof(char), 1, students);
        fwrite(entries[i].first_name, sizeof(char) * strlen(entries[i].first_name), 1, students);
        fwrite(&aux, sizeof(char), 1, students);
        fwrite(entries[i].last_name, sizeof(char) * strlen(entries[i].last_name), 1, students);
        fwrite(&aux, sizeof(char), 1, students);
    }
}

student_t* read_student_file(FILE* students, int* nr_entries) {
    student_t* ret;
    fgets(nr_entries, sizeof(int), students);
    ret = malloc(sizeof(student_t) * (*nr_entries));
    int i;
    char* aux;
    aux = fgetc(students);
    for (i = 0; i < *nr_entries; ++i) {
        ret[i].student_id = fgetc(students);
        fgets(&aux,sizeof(int),students);
        fread(&ret[i].NIF,sizeof(char),MAX_CHARS_NIF,students);
        fgets(&aux,sizeof(char) +1,students);
        ret[i].first_name = loadstr(students);
        ret[i].last_name = loadstr(students);
    }
    return ret;
}

char* loadstr(FILE* students) {
    char c;
    char* ret = NULL;
    int size = 0;
    fread(&c, sizeof(char), 1, students);
    while(c != '\0' && c != EOF) {
        ret = realloc(ret, sizeof(char)* size + 1);
        ret[size] = c;
        ++size;
        fread(&c, sizeof(char), 1, students);
    }

    return ret;
}


void show_students(student_t* students, int nr_entries) {
    int i;
    for (i = 0; i < nr_entries; ++i) {
        printf("[Entry #%d]\n", i);
        printf("\tstudent_id = %d\n", students[i].student_id);
        printf("\tNIF = %s\n", students[i].NIF);
        printf("\tfirst_name = %s\n", students[i].first_name);
        printf("\tlast_name = %s\n", students[i].last_name);
    }
}

int find_duplicate(student_t* students,int nr_entries,int id){
    for(int i = 0; i < nr_entries;i++){
        if(students[i].student_id == id)
            return -1;
    }
    return 0;
}

student_t* append_records(FILE* file, student_t* recordSt,int* fileEntries, int* recordEntries) {
    student_t* ret = read_student_file(file, fileEntries);
    for (int i = 0; i < *recordEntries; ++i) {
        if (find_duplicate(ret, *fileEntries, recordSt[i].student_id) == 0) {
            ret = realloc(ret, sizeof(student_t) * (*fileEntries + 1));
            ret[*fileEntries].student_id = recordSt[i].student_id;
            strcpy(ret[*fileEntries].NIF, recordSt[i].NIF);
            ret[*fileEntries].first_name = strdup(recordSt[i].first_name);
            ret[*fileEntries].last_name = strdup(recordSt[i].last_name);
            ++(*fileEntries);
        }
        else 
            printf("Duplicate ID: %d\n", recordSt[i].student_id);
    }
    // free??
    return ret;
}

void find_id(student_t* students, char* id_list[], int st_size, int arg_size) {
    int id;
    for(int i = 0; i < arg_size; ++i) {
        id = atoi(id_list[i]);
        for(int j = 0; j < st_size; ++j) {
            if (id == students[j].student_id) {
                printf("[Entry #%d]\n", j);
                printf("\tstudent_id = %d\n", students[j].student_id);
                printf("\tNIF = %s\n", students[j].NIF);
                printf("\tfirst_name = %s\n", students[j].first_name);
                printf("\tlast_name = %s\n", students[j].last_name);
                break;
            }
            if (j == (st_size - 1)) 
                printf("No entry was found\n");
        }
    }
}

void find_NIF(student_t* students, char* NIF_list[], int st_size, int arg_size) {
    char* NIF;
    
    for(int i = 0; i < arg_size; ++i) {
        for(int j = 0; j < st_size; ++j) {
            if (strcmp(NIF_list[i], students[j].NIF) == 0) {
                printf("[Entry #%d]\n", j);
                printf("\tstudent_id = %d\n", students[j].student_id);
                printf("\tNIF = %s\n", students[j].NIF);
                printf("\tfirst_name = %s\n", students[j].first_name);
                printf("\tlast_name = %s\n", students[j].last_name);
                break;
            }
            if (j == (st_size - 1)) 
                printf("No entry was found\n");
        }
    }
}

int main(int argc, char *argv[]) {
    int opt;
    FILE* file;
    char* fileName;
    bool l = false, c = false, a = false,q = false, i = false, n = false;
    char mode[4] = "rb";

	while((opt = getopt(argc, argv, "f:hlcaqin")) != -1) {
		switch(opt) {
        case 'f':
            fileName = optarg;
            break;
		case 'h':
			fprintf(stderr,"Usage: %s -f file [ -h | -l | -c | -a | -q [ -i|-n ID] ] ] [ list of records ]\n" ,argv[0]); 
			exit(0);
		case 'l':
            l = true;
            strcpy(mode, "rb");
			break;
		case 'c':
            c = true;
            strcpy(mode, "wb");
			break;
		case 'a':
            a = true;
            strcpy(mode, "rb+");
			break;
        case 'q':
            q = true;
            strcpy(mode, "rb");
            break;
        case 'i':
            if (!q) {
                fprintf(stderr,"error\n");
                exit(EXIT_FAILURE);
            }
            i = true;
            break;
        case 'n':
            if (!q) {
                fprintf(stderr,"error\n");
                exit(EXIT_FAILURE);
            }
            n = true;
            break;
		default:
			exit(EXIT_FAILURE);
		}
	}
    student_t* students;
    if ((file = fopen(fileName, mode)) == NULL) 
        err(2,"Missing file\n");
    if (l) {
        int nr_entries;
        students = read_student_file(file, &nr_entries);
        show_students(students, nr_entries);
        free(students);
    }
    else if (a) {
        int nr_entries;
        int entries = argc - optind;
        student_t* records = parse_records(&argv[optind], &entries);
        student_t* new_students = append_records(file, records, &nr_entries, &entries);
        fseek(file, 0, SEEK_SET);
        fwrite(&nr_entries, sizeof(int), 1, file);
        dump_entries(new_students, nr_entries, file);
        free(records);
        free(new_students);
    }
    else if (c) {
        int entries = argc - optind;
        students = parse_records(&argv[optind], &entries); 
        fwrite(&entries, sizeof(int), 1, file);
        dump_entries(students, entries, file);
        printf("%d records written succesfully\n", entries);
        free(students);
    }
    else if (q) {
        int nr_entries;
        students = read_student_file(file, &nr_entries);
        if (i)
            find_id(students, &argv[optind], nr_entries, argc - optind);
        else if (n)
            find_NIF(students, &argv[optind], nr_entries, argc - optind);
        free(students);
    }
    fclose(file);
}