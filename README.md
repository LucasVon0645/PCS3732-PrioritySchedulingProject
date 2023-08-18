# Simple MFQS
O projeto "Simple MFQS" objetiva oferecer uma **demonstração do funcionamento do algoritmo MFQS** (_Multi-Level Feedback Queue Scheduler_) em processador físico ARM-v4.

A implementação presente neste repositório foi realizada visando a execução de um escalonador MFQS na *placa _Evaluator7T_*, disponibilizada aos integrantes da equipe no oferecimento da disciplina "Laboratório de Processadores" em 2023 (PCS3732 - 2023). Especificações referentes ao funcionamento da referida placa, tal como os recursos para o controle de interrupções e para acesso aos dispositivos de I/O, podem ser consultadas neste [link](https://developer.arm.com/documentation/dui0134/latest/).

### Sobre o escalonador implementado
Como principal característica do algoritmo MFQS (_Multi-Level Feedback Queue Scheduler_), tem-se alocação de processos em uma estrutura de fila multi-nível. Nesta estrutura, cada nível agrupa processos com mesma prioridade de execução. 

De modo a facilitar a demonstração do algoritmo, **optou-se por adotar apenas 3 níveis de prioridade na fila multi-nível prevista no MFQS**.

Dentre as diversas variantes possíveis do algortimo MFQS, optou-se por uma versão que contempla as seguintes funcionalidades
1. **Controle de quantum dos processos** é realizado a partir de interrupções IRQ geradas por um **timer**;
2. Filas de maior prioridade possuem limites de quantum de execução menores do que os níveis menos prioritários;
3. Processos podem possuir sua prioridade aumentada caso apresentem um tempo de espera superior aos limites pré-estabelecido nas filas em que se encontram (**_aging_**).

Para o entendimento mais aprofundo do funcionamento deste algoritmo, bem como de seus objetivos, recomendamos a leitura do documento disponibilizado neste [link](https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-sched-mlfq.pdf).

### Hipóteses simplificadoras
Devido à complexidade envolvida no desenvolvimento de funcionalidades de manipulação de **processos reais** em um sistema operacional, optou-se por simplificar o funcionamento do escalonador MFQS deste projeto para um **mecanismo de escalonamento de _threads_**, ao invés de processos propriamente dito.

As _threads_ executadas para essa finalidade consitem na execução das funções *os_thread* e *user_thread* declaradas no arquivo "./main.c". 

### Demonstração
Uma breve explicação e demonstração do funcionamento do projeto desenvolvido pode ser consultada no vídeo disponibilizado neste [link](https://youtu.be/2lXGRk97ypg).

### Instruções para Compilação e Execução na Placa Evaluator
Para a execução do projeto, é necessário executar as etapas subsequentes:
1. Compilar os arquivos fonte do projeto por meio da execução do comando `make` no diretório raiz do projeto;
2. Interconectar a placa Evaluator7T ao computador por meio de uma interface J-Tag e configurá-la com a execução do comando `make ocd`;
3. Executar o comando `make gdb` para iniciar o carregamento do código compilado à placa Evalator7T.

obs.: Verique se a execução do código na interface do GDB é iniciada no endereço "0x8000". Caso contrário, execute um _jump_ para o endereço do vetor de interrupções: `j *0`

### Estrutura de diretórios do repositório
Na atual versão deste repositório, o código utilizado para desenvolvimento e testes do projeto foi organizado com a seguinte estrutura de diretórios e arquivos:

1. [kernel.ld](../kernel.ld): arquivo que contém as instruções a serem seguidas pelo _linker_ para a compilar do projeto;
2. [os](../os): diretório que contém a implementação do escalonador e das principais funcionalidades desempenhadas pelo sistema operacional, as quais são necessárias ao funcionamento do MFQS;
3. [test](../test): diretório criado para execução de testes pontuais do funcionamento do escalonador

As informações do diretório [os](../os) são organizadas da seguinte forma:
- [kernel.s](../os/kernel.s): contém a implementação das principais funcionalidade em baixo nível necessárias ao funcionamento do escalonador, tais como a funções de _dispatcher_ e pontos de entrada para o tratamento de interrupões e chamadas de sistema;
- [os/scheduler](../os/scheduler): contém a implementação das políticas da do algoritmo MFQS conforme as decisões de projeto da equipe. Neste diretório, estão presentes a codificação de mecanismos como:
    - a busca na fila multi-nível pela próxima thread a ser executada
    - atualização dos parâmetros de cada thread envolvidos no escalonamento (slots de tempo restantes e tempo de espera)
    - mudança dinâmica da prioridade das _threads_
- [os/boot](../os/boot): contém a implementação da função _void boot()_, a qual é responsável por inicializar as estruturas de dados da fila multi-nível e instanciar as threads a serem executadas na demonstração;
- [os/queue](os/queue): contém a implementação da fila multi-nível utilizada pelo algoritmo MFQS. Nesse diretório, estão presentes
    - a definação da interface da _Task Control Block_ (TCB), o qual permite o armazenamento do contexto de cada _thread_
    - a definição das interfaces de acesso às filas do escalonador
    - a codificação das rotinas que manipulam a fila multi-nível (inserção e deleção de _threads_)
- [os/iomanager](../os/iomanager.c): implemeta rotinas que configuram e acessam dispostivos de saída utilizados na demonstração do projeto;
- [os/interrupts](../os/interrupts): contém rotinas responsáveis pela configuração e manipulação das interrupções de timer e do user_button da placa Evaluator7T;
- [os/os](../os/os.c): contempla as interfaces e implementações das chamadas de sistema que permitem as _threads_
    - acessarem informações disponíveis na TCB;
    - executarem as funções de _yield_ e _halt_. Estas, por sua vez, são explicadas na seção subsequente de "Aspectos relevantes de implementação".
- [os/stubs](../os/stubs.c): contém uma implementação da _stub_ `_sbrk(int incr)`, a qual permite, em muitos sistemas operacionais, o oferecimento do serviço de alocação de memória dinâmica. Tal _stub_ é atua como subsídio da função _malloc_, a qual é utilizada no projeto para a criação de _threads_ e das estruturas evolvidas na implementação do escalonador;

### Alguns aspectos relevantes de implementação
#### Concessão de Tempo de Execução e Função Yield
Para a simulação dos efeitos de uma concessão de tempo de execução realizada por uma _thread_ antes que seu respectivo _slot_ de tempo seja esgotado no algoritmo MFQS, foram implementadas duas funcionalidades.

1. **Função `void yield( )`:** Uma _thread_ pode conceder tempo de execução a uma outra a partir da realização da chamada de sistema _yield_, implementada no projeto. Nessa chamada de sistema, a instrução `swi` prevista na arquitetura ARMv4 é executada logo após o registrador `R0` receber o código "1". Tal código permite o sistema operacional fictício do projeto identificar em [kernel.s](../os/kernel.s) que o serviço solicitado pela _thread_ corresponde a uma concessão do tempo de execução.

2. **Interrupção de botão:** Na demonstração do projeto, de modo a facilitar a visualizaçãodo referido fenômeno de concessão de tempo de execução, optou-se por desenvolver uma rotina que retira a  _thread_ atual da execução em CPU assim que uma interrupção de botão da Evaluator é gerada. Para tanto, foi necessário implementar uma rotina em _assembly_ que realiza o tratamento de uma interrupção de botão da placa para essa finalidade. Nela, as mesmas rotinas usadas para implementação do serviço da chamada de sistema _yield_ também são chamadas.

Em  ambos os casos, destaca-se que a _thread_ em execução é retirada da posição frontal de sua respectiva fila e deslocada para o final da mesma. Em outras palavras, a prioridade da _thread_ é mantida.
 
Todavia, caso os _slots_ de tempo restantes da _thread_ em execução tenham sido esgotados, destaca-se que o rebaixamento de prioridade acontece, com a _thread_ sendo deslocada para o nível de prioridade imediatamente abaixo.

Ressalta-se que tal mecanismo de rebaixamento é importante para evitar que uma _thread_ explore a chamada `yield` para se manter indefinidamente em uma mesma prioridade, o que poderia prejudicar a execução de outras _threads_.

#### Função Halt
De modo a permitir que a _thread_ em execução seja finalizada, criou-se uma chamada de sistema específica para esse propósito: `halt()`.

Para a implementação dessa chamada de sistema, executa-se uma interrupção `swi`. O tratamento dessa interrupção de sistema envolve as seguintes ações:
1. Remover o nó correspondente a _thread_ em execução é removida da estrutura da fila multinível;
2. Realizar um novo escalonamento e troca de contexto;


#### Condição de fila multinível vazia
Quando não há mais _threads_ a serem executadas, isto é, quando a fila multi-nível MFQS se encontra vazia, optou-se por codificar o escalonador para que ele escalone para execução uma thread específica, denominada de `os_thread`. Tal _thread_ é executada indefinidamente até que uma nova thread surja na fila multinível.

#### Chamadas de sistema auxiliares

De modo a permitir que _thread_ acesse informações armazenadas em sua TCB, tal como o seu respectivo tempo de CPU total, sua prioridade e seu identificador _tid_, também foram implementadas chamadas de sistema específicas para esses propósitos:

1. `int getpid( )`: retorna o identificador `tid` da _thread_ que faz a chamada;
2. `int get_priority( )`: retorna a prioridade da _thread_ que faz a chamada, podendo ser 0, 1 ou 2. Quanto maior o valor númerico da prioridade, menor a prioridade;
3. `int get_cpu_time( )`: retorna o tempo total, em `ticks` de relógio, durante o qual uma _thread_ executou em CPU.

## Autores
- [Dênio Araújo de Almeida](https://github.com/Denio-Almeida)
- [Francisco Cavalheiro Mariani](https://github.com/franos-pj)
- [Lucas von Ancken Garcia](https://github.com/LucasVon0645)

### Agradecimentos Especiais
- [Professor Bruno Abrantes Basseto](https://github.com/bru4bas)
- [Gabriel Zambelli](https://github.com/GabZamba)