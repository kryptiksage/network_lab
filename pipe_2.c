#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define buf 128

int main()
{
	FILE *fout;
	int fin;
	int pipe_1[2], pipe_2[2];
	char message[buf], read_m[buf], write_m[buf];
	int status1 = pipe(pipe_1);
	if(status1!=0)
	{
		printf("Could not create pipe 1");
	}
	int status2 = pipe(pipe_2);
	if(status2!=0)
	{
		printf("Could not create pipe 2");
	}
	printf("Enter first string: ");
	scanf("%s",message);
	int pid = fork();
	if(pid!=0)
	{
		close(pipe_1[0]);
		close(pipe_2[1]);
		write(pipe_1[1], message, sizeof(message));

		read(pipe_2[0], read_m, sizeof(read_m));
		printf("\nFinal string:\n%s", read_m);
		close(pipe_1[1]);
		close(pipe_2[0]);
	}
	else
	{
		char read_m_2[buf];
		close(pipe_1[1]);
		close(pipe_2[0]);
		read(pipe_1[0], read_m, sizeof(read_m));

		printf("Enter second string: ");
		scanf("%s", read_m_2);
		fout = fopen("concat.txt", "w");
		fprintf(fout,read_m, sizeof(read_m_2));
		fprintf(fout,read_m_2, sizeof(read_m));		
		fclose(fout);

		fin = open("concat.txt", O_RDONLY);
		read(fin, write_m, buf);
		write(pipe_2[1], write_m, sizeof(write_m));
		close(fin);
		close(pipe_1[0]);
		close(pipe_2[1]);
	}
}

