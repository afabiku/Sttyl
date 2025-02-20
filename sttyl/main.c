#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <signal.h>

// Handles terminal attributes based on the attribute name and flag
int set_terminal_attribute(const char *attribute, int flag) {
  struct termios term;

  if (tcgetattr(STDIN_FILENO, &term) != 0) {
    return -1;
  }

  if (strcmp(attribute, "icrnl") == 0) {
    if (flag == 1) {
      term.c_iflag |= ICRNL;
    } else {
      term.c_iflag &= ~ICRNL;
    }
  } else if (strcmp(attribute, "onlcr") == 0) {
    if (flag == 1) {
      term.c_oflag |= ONLCR;
    } else {
      term.c_oflag &= ~ONLCR;
    }
  } else if (strcmp(attribute, "olcuc") == 0) {
    if (flag == 1) {
      term.c_oflag |= OLCUC;
    } else {
      term.c_oflag &= ~OLCUC;
    }
  } else if (strcmp(attribute, "tabs") == 0) {
    if (flag == 1) {
      term.c_oflag |= TABDLY;
    } else {
      term.c_oflag &= ~TABDLY;
    }
  } else if (strcmp(attribute, "icanon") == 0) {
    if (flag == 1) {
      term.c_lflag |= ICANON;
    } else {
      term.c_lflag &= ~ICANON;
    }
  } else if (strcmp(attribute, "isi") == 0) {
    if (flag == 1) {
      term.c_iflag |= INPCK;
    } else {
      term.c_iflag &= ~INPCK;
    }
  } else if (strcmp(attribute, "echo") == 0) {
    if (flag == 1) {
      term.c_lflag |= ECHO;
    } else {
      term.c_lflag &= ~ECHO;
    }
  } else if (strcmp(attribute, "echoe") == 0) {
    if (flag == 1) {
      term.c_lflag |= ECHOE;
    } else {
      term.c_lflag &= ~ECHOE;
    }
  } else {
    printf("Invalid attribute: %s\n", attribute);
    return -1;
  }

  if (tcsetattr(STDIN_FILENO, TCSANOW, &term) != 0) {
    return -1;
  }

  return 0;
}

// Handles erase character
int alter(char input) {
  struct termios term;

  if (tcgetattr(STDIN_FILENO, &term) != 0) {
    return -1;
  }

  term.c_cc[VERASE] = input;
  if (tcsetattr(STDIN_FILENO, TCSANOW, &term) != 0) {
    return -1;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("intr = ^C; erase = ^H; kill = ^U; start = ^Q; stop = ^S;\n");
    printf("brkint -inpck icrnl -ixany onlcr tabs\n");
    printf("iexten echo -echoe -echok\n");
    return 0;
  }

  for (int i = 1; i < argc; ++i) {

    // Changes which button is used as delete
    if (strcmp(argv[i], "erase") == 0) {
      if (i + 1 < argc) {
        if (alter(argv[i + 1][0]) == -1) {
          printf("Error changing erase\n");
        }
        i++; // Skip to next argument 
      } 
      else {
        printf("Provide a character to set as erase\n");
      }
    } 

    // Used to kill a process, get PID using "ps Aux"
    else if (strcmp(argv[i], "kill") == 0) {
      if (i + 1 < argc) {
        int PID = atoi(argv[i + 1]);
        if (kill(PID, SIGKILL) == 0) {
          printf("Killed process %d\n", PID);
        } 
        else {
          printf("Error killing process %d\n", PID);
        }
        i++; // Skip the next argument
      } 
      else {
        printf("Provide a process ID to kill\n");
      }
    } 

    // Adds \r to new lines
    else if (strcmp(argv[i], "icrnl") == 0) {
      if (set_terminal_attribute("icrnl", 1) == -1) {
        printf("Error setting icrnl\n");
      }
    } 
    else if (strcmp(argv[i], "-icrnl") == 0) {
      if (set_terminal_attribute("icrnl", 0) == -1) {
        printf("Error unsetting icrnl\n");
      }
    } 
    
    // Controls the output of new lines

    else if (strcmp(argv[i], "onlcr") == 0) {  // default
      if (set_terminal_attribute("onlcr", 1) == -1) {
        printf("Error setting onlcr\n");
      }
      printf("Error setting onlcr\n");

    } 
    else if (strcmp(argv[i], "-onlcr") == 0) { // extra space added
      if (set_terminal_attribute("onlcr", 0) == -1) {
        printf("Error unsetting onlcr\n");
      }
    } 

    // Changes lower case to upper case
    else if (strcmp(argv[i], "olcuc") == 0) {
      if (set_terminal_attribute("olcuc", 1) == -1) {
        printf("Error setting olcuc\n");
      }
    } 
    else if (strcmp(argv[i], "-olcuc") == 0) {
      if (set_terminal_attribute("olcuc", 0) == -1) {
        printf("Error unsetting olcuc\n");
      }
    }   

    // Controls the how tabs work
    else if (strcmp(argv[i], "tabs") == 0) {
      if (set_terminal_attribute("tabs", 1) == -1) {
        printf("Error setting tabs\n");
      }
    } 
    else if (strcmp(argv[i], "-tabs") == 0) {
      if (set_terminal_attribute("tabs", 0) == -1) {
        printf("Error unsetting tabs\n");
      }
    } 

    // When disabled, input is read immediately
    else if (strcmp(argv[i], "icanon") == 0) {
      if (set_terminal_attribute("icanon", 1) == -1) {
        printf("Error setting icanon\n");
      }
    } 
    else if (strcmp(argv[i], "-icanon") == 0) {
      if (set_terminal_attribute("icanon", 0) == -1) {
        printf("Error unsetting icanon\n");
      }
    } 

    // Controls the input check    
    else if (strcmp(argv[i], "isi") == 0) {
      if (set_terminal_attribute("isi", 1) == -1) {
        printf("Error setting isi\n");
      }
    } 
    else if (strcmp(argv[i], "-isi") == 0) {
      if (set_terminal_attribute("isi", 0) == -1) {
        printf("Error unsetting isi\n");
      }
    } 

    // Controls whether user input is shown
    else if (strcmp(argv[i], "echo") == 0) {
      if (set_terminal_attribute("echo", 1) == -1) {
        printf("Error setting echo\n");
      }
    } 
    else if (strcmp(argv[i], "-echo") == 0) {
      if (set_terminal_attribute("echo", 0) == -1) {
        printf("Error unsetting echo\n");
      }
    } 

    // If disabled, backspace will not delete the character on the screen, but will still delete the character in the buffer
    else if (strcmp(argv[i], "echoe") == 0) {
      if (set_terminal_attribute("echoe", 1) == -1) {
        printf("Error setting echoe\n");
      }
    } 
    else if (strcmp(argv[i], "-echoe") == 0) {
      if (set_terminal_attribute("echoe", 0) == -1) {
        printf("Error unsetting echoe\n");
      }
    } 
    
    else {
      printf("Invalid option: %s\n", argv[i]);
    }
  }

  return 0;
}