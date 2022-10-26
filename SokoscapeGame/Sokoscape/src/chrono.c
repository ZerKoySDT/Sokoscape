#include "chrono.h"

int changeTime(char timer[6]){
  if(timer[4] > '0'){   // Si la seconde n'est pas 0
    timer[4] -= 1;    // On diminue de 1
  } else {
    timer[4] = '9';   // Sinon on met le chiffre des unités pour les secondes à 9
    if(timer[3] > '0'){   // Si le chiffre des dizaines pour les secondes n'est pas à 0
      timer[3] -= 1;    // On diminue de 1
    } else {
      timer[3] = '5';   // Sinon, on met le chiffre des dizaines à 5
      if(timer[1] > '0'){   // Si le chiffre des unités des minutes n'est pas 0
        timer[1] -= 1;    // On le diminue
      } else {
        return 1;   // Sinon le chronomètre est arrivé à sa fin
      }
    }
  }
  return 0;
}
