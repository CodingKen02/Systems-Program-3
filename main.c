#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <math.h>

#define bufSIZE 1024

int testError (int val, const char *msg)  //error checking and message output
{
    if (val == -1)
    {
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}

int main(int argc, char *argv[])
{
    
    double convert;
    double x, y, z;
    double roll;
    double pitch;
    double yaw;
    int i = 0;
    int pd;
    int rd;
    int fd;
    double pi = M_PI;
    
    fd = testError(open("accl.dat", O_RDONLY), "open");
    pd = testError(open("angl.dat", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR), "open for write only");
    
    while (1)
    {
        rd = testError(read(fd, &x, sizeof(double)), "read");
        rd = testError(read(fd, &y, sizeof(double)), "read");
        rd = testError(read(fd, &z, sizeof(double)), "read");
        //printf("%lf %lf %lf\n",x,y,z);
        if (rd <= 0) break;
    
        roll = atan(y/sqrt(x * x + z * z) * (180 / pi));
        pitch = atan(-x/sqrt(x * x + z * z) * (180 / pi));
        yaw = atan(z/sqrt(x * x + y * y) * (180 / pi));
        
        printf("Roll: %.03lf, Pitch: %.03lf, Yaw: %.03lf\n", roll, pitch, yaw);
        
        testError(write(pd, &convert, sizeof(double)), "write");
    }
    close(fd);
    close(pd);
}