#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>


void right_rotate(rbtree *t, node_t *node){//left 로테이트에서 방향들만 수정
  node_t *y=node->left;
  node->left=y->right; 
  if(y->right != t->nil) 
  y->right->parent = node; 
  y->parent= node->parent;
  if(node->parent == t->nil) t->root =y;
  else if (node == node->parent->left) {
    node->parent->left = y;         
  }
  else node->parent->right = y;         
  

  y->right = node;                    
  node->parent = y;  
}
void left_rotate(rbtree *t, node_t *node){
  node_t *y=node->right;
  node->right=y->left; //서브트리부터 옮기고
  if(y->left != t->nil)
  y->left->parent = node; //서브트리가 NIL이 아니면 부모도 노드로 바꿔줌(NIL이면 부모가 없기 때문)
  y->parent= node->parent;//부모 바꿔주고
  if(node->parent == t->nil) t->root =y;
  else if (node == node->parent->left) {
    node->parent->left = y;           // x가 왼쪽 자식이면 y를 왼쪽에
  }
  else node->parent->right = y;          // x가 오른쪽 자식이면 y를 오른쪽에
  

  y->left = node;                     // x를 y의 왼쪽 자식으로
  node->parent = y;  
}
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *temp = t->root;
  while(temp != t->nil){//같을때까지 루프
    if(temp->key == key)return temp;//테스트케이스 통과하려면 NULL이어야댐.
    else if(temp->key <key)temp=temp->right;//현재 포인터 키가 찾으려는 키보다 작으면 포인터를 오른쪽으로
    else temp=temp->left;//아님 왼쪽

    
  }
  return NULL;
}
void rbtree_insert_fix(rbtree *t, node_t *node){
  //부모가 black일때는 문제 없으므로
  while (node->parent->color==RBTREE_RED){//부모가 레드면 반복적으로 픽스
    if(node->parent == node->parent->parent->left){//현재 노드의 부모가 조부모의 왼쪽 노드일때
      node_t *u = node->parent->parent->right; //삼촌노드 정의

      if (u->color==RBTREE_RED){//현재 부모 red 삼촌 red인 경우 == case1
        node->parent->parent->color = RBTREE_RED; //조부모를 red으로 바꾸고
        node->parent->color = RBTREE_BLACK; //부모는 black
        u->color = RBTREE_BLACK; //삼촌도 BLACK
        node=node->parent->parent;
      }
      else{  //삼촌이 red가 아니고
        if(node == node->parent->right){//꺾인 상황, 즉 체크중인 노드가 부모의 오른쪽일때.  == case 2, case2는 3으로 바꿔서 진행
          node=node->parent;//rotate 전에 기준으로 할 부모로 포인터 이동.(수정하면 case3로 바뀌는데 포인터를 옮겨놔야 해당 노드 기준으로case3 검사하기 때문)
          left_rotate(t,node);
        }
        //==case3
        node->parent->color=RBTREE_BLACK;//부모 색 black으로
        node->parent->parent->color=RBTREE_RED;//조부모는 red로
        right_rotate(t,node->parent->parent);//조부모 기준으로 right rotate
      }
    }
    else{//부모가 조부모의 오른쪽 노드일때. --> 바로 위와 방향만 반대로
      node_t *u = node->parent->parent->left; //삼촌노드 정의

      if (u->color==RBTREE_RED){//현재 부모 red 삼촌 red인 경우 == case1
        node->parent->parent->color = RBTREE_RED; //조부모를 red으로 바꾸고
        node->parent->color = RBTREE_BLACK; //부모는 black
        u->color = RBTREE_BLACK; //삼촌도 BLACK
        node=node->parent->parent;
      }
      else{  //삼촌이 red가 아니고
        if(node == node->parent->left){//꺾인 상황, 즉 체크중인 노드가 부모의 왼쪽일때.  == case 2, case2는 3으로 바꿔서 진행
          node=node->parent;//rotate 전에 기준으로 할 부모로 포인터 이동.(수정하면 case3로 바뀌는데 포인터를 옮겨놔야 해당 노드 기준으로case3 검사하기 때문)
          right_rotate(t,node);
        }
        //==case3
        node->parent->color=RBTREE_BLACK;//부모 색 black으로
        node->parent->parent->color=RBTREE_RED;//조부모는 red로
        left_rotate(t,node->parent->parent);//조부모 기준으로 right rotate
      }
    }
  
  }
  t->root->color = RBTREE_BLACK; //픽스 후에 루트 색깔은 BLack으로 무조건 바꿔주면 특성1 위배 x,while탈출하는 경우는 root까지 가서 root의 부모 NIL을 만나 BLack임을 확인한 경우
}

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *NIL = (node_t *)malloc(sizeof(node_t));//NIL 정의 시작
  NIL->color = RBTREE_BLACK;
  NIL->left=NIL;
  NIL->parent=NIL;
  NIL->right=NIL;

  p->root = NIL;
  p->nil = NIL;
  return p;
}


