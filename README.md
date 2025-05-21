# Pong Showdown
## 📌Descrição do Projeto
Pong Showdown é um jogo de arcade em linha de comando desenvolvido em C, baseado no clássico Pong, com elementos modernos como placares persistentes, física de bola aprimorada e menus interativos.

O projeto foi desenvolvido para a disciplina de Programação Imperativa e Funcional (PIF), atendendo a todos os requisitos obrigatórios:

✔ Estruturas (structs)

✔ Ponteiros e alocação dinâmica

✔ Listas encadeadas (histórico de placares)

✔ Matrizes (renderização do campo)

✔ Leitura/escrita em arquivo (salvamento de scores)

✔ Biblioteca CLI-LIB obrigatória (controle de tela, teclado e timer)

## 🎮 Como Jogar

### Controles

**Jogador 1 (Esquerda):**

W – Mover para cima

S – Mover para baixo

**Jogador 2 (Direita):**

I – Mover para cima

K – Mover para baixo

**Menu:**

ESPAÇO – Iniciar jogo

Q – Sair

R – Resetar placares

## 📝 Regras

O primeiro jogador a marcar 10 pontos vence.

A bola acelera após cada rebatida.

Placar salvo automaticamente em pong_scores.dat.

## 🛠️Compilação

**GCC instalado**

**Linux/MacOS (ou WSL no Windows)**

## 🕹️Execução

git clone https://github.com/GUSTAVO-PEDROSA-MIRANDA/PongShowdown.git

cd PongShowdown

make

./pong
