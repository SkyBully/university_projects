/*
2. (3 body) Program bude fungovať aj pod OS Linux (respektíve pod iným OS).
9. (3 body) S prepínačom "-c" v kombinácii s "-i", resp. "-u" sa bude program správať ako klient, teda pripojí sa na daný soket a bude do neho posielať svoj štandardný vstup a zobrazovať prichádzajúci obsah na výstup.
11. (2 body) S prepínačom "-i" bude možné zadať aj IP adresu na ktorej bude program očakávať spojenia (nielen port).
+ 18. (5 bodov) Ak je niektoré spojenie nečinné zadanú dobu, bude zrušené.
+ 19. (1 bod) Doba nečinnosti z predchádzajúceho bodu môže byť zadaná za argumentom "-t" a/alebo ako premenná prostredia.
27. (5 bodov) Použitie signálov. E. g. znovunačítanie konfiguračného súboru po príchode zvoleného signálu, zachytenie Ctrl+C, vykonanie príkazu halt, quit a help (alebo iné).
30. (1 bod) Dobré komentáre, resp. dokumentácia, v anglickom jazyku.
*/

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pwd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>          
#include <sys/types.h>
#include <time.h> 

#include <netdb.h>
#include <memory.h>

#include <sys/un.h>

#include <signal.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/utsname.h>

#include <pthread.h>


#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>



#define MAX 1000 
//#define PORT 8080 
#define SA struct sockaddr 

#define SERVER  0
#define CLIENT  1
#define HOST    2
int LOCAL = 0;
int MODE;

char local_socket[30];
int port = 8080;
char *adrr_ip;

int check_signal = 0;
int check_client = 0;
int check_halt = 0;


int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);


char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &lsh_help,
  &lsh_exit
};

int lsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}


//directory movement function
int lsh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;
}

