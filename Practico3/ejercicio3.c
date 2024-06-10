#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int main(){

        DIR *dir = opendir("/");
	
	struct dirent * entry = readdir(dir);

	while(entry != NULL)
	{
		printf("%s\n", entry->d_name);
		entry = readdir(dir);
	}

	closedir(dir);

        return 0;
}

