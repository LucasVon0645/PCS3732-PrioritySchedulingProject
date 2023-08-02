#define MAX_THREAD_NUM 10

typedef struct {
    uint32_t regs[17];   // contexto (17 registradores)
    uint32_t tid;        // identificador da thread
    uint32_t priority;   // prioridade atual da thread
    uint32_t exc_slots;  // numero de execucoes restantes
} tcb;

typedef struct {
    tcb* tcb_t;
    node* next_node;
    node* previous_node;
} node;

 
// Estrutura da tabela de TCBs
typedef struct {
    node* list_head;
    node* list_end;
} thread_table;


void add_to_table(thread_table* table, tcb* new_tcb);
int delete_from_table_by_id(thread_table* table, int tid);