//help function
int lsh_help(char **args)
{
  int i;
  printf("\nAutor: Robert Yamkovyi\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < lsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }
  printf("support for special characters like: |, #, <, >\n\n");
  return 1;
}

//exit function
int lsh_exit(char **args)
{
  return 0;
}

//open file if command have <
void open_file(char **args, int *fd_r, int i)
{
  if(args[i + 1] == NULL)
    perror("lsh");
  
  *fd_r = open(args[i + 1], 0644);
  args[i] = NULL;
}

//create file if command have >
void creat_file(char **args, int *fd_w, int i)
{
  if(args[i + 1] == NULL)
    perror("lsh");
 
  *fd_w = creat(args[i + 1], 0644);
  args[i] = NULL;
}

char **lsh_split_line(char *line, char *delim);

//main function where run splited commands 
//and sent to the file descriptors
int lsh_launch(char **args, int sockfd)
{
  //pid_t pid, wpid;
  int status;
  
  int count = 0;
  while (args[count]){
    count++;
  }
  int i = 0;
  char ***commands = (char***)malloc(sizeof(char**) * count);
    
  
  while (i < count){
    commands[i] = lsh_split_line(args[i], " ");//split line by " "
    //printf("*%s\n", args[i]);
    i++;
  }
  int check_gl = 0;

  i = 0;
  int k = 0;
  while (i < count){
    while(commands[i][k]){
      if ((strcmp(commands[i][k], "<") == 0) || (strcmp(commands[i][k], ">") == 0))
        check_gl = 1;
      k++;
    }
    i++;
  }

  int out_to_client, stderr_to_client;
  int check;

  pid_t pid = fork();
  if (pid == 0) {
		int fd_w;
    int fd_r;
    int i = 0;
		//for (i = 0; i < count; i++) {
    while (i < count) {
		  
      fd_r = STDIN_FILENO;
			fd_w = STDOUT_FILENO;

			
      int fd[2];
			if (i < count - 1){ 
        pipe(fd);
      }
      if(i == count - 1 && MODE == SERVER && check_gl == 0){
        check = 1;
        out_to_client = dup(STDOUT_FILENO); 
        stderr_to_client = dup(STDERR_FILENO); 
        dup2(sockfd,STDOUT_FILENO); 
        dup2(sockfd,STDERR_FILENO); 
      }
			
      pid_t pid_c = fork();
			if (pid_c == 0) {
				if (i < count - 1) 
          fd_w = fd[1];
        
        /////////////////////////
        int j = 0;
        while(commands[i][j]){
          //printf("@**%s\n", commands[i][j]);
          if ((strcmp(commands[i][j], "<") == 0) && j != 0) {
            open_file(commands[i], &fd_r, j);
            break;
          }
          if ((strcmp(commands[i][j], ">") == 0) && j != 0) {
            creat_file(commands[i], &fd_w, j);
            break;
          }
          j++;
        }
        /////////////////////////
      
				dup2(fd_r, STDIN_FILENO);
        if(check == 1) dup2(sockfd, STDOUT_FILENO);
				else dup2(fd_w, STDOUT_FILENO);

				
				if (execvp(commands[i][0], commands[i]) == -1) 
          perror("lsh");
				exit(EXIT_FAILURE);
			}
      else if(pid_c < 0) perror("lsh");
			
			fd_r = fd[0];
			dup2(fd_r, STDIN_FILENO);
			wait(NULL);
			if (i < count - 1) 
        close(fd[1]);
      i++;
		}
		
		exit(EXIT_FAILURE);
	}
  else if(pid < 0) perror("lsh");

  //if(check_gl == 0){
  dup2(out_to_client,STDOUT_FILENO); 
  dup2(stderr_to_client,STDERR_FILENO); 
  close(out_to_client); 
  close(stderr_to_client);

	wait(NULL);

	return 1;

}

char **lsh_split_line(char *line, char *delim);

//split commands by ;
char **split_cmd(char *commands)
{
  char **args = lsh_split_line(commands, ";");
  return args;
}

//split comments by #
char **split_by_sharp(char *line)
{
  char **args = lsh_split_line(line, "#");
  args = split_cmd(args[0]);
  return args;
}

//preparing commands for execution
int lsh_exec(char *line, int sockfd)
{
  if(strlen(line) == 0) return 1; 
  char **args = split_by_sharp(line);
  char **commands;
  int i = 0;
  int tmp;
  for (i = 0; i < lsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }
  i = 0;
  while(args[i]){
    //char **commands = lsh_split_line(line, "|");
    //printf("argv = %s\n", args[i]);
    commands = lsh_split_line(args[i], "|");
    tmp = lsh_launch(commands, sockfd);
    i++;
  }
  //char **commands = lsh_split_line(line, "|");

  //return lsh_launch(commands);
  return tmp;
}

//check on command cd, help or exit
int lsh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    return 1;
  }

  for (i = 0; i < lsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  //return lsh_launch(args);
}

#define LSH_RL_BUFSIZE 1024

//read line from standart input while != '\n'
char *lsh_read_line(void)
{
  int bufsize = LSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += LSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

//function for split line by some delimiter
char **lsh_split_line(char *line, char *delim)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, delim);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, delim);
  }
  tokens[position] = NULL;
  return tokens;
}


//get name of user with assembler
char *get_user() {
    struct passwd *pws;
    int id;
    asm("mov $24, %%eax;"
        "int $0x80;"
        : "=a"(id));
    pws = getpwuid(id);
    return pws->pw_name;
}

