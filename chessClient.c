#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>

int verificare(char miscare[5], int tabla[8][8]);
int update(int tabla[8][8],char miscare[5]);
int inittabla(int tabla[8][8]);
int verifsah(int tabla[8][8]);

int main(int argv, char *argc)
{

/* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);

  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

int rasp,tabla[8][8], state;
char miscare[5],rasp[1];
inittabla(tabla);

// state 1- clientul trimite miscari de sah la server si asteapta sa fie verificate, daca miscarea e buna se intra in starea 2
// state 2- clientul asteapta ca celalalt client sa faca o miscare corecta,o primeste de la server si trece in starea 1
do{
if(state == 1)
{
  /* citirea mesajului */
  bzero (msg, 100);
  printf ("[client]Introduceti miscarea: ");
  fflush (stdout);
  read (0, msg, 100);

  /* trimiterea miscarii la server */
  if (write (sd, miscare, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }

  /* citirea raspunsului dat de server
     (apel blocant pina cind serverul raspunde) */
  if (read (sd, rasp, sizeof(rasp)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  if (strcmp(rasp, 'D') == 1)
    {
      update(tabla, miscare);
      state = 2;
      afisare(tabla);
    }
}

if(state == 2)
{
    read (sd, miscare, sizeof(miscare));
    update(tabla, miscare);
    state = 1;
}

}while(verifsah(tabla) == 0);

  /* inchidem conexiunea, am terminat */
  close (sd);

// functie care da update la tabla de sah cu miscarea valida
int update(int tabla[8][8], char miscare[5])
{
        int lin1,col1,lin2,col2;

        lin1 = miscare[0] - 'a';
        col1 = miscare[1] - '0';
        lin2 = miscare[3] - 'a';
        col2 = miscare[4] - '0';

        int tippiesa = tabla[lin1][col1];
        tabla[lin1][col1] = 0;
        tabla[lin2][col2] = tippiesa;

return tabla;
}

// pune tabla in pozitia initiala
int inittabla(int tabla[8][8])
{int i,j;

for(i = 0;i < 8; i++)
for(j = 0;j < 8; j++)
tabla[i][j] = 0;

for(i = 0;i < 2; i++)
for(j = 0;j < 8; j++)
tabla[i][j] = -1;

for(i = 6;i < 8; i++)
for(j = 0;j < 8; j++)
tabla[i][j] = 1;

return tabla;
}

//verifica daca jocul e terminat (daca a ramas doar un pion)
int verifsah(int tabla[8][8])
{int sah = 0,i,j,count;

for(i = 0;i < 8; i++)
for(j = 0;j < 8; j++)
        if(tabla[i][j] != 0)
        count++;
if(count <= 1)
sah = 1;

return sah;
}

//afiseaza tabla
void afisare(int tabla[8][8])
{int i,j;
for(i = 0;i < 8;i++)
{printf("\n");
for(j = 0;j < 8;j++)
printf("%d", tabla[i][j]);
}}

return 0;
}

