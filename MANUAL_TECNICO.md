# Manual Tecnico

## 1. Introducao

Este documento apresenta o manual tecnico do sistema de gestao de tickets de helpdesk informatico desenvolvido em linguagem C.

O programa funciona em modo consola e permite gerir pedidos de suporte, tecnicos, categorias de tickets, historico de intervencoes, relatorios e alertas SLA. A aplicacao usa listas ligadas em memoria e guarda os dados em ficheiros locais.

## 2. Objetivo do programa

De forma resumida, o sistema permite:

- Registar administradores e tecnicos.
- Validar tecnicos antes de estes poderem aceder ao sistema.
- Criar, listar, editar, remover, filtrar e ordenar tickets.
- Atribuir tickets a tecnicos.
- Aceitar, atualizar, comentar e delegar tickets.
- Consultar historico de alteracoes.
- Gerar relatorios estatisticos.
- Exportar tickets para CSV.
- Guardar e carregar dados automaticamente.

## 3. Requisitos

### Requisitos de software

- Sistema operativo Windows.
- Compilador GCC.
- Opcionalmente, `make` instalado para usar o `Makefile`, RECOMENDADO.
- Terminal ou linha de comandos.

## 4. Compilacao

O projeto inclui um `Makefile` com as regras de compilacao. Existem duas formas principais de compilar.

### Opcao 1: Compilar com Make

Na raiz do projeto, executar:

```bash
make
```

Este comando usa a regra `all` do `Makefile` e gera o executavel `exe`.

Para compilar e executar diretamente:

```bash
make run
```

Para limpar o executavel gerado:

```bash
make clean
```

### Opcao 2: Compilar manualmente com GCC

Caso o utilizador nao tenha `make` instalado, pode usar diretamente o comando original equivalente ao `Makefile`:

```bash
gcc -Wall -Wextra -g src/main.c src/funcoes_users.c src/funcoes_ticket.c src/funcoes_utilitarias.c src/funcoes_historico.c src/funcoes_ticketTypes.c src/funcoes_exit.c src/persistencia.c src/ui/ui.c src/input/input.c src/logs/log.c -o exe
```

Em Windows, o compilador ira normalmente gerar `exe.exe`.

## 5. Execucao

(Caso nao tenha utilizado o comando `make run`).
Depois de compilar, executar o programa com:

```bash
.\exe.exe
```

Ou, dependendo do ambiente:

```bash
./exe
```

No arranque, o programa tenta carregar os dados persistidos a partir da pasta `data/`. Se os ficheiros ainda nao existirem, o sistema mostra um aviso e continua a execucao.

No primeiro arranque, se nao existirem utilizadores, e criado automaticamente um administrador.

## 6. Funcionalidades principais

### Administrador

O perfil de administrador permite:

- Criar tickets.
- Editar tickets.
- Listar todos os tickets.
- Ver detalhes de um ticket por ID.
- Remover tickets.
- Filtrar tickets por prioridade, tipo ou estado.
- Ordenar tickets por data, prioridade, tecnico ou ID.
- Gerir categorias de tickets.
- Listar tecnicos.
- Validar tecnicos pendentes.
- Ver tickets sem tecnico associado.
- Atribuir tecnico a um ticket.
- Consultar historico de tickets.
- Ver tempos medios de resolucao.
- Gerar relatorios.
- Consultar alertas SLA.

### Tecnico

O perfil de tecnico permite:

- Ver os seus tickets.
- Aceitar tickets pendentes.
- Atualizar o estado de tickets.
- Adicionar comentarios ou acoes realizadas.
- Delegar tickets a outro tecnico.
- Exportar tickets para CSV.
- Consultar alertas SLA, quando existirem.

### Autenticacao e registo

O sistema suporta:

- Login por `username` e `password`.
- Registo de novos tecnicos.
- Alteracao de password.
- Validacao de tecnicos pelo administrador.
- Bloqueio de login para tecnicos ainda nao validados.

## 7. Exemplos de utilizacao

### Criar um ticket como administrador

1. Executar o programa.
2. Escolher `1 - Login`.
3. Entrar com credenciais de administrador.
4. Escolher `1 - Adicionar ticket`.
5. Selecionar a categoria.
6. Inserir descricao.
7. Escolher prioridade.
8. Indicar o utilizador que reportou o problema.
9. Confirmar a criacao.

### Validar um tecnico

1. Entrar como administrador.
2. Escolher `10 - Validar tecnicos pendentes`.
3. Consultar a lista de tecnicos pendentes.
4. Inserir o ID do tecnico a validar.
5. Confirmar a operacao.

### Aceitar um ticket como tecnico

1. Entrar como tecnico validado.
2. Escolher `2 - Aceitar ticket pendente`.
3. Ver a lista de tickets pendentes.
4. Inserir o ID do ticket.
5. O ticket passa a ficar associado ao tecnico.

### Gerar um relatorio mensal

1. Entrar como administrador.
2. Escolher `16 - Gerar relatorio`.
3. Escolher `1 - Mensal Estatistico`.
4. Inserir o mes e ano no formato `mm/YYYY`.
5. O relatorio e criado na pasta `reports/`.
