#include "20171672.h"

/*------------------------------------------------*/
/*Function : InsertNode()*/
/*Purpose : Put user's input into linked list for 'History' command.*/
/*No return value.*/
/*------------------------------------------------*/

void InsertNode(NODE *head, char input[]){
    
    struct NODE *new = malloc(sizeof(struct NODE));
    strcpy(new->command, input);
    new->link = head->link;
    head->link = new;
}

/*------------------------------------------------*/
/*Function : FreeNode()*/
/*Purpose : Free the node.*/
/*No return value.*/
/*------------------------------------------------*/

void FreeNode(NODE *head){

    while(head->link != NULL){
        NODE *tmp = head;
        head = head->link;
        free(tmp);
    }

}

/*------------------------------------------------*/
/*Function : FreeHash()*/
/*Purpose : Free the hashtable.*/
/*No return value.*/
/*------------------------------------------------*/

void FreeHash(){

    for(int i = 0; i < HASHSIZE; i++){
        while(hashtable[i] != NULL){
          HASH *tmp = hashtable[i];
          hashtable[i] = hashtable[i]->next;
          free(tmp);
        }
    }
}

/*------------------------------------------------*/
/*Function : help()*/
/*Purpose : Function 'help' command.*/
/*No return value.*/
/*------------------------------------------------*/

int help(){

	printf("h[elp]\nd[ir]\nq[uit]\nhi[story]\ndu[mp] [start, end]\ne[dit] address, value\nf[ill] start, end, value\nreset\nopcode mnemonic\nopcodelist\n\n");

}

/*------------------------------------------------*/
/*Fucntion : dir()*/
/*Purpose : Function 'dir' command. 
            Print current directory's file name and state. */
/*No return value*/
/*------------------------------------------------*/

void dir(DIR *dp, struct dirent *file, struct stat buf){
    
    dp = opendir(".");

    if(dp == NULL){
      printf("There is no directory.\n");
      return;
    }

    while(1){
      file = readdir(dp);
      if(!file) break;
      
      if(strcmp(file->d_name, ".") == 0) continue;
      if(strcmp(file->d_name, "..") == 0) continue;
      stat(file->d_name, &buf);
      if(buf.st_mode & S_IFDIR){
        printf("%s/   ", file->d_name);
      }
      else if(buf.st_mode & S_IXUSR){
        printf("%s*   ", file->d_name);
      }
      else{
        printf("%s    ", file->d_name);

      }

    }
    
    closedir(dp);

    printf("\n");
}

/*------------------------------------------------*/
/* 
   Function : history()
   Purpose : Function 'history' command.
             Print all the previous valid commands in the linked list.
   No return value.
*/
/*------------------------------------------------*/

int history(NODE *current){
    int count=1;
    while(current != NULL){
      printf("%d  %s\n", count,current->command);
      current = current->link;
      count++;
    }

}

/*------------------------------------------------*/
/*
    Function : dump()
    Purpose : Function 'dump', 'dump start', and 'dump start, end' command.
              Print the memory address, memory content, and content in ASCII
              code.
    No return value.
*/
/*------------------------------------------------*/

