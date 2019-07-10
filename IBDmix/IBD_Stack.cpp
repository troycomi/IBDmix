#include <iostream>
#include <stdlib.h>
#include "IBD_Stack.hpp"

struct IBD_Node* pool = nullptr;
int buff_size = 10;

void allocate(int buffer_size){
    // assumes only called when pool is nullptr
    struct IBD_Node* temp = (struct IBD_Node*) malloc(
            sizeof(struct IBD_Node) * buffer_size);
    pool = temp;
    for (int i = 0; i < buffer_size; i++){
        temp[i].next = &temp[i+1];
    }
    temp[buffer_size-1].next = nullptr;
}

struct IBD_Node* get_node(unsigned long int position, double lod){
    if (pool == nullptr)
        allocate(buff_size);

    struct IBD_Node* result = pool;
    result->position = position;
    result->lod = lod;
    result->cumulative_lod = lod;
    pool = pool->next;
    return result;
}

void push(struct IBD_Node *&top, struct IBD_Node *new_node){
    new_node->next = top;
    top = new_node;
}

struct IBD_Node * pop(struct IBD_Node *& top){
    struct IBD_Node *result = top;
    top = top-> next;
    return result;
}

void reclaim_node(struct IBD_Node *node){
    push(pool, node);
}

void reclaim_after(struct IBD_Node *start){
    reclaim_between(start, nullptr);
}

void reclaim_between(struct IBD_Node *start,
        struct IBD_Node *end){
    if(start == nullptr)
        return;
    // move all nodes after start into pool
    struct IBD_Node* ptr = start;
    //find last node
    for (; ptr->next != end; ptr = ptr->next)
        ;
    ptr->next = pool;
    pool = start->next;
    start->next = end;
}

void reclaim_all(struct IBD_Node *&top){
    if(top == nullptr)
        return;
    reclaim_after(top);
    top->next = pool;
    pool = top;
    top = nullptr;
}

struct IBD_Node* reverse(IBD_Node* top){
    // reverse the order of the list, returning the new top
    // assumes nothing before top
    // performed in place!
    IBD_Node *result = nullptr;
    while(top != nullptr)
        push(result, pop(top));

    return result;
}

int stack_length(struct IBD_Node * top){
    int count = 0;
    for(struct IBD_Node* ptr = top; ptr != nullptr; ptr = ptr->next)
        count++;
    return count;
}

void display_stack(struct IBD_Node* top){
    for(struct IBD_Node* ptr = top; ptr != nullptr; ptr = ptr->next)
        std::cout << ptr-> position << " ";
    std::cout << '\n';
}

int pool_length(void){
    return stack_length(pool);
}

void display_pool(void){
    std::cout << "pool: ";
    display_stack(pool);
}
