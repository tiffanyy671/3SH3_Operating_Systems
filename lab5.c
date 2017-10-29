/*
 * lab5.c
 *
 *  Created on: April 7, 2016
 *      Author: Bradley Kohler
 */

//includes
#include <sys/types.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>


//color scheme
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


//global variables
int count = 0;


//global functions
void openli(char * dir_name);
void fileinfo(struct stat buf);
int main(int argc, char **argv);


//function openli
/*
opens a directory and reads all of the file contents.
this function is transversal.
*/
void openli(char * dir_name){

  //function variables
  struct dirent *readDir;
  DIR * dir;
  dir = opendir(dir_name); //opendir
  int i; //for iteration purposes
  struct stat buf; //for file
  char dest[200]; //char * is bound to a 200 char directory

  //add tabs
  for(i=0;i<count;i++){
    printf("\t");
  }

  //change indentation for future printf
  count++;

      //search for directory
      if(dir == NULL) {
      printf("ERROR:\n(No directory found).\n");
      }

      else {
      printf("%sDirectory %s:\n" , KGRN,dir_name);
      //print all files in directory
      while((readDir = readdir(dir)) != NULL) {
        if(strcmp(readDir->d_name, "..") && strcmp(readDir->d_name, ".")){

              //store the current file in dest with path
              sprintf(dest, "%s/%s", dir_name , readDir->d_name);

              stat(dest, &buf);

              //if file in buf is not a directory
              if(!S_ISDIR(buf.st_mode)){

              //print indentation
              for(i=0;i<count;i++){
                printf("\t");
              }

              //print file to screen
              printf("%sfile_name: %s/%s\n", KCYN, dir_name, readDir->d_name);

              }

              //if file is a directory
              else{

                openli(dest);

              }
        }
      }

      //change indentation for future printf
      count --;

      //add tabs
      for(i=0;i<count;i++){
        printf("\t");
      }

      //print done when finished reading directory contents
      printf("%sDone Reading Directory %s:\n" , KYEL,dir_name);
      //close the dir
      closedir(dir);
      }
}


//function fileinfo
/*
prints the info of a file.
info includes:
-permissions
-links
-group name
-user name
-size in bloacks
-size in bytes
-last modified
*/
void fileinfo(struct stat buf){

  //print permissions
  printf("\n%spermissions-\n",KWHT);
  printf( (buf.st_mode & S_IRUSR) ? "r" : "-");
  printf( (buf.st_mode & S_IWUSR) ? "w" : "-");
  printf( (buf.st_mode & S_IXUSR) ? "x" : "-");
  printf( (buf.st_mode & S_IRGRP) ? "r" : "-");
  printf( (buf.st_mode & S_IWGRP) ? "w" : "-");
  printf( (buf.st_mode & S_IXGRP) ? "x" : "-");
  printf( (buf.st_mode & S_IROTH) ? "r" : "-");
  printf( (buf.st_mode & S_IWOTH) ? "w" : "-");
  printf( (buf.st_mode & S_IXOTH) ? "x" : "-");
  printf("\n\n");

  //print links
  printf("%snumber of Links-\n%d\n\n",KCYN ,buf.st_nlink);

  //print name
  struct group * grp;
  struct passwd * pwd;
  struct tm * tim;

  if ((grp = getgrgid(buf.st_gid)) != NULL)
    printf("%sgroup name-\n%-8.8s\n",KYEL,grp->gr_name);
  else
    printf("%sno group-\n%-8d\n",KYEL,buf.st_gid);
  printf("\n");
  if ((pwd = getpwuid(buf.st_uid)) != NULL)
    printf("%sowner's name-\n%-8.8s\n",KYEL,pwd->pw_name);
  else
    printf("%sno owner-\n%-8d\n",KYEL,buf.st_uid);
  printf("\n");

  //print size
  printf("%sfile size-\n%d bytes\n",KCYN,buf.st_size);
  printf("\n");
  printf("%sfile size-\n%d blocks\n",KCYN,buf.st_blocks);
  printf("\n");
  printf("%slast modified-\n%d\n\n",KWHT,buf.st_mtime);
}


//main function
/*
*/
int main(int argc, char **argv){

//function variables
struct stat buf;

//use argv[1] for buf
if(stat(argv[1], &buf) < 0 )
  return 1;

//if file is a directory
if(S_ISDIR(buf.st_mode)) {
  openli(argv[1]);
}

//if file is not a directory
else if (!S_ISDIR(buf.st_mode)) {
  fileinfo(buf);
}

//return
return 1;
}
