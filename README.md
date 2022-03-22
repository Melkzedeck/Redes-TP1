# Documentação do TP1 de Rede de Computadores
Melkzedeck Miranda - 2018070775
## Introdução

Com base no contexto de pandemia da Covid-19, é de suma importância que seja usada uma tecnologia de Redes de Computadores para ajudar a combater o avanço de novas infecções, gerenciando postos de vacinação com a independência de um computador central físico. O Trabalho Prático 1 consiste em um sistema servidor cliente para troca de mensagens a fim de que as pessoas possam encontrar um local de vacinação a partir da sua posição geográfica.Os requisitos consistem em ser aplicado usando o protocolo TCP/IP e usar quatro tipos de mensagens para comunicação: *add* e *rm* para adicionar e remover, respectivamente, um local de vacinação, *list* para listar os locais de vacinação e *query* para encontrar o posto de vacinação mais próxima.Essa é a base do protocolo que será implementada no TP.

## Implementação




O gerenciamento dos postos de saúde é usado integralmente no programa *server*,e o programa *client* serve para que o usuário consiga enviar as mensagens pela rede. O servidor se comunica com somente um único cliente e após a sua desconexão, o servidor poderá se conectar novamente com um cliente mantendo seus dados,finalizando somente quando o cliente mandar a mensagem “kill”. 

Foi criado um tipo de dado para representar os locais de vacinação e usado um contêiner para armazená-los:



```C++
//lado do servidor
typedef struct local{
    int x;
    int y;
}local;
std::vector<local> mundo;
```
