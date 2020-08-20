#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#include<unistd.h>

#include <errno.h>

int is_space(char);
int isDelimiter(char);
int stringComparator(void *a, void *p);
int intComparator(void *a, void *b);
int insertionSort( void* toSort, int (*comparator)(void*, void*));
int quickSort( void* toSort, int (*comparator)(void*, void*) );
void quickSortHelper(void**, int, int, int (*comparator)(void*, void*));
int split(void**, int, int, int (*comparator)(void*, void*));


typedef struct node{
  void* data;
  struct node * next;
  int length;
  int isEmpty;
}Node;

int is_integer = 0;

int main(int argc, char** argv){

  char * mode = argv[1];

  if(argc != 3){
    perror("Error in the amount of command line arguments, please only input a mode specification flag and a filename\nResponse: FATAL ERROR\n");
    exit(-1);
  }

  if(mode[0] != '-' || (mode[1] != 'i' && mode[1] != 'q')){
    perror("Error with your sorting mode flag, either use -q or -i\nResponse: FATAL ERROR\n");
    exit(-1);
  }

  int fd = open(argv[2], O_RDONLY);

  if(fd < 0){
    int errsv = errno;
    perror("Error opening the file.\n Response: FATAL ERROR\n");
    exit(-1);
  }

  char* buff = malloc(sizeof(char) * 10000);
  if(buff == NULL){
    perror("Error using Malloc() for buff. \nResponse: FATAL ERROR\n");
    exit(-1);
  }
  int buff_idx = 0;
  int buff_size = 10000;
  int tkns_read = 0;

  char read_char;


  Node* head = malloc(sizeof(Node));
  if(head == NULL){
    perror("Error allocating memory to create the linked list\nResponse: FATAL ERROR\n");
    exit(-1);
  }
  Node* pointer = head;

  //loop over every character in the file
  int num_bytes;
  int i;
  int empty_tkn = 1;
  int er = 1;

  while(1){
      num_bytes = read(fd, &read_char, 1);
      if(num_bytes < 0){
        perror("Error reading a character from the file\nResponse: Will treat the missing character as a delimiter, since we do not know if it was one or not\n");
        er = 0;
      }

      if(read_char == '\0' && tkns_read == 0){
        printf("WARNING: The file you entered is empty.\n");
        break;
      }

      if(isDelimiter(read_char) || num_bytes == 0 || er == 0){
        if(read_char == '\0' && empty_tkn == 1){
          break;
        }

        er = 1;

        int tmp = 0;
          char* temp = malloc(sizeof(char) * buff_size);
          if(temp == NULL){
            perror("Error using Malloc() to create a temporary buffer.\nResponse: Will sacrifice the token and move to the next one.\n");
            continue;
          }


          if(empty_tkn == 1){
            for(i = 0; i < buff_idx; i++){
              temp[tmp] = buff[i];
              tmp++;
            }
          }else{
            for (i = 0; i < buff_idx; i++){
              if (!isspace(buff[i])) {
                temp[tmp] = buff[i];
                tmp++;
              }
            }
          }

          temp[tmp] = '\0';


          pointer->data = (void*)temp;
          pointer->length = tmp;
          pointer->isEmpty = empty_tkn;

          empty_tkn = 1;

          tkns_read++;

          buff_idx = 0;
          if(num_bytes == 0){
            break;
          }

          Node* n = malloc(sizeof(Node));
          if(n == NULL){
            perror("Error using Malloc() to expand the linked list.\nResponse: Broke the while loop before completion, not every token was read.\n");
            break;
          }

          pointer->next = n;

          pointer = n;
          

      }else{

        buff[buff_idx] = read_char;
        buff_idx++;

        if(isspace(read_char) == 1){
          empty_tkn = 0;
        }

        if(buff_idx == buff_size){
          buff_size *= 2;
          char *newbuf = realloc(buff, buff_size);
          if (!newbuf) {
            perror("Error using Realloc() for *newbuf. This was done because a token was too large for the original buffer size.\nResponse: Cut the token short to fit in the original buffer.\n");
            buff[buff_idx - 1] = '\0';
            er = 0;
            continue;
          }
          buff = newbuf;
        }

        if(isalpha(read_char)){
          is_integer = 1;
        }
        
        if(is_space(read_char) == 0){
          empty_tkn = 0;
        }
      }

  }

  free(buff);

  if(head == NULL){
    return 0;
  }

  Node* t = head;
  while(t->next != NULL){
    if(t->next->isEmpty == 1 && t->next->next == NULL ){
      Node* tmp = t->next;
      free(tmp);
      t->next = NULL;
      break;
    }
    t = t->next;
  }


  int x;
  if(mode[1] == 'q'){
    if(is_integer == 0){
      x = quickSort((void*)head, intComparator);
    }else{
      x = quickSort((void*)head, stringComparator);
    }
  }else{
    if(is_integer == 0){
      x = insertionSort((void*)head, intComparator);
    }else{
      x = insertionSort((void*)head, stringComparator);
    }
  }

  t = head;
  while(t != NULL){
    printf("%s\n", (char*)t->data);
    t = t->next;
  }

  

  t = head;
  while(t != NULL){
    Node* t2 = t;
    t = t->next;
    free(t2);
  }


  close(fd);

  return 0;
}


