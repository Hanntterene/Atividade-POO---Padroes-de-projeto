# Sistema de Reservas de Hotéis - POO com Padrões de Projeto

Este projeto é um sistema simples de reservas de hotéis, desenvolvido em C++ para fins acadêmicos, utilizando conceitos de Programação Orientada a Objetos e padrões de projeto, como Singleton e Strategy.

## Funcionalidades

- **Login de Atendentes:** Apenas usuários autenticados podem acessar o sistema.
- **Cadastro de Reservas:** Permite cadastrar reservas para clientes, escolhendo localidade, tipo de quarto, data e diárias.
- **Política de Descontos:** Aplicação de diferentes estratégias de desconto (sem desconto, VIP, baixa temporada, feriado).
- **Confirmação de Reservas:** Confirmação de reservas mediante pagamento.
- **Visualização de Reservas:** Listagem de todas as reservas cadastradas.
- **Persistência:** As reservas são salvas e carregadas automaticamente de um arquivo `reservas.csv`.

## Padrões de Projeto Utilizados

- **Singleton:** Para garantir que exista apenas um controlador de reservas no sistema.
- **Strategy:** Para aplicar diferentes políticas de desconto de forma flexível.

## Como Compilar

Compile apenas o arquivo `hoteis.cpp`:

```sh
g++ -std=c++11 -o hoteis hoteis.cpp
```

## Como Executar

No terminal, execute:

```sh
./hoteis
```
ou, no Windows:
```sh
hoteis.exe
```

## Estrutura do Projeto

- `hoteis.cpp` — Código-fonte principal do sistema.
- `reservas.csv` — Arquivo gerado automaticamente para persistência das reservas.
- `readme.md` — Este arquivo de documentação.

## Observações

- O sistema já vem com alguns atendentes cadastrados (veja no código).
- O arquivo de reservas é carregado automaticamente após o login e salvo sempre que há alterações.
- O código é auto-contido, não depende de outros arquivos de cabeçalho.

---

Desenvolvido para fins didáticos na disciplina de Programação Orientada a Objetos.