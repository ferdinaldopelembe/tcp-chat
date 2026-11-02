# TCP Chat

Chat de terminal desenvolvido em **C++17**, com comunicação **TCP** através do módulo de rede da **SFML**. Um servidor central recebe mensagens e distribui-as aos restantes clientes ligados.

Projeto de estudo de sockets, arquitetura cliente-servidor, serialização de mensagens e concorrência.

## Funcionalidades

- Ligação de vários clientes ao mesmo servidor.
- Escolha de um nome ao iniciar cada cliente.
- Envio de mensagens para todos os outros participantes.
- Receção de mensagens numa thread dedicada no cliente.
- Apresentação das mensagens recebidas enquanto o utilizador escreve.
- Histórico em memória durante a execução de cada cliente.
- Registo de ligações, mensagens e desconexões no terminal do servidor.
- Comando `q!` para sair do cliente.

## Tecnologias

| Tecnologia | Utilização |
| --- | --- |
| C++17 | Implementação do cliente e do servidor |
| SFML Network | Sockets TCP e serialização com `sf::Packet` |
| SFML System | Pausas entre verificações dos sockets |
| `std::thread` | Receção de mensagens no cliente |
| GNU Make | Compilação e execução |

Os cabeçalhos incluídos no repositório identificam a **SFML 3.1.0**. O cliente utiliza `<conio.h>`, `kbhit()` e `getch()`, e o Makefile utiliza `cls` e executáveis `.exe`: as instruções abaixo destinam-se a **Windows**.

## Como funciona

1. O servidor abre a porta TCP `53000` e verifica novas ligações e mensagens usando sockets não bloqueantes.
2. O cliente liga-se ao servidor e pede um nome ao utilizador.
3. Ao carregar em **Enter**, o cliente envia um pacote com o nome e o texto da mensagem.
4. O servidor distribui o texto no formato `nome> mensagem` a todos os clientes, exceto ao remetente.
5. O remetente apresenta a sua própria mensagem como `me> mensagem`.

O nome é enviado em cada mensagem; não existe registo de conta nem autenticação.

## Requisitos

- Windows.
- Compilador `g++` com suporte a C++17, compatível com as bibliotecas SFML incluídas.
- GNU Make, disponível como `make` ou `mingw32-make`, conforme a instalação.
- Dependências em `include/`, `lib/` e DLLs em `bin/`.

As bibliotecas pré-compiladas devem corresponder à arquitetura e ao compilador utilizados. Se houver incompatibilidade, substitui os ficheiros da SFML por uma distribuição compatível.

## Compilação e execução

### 1. Obter o projeto

```bash
git clone https://github.com/ferdinaldopelembe/tcp-chat.git
cd tcp-chat
```

Executa os comandos seguintes a partir da raiz do projeto, em **três terminais separados**.

### 2. Iniciar o servidor

```bash
make s DEFINES= SFML_FLAGS="-lsfml-network -lsfml-system"
```

### 3. Iniciar o primeiro cliente

```bash
make c1 DEFINES= SFML_FLAGS="-lsfml-network -lsfml-system"
```

### 4. Iniciar o segundo cliente

```bash
make c2 DEFINES= SFML_FLAGS="-lsfml-network -lsfml-system"
```

Se o teu ambiente disponibilizar `mingw32-make`, utiliza esse nome em vez de `make`.

Cada alvo **compila e executa** o programa. O alvo predefinido (`make` ou `make all`) inicia apenas o servidor. Os alvos disponíveis para utilização são `s`, `c1` e `c2`.

> Os comandos acima removem a definição `SFML_STATIC` do Makefile e ligam apenas os módulos Network e System, usando as bibliotecas de importação e as DLLs incluídas. O Makefile original define `SFML_STATIC`, mas usa nomes de bibliotecas sem o sufixo `-s`; uma compilação estática exige ajustar as bibliotecas e respetivas dependências.

Depois de compilares, também podes executar os programas diretamente no PowerShell:

```powershell
.\bin\server.exe
```

Noutros terminais:

```powershell
.\bin\client1.exe
```

```powershell
.\bin\client2.exe
```

Mantém as DLLs necessárias na mesma pasta dos executáveis.

## Utilização

1. Inicia o servidor antes dos clientes.
2. Introduz um nome em cada cliente quando aparecer `Enter your name>`.
3. Escreve uma mensagem e carrega em **Enter** para enviar.
4. Usa **Backspace** para apagar caracteres durante a escrita.
5. Escreve `q!` e carrega em **Enter** para sair.
6. Usa **Ctrl+C** no terminal do servidor para o terminar.

