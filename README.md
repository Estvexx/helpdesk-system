# name do Projeto

Breve descrição do que o projeto faz numa ou duas frases.

## 📋 Descrição

Explicação mais detalhada do propósito do projeto, o problema que resolve e o contexto em que foi desenvolvido.

## 🛠️ Tecnologias

- **Linguagem:** C
- **Compilador:** GCC
- **Build:** Make

## 📁 Estrutura do Projeto

```
projeto/
├── src/
│   ├── main.c        # Ponto de entrada
│   ├── funcoes.c     # Implementação das funções
│   └── funcoes.h     # Declarações e protótipos
├── .gitignore
├── Makefile
└── README.md
```

## Instalação do Make (Windows)

```bash
winget install GnuWin32.Make
```

Depois de instalar, adiciona ao PATH:

1. Pesquisa **"variáveis de ambiente"** no Windows
2. Em **PATH** clica **Editar**
3. Adiciona o seguinte caminho:

```
C:\Program Files (x86)\GnuWin32\bin
```

4. Reinicia o terminal

## ⚙️ Compilação

Certifica-te de que tens o `gcc` e o `make` instalados.

Para compilar (só compilar):

```bash
make
```

Para compilar e executar:

```bash
make run
```

Para limpar os ficheiros compilados:

```bash
make clean
```

## 🚀 Como Usar

```bash
./programa
```

Exemplo de utilização:

```bash
./exe argumento1 argumento2
```

### Primeiro acesso

Ao iniciar pela primeira vez, usa as credenciais por defeito do administrador:

- **Utilizador:** `admin`
- **Password:** `admin`

⚠️ No primeiro login, será obrigatório alterar a password do administrador.

## 👤 Autor

**Francisco Esteves**
**Rodrigo Pinto**

- GitHub: [@franciscoesteves](https://github.com/franciscoesteves)

## 📄 Licença

Este projeto está sob a licença MIT. Consulta o ficheiro [LICENSE](LICENSE) para mais detalhes.