/*
    char *system_name() {
    int  stdout_bk;
    stdout_bk = dup(fileno(stdout));
    int pipefd[2];
    pipe(pipefd);
    dup2(pipefd[1], fileno(stdout));
    asm(
        "xor %%eax, %%eax;"
        "sub $122, %%eax;"
        "lea (%%esp, %%eax, 4), %%esp;"
        "neg %%eax;"
        "mov %%esp, %%ebx;"
        "int $0x80;"
        "mov $4, %%al;"
        "lea -3(%%eax), %%ebx;"
        "lea 65(%%esp), %%ecx;"
        "lea 60(%%eax), %%edx;"
        "int $0x80;"
        ::"b"(50), "c"(50), "d"(50)
    );
    fflush(stdout);
    write(pipefd[1], stdout, 9);
    close(pipefd[1]);
    dup2(stdout_bk, fileno(stdout));
    char buf[101];
    read(pipefd[0], buf, 100);
    char *meno = (char *)malloc(50 * sizeof(char));
    strcpy(meno, buf);
    return meno;
};
*/

//function host working
void lsh_loop(char *line, int sockfd)
{
  //char *line;
  char **args;
  int status;
  //char **commands;

  if(MODE == HOST){
    do
    {
      printf("%s> ", get_user());
      line = lsh_read_line();
      //status = lsh_execute(args);
      status = lsh_exec(line, sockfd);
    }while(status);
  }


  //do {
    //printf("loop\n");
    //printf("%s> ", get_user());
    //printf("\nloop1\n");
    //printf("len = %d\ncommand: %s\n", strlen(line), line);
    
    //line = lsh_read_line();
    //args = lsh_split_line(line);
    //status = lsh_execute(args);
    //status = lsh_execute(line);
    status = lsh_exec(line, sockfd);
    //free(line);
    //free(args);
  //} while (status);
}

//get system name for promt
char *get_system_name() {
	struct utsname * buff = malloc(sizeof(struct utsname));
	uname(buff);
	return buff->nodename;
}

//get time for promt
void format_time(){
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    printf("%d:%d ", timeinfo->tm_hour, timeinfo->tm_min);
}

//function for catch signals ctrl+c
void handler(){
  check_signal = 1;
}

struct timeval timeout;
time_t client_time = 5;
//u_int32_t client_time = 5;
void func_client(int sockfd) 
{ 
    
    char buff[MAX]; 
    char exit_client[5] = "exit"; 
    int n;
    int count_cmd = 0;
    int check_sharp = 0;
    u_int32_t hours;
		u_int32_t minutes;


    int result, nread;
    fd_set inputs, testfds;
   
    FD_ZERO(&inputs);
    FD_SET(0, &inputs);
    char buffer[128];
    for (;;) { 
        format_time();
        printf("%s@%s# ", get_user(), get_system_name()); 
        bzero(buff, sizeof(buff)); 
        //memset(buff, '\0', MAX);
        //printf("Enter the string : ");
        //promt
        

        ////////////////////////////////////////////////////
        testfds = inputs;
        //timeout.tv_sec = 10;
        timeout.tv_sec = client_time;
        timeout.tv_usec = 500000;
        result = select(FD_SETSIZE+1, &testfds, (fd_set *)NULL, (fd_set*)NULL, &timeout);
        switch(result) {
          case 0:
            printf("timeout\n");
            printf("Client Exit...\n");
            write(sockfd, exit_client, sizeof(exit_client)); 
            exit(1);            
          break;
          case -1:
            perror("select\n");
            exit(1);
          default:
            if (FD_ISSET(0, &testfds)) {
              ioctl(0, FIONREAD, &nread);
              if (nread == 0) {
                printf("keyboard donen\n");
                exit(0);
              }
              //nread = read(0, buffer, nread);
              //buffer[nread] = 0;
              //printf("read %d from keyboard: %s\n", nread, buffer);
              //printf("read from keyboard \n");
            }
          break;
        }
        ////////////////////////////////////////////////////

        struct sigaction sah;
        sah.sa_handler = handler;
        sigaction(SIGINT, &sah, NULL);
        if(check_signal == 1){
          check_signal = 0;
          lsh_help(NULL);
          continue;
        }

        n = 0; 
        count_cmd = 0;
        while ((buff[n++] = getchar()) != '\n'){
          if(buff[n - 1] == '#') check_sharp = 1;
          if(check_sharp == 1) continue;
          if(buff[n - 1] == ';') count_cmd++;
          if(buff[n - 1] == '>') count_cmd--;
        }
        
        
        
        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            buff[n - 1] = '\0'; 
            write(sockfd, buff, sizeof(buff)); 
            break; 
        }
        if ((strncmp(buff, "halt", 4)) == 0) { 
            printf("Client Exit...\n");
            printf("Server Exit...\n");
            buff[n - 1] = '\0'; 
            write(sockfd, buff, sizeof(buff)); 
            break; 
        }
          
        buff[n - 1] = '\0'; 
        write(sockfd, buff, sizeof(buff)); 
        while(count_cmd >= 0){
          //printf("count_cmd = %d\n", count_cmd);
          bzero(buff, sizeof(buff)); 
          read(sockfd, buff, sizeof(buff));
          count_cmd--; 
          //sleep(3);
        
          printf("%s\n", buff); 
          //printf("From Server : %s\n", buff); 
        }
        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            break; 
        } 
    } 
} 