void delete_recursive_rbtree(rbtree *t,node_t *node){
  if (node == t->nil) return;
	
  delete_recursive_rbtree(t,node->left);
  delete_recursive_rbtree(t,node->right);
  free(node);
	
}
void delete_rbtree(rbtree *t) {
  node_t *node = t->root;
  delete_recursive_rbtree(t,node);//루트랑 t 넘겨주고
  free(t->nil); //NIL노드도 방빼고
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // if(rbtree_find(t,key)) return t->root;//중복
  node_t *node = (node_t *)malloc(sizeof(node_t));
  node->key = key;
  node->parent = t->nil;//이거만 수정할거임
  
  node_t *cur = t->root;
  node_t *prev = t->nil;
  while(cur!=t->nil){//비어있는 리프노드까지 이동(==nil)
    prev=cur;
    if((node->key) < (cur->key))cur = cur->left;
    else cur = cur->right;
  }
  node->parent = prev;
  if (prev == t->nil) {
    t->root = node;
  }
  else if((node->key)<(prev->key)){//추가한 노드의 키보다 부모노드가 크면
    prev->left = node;//부모의 왼쪽 노드로
  }
  else prev->right = node;//아니면 부모의 오른쪽 노드로
  node->left = t->nil;
  node->right = t->nil;
  node->color = RBTREE_RED;  // 노드추가는 무조건 RED
  rbtree_insert_fix(t, node);
  return node;
}






node_t *rbtree_min(const rbtree *t) {
  node_t *temp = t->root;
  while(temp->left != t->nil){//왼쪽자식이 nil이면 스톱.
    temp=temp->left;
  }
  return temp;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *temp = t->root;
  while(temp->right != t->nil){//오른쪽자식이 nil이면 스톱.
    temp=temp->right;
  }
  return temp;
}
void rbtree_erase_fix(rbtree *t, node_t *node){
  while((node != t->root) && (node ->color == RBTREE_BLACK)){//루트가 아니며, 색이 블랙인 경우를 double black으로 판단.
    if (node == node->parent->left) { // insert fix와 비슷하게 체크하는 노드가 왼쪽인 경우부터 확인
      node_t *brother = node->parent->right;
      if(brother->color == RBTREE_RED){//case1: 형제노드가 빨갈때. -> case1은 진행하면 2,3,4중 하나로 바뀜.
        brother->color=RBTREE_BLACK;
        node->parent->color =  RBTREE_RED;
        left_rotate(t,node->parent);
        brother = node->parent->right;
      }
      if((brother->left->color == RBTREE_BLACK) && (brother->right->color == RBTREE_BLACK)){//case2 체크 할 노드와 형제가 둘다 검정일때
        brother->color=RBTREE_RED;
        node = node->parent; // 다음 체크할 노드로 설정
      }
      else {
        if(brother->right->color == RBTREE_BLACK){//case3: 형제의 왼쪽 자식이 빨강인경우,꺾인경우 임. 펴가지고 case4로 넘김
          brother->left->color = RBTREE_BLACK;
          brother->color = RBTREE_RED;
          right_rotate(t,brother);
          brother=node->parent->right;
        }
        //case4: 3에서는 무조건 여기로 넘어옴. 형제의 오른쪽이 red인 경우고 부모랑 형제의 왼쪽 자식 색까은 상관없음.
        brother->color=node->parent->color;
        node->parent->color=RBTREE_BLACK;
        brother->right->color=RBTREE_BLACK;
        left_rotate(t,node->parent);
        node = t->root;
      }
    }
    else{
      node_t *brother = node->parent->left;
      if(brother->color == RBTREE_RED){//case1: 형제노드가 빨갈때. -> case1은 진행하면 2,3,4중 하나로 바뀜.
        brother->color=RBTREE_BLACK;
        node->parent->color =  RBTREE_RED;
        right_rotate(t,node->parent);
        brother = node->parent->left;
      }
      if((brother->left->color == RBTREE_BLACK) && (brother->right->color == RBTREE_BLACK)){//case2 체크 할 노드와 형제가 둘다 검정일때
        brother->color=RBTREE_RED;
        node = node->parent; // 다음 체크할 노드로 설정
      }
      else {
        if(brother->left->color == RBTREE_BLACK){//case3: 형제의 왼쪽 자식이 빨강인경우,꺾인경우 임. 펴가지고 case4로 넘김
          brother->right->color = RBTREE_BLACK;
          brother->color = RBTREE_RED;
          left_rotate(t,brother);
          brother=node->parent->left;
        }
        //case4: 3에서는 무조건 여기로 넘어옴. 형제의 오른쪽이 red인 경우고 부모랑 형제의 왼쪽 자식 색까은 상관없음.
        brother->color=node->parent->color;
        node->parent->color=RBTREE_BLACK;
        brother->left->color=RBTREE_BLACK;
        right_rotate(t,node->parent);
        node = t->root;
      }
    }
  }
  node->color = RBTREE_BLACK;  //<--이거 건들면 파일구조 무너짐.

}