void dump(unsigned char *BUFFER, int *START_ADDRESS, int *END_ADDRESS){
  
  int i=0, j = 0, k = 0;
  int MEMORY_NUM = 0, LIMIT_MEMORY = 160;
  int START_X=(*START_ADDRESS)/16, START_Y= (*START_ADDRESS)%16;
  int Hex_Num = START_X, END_SIGN=0, IsCompleteLine = 0;

  unsigned char **NEW_BUFFER;
  NEW_BUFFER = (unsigned char**)malloc(sizeof(unsigned char*)*MEGA_BYTE_ROW);
  for(i=0; i < MEGA_BYTE_ROW; i++){
      NEW_BUFFER[i] = (unsigned char *)malloc(sizeof(char) * 16);
  }

  for(i=START_X; i < 11+START_X; i++){
        for(k = 0; k < 16; k++){
            NEW_BUFFER[i][k] = BUFFER[j + 16*START_X];
            j++;
        }
  }

  if(*END_ADDRESS != 0){
      LIMIT_MEMORY = *END_ADDRESS - *START_ADDRESS + 1; // LIMIT SETTING
  } 

  i=START_X;
  while(1){
      for(k=0; k < 16; k++){
       if(i*16 + k > 0xfffff){
         printf("   ");
         END_SIGN = 1;
       }

       if(MEMORY_NUM == LIMIT_MEMORY){
           END_SIGN = 1;
       }
       if(END_SIGN == 1){
           if(k == 0){
               IsCompleteLine = 1;
               break;
           }
           else printf("   ");
       }
       else{
          if(k == 0) printf("%05x", Hex_Num*16);

          if(i == START_X  && k < START_Y){
            printf("   ");
          }
          else{
            printf(" %02X", NEW_BUFFER[i][k]);
            MEMORY_NUM++;
          }
        }
      }
    if(IsCompleteLine == 1) break;
    printf(" ; ");
    for(k=0; k < 16; k++){
        if(i == START_X && k < START_Y){
          printf(".");
        }
        else{
            if(END_SIGN != 1){
                if(NEW_BUFFER[i][k] >= 0x20 && NEW_BUFFER[i][k] <= 0x7E){
                printf("%C", NEW_BUFFER[i][k]);
                }
                else printf(".");
            }
            else printf(".");
        }
    }
   
    printf("\n");
    if(END_SIGN == 1) break;
    Hex_Num++;
    i++;
  
  }
      printf("\n");

      for(i=0; i < MEGA_BYTE_ROW; i++){
          free(NEW_BUFFER[i]);
      }
      free(NEW_BUFFER);

}

/*------------------------------------------------*/
/*
      Function : edit()
      Purpose : Function 'edit address value' command.
      No return value.
*/
/*------------------------------------------------*/

void edit(unsigned char *BUFFER, int *USER_ADDRESS, int *EDIT_VALUE){
    
    BUFFER[(unsigned int)(*USER_ADDRESS)] = (unsigned char)(*EDIT_VALUE);

}

/*------------------------------------------------*/
/*
      Function : fill()
      Purpose : Function 'fill' command.
      No return value.
*/
/*------------------------------------------------*/

void fill(unsigned char *BUFFER, int *START_ADDRESS, int *END_ADDRESS, int *FILL_VALUE){
    int i;

    for(i = *START_ADDRESS; i < *END_ADDRESS; i++){
        BUFFER[i] = (unsigned char)(*FILL_VALUE);
    }

}

/*------------------------------------------------*/
/*
    Function : add_hash()
    Purpose : Insert a node into hashtable of specific index.
    No return value.
*/
/*------------------------------------------------*/

void add_hash(int key, HASH* node){
    int index = key % HASHSIZE;
    HASH* cur = (HASH*)malloc(sizeof(HASH));

    if(hashtable[index] == NULL){
        hashtable[index] = node;
    }
    else{
        cur = hashtable[index];
        while(cur->next != NULL){
            cur = cur->next;
        }
        cur->next = node;
    }

}

/*------------------------------------------------*/
/*
    Function : create_hash()
    Purpose : Create hashtable by reading from the file.
    No return value.
*/
/*------------------------------------------------*/

void create_hash(){
    FILE *fp = fopen("opcode.txt", "r");
    char *code, *name;
    int MAX_SIZE = 100;
    srand(time(NULL));

    if(fp == NULL){
        printf("File Open Error!\n");
        return;
    }

    while(!feof(fp)){
        code = (char*)malloc(sizeof(char)*MAX_SIZE);
        name = (char*)malloc(sizeof(char)*MAX_SIZE);

        fscanf(fp, "%s%*[ \t]%s%*[ \t]%*s\n", code, name);
        HASH* node = (HASH*)malloc(sizeof(HASH));
        node->id = rand()%20;
        strcpy(node->code, code);
        strcpy(node->name, name);
        node->next = NULL;
        add_hash(node->id, node);
    }

    free(code);
    free(name);
    fclose(fp);
}

