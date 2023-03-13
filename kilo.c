/*** includes ***/
#include<unistd.h>
#include <termios.h>
#include <stdlib.h>
#include<ctype.h>
#include <stdio.h>
#include <errno.h>

/*** data ***/
struct termios orig_termios;

/*** terminal ***/
void die(const char *s)
{
    perror(s);
    exit(1);
}

void disableRawMode()
{
      if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode()
{
      if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios;

    //disable control flags
 // I - Input flag O - Output flag
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag &= ~(CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN |ISIG );
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
// ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
//TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
      if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");

}

/*** init ***/
int main()
{
    enableRawMode();
    while(1){
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
        // iscntrl() tests whether a character is a control character.
        if(iscntrl(c)){
            printf("%d\r\n",c);
        }else{
            printf("%d('%c)\r\n", c,c);
        }
        if(c=='q') break;
    }
    return 0;
}

