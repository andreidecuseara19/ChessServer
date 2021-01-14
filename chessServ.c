#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>

#define PORT 8989
#define BUFSIZE 4096
#define SOCKETERROR (-1)
#define SERVER_BACKLOG 1

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

//definire functii
int handle_connection(int client_socket, int tabla[8][8], int state);
int check(int exp, const char *msg);
int verificare(char miscare[5], int tabla[8][8]);
int update(int tabla[8][8],char miscare[5]);
int inittabla(int tabla[8][8]);
int verifsah(int tabla[8][8]);

int main(int argc, char *argv[])
{
		int tabla[8][8];
        int quit = 0, comandaCaz = 0, init = 0;
        int pipeParinteCopil[2];
        int pipeCopilParinte[2];
        int pipeMiscare[2];

        printf("Procesul a fost initializat... \n");

        pipe(pipeMiscare);

        pid_t pid;


        int server_socket, client_socket, addr_size, nr_con;
        SA_IN server_addr, client_addr;

        check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Eroare la crearea socket-ului");

//init the adress struct
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(PORT);

        check(bind(server_socket,(SA*)&server_addr, sizeof(server_addr)), "Eroare la bind");
        check(listen(server_socket, SERVER_BACKLOG), "Eroare la listen");

inittabla(tabla);
while(quit == 0)
    switch(pid = fork())
    {
        case -1:
        {
        //Caz Eroare
                printf("Eroare la crearea procesului... \n");
                exit(0);
        }

        case 0:
        {
        //Proces Copil
        printf("S-a intrat in copil \n");
        //
        printf("Se asteapta conexiunea...\n")
        //Se asteapta si se accepta conexiunea
        addr_size = sizeof(SA_IN);

        check(client_socket = accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size), "Eroare la acceptare");

        printf("Conectare reusita !\n");
        nr_con = nr_con+1;

        int state = 2;
        handle_connection(client_socket, tabla [8][8], state);


        }

        default:
        {
        //Proces Parinte


        //
        printf("S-a intrat in parinte \n");

        printf("Se asteapta conexiunea...\n")
        //Se asteapta si se accepta conexiunea
        addr_size = sizeof(SA_IN);

        check(client_socket = accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size), "Eroare la acceptare");

        printf("Conectare reusita !\n");

        int state = 1;
        handle_connection(client_socket, tabla[8][8], state);

        }
    }

int handle_connection(int client_socket, int tabla[8][8], int state)
{
        char buffer[BUFSIZE];
        size_t bytes_read;
        int msgsize = 0;
        char rasp;

// state 1- clientul trimite miscari de sah la server si asteapta sa fie verificate, daca miscarea e buna se intra in starea 2
// state 2- clientul asteapta ca celalalt client sa faca o miscare corecta,o primeste de la server si trece in starea 1
if(state == 1)
{
        rasp = 'R';
        write(client_socket, rasp, sizeof(rasp));

        do
        {


        while((bytes_read = read(client_socket, buffer+msgsize, sizeof(buffer)-msgsize-1)) > 0 )
        {
                msgsize += bytes_read;
                if(msgsize > BUFSIZE-1 || buffer[msgsize-1] == '\n') break;
        }
        strcpy(client[nr_con].nume, buffer)
        client[nr_con].adresa = client_socket;

// Daca miscarea primita de la client este valida, se trimite inapoi D si se trece in starea 2, daca nu, se asteapta alta miscare de la client
        if(verificare(miscare, tabla) == 0)
        {rasp = 'N';}
        if(verificare(miscare, tabla) == 1)
        {rasp = 'D';
        state = 2;
        write(pipeMiscare[1], miscare, sizeof(miscare));
        update(tabla, miscare);}
        write(client_socket, rasp, sizeof(rasp));

        }while(verificare(miscare, tabla) == 0)

}

if(state == 2)
{
        rasp = 'R';
        read(pipeMiscare[0], miscare, sizeof(miscare));
        update(tabla, miscare);
        write(client_socket, miscare, sizeof(miscare));
        state = 1;
}

return tabla;
}

int verificare(char miscare[5], int tabla[8][8])
{int rasp = 0;

        // a-h -> 0-7 ; "0"-"7" - 0-7
        int lin1, col1, lin2, col2;

        lin1 = miscare[0] - 'a';
        col1 = miscare[1] - '0';
        lin2 = miscare[3] - 'a';
        col2 = miscare[4] - '0';

        if(lin1 >7 || lin1 <0)
        {return rasp;}
        if(col1 >7 || col1 <0)
        {return rasp;}
        if(lin2 >7 || lin2 <0)
        {return rasp;}
        if(col2 >7 || col2 <0)
        {return rasp;}

        int tippiesa = tabla[lin1][col1];
        switch(tippiesa)
        {
                case 0:
                return rasp;

                case 1:
                {
                        if(lin2 <= lin1)
                        return rasp;
                        else if(lin2 - lin1 >1)
                        return rasp;
                        else if(col2 - col1 >1)
                        return rasp;
                        else if(lin2 == lin1)
                        return rasp;
                        else if(col2 == col1 && lin2 == lin1)
                        return rasp;
                        else if(col2 == col1 && tabla[lin2][col2] == -1)
                        return rasp
                        else rasp = 1;
                }

                case -1:
                {
                        if(lin2 <= lin1)
                        return rasp;
                        else if(lin2 - lin1 <-1)
                        return rasp;
                        else if(col2 - col1 <-1)
                        return rasp;
                        else if(lin2 == lin1)
                        return rasp;
                        else if(col2 == col1 && lin2 == lin1)
                        return rasp;
                        else if(col2 == col1 && tabla[lin2][col2] == 1)
                        return rasp
                        else rasp = 1;
                }
        }

return rasp;
}

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

return 0;
}
