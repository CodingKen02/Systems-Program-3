/*
----------------------------------------------------------
Program: smain.c
Date: September 5, 2023
Student Name & NetID: Kennedy Keyes kfk38
Description: This program opens the input file, "accl.dat", 
to read, so it can then create another file, "angl.dat" to 
write in. The program reads the triples one at a time to use
the x, y, and z values for the roll, pitch, and yaw calculations.
It then prints the math answers to the monitor and output file. 
Then closes.
----------------------------------------------------------
*/

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int testError(int val, const char *msg)
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
  int fd, pd, rd;
  double X, Y, Z;
  double roll, pitch, yaw;

  fd = testError(open("accl.dat", O_RDONLY), "open"); // opens the input file for reading

  pd = testError(open("angl.dat", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR), "open for write"); // opens the output file for writing

  // reads x, y, and z values from the input file, one triple at a time
  while (1) 
  {
    rd = testError(read(fd, &X, sizeof(double)), "read");
    rd = testError(read(fd, &Y, sizeof(double)), "read");
    rd = testError(read(fd, &Z, sizeof(double)), "read");
    if (rd <= 0) break;

    // calculating roll, pitch, and yaw values
    roll = atan(Y / sqrt(X * X + Z * Z)) * (180.0 / M_PI);
    pitch = atan(-X / sqrt(Y * Y + Z * Z)) * (180.0 / M_PI);
    yaw = atan(Z / sqrt(X * X + Y * Y)) * (180.0 / M_PI);

    printf("Roll: %.03lf, Pitch: %.03lf, Yaw: %.03lf\n", roll, pitch, yaw); // prints the values to the monitor

    // writes the values to the output file
    testError(write(pd, &roll, sizeof(double)), "write");
    testError(write(pd, &pitch, sizeof(double)), "write");
    testError(write(pd, &yaw, sizeof(double)), "write");
  }

  close(fd);
  close(pd);

  exit(EXIT_SUCCESS);
}