//client initialization
int client() 
{ 
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli;
    struct sockaddr_un serv_addr, clnt_addr;
    int len;

    if(LOCAL == 1){
      sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
      if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
      } 
      else
          printf("Local Socket successfully created..\n");

      bzero(&serv_addr, sizeof(serv_addr)); 

      serv_addr.sun_family = AF_UNIX; 
      strncpy(serv_addr.sun_path, local_socket, strlen(local_socket) + 1);
      len = sizeof(serv_addr.sun_family) + strlen(serv_addr.sun_path);

      if (connect(sockfd, (SA*)&serv_addr, sizeof(serv_addr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
      }
    }else{
  
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr(adrr_ip); 
    servaddr.sin_port = htons(port); 
  
    // connect the client socket to server socket 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
    }//!!!!!!!
    // function for chat 
    func_client(sockfd); 
  
    // close the socket 
    close(sockfd); 
} 
  

void func_server(int sockfd) 
{ 
    char buff[MAX]; 
    int n; 
    
    
    for (;;) { 
        bzero(buff, MAX);
        memset(buff, '\0', MAX); 
  
        
        read(sockfd, buff, sizeof(buff));
        if (strncmp("halt", buff, 4) == 0) {
            check_halt = 1;
            printf("Server Exit...\n"); 
            break; 
        }
        if (strncmp("exit", buff, 4) == 0) {
            check_client = 0;
            printf("Client Leaved...\n"); 
            break; 
        } 
        //printf("bufflen = %d\n%sp\n", strlen(buff), buff);
        lsh_loop(buff, sockfd);
        
        //printf("From client: %s\nTo client : ", buff); 
        bzero(buff, MAX);
        n = 0; 
        
        //while ((buff[n++] = getchar()) != '\n') 
        //    ; 
        //buff[n - 1] = '\0';
        
        //write(sockfd, buff, sizeof(buff)); 
  
        
        if (strncmp("exit", buff, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        } 
    } 
} 
  
//server initialization
int server() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
    struct sockaddr_un serv_addr, clnt_addr;

    if(LOCAL == 1){
      sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
      if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
      } 
      else
          printf("Local Socket successfully created..\n");

      bzero(&serv_addr, sizeof(serv_addr)); 
      unlink(local_socket);

      serv_addr.sun_family = AF_UNIX; 
      strncpy(serv_addr.sun_path, local_socket, strlen(local_socket) + 1);
      len = sizeof(serv_addr.sun_family) + strlen(serv_addr.sun_path);

      if (bind(sockfd, (SA*)&serv_addr, len) < 0) {
        printf("Ошибка связывания сокета с адресом\n");
        exit(1);
      }
    }else{
  
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    //servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_addr.s_addr = inet_addr(adrr_ip);
    servaddr.sin_port = htons(port); 
    //}
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
    }//!!!!!!!!!!!!!
    for(;;){
      if(check_halt == 1) break;
      // Now server is ready to listen and verification 
      if ((listen(sockfd, 5)) != 0) { 
          printf("Listen failed...\n"); 
          exit(0); 
      } 
      else
          printf("Server listening..\n"); 
      len = sizeof(cli); 
    
      // Accept the data packet from client and verification 
      connfd = accept(sockfd, (SA*)&cli, &len); 
      if (connfd < 0) { 
          printf("server acccept failed...\n"); 
          exit(0); 
      } 
      else
          {
            printf("server acccept the client...\n");
            check_client = 1;
          }
    
      
      // Function for chatting between client and server 
      func_server(connfd); 
    }
    // After chatting close the socket 
    close(sockfd); 
} 



