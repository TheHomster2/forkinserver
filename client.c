#include "pipe_networking.h"

int main() {
  int to_server;
  int from_server;
  char buffer[BUFFER_SIZE];

  from_server = client_handshake( &to_server );
  // printf("toserver:%d\n from_server: %d\n", to_server, from_server);
  // exchange messages until user disconnects
  while (1) {
    printf("enter data: ");
    fgets(buffer, sizeof(buffer), stdin);
    *strchr(buffer, '\n') = 0;

    write(to_server, buffer, sizeof(buffer));

    printf("sent:%s\n", buffer);
    read(from_server, buffer, sizeof(buffer));
    perror("read");
    printf("received: [%s]\n", buffer);
  }
}
