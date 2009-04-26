#include <stdio.h>
#include <string.h>
#include "../common/Matrix.h"

int main(int argc, char **argv)
{
    char buffer[300];
    if (argc<2)
    {
        printf("You must give a matrix file data to convert as parameter.\n");
        return 1;
    }
    printf("converting %s.\n",argv[1]);
    Matrix m(argv[1],0);
    printf("Loaded %i lines and %i columns\n",m.nLine(),m.nColumn());
    strcpy(buffer,argv[1]);
    strcat(buffer,".txt");
    m.save(buffer,1);
}
