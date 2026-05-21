#include "ui.h"
#include <stdio.h>
#include <string.h>

void Navbar() {
  //  printf("\033[0;32m");

  // clang-format off
  /* prettier-ignore */
    printf(
        "    ░██     ░██            ░██                   ░██                       ░██                                                   ░██                               \n"
        "    ░██     ░██            ░██                   ░██                       ░██                                                   ░██                               \n"
        "    ░██     ░██  ░███████  ░██ ░████████   ░████████  ░███████   ░███████  ░██    ░██         ░███████  ░██    ░██  ░███████  ░████████  ░███████  ░█████████████  \n"
        "    ░██████████ ░██    ░██ ░██ ░██    ░██ ░██    ░██ ░██    ░██ ░██        ░██   ░██ ░██████ ░██        ░██    ░██ ░██           ░██    ░██    ░██ ░██   ░██   ░██ \n"
        "    ░██     ░██ ░█████████ ░██ ░██    ░██ ░██    ░██ ░█████████  ░███████  ░███████           ░███████  ░██    ░██  ░███████     ░██    ░█████████ ░██   ░██   ░██ \n"
        "    ░██     ░██ ░██        ░██ ░███   ░██ ░██   ░███ ░██               ░██ ░██   ░██                ░██ ░██   ░███        ░██    ░██    ░██        ░██   ░██   ░██ \n"
        "    ░██     ░██  ░███████  ░██ ░██░█████   ░█████░██  ░███████   ░███████  ░██    ░██         ░███████   ░█████░██  ░███████      ░████  ░███████  ░██   ░██   ░██ \n"
        "                               ░██                                                                             ░██                                                 \n"
        "                               ░██                                                                       ░███████                                                  \n"
    );
  // clang-format on
  // Reset cor
  // printf("\033[0m");
}

void Title(const char *title, const char *icon) {
  int box_width = 50;        // largura interna da caixa
  int icon_visual_width = 2; // emojis ocupam 2 colunas visuais
  int title_len = strlen(title);
  int total_visual =
      icon_visual_width + 1 + title_len; // ícone + espaço + título

  int padding_total = box_width - total_visual;
  int padding_left = padding_total / 2;
  int padding_right = padding_total - padding_left;

  printf("\n");
  printf("╔══════════════════════════════════════════════════╗\n");
  printf("║%*s%s %s%*s║\n", padding_left, "", icon, title, padding_right, "");
  printf("╚══════════════════════════════════════════════════╝\n");
  printf("\n");
}

void Separator() {
  printf("──────────────────────────────────────────────────\n");
}