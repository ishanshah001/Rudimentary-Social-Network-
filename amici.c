/// @file amici.c
///
/// @author Ishan Shah is4761
///

#include <stdio.h>
#include <string.h>
#include <limits.h> //CHAR_MAX
#include <stdlib.h>

#include "amici.h"  //function declarations, person_t declaration
#include "hash.h"    // hash functions for different key types
#include "table.h"   // ht_create, ht_destroy, and other interface functions

#define BUFFER_SIZE 1024 // maximum length of any single input command line 
#define MAX_FRIENDS 1000 // maximum number of friends

//global declaration
int people=0; // number of prople in the network
int unique_friendship=0; // number of unique friendships in the network
Table table;


void l_ptr_str_print(const void* key, const void* value){

    char *k = (char*)key;
    person_t *v = (person_t*)value;
    printf("%s : %s", k, v -> first_name);

}


void delete_l_ptr_str(void* key, void* value){
    free(key);
    
    person_t *person = (person_t*)value;
    free(person->first_name);
    free(person->last_name);
    free(person->friends);
    free(value);
}


void stats(){
  
  printf("Statistics:  ");
  if(people==1){
    printf("%d person, ",people);
  }
  else{
    printf("%d people, ",people);
  }
  if(unique_friendship==1){
    printf("%d friendship",unique_friendship);
  }
  else{
    printf("%d friendships",unique_friendship);
  }
  printf("\n");

}



int add(char *fname, char *lname, char *handle, Table table){

  if(ht_has(table,(const void*)handle)){
    fprintf(stderr,"error: handle '%s' is already taken. Try another handle.\n",handle);
    return 1;
  }
  //initialise dynamically
  person_t *new = malloc(sizeof(person_t));
  (new->first_name) = malloc(strlen(fname)+1);
  (new->last_name) = malloc(strlen(lname)+1);
  (new->handle) = malloc(strlen(handle)+1);
  new->friends= malloc(sizeof(person_t)*MAX_FRIENDS);
  new -> friend_count = 0;
  new -> max_friends = MAX_FRIENDS;
  strncpy(new -> first_name, fname, strlen(fname) + 1);
  strncpy(new -> last_name, lname,  strlen(lname) + 1);
  strncpy(new -> handle, handle, strlen(handle)+1);
  
  ht_put(table,(const void*)(new -> handle), (const void*)new);
 
  people+=1;
  return 0;
}


bool check_friend(person_t *first, person_t *second){
  
  for(int i=0; i<((int)first->friend_count);i++){
      if(!strncmp(first->friends[i]->handle,second->handle,strlen(first->friends[i]->handle))){
        return true;
      }
    }
    return false;
  
}

void init_table(void){
  if(people>0){
    ht_destroy(table);
  }
  table = ht_create(str_hash, str_equals, l_ptr_str_print, delete_l_ptr_str );
  
  people=0;
  unique_friendship=0;
}


int size(char *handle, Table table){

	
  if(ht_has(table,(const void*)handle)){
  
    person_t *person = (person_t*)ht_get(table,(const void*)handle);
    printf("User %s %s ('%s') ",person->first_name,person->last_name,handle);
    
    if(person->friend_count>1){
      printf("has %ld friends\n",person->friend_count);
    }
    else if(person->friend_count==1){
      printf("has 1 friend\n");
    }
    else{
    printf("has no friends\n");
    }
  }
  else{
    fprintf(stderr,"Given handle does not exist\n");
  }  
  return 0;
 }

int make_friend(char *handle1, char *handle2, Table table){

   if(!strncmp(handle1,handle2,strlen(handle1))){
     fprintf(stderr,"error: User cannot be their own 'friend'\n");
     return 1;
   }
  
  if(ht_has(table,(const void*)handle1) && ht_has(table,(const void*)handle2)){
  
    person_t *first = (person_t*)ht_get(table,(const void*)handle1);
    
    person_t *second = (person_t*)ht_get(table,(const void*)handle2);
    
    if(check_friend(first,second)){
      fprintf(stderr,"error: '%s' and '%s' are already friends.\n", first->handle,second->handle);
      return 1;
    }
    
    first->friends[first->friend_count]=second;
    first->friend_count+=1;
    
    second->friends[second->friend_count]=first;
    second->friend_count+=1;
  }
  else if(!ht_has(table,(const void*)handle1)){
    fprintf(stderr,"error: '%s' is not a known handle\n",handle1);
    return 1;
  }
  else{
    fprintf(stderr,"error: '%s' is not a known handle\n",handle2);
    return 1;
  }
  printf("%s and %s are now friends\n",handle1,handle2);
  unique_friendship+=1;
  return 0;
}