/*------------------------------------------------*/
/*    
      Function : opcodelist()
      Purposse : Function 'opcodelist' command.
      No return value.
*/
/*------------------------------------------------*/

void opcodelist(){

    for(int i = 0; i < HASHSIZE; i++){
        printf("%d : ", i);
        if(hashtable[i] != NULL){
            HASH* node = hashtable[i];
            while(node->next != NULL){
                printf("[%s, %s] -> ", node->name, node->code);
                node = node->next;
            }
            printf("[%s, %s]", node->name, node->code);
        }

        printf("\n");
    }

}

/*------------------------------------------------*/
/*
    Function : find_opcode()
    Purpose : Function 'opcode menomonic' command.
    Return value : IsCommanded 0 or 1
*/
/*------------------------------------------------*/


int find_opcode(char *INPUT_MN){

    FILE *fp = fopen("opcode.txt", "r");
    char *code, *name;
    int MAX_SIZE = 100;

    if(fp == NULL){
        printf("File Open Error!\n");
        return 0 ;
    }

    while(!feof(fp)){
        code = (char*)malloc(sizeof(char)*MAX_SIZE);
        name = (char*)malloc(sizeof(char)*MAX_SIZE);
        fscanf(fp, "%s%*[ \t]%s%*[ \t]%*s\n", code, name);
        if(strcmp(name, INPUT_MN) == 0){
            printf("opcode is %s.\n", code);
            free(code);
            free(name);
            fclose(fp);
            return 1;
        }

    }

    free(code);
    free(name);
    fclose(fp);
    return 0;

}

/* Program Start */