int is_space(char c){
  if(c == '\n' || c == ' ' || c == '\t' || c == '\f' || c == '\r' || c == 'v'){
    return 1;
  }else{
    return 0;
  }
}

int isDelimiter(char c){
  if(c == ',' || c == '\0' || c == EOF){
    return 1;
  }else{
    return 0;
  }
}


int insertionSort( void* toSort, int (*comparator)(void*, void*)){

  Node* head = (Node*)toSort;
  Node* current= head;
  Node* ptr=NULL;
  void* temp=NULL;
  while(current!=NULL){
      ptr=head;
      if(((*comparator)(current->data,ptr->data))<0){
          temp=current->data;
          current->data=ptr->data;
          ptr->data=temp;
      }
      while(ptr!=NULL){
          if(((*comparator)(current->data,ptr->data))<0){
              temp=current->data;
              current->data=ptr->data;
              ptr->data=temp;
          }
          ptr=ptr->next;
      }
      current=current->next;
  }
  return 0;
}

int quickSort( void* toSort, int (*comparator)(void*, void*) ){
  Node* head = (Node*)toSort;

  Node *current = head;
  int i = 0;
  while(current != NULL){
      i++;
      current = current->next;
  }

  void* arr[i];
  current = head;
  int j = 0;
  while (current != NULL){
      arr[j] = current->data;
      current = current->next;
      j++;
  }

  quickSortHelper(arr, 0, i - 1, (*comparator));

  current = head;
  j = 0;
  while(current != NULL){
    current->data = arr[j];
    current = current->next;
    j++;
  }

  toSort = (void*)head;

  return 0;
 
}

void quickSortHelper(void* arr[], int low, int high, int (*comparator)(void*, void*)){  
  if (low < high){  
    int pi = split(arr, low, high, (*comparator));  
    quickSortHelper(arr, low, pi - 1, (*comparator));  
    quickSortHelper(arr, pi + 1, high, (*comparator));  
  }  
}  

int split (void* arr[], int low, int high, int (*comparator)(void*, void*)){  
    void* pivot = arr[high];  
    int i = (low - 1);  
    int j;
    for (j = low; j <= high - 1; j++){  
 
      if ((*comparator)(arr[j], pivot) < 0){  
        i++; 
        void* temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
      }  
    } 

    void* temp = arr[i+1];
    arr[i+1] = arr[high];
    arr[high] = temp; 
    return (i + 1);  
}  


//reutns -1 if a comes first, 1 if b comes first, 0 if they are the same
int stringComparator(void *a, void *b){
    char *a1 = (char*)a;
    char *b1 = (char*)b;
    int i=0;
    if ((a1[0]=='\0')&&(b1[0]=='\0')){
        return 0;
    }
    if (a1[0]=='\0'){
        return -1;
    }
    if (b1[0]=='\0'){
        return 1;
    }
    while(a1[i] != '\0' && b1[i] != '\0'){
        if (a1[i]<b1[i]){
            return -1;
        }
        if (a1[i]>b1[i]){
            return 1;
        }
        i++;
    }
    return 0;
}

//returns a negative if a comes first, returns a positive if b comes first
int intComparator(void *a, void *b){
    int a2 = atoi(a);
    int b2 = atoi(b);
    return a2-b2;
}