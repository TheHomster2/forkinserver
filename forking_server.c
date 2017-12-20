#include "pipe_networking.h"
#include <signal.h>

void process(char *s);
void subserver(int from_client);

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("luigi");
    exit(0);
  }
}

int main() {
  int from_client;

  while(1){
    from_client = server_setup();
    subserver(from_client);
  }
}

// what the subserver does
void subserver(int from_client) {
  // if parent stop
  if(fork()){
    close(from_client);
    return;
  }else{
    // finish handshake
    int to_client = server_connect(from_client);

    // printf("toclien: %d\nfromclient: %d\n", to_client, from_client);
    // handle all client requests
    char buf[BUFFER_SIZE];
    while(read(from_client, buf, sizeof(buf))){
      printf("recieved: [%s]\n", buf);
      process(buf);
      write(to_client, buf, sizeof(buf));
    }
    exit(0);
  }
}

// rot 13 and does upper-lowercase swapping
void process(char * buf) {
  int i = 0;
  while(i < strlen(buf)){
    if (('a' <= buf[i]) && (buf[i] < 'a' + 13))
      buf[i] += 'A' - 'a' + 13;
    else if (('A' <= buf[i]) && (buf[i] < 'A' + 13))
      buf[i] += 'a' - 'A' + 13;
    else if (('Z' - 13 < buf[i]) && (buf[i] <= 'Z'))
      buf[i] += 'a' - 'A' - 13;
    else if (('z' - 13 < buf[i]) && (buf[i] < 'z'))
      buf[i] += 'A' - 'a' - 13;
    i ++;
  }
}
