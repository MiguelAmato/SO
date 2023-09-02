#define MAX_CHARS_NIF  9

typedef struct {
    int student_id; 
    char NIF[MAX_CHARS_NIF+1];  
    char* first_name;
    char* last_name;
} student_t;