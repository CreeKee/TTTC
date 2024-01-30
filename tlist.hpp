#include "includes.h"

#ifndef TILE_LIST
#define TILE_LIST

class tlist{
    public:
    tnode* head;
    uint32_t count;

    tlist(){head = nullptr; count = 0;}

    //void insert(tnode* newnode){newnode->next = head; newnode->prev = head->prev; head->prev = newnode; count++;}
    void insert(int x, int y){

        tnode* newnode = new tnode;

        newnode->crds.x = x;
        newnode->crds.y = y;

        if(head){
            newnode->next = head; 
            newnode->prev = head->prev; 
            head->prev->next = newnode;
            head->prev = newnode;
        }
        else{
            newnode->next = newnode; 
            newnode->prev = newnode; 
            head = newnode;
        }  
        count++;
    }
    coord pop(){
        coord ret;
        tnode* oldhead = head;

        tnode* cur = head;

        do{
            cur = cur->next;
        }while(cur != head);

        if(head){
            ret.x = head->crds.x;
            ret.y = head->crds.y;
            
            if(head == head->next){
                head = nullptr;
            }
            else{
                head->prev->next = head->next;
                head->next->prev = head->prev;
                head = head->next;
            }

            delete oldhead;
        }
        else{
            ret.x = 0;
            ret.y = 0;
        }
        count--;
        return ret;
    }
    tlist copy(){
        tlist newlist;
        tnode* cur = head;

        if(cur){
            do{
                newlist.insert(cur->crds.x, cur->crds.y);
                cur = cur->next;
            }while(cur != head);
        }

        return newlist;

    }

    bool isEmpty(){return head == nullptr;}
};


#endif
