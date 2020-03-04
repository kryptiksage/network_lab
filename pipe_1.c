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
	printf("Enter a string: ");
	scanf("%[^\t]",message);
	int pid = fork();
	if(pid!=0)
	{
		close(pipe_1[0]);
		close(pipe_2[1]);
		write(pipe_1[1], message, sizeof(message));

		read(pipe_2[0], read_m, sizeof(read_m));
		printf("\nCounts:\n%s", read_m);
		close(pipe_1[1]);
		close(pipe_2[0]);
	}
	else
	{
		close(pipe_1[1]);
		close(pipe_2[0]);
		read(pipe_1[0], read_m, sizeof(read_m));

		int c_count=0, w_count=0, l_count=0;
		if(strlen(read_m)!=0)
		{
			w_count++; l_count++;
		}
		for(int i=0;i<strlen(read_m);i++)
		{
			if(read_m[i] != ' ' && read_m[i] != '\n')
				c_count++;
			if(read_m[i]==' ' || read_m[i]=='\n')
				w_count++;
			if(read_m[i]=='\n')
				l_count++;
		}
		fout = fopen("counts.txt", "w");
		fprintf(fout, "Characters: %d\n", c_count);
		fprintf(fout, "Words: %d\n", w_count);
		fprintf(fout, "Lines: %d\n", l_count);
		fclose(fout);

		fin = open("counts.txt", O_RDONLY);
		read(fin, write_m, buf);
		write(pipe_2[1], write_m, sizeof(write_m));
		close(fin);
		close(pipe_1[0]);
		close(pipe_2[1]);
	}
}