int main(int argc, char **argv)
{
  // Load config files, if any.
  int i = 0;
  adrr_ip = "127.0.0.1";
  while(argv[i]){
    //char *local_socket = malloc(sizeof(char) * 30);
    memset(local_socket, '\0', 30);
    //printf("i = %d   *%s\n", i, argv[i]);
    if(strcmp(argv[i], "-s") == 0){
      if((argv[i + 1]) && (strcmp(argv[i + 1], "-u") == 0)){
        if(!argv[i + 2]){
          printf("Error: addr of local socket\n");
          exit(1);
        }
        strcpy(local_socket, argv[i + 2]);
        LOCAL = 1;
      }
      else if((argv[i + 1]) && (strcmp(argv[i + 1], "-i") == 0)){
        if(!argv[i + 2]){
          printf("Error: IP\n");
          exit(1);
        }
        adrr_ip = argv[i + 2];
        printf("IP = %s\n", adrr_ip);
        if((argv[i + 3]) && (strcmp(argv[i + 3], "-p") == 0)){
          if(!argv[i + 4]){
            printf("Error: port\n");
            exit(1);
          }
          port = atoi(argv[i + 4]);
        }
      }
      else if((argv[i + 1]) && (strcmp(argv[i + 1], "-p") == 0)){
        printf("PORT\n");
        if(!argv[i + 2]){
          printf("Error: port\n");
          exit(1);
        }
        port = atoi(argv[i + 2]);
      }
      MODE = SERVER;
      server();
      return EXIT_SUCCESS;
    }
    else if (strcmp(argv[i], "-c") == 0){
      if((argv[i + 1]) && (strcmp(argv[i + 1], "-u") == 0)){
        if(!argv[i + 2]){
          printf("Error: addr of local socket\n");
          exit(1);
        }
        strcpy(local_socket, argv[i + 2]);
        LOCAL = 1;
      }
      else if((argv[i + 1]) && (strcmp(argv[i + 1], "-i") == 0)){
        if(!argv[i + 2]){
          printf("Error: IP\n");
          exit(1);
        }
        adrr_ip = argv[i + 2];
        printf("IP = %s\n", adrr_ip);
        if((argv[i + 3]) && (strcmp(argv[i + 3], "-p") == 0)){
          if(!argv[i + 4]){
            printf("Error: port\n");
            exit(1);
          }
          port = atoi(argv[i + 4]);
        }
      }
      else if((argv[i + 1]) && (strcmp(argv[i + 1], "-t") == 0)){
        printf("TIME\n");
        if(!argv[i + 2]){
          printf("Error: time\n");
          exit(1);
        }
        client_time = (time_t)(atoi(argv[i + 2]));
      }
      else if((argv[i + 1]) && (strcmp(argv[i + 1], "-p") == 0)){
        if(!argv[i + 2]){
          printf("Error: port\n");
          exit(1);
        }
        port = atoi(argv[i + 2]);
      }
      MODE = CLIENT;
      client();
      return EXIT_SUCCESS;
    }
    else if(i == 0 && !(argv[i + 1])){
      MODE = HOST;
      printf("HOST\n");
      lsh_loop(NULL, 0);
    }
    i++;
  }
  

  // Run command loop.
  //lsh_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}