void rb_transplant(rbtree *t, node_t *u, node_t *v){//트리에서 지워질 u노드 자리에 v를 대체
  if (u->parent==t->nil) t->root = v; // 지울 노드가 루트면 v를 루트로 설정
  //부모기준 좌우부터 정리
  else if(u == u->parent->left) u->parent->left=v;//지울노드가 부모의 왼쪽 노드일때

  else u->parent->right=v;
  v->parent = u->parent;
}
node_t *rbtree_min_from(node_t *node, node_t *nil) {//서브트리 최솟값찾기.
  while (node->left != nil)
    node = node->left;
  return node;
}


int rbtree_erase(rbtree *t, node_t *p) {
  node_t *y = p;
  node_t *x;
  color_t y_original_color = y->color; //삭제되는 색 지정
  //자녀 한명만 우에 있는경우
  if (p->left == t->nil){
    x = p->right;
    rb_transplant(t,p,p->right);
  }
  //자녀 한명만 좌에 있는 경우
  else if (p->right == t->nil){
    x = p->left;
    rb_transplant(t,p,p->left);
  }
  //자녀가 두명 이상인 경우
  else{
    y= rbtree_min_from(p->right,t->nil); //y는 오른쪽 서브트리의 가장 작은 값을 가리키는 포인터임.
    y_original_color = y->color; // 이 조건에서 y는 실질적으로 삭제할노드를 가리키므로 색 갱신.
    x = y->right;
    if(y != p->right){//처음 지우려던 위치 바로 한칸 아래가 아닐경우
      rb_transplant(t,y,y->right);//y적출하고 y->right 붙이고, 안에서 y의 오른쪽이 nil이면 알아서 nil 부모 세팅해줌
      //이 다음에 한번더 rb_transplant를 통해 제거했던 y를 z에 붙일거기 때문에 y의 오른쪽 세팅.
      y->right = p->right;
      y->right->parent = y;
    }
    else x->parent = y;//처음 지우려던 위치 바로 한칸 아래일경우, x == nil임.
       // fix 가서 사용해야하므로 nil에 부모 설정.(nil의 더블블랙.)
    rb_transplant(t,p,y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
    
  }
  free(p);
  if (y_original_color==RBTREE_BLACK) rbtree_erase_fix(t,x);//삭제되는 색이 black이면 조정 거치기.
  return 0;
}
void recursive_to_array(const rbtree *t,node_t *temp,key_t *arr, const size_t n,int *i){
  if(temp== t->nil) return;//종료조건
  recursive_to_array(t,temp->left,arr,n,i);
  arr[*i]=temp->key;
  (*i)++;
  recursive_to_array(t,temp->right,arr,n,i);
}
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  node_t *temp = t->root;
  int i=0;
  recursive_to_array(t,temp,arr,n,&i);
  return 0;
}
