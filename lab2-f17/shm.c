#include "param.h"
#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

struct {
  struct spinlock lock;
  struct shm_page {
    uint id;
    char *frame;
    int refcnt;
  } shm_pages[64];
} shm_table;

void shminit() {
  int i;
  initlock(&(shm_table.lock), "SHM lock");
  acquire(&(shm_table.lock));
  for (i = 0; i< 64; i++) {
    shm_table.shm_pages[i].id =0;
    shm_table.shm_pages[i].frame =0;
    shm_table.shm_pages[i].refcnt =0;
  }
  release(&(shm_table.lock));
}

int shm_open(int id, char **pointer) {
//you write this
  acquire(&(shm_table.lock));
  int i;
  int flag = 0;
  for(i=0; i<64; i++){
    // id found
    if(shm_table.shm_pages[i].id == id){
      flag = 1;
      uint size = PGROUNDUP(myproc()->sz);
      mappages(myproc()->pgdir, (char*)size, PGSIZE, V2P(shm_table.shm_pages[i].frame), PTE_W|PTE_U);
      shm_table.shm_pages[i].refcnt++; 
      *pointer = (char*)size;
      myproc()->sz = size;
    }
  }
  release(&(shm_table.lock));
  if(flag)
    return 0;
  // id not found
  acquire(&(shm_table.lock));
  for(i=0; i<64; i++){
    if(shm_table.shm_pages[i].id == 0){
      shm_table.shm_pages[i].id = id;
      shm_table.shm_pages[i].frame = kalloc();
      shm_table.shm_pages[i].refcnt++;
      memset(shm_table.shm_pages[i].frame, 0, PGSIZE);
      uint size = PGROUNDUP(myproc()->sz);
      mappages(myproc()->pgdir, (char*)size, PGSIZE, V2P(shm_table.shm_pages[i].frame), PTE_W|PTE_U);
      *pointer = (char*)size;
      myproc()->sz = size;
      break;
    }
  }
release(&(shm_table.lock));
return 0; //added to remove compiler warning -- you should decide what to return
}


int shm_close(int id) {
//you write this too!
  int i;
  acquire(&(shm_table.lock));
  for(i=0; i<64; i++){
    if(shm_table.shm_pages[i].id == id){
      shm_table.shm_pages[i].refcnt--;
      if(shm_table.shm_pages[i].refcnt ==0){
        shm_table.shm_pages[i].id = 0;
        shm_table.shm_pages[i].frame = 0;
        shm_table.shm_pages[i].refcnt = 0;
      }
      break;
    }
  }
  release(&(shm_table.lock));
  return 0; //added to remove compiler warning -- you should decide what to return
}
