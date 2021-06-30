# Documentação do TP1 de Rede de Computadores
Melkzedeck Miranda - 2018070775
## Introdução

Com base no contexto de pandemia da Covid-19, é de suma importância que seja usada uma tecnologia de Redes de Computadores para ajudar a combater o avanço de novas infecções, gerenciando postos de vacinação com a independência de um computador central físico. O Trabalho Prático 1 consiste em um sistema servidor cliente para troca de mensagens a fim de que as pessoas possam encontrar um local de vacinação a partir da sua posição geográfica.Os requisitos consistem em ser aplicado usando o protocolo TCP/IP e usar quatro tipos de mensagens para comunicação: *add* e *rm* para adicionar e remover, respectivamente, um local de vacinação, *list* para listar os locais de vacinação e *query* para encontrar o posto de vacinação mais próxima.Essa é a base do protocolo que será implementada no TP.

## Implementação

A implementação do software consiste em duas partes: comunicação em Rede e gerenciamento dos locais dos postos de saúde. Para a comunicação em Rede, foram feitas quatro Classes: Adress, Tclient, STclient e Tserver, usando a interface POSIX e soquetes de rede na linguagem C. O restante do trabalho foi implementado em C++.

A classe *Adress* serve para tratamento de endereços IP (IPv4 e IPv6) e portas usadas na comunicação na rede.  

```C++
    /*Construtor da classe Adress usando a versão ('4'-(IPv4) ou '6'-(IPv6))
     e a porta(string) como argumento - lado do servidor*/
    Adress::Adress(char v, std::string port_s);

    /*Construtor usando o endereco(string) e porta(string) - lado cliente*/
    Adress::Adress(std::string addrstr, std::string port_s);
```
A classe *Tclient* serve para inicialização, conexão e encerramento no final do programado soquete, e envio e recebimento de mensagens visando a comunicação do lado do Cliente.
```C++
    //Construtor do socket com endereco
    Tclient::Tclient(const Adress&);

    /* define um tamanho maximo de fluxo de mensagem da classe
    Tamanho default: 1024*/
    static void Tclient::setTAM(const int& t);

    /* fluxo de saída de string:
    envia para o socket a string
    e retorna a qntd de bits enviadas*/
    ssize_t Tclient::operator<<(const std::string&);

    /* fluxo de entrada de string: recebe uma msg do socket, 
    coloca em uma string e retorna a qntd de bytes recebidas*/
    ssize_t Tclient::operator>>(std::string&);
```

A classe *STclient* (Server-Tclient) é uma classe derivada da classe Tcliente e é usada no lado do servidor,atuando na comunicação do servidor com o cliente. A motivação do uso da herança consiste na reutilização da interface por conta de similaridade em suas funcionalidades, e por ter uma única funcionalidade a mais que a classe pai. 
```C++
class STclient : public Tclient{
	Adress addr_;
public:
	STclient(int, Adress);
	STclient& operator=(const STclient&);
	Adress addr();
};
```



A classe *Tserver* serve para inicializar, ligar (bind) a um endereço e deixar o soquete na escuta (listen) para novas conexões, visando a comunicação no lado do servidor e, no término de sua utilização, o fechamento do descritor do soquete. Quando um objeto da classe Tserver identifica uma nova conexão, ele retorna um objeto da classe STclient para que a comunicação seja estabelecida. 

```C++

    //Construtor do socket com endereco
    Tserver::Tserver(Adress&);

    // Espera conexao e retorna o cliente conectado
    STclient Tserver::waitConection();
```

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
