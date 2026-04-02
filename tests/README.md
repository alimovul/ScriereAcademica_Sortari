## 📌 Nota privind fisierele de test

Pentru a mentine acest repo eficient si pentru a respecta bunele practici privind stocarea pe Git, fisierele de test nu au fost incarcate direct in proiect. Seturile de date au o dimensiune considerabila, ceea ce ar fi ingreunat semnificativ clonarea si navigarea repository-ului.
In schimb, am inclus codul sursa C++ necesar pentru a le genera local.

### ⚙️ Instructiuni pentru generarea testelor

Pentru a obtine setul complet de teste necesar evaluarii, va rog sa rulati scriptul de generare urmand pasii de mai jos:

1. **Compilarea generatorului:**
   Deschideti un terminal in directorul proiectului si compilati sursa C++:
   ```bash
   g++ testgen.cpp -o testgen
   ./testgen