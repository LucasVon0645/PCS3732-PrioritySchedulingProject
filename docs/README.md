# Simple MFQS
O projeto "Simple MFQS" objetiva oferecer uma **demonstração do funcionamento do algoritmo MFQS** (_Multi-Level Feedback Queue Scheduler_) em processador ARM-v4.

A implementação presente neste repositório foi realizada visando a execução de um escalonador MFQS na *placa _Evaluator7T_*, disponibilizada aos intergrantes da equipe no oferecimento da disciplina "Laboratório de Processadores" em 2023 (PCS3732 - 2023). Especificações referentes ao funcionamento da referida placa, tal como os recursos para o controle de interrupções e para acesso aos dispositivos de I/O, podem ser consultadas neste [link](https://developer.arm.com/documentation/dui0134/latest/).

### Sobre o escalonador implementado
Como principal característica do algoritmo MFQS (_Multi-Level Feedback Queue Scheduler_), tem-se alocação de processos em uma estrutura de fila multi-nível. Nesta estrutura, cada nível agrupa processos com mesma prioridade de execução. 

De modo a facilitar a demonstração do algoritmo, *optou-se por adotar apenas 3 níveis de prioridade na fila multi-nível prevista no MFQS*.

Dentre as diversas variantes possíveis do algortimo MFQS, optou-se por uma versão que contempla as seguintes funcionalidades
1. **Controle de quantum dos processos** é realizado a partir de interrupções IRQ geradas por um **timer**;
2. Filas de maior prioridade possuem limites de quantum de execução menores do que os níveis menos prioritários;
3. Processos podem possuir sua prioridade aumentada caso apresentem um tempo de espera superior ao limites pré-estabelecido na fila em que se encontram (**_aging_**).

Para o entendimento mais aprofundo do funcionamento deste algoritmo e de seus obteivos, recomendamos a leitura do documento disponibilizado neste [link](https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-sched-mlfq.pdf).

### Hipóteses simplificadoras
Devido à complexidade envolvida no desenvolvimento de funcionalidades de manipulação de **processos reais** em um sistema operacional, optou-se por simplificar a demonstração do funcionamento do escalonador MFQS, neste projeto, para um **mecanismo de escalonamento de _threads_** ao invés de processos propriamente dito.

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


No diretório [os](../os), estão presentes:
- [kernel.s](../os/kernel.s): contém a implementação das principais funcionalidade em baixo nível necessárias ao funcionamento do escalonador, tais como a funções de _dispatcher_ e pontos de entrada para o tratamento de interrupões e chamadas de sistema;
- [os/scheduler](../os/scheduler): contém a implementação das políticas da do algoritmo MFQS conforme as decisões de projeto da equipe. Neste diretório, estão presentes a codificação de mecanismos como:
    - a busca na fila multi-nível pela próxima thread a ser executada
    - atualização dos parâmetros de cada thread envolvidos no escalonamento (slots de tempo restantes e tempo de espera)
    - mudança dinâmica da prioridade das _threads_
- [os/boot](../os/boot): contém a implementação da função _void boot()_, a qual é responsável por inicializar as estruturas de dados da fila multi-nível e instanciar as threads a serem executadas na demonstração;
- [os/queue](os/queue): contém a implementação da fila multi-nível utilizada pelo algoritmo MFQS. Nesse diretório, estão presentes:
    - definação da interface da _Task Control Block_
    - definição das interfaces de acesso às filas do escalonador
    - codificação das rotinas que manipulam a fila multi-nível (inserção e deleção de _threads_)
- [os/iomanager](../os/iomanager.c):
- [os/interrupts](../os/interrupts):
- [os/os](../os/os.c):
- [os/stubs](../os/stubs.c):

### Aspectos relevantes de implementação
#### Função Yield
#### Função Halt
#### Condição de fila multinível vazia
#### Chamadas de sistema

## Autores
- [Dênio Araújo de Almeida](https://github.com/Denio-Almeida)
- [Francisco Cavalheiro Mariani](https://github.com/franos-pj)
- [Lucas von Ancken Garcia](https://github.com/LucasVon0645)

### Agradecimentos Especiais
- [Professor Bruno Abrantes Basseto](https://github.com/bru4bas)
- Gabriel Zambelli