int main(){

	char *input = (char*)malloc(sizeof(char)*MAX_INPUT_SIZE);
    char *TEMP_IN = NULL, *TOK1 = NULL, *TOKEN=NULL, *ORG_IN=NULL, *TOK2 = NULL, *TOK3 = NULL, *ORG = NULL;
    char *AA, *BB, **ARR_FILL, *INPUT_MN, *temp, *O;
    int IsCommanded=0, IsFirst=0, IsHistory=0, IsOneNum=0;
    int LAST_ADDRESS = 0, LIMIT_ADDRESS=0, EDIT_ADDRESS = 0, EDIT_MEMORY = 0, FILL_START =0, FILL_END =0, FILL_MEMORY = 0;

    DIR *dp;
    struct dirent *file;
    struct stat buf;
    unsigned char* BUFFER = (unsigned char*)calloc(MEGA_BYTE, sizeof(unsigned char));
    dp = opendir(".");

	struct NODE *head = malloc(sizeof(struct NODE));
    head->link = NULL;
    NODE *curr;

    create_hash();

	while(1){

        if(IsCommanded == 1){
            if(IsFirst == 0){
              InsertNode(head, input);
              curr = head->link;
              IsFirst = 1;
              IsCommanded = 0;
            }
            else{
                if(IsHistory == 0){
                  InsertNode(curr,input);
                  curr = curr->link;
                  curr->link = NULL;
                }
                IsCommanded = 0;
            }
        }

        printf("sicsim> ");
        scanf("%[^\n]%*c", input);

        if(input[0] == 0){
            getchar();
            printf("You should input something. Please input again.\n");
            continue;
        }

        AA = (char*)malloc(sizeof(char)*MAX_ARR_SIZE);
        BB = (char*)malloc(sizeof(char)*MAX_ARR_SIZE);
        ARR_FILL = (char **)malloc(sizeof(char*)*MAX_ARR_SIZE);
        for(int i = 0; i < MAX_ARR_SIZE; i++){
           ARR_FILL[i] = (char*)malloc(sizeof(char) * strlen(input));
        }

        INPUT_MN = (char*)malloc(sizeof(char)*MAX_ARR_SIZE);

        ORG_IN = (char*)malloc(sizeof(char)*strlen(input));
        TEMP_IN = (char*)malloc(sizeof(char)*strlen(input));
        ORG = (char*)malloc(sizeof(char)*strlen(input));
        O = (char*)malloc(sizeof(char)*MAX_ARR_SIZE);
        temp = (char*)malloc(sizeof(char)*MAX_ARR_SIZE);
  
        strcpy(TEMP_IN, input);
        strcpy(ORG_IN, input);
        strcpy(ORG, input);
        strcpy(O, input);
        TOKEN = strtok(TEMP_IN, " \t");
      

		if(strcmp(TOKEN, "q") == 0 || strcmp(TOKEN, "quit") == 0){
            FreeNode(head);
            FreeHash();
            free(input);
            free(BUFFER);
            free(AA);
            free(BB);
            free(TEMP_IN);
            free(ORG_IN);
            free(INPUT_MN);
            free(O);
            free(ORG);
			break;
		}
		if(strcmp(TOKEN, "h") == 0 || strcmp(TOKEN, "help") == 0){
            IsCommanded = 1;
			help();
		}
        if(strcmp(TOKEN, "d") == 0 || strcmp(TOKEN, "dir") == 0){
            IsCommanded = 1;
            dir(dp,file,buf);
        }
        if(strcmp(TOKEN, "hi") == 0 || strcmp(TOKEN, "history") == 0){
            if(IsFirst == 0){
              InsertNode(head, input);
              curr = head->link;
              IsFirst = 1;
              IsCommanded = 1;
              IsHistory = 1;
              history(head->link);
            }
            else{
             InsertNode(curr, input);
             curr = curr->link;
             curr->link = NULL;
             history(head->link);
             IsHistory = 1;
             IsCommanded = 1;
            }
        }
  
        if(strcmp(TOKEN, "du") == 0 || strcmp(TOKEN, "dump") == 0){
            IsOneNum = 0;

            if(LAST_ADDRESS > 0xfffff) LAST_ADDRESS = 0;

            TOK3 = strtok(ORG, " \t");
            while(TOK3 != 0){
                strcpy(temp, TOK3);
                TOK3 = strtok(NULL, " \t");
            }

            if(strcmp(temp, "dump") == 0 || strcmp(temp, "du")==0 ){
                dump(BUFFER, &LAST_ADDRESS, &LIMIT_ADDRESS);
                LAST_ADDRESS++;
                IsCommanded = 1;
            } // If it is just "dump " command.
            else{
                TOK1 = strtok(ORG_IN, ",");

                while(TOK1 != 0){
                    strcpy(BB, TOK1);
                    TOK1 = strtok(NULL,",");
                }

                if((strstr(BB, "dump") != NULL) || (strstr(BB, "du") != NULL)) IsOneNum = 1;

                if(IsOneNum == 1){

                  TOK2 = strtok(BB, " \t");
                  while (TOK2 != 0){
                      strcpy(AA, TOK2);
                      TOK2 = strtok(NULL, " \t");
                  }
                  sscanf(AA, "%x", &LAST_ADDRESS);
                  if(LAST_ADDRESS > 0xfffff || LAST_ADDRESS < 0x00000){
                      printf("Invalid Input!\n");
                      continue;
                  }

                  dump(BUFFER, &LAST_ADDRESS, &LIMIT_ADDRESS);
                  LAST_ADDRESS++;
                  IsCommanded = 1;
                } // If it is ' dump AA ' command.
                else{

                    TOK2 = strtok(ORG_IN, " \t");
                    while(TOK2 != 0){
                        strcpy(AA, TOK2);
                        TOK2 = strtok(NULL, " \t");
                    }

                    sscanf(AA, "%x", &LAST_ADDRESS);
                    sscanf(BB, "%x", &LIMIT_ADDRESS);

                    if((LAST_ADDRESS > 0xfffff ) || (LAST_ADDRESS < 0x00000 ) || (LIMIT_ADDRESS > 0xfffff) || (LIMIT_ADDRESS < 0x00000) || (LAST_ADDRESS >= LIMIT_ADDRESS)){
                        //Range Error
                        printf("Invalid Input!\n");
                        continue;
                    }

                    dump(BUFFER, &LAST_ADDRESS, &LIMIT_ADDRESS);

                    LAST_ADDRESS++;
                    IsCommanded = 1;
                    LIMIT_ADDRESS = 0;


                } // If it is ' dump AA , BB ' command.

            }
        }

        if(strcmp(TOKEN, "edit") == 0 || strcmp(TOKEN, "e") == 0){


            if(strcmp(input, "edit") == 0 || strcmp(input, "e") == 0){
                IsCommanded = 0;
                printf("Invalid Input!\n");
                continue;
            }
            else{
                TOK1 = strtok(ORG_IN, ",");
                while(TOK1 != 0){
                    strcpy(BB, TOK1);
                    TOK1 = strtok(NULL, ",");
                }

                TOK2 = strtok(ORG_IN, " \t");
                while(TOK2 != 0){
                    strcpy(AA, TOK2);
                    TOK2 = strtok(NULL, " \t");
                }

                sscanf(AA, "%x", &EDIT_ADDRESS);
                sscanf(BB, "%x", &EDIT_MEMORY);
                if((EDIT_ADDRESS > 0xfffff) || (EDIT_ADDRESS < 0x00000) ||  (EDIT_MEMORY < 0x00) || (EDIT_MEMORY > 0xff)){
                    printf("Invalid Input!\n");
                    continue;
                }
                edit(BUFFER, &EDIT_ADDRESS, &EDIT_MEMORY);
                IsCommanded = 1;
            }
        }

        if(strcmp(TOKEN, "f") == 0 || strcmp(TOKEN, "fill") == 0){
            int j = 0;

            TOK1 = strtok(ORG_IN, ",");
            while(TOK1 != NULL){
                strcpy(ARR_FILL[j], TOK1);
                TOK1 = strtok(NULL, ",");
                j++;
            }

            TOK2 = strtok(ARR_FILL[0], " \t");
            while(TOK2 != 0){
                strcpy(AA, TOK2);
                TOK2 = strtok(NULL, " \t");
            }

            sscanf(AA, "%x", &FILL_START);
            sscanf(ARR_FILL[1], "%x", &FILL_END);
            sscanf(ARR_FILL[2], "%x", &FILL_MEMORY);

            if((FILL_START >= FILL_END) || (FILL_START > 0xfffff) || (FILL_START < 0x00000) || (FILL_END > 0xfffff) || (FILL_END < 0x00000) || (FILL_MEMORY < 0x00) || (FILL_MEMORY > 0xff)){
                printf("Invalid Input!\n");
                continue;
            }

            fill(BUFFER, &FILL_START, &FILL_END, &FILL_MEMORY);
            IsCommanded = 1;

        }

        if(strcmp(TOKEN, "reset") == 0){
            
            BUFFER = (unsigned char*)calloc(MEGA_BYTE, sizeof(unsigned char));
            IsCommanded = 1;
        }

        if(strcmp(TOKEN, "opcodelist") == 0){
            opcodelist();
            IsCommanded = 1;

        }
        
        if(strcmp(TOKEN, "opcode") == 0){

            TOK1 = strtok(ORG_IN, " \t");
            while(TOK1 != 0){
                strcpy(INPUT_MN, TOK1);
                TOK1 = strtok(NULL, " \t");
            }

            IsCommanded = find_opcode(INPUT_MN);

        }

        if(IsCommanded == 0){
            printf("There is no command or invalid input. Please input agian.\n");
            continue;
        }
  
    }

	return 0;

}