int unfriend(char *handle1, char *handle2, Table table){
  
  if(!strncmp(handle1,handle2,strlen(handle1))){
     fprintf(stderr,"error: User cannot be their own 'friend'\n");
     return 1;
   }
  
  //check if friends
  if(ht_has(table,(const void*)handle1) && ht_has(table,(const void*)handle2)){
    
    person_t *first = (person_t*)ht_get(table,(const void*)handle1);
    
    person_t *second = (person_t*)ht_get(table,(const void*)handle2);
    
    if(!check_friend(first,second)){
      fprintf(stderr,"error: Not friends\n");
      return 1;
    }
    
    int index=0;
    //find the index
    for(int i=0; i<((int)first->friend_count);i++){
      if(!strncmp(first->friends[i]->handle,second->handle,strlen(first->friends[i]->handle))){
        break;
      }
      index++;
    }
    //makes changes to the linking
    for(int j=index; j<((int)first->friend_count)-1;j++){
      first->friends[j]=first->friends[j+1];
    }
    
    first->friend_count-=1;
    index=0;
    for(int i=0; i<((int)second->friend_count);i++){
      if(!strncmp(second->friends[i]->handle,first->handle,strlen(second->friends[i]->handle))){
        break;
      }
      index++;
    }
    for(int j=index; j<((int)second->friend_count)-1;j++){
      second->friends[j]=second->friends[j+1];
    }
  
    second->friend_count-=1;
  }
  else if(!ht_has(table,(const void*)handle1)){
    fprintf(stderr,"error: '%s' is not a known handle\n",handle1);
    return 1;
  }
  else{
    fprintf(stderr,"error: '%s' is not a known handle\n",handle2);
    return 1;
  }
 
  unique_friendship-=1;
  return 0;
}

int print_person(char *handle, Table table){
	
  if(ht_has(table,(const void*)handle)){
    
    person_t *person = (person_t*)ht_get(table,(const void*)handle);
    printf("User %s %s ('%s') ",person->first_name,person->last_name,handle);
    
    if(person->friend_count>0){
      if(person->friend_count>1){
        printf("has %ld friends\n",person->friend_count);
      }
      else{
        printf("has 1 friend\n");
      }
      
      for(int i=0;i<(int)person->friend_count;i++){
        printf("\t%s %s ('%s')\n",person->friends[i]->first_name,person->friends[i]->last_name,person->friends[i]->handle);
      }
    }
    
    else{
    printf("has no friends\n");
    }
    
  }
  else{
    
    fprintf(stderr,"error: '%s' is not a known handle\n",handle);
    
  }
  return 0;
}

// When it begins executing, the network is empty, with no people in it. 
// The program prompts the user for a command
// when the user enters a command, the program validates it, and then performs it. 
// It continues this process until either it runs out of input data, or a quit command is entered by the user.
int main(){
  
  char input[CHAR_MAX];
  
  char *token;
  char *delim=" \n";
  
  init_table();
  
  do{
    printf("amici> ");
    
    if(fgets(input, BUFFER_SIZE, stdin)==NULL){
      return -1;
    }
    
    char *cmd[BUFFER_SIZE] = {NULL};
    int args=0;
    token=strtok(input,delim);
    while(token!=NULL){
      cmd[args++]=token;
      token = strtok(NULL,delim);
    }
    
    if(!(strncmp(cmd[0],"add",strlen(cmd[0]))) && args==4){
      add(cmd[1],cmd[2],cmd[3],table);
    }
    else if(!(strncmp(cmd[0],"quit",strlen(cmd[0]))) && args==1){
      ht_destroy(table);
      break;
    }
    else if(!(strncmp(cmd[0],"friend",strlen(cmd[0]))) && args==3){
      make_friend(cmd[1],cmd[2],table);
    }
    else if(!(strncmp(cmd[0],"print",strlen(cmd[0]))) && args==2){
      print_person(cmd[1],table);
    }
    else if(!(strncmp(cmd[0],"stats",strlen(cmd[0]))) && args==1){
      stats();
    }
    else if(!(strncmp(cmd[0],"init",strlen(cmd[0]))) && args==1){
      init_table();
    }
    else if(!(strncmp(cmd[0],"size",strlen(cmd[0]))) && args==2){
      size(cmd[1],table);
    }
    else if(!(strncmp(cmd[0],"unfriend",strlen(cmd[0]))) && args==3){
      unfriend(cmd[1],cmd[2],table);
    }
    else{
      continue;
    }
  }while(1);  
  return 0;
}
