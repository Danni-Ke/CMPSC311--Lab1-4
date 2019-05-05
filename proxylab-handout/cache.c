#include "cache.h"
#include "csapp.h"
//In proxy
//check if same request before parse
//insert cache block after finshing getting web content


//In cache.c
//Search cacheNode, if hit, put it on the front of list
//Insert_node, before insert check if oversize
//also check if full, if full remove_node
//Remove_node, remove the last node(LRU)

//initilize cacheNode at the beginning
void init_cacheNode(cNode *c_node, char *uri, char *data, size_t size){
  //printf("inside the init_cacheNode\n");
     c_node->content_size = size;
     c_node->prev = NULL;
     c_node->next = NULL;
     if(c_node->content_size <= MAX_OBJECT_SIZE){
	strcpy(c_node->uri_index, uri);
        c_node->web_content= Malloc(c_node->content_size);
        memcpy(c_node->web_content,data,c_node->content_size);
     }
     else{
        free_cacheNode(c_node);
     }
}
//initilize cache
void init_cache(pCache *cache){
   // printf("inside the init_cache\n");
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;
}

//free cache Node if needed
void free_cacheNode(cNode *c_node){
  //printf("inside the free_cacheNode\n");
   if(c_node->web_content != NULL)
        Free(c_node->web_content);
   Free(c_node);
}
//free cache after the program is finished
void free_cache(pCache *cache){
    cNode *temp = cache->head;
    while(temp!=NULL)
    { 
      free_cacheNode(temp);
      temp=temp->next;
    }

    Free(cache);

}
//return 0 if insert sucessfully
int insert_cacheNode(pCache *cache,cNode *c_node){
     //printf("inside the insert_cacheNode\n");
     if(c_node == NULL)
        return 1;
     //check if it's oversized. if it's oversize, discard
     if(c_node->content_size > MAX_OBJECT_SIZE){
	free_cacheNode(c_node);
	printf("Oversized content, is unable to be cached");
        return 1;
     } 
     //if the cache is not empty
     if(cache->head!=NULL){
        //if all cache block plus the new node is not oversized
        while((cache->size+c_node->content_size)> MAX_CACHE_SIZE){ 
             remove_tail(cache);}
         c_node->next = cache->head;
         (cache->head)->prev = c_node;
         cache->head = c_node;
     }
     else{//if the cache is empty, the size will never be over;
         cache->head = c_node;
         cache->tail = c_node;
     }
     //update the size for the cache if add new node
     cache->size = cache->size + c_node->content_size;
     return 0;
}
//helper function to debug the code
void print(pCache *cache){
   cNode *temp = cache->head;
   while(temp != NULL ){
     printf("Uri:%s\n",temp->uri_index);
     printf("Web:%s\n",temp->web_content);
     temp = temp->next;
   }
}
//remove the last node in function
void remove_tail(pCache *cache){
  // printf("inside the remove_tail\n");
   //there is not case that there is only one node
   cNode *temp = NULL;
   temp = cache->tail;
   cache->tail = temp -> prev;
   (cache->tail)->next = NULL;
   cache->size = cache->size-temp->content_size;
   free_cacheNode(temp);
}
//search the node by uri
cNode *search_uri(pCache *cache, char uri[MAXLINE]){
  //printf("inside the search_uri\n");
    cNode *object = Malloc(sizeof(cNode));
    object = cache->head;
    if(cache->head == NULL)
       return NULL;
    while(object != NULL){
      //check if the uri is the same, if it's the same return the node
      if( strcmp(uri,object->uri_index) == 0){
           if(object->prev != NULL){//not head
                if(object->next == NULL){//if last node
		  //In LRC policy, move it from the last one to the head
                     cache->tail=object->prev;
                     object->prev->next=NULL;
                 }
                else{//middle
                  //In LRC policy, move it to the head
                     object->prev->next=object->next;
                     object->next->prev= object->prev;
                 }
                object->next = cache->head;
                object->prev = NULL;
                cache->head->prev = object;
                cache->head = object;
            }
	   // printf(object->web_content);
         return object;
      }
     object=object->next;
    }
  //no free_cacheNode(object), since it's freed in the free cache
   return NULL;
 }      