Exemplo com dois participantes:

| Terminal de Fred | Terminal de Ana |
| --- | --- |
| `me> Olá, Ana!` | `Fred> Olá, Ana!` |
| `Ana> Olá, Fred!` | `me> Olá, Fred!` |

`client1.exe` e `client2.exe` são compilações do mesmo código, destinadas a facilitar os testes. Podes abrir outras instâncias para adicionar participantes.

## Configuração de rede

O endereço e a porta estão definidos em [`include/constants.hpp`](include/constants.hpp):

```cpp
const int PORT = 53000;
const sf::IpAddress IP = sf::IpAddress(127, 0, 0, 1);
```

### Teste no mesmo computador

Mantém `127.0.0.1` e abre o servidor e os clientes em terminais separados.

### Teste entre computadores na mesma rede

1. Liga os computadores à mesma rede local.
2. No computador que executa o servidor, usa `ipconfig` para consultar o endereço IPv4 da interface de rede.
3. Altera `IP` em `include/constants.hpp` para esse endereço. Por exemplo:

   ```cpp
   const sf::IpAddress IP = sf::IpAddress(192, 168, 1, 10);
   ```

4. Recompila os clientes com a nova configuração.
5. Permite a aplicação ou a porta **TCP 53000** na firewall do computador servidor, para a rede utilizada.
6. Inicia o servidor e, depois, os clientes nos outros computadores.

O valor de `IP` é usado pelos clientes. O servidor escuta na porta configurada sem restringir explicitamente a ligação ao endereço de loopback. Se alterares a porta, recompila o servidor e os clientes.

## Organização do código

| Ficheiro ou pasta | Responsabilidade |
| --- | --- |
| `server.cpp` | Aceitar ligações, receber mensagens e distribuí-las |
| `client.cpp` | Ligação, entrada pelo teclado e receção em paralelo |
| `include/constants.hpp` | Endereço do servidor e porta TCP |
| `include/user.hpp` | Declaração da classe `User` |
| `include/message.hpp` | Declaração da classe `Message` |
| `src/user.cpp` | Criação do utilizador e atribuição do nome |
| `src/message.cpp` | Construção de mensagens |
| `Makefile` | Alvos de compilação e execução |
| `include/SFML/` | Cabeçalhos da SFML |
| `lib/` | Bibliotecas de ligação |
| `bin/` | DLLs e destino dos executáveis gerados |

## Limitações atuais

- As mensagens e os nomes circulam sem encriptação ou autenticação.
- O histórico fica apenas na memória de cada cliente; novos participantes não recebem mensagens anteriores.
- Não existem mensagens privadas, salas, transferência de ficheiros ou reconexão automática.
- A thread de receção e a thread principal partilham o histórico e o texto de entrada sem sincronização, o que pode causar condições de corrida e interferência na apresentação do terminal.
- O envio não bloqueante não implementa uma fila de reenvio para os estados `Partial` ou `NotReady`.
- O estabelecimento da ligação e a desconexão do servidor no cliente não têm tratamento completo.
- O cliente requer adaptações para Linux e macOS devido à utilização de `<conio.h>`.

Estas limitações tornam a implementação adequada a estudo e testes controlados; a utilização em produção exige trabalho adicional.

## Resolução de problemas

| Problema | Verificação |
| --- | --- |
| `g++` ou `make` não reconhecido | Confirma a instalação e a configuração do `PATH`; tenta `mingw32-make` quando aplicável |
| Erro ao abrir a porta do servidor | Verifica se já existe outro servidor ou processo a utilizar a porta `53000` |
| Cliente não consegue ligar | Confirma que o servidor está ativo e verifica o endereço, a porta, a rede e a firewall |
| DLL da SFML não encontrada | Confirma que as DLLs estão junto dos executáveis, em `bin/` |
| Erros de ligação ou arquitetura | Verifica a compatibilidade entre o compilador, a arquitetura e as bibliotecas SFML |
| Mensagens ou texto sobrepostos | A interface redesenha o terminal e ainda não sincroniza as operações das duas threads |

## Autor

[Ferdinaldo Pelembe](https://github.com/ferdinaldopelembe)

## Licença

O repositório não inclui uma licença própria para o código da aplicação. Os componentes de terceiros mantêm as respetivas licenças; a licença da SFML consta dos seus cabeçalhos.
