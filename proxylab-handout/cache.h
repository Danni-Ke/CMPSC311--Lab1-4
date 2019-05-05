#ifndef __CACHE_H__
#define __CACHE_H__
/*
 *cache.h by Danni Ke
 *The basic idea for the cache is using doublely linked list to manage 
 *the cache block, also the LRU policy is implementd by removing tail in 
 *this part.
 */

#include "csapp.h"


#define MAX_CACHE_SIZE 16777216
#define MAX_OBJECT_SIZE 8388608

typedef struct cacheNode{ 
   char uri_index[MAXLINE];
   char *web_content;
   struct cacheNode *next;
   struct cacheNode *prev;
   int content_size;
}cNode;

typedef struct proxyCache{
    cNode *head;
    cNode *tail;
    int size;   
}pCache;

void print(pCache *cache);
void init_cacheNode(cNode *c_node, char *uri, char *data, size_t size);
void init_cache(pCache *cache);
void free_cacheNode(cNode *c_node);
void free_cache(pCache *cache);
int insert_cacheNode(pCache *cache,cNode *c_node);
void remove_tail(pCache *cache);
cNode *search_uri(pCache *cache, char *uri);

#endif
