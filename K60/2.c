#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "WD_generic_graph.h"

void trimLeft(char s[]){
  int i=0, j;
  while(s[0]==' '){
    for(j=0;j<strlen(s)-1;j++){
      s[j]=s[j+1];
    }
    s[strlen(s)-1]='\0';
  }
}

void trimRight(char s[]){
  int i=strlen(s)-1;
  while(s[i]==' '){
    s[i--]='\0';
  }
}

void trimMiddle(char s[]){
  int i=0,j;
  while(s[i]==' ') i++;
  for(i;i<strlen(s);i++){
    if((s[i]==' ')&&(s[i+1]==' ')){
  for(j=i;j<strlen(s)-1;j++){
    s[j]=s[j+1];
  }
  s[strlen(s)-1]='\0';
  i--;
      }
  }
}

// void toLowcase(char s[]){
//   for(int i=0;i<strlen(s);i++){
//     tolower(s[i]);
//   }
// }

void trim(char s[]){
  // toLowcase(s);
  trimLeft(s);
  trimRight(s);
  trimMiddle(s);
}

int split(char *s, char c, char **substr){
    int i = 0, n = 0, j;
    char *ss = malloc(strlen(s));

    while(i < strlen(s)){
        
        j = 0;

        for(i; i < strlen(s); i++){
            if(s[i] == c) break;
            ss[j++] = s[i];
        }
        ss[j] = '\0';
        trim(ss);
        
        substr[n++] = strdup(ss);

        i++;
    }

    free(ss);
    return n;
}

int getFileLine(FILE *fptr){
    char c;
    int n = 0;
    c = fgetc(fptr);
    while(c != EOF) {
        if(c=='\n') n++;
        c = fgetc(fptr);
    }
    fseek(fptr,0,SEEK_SET);
    return n;
}

int getNum(char *s){
    int num = 0;
    for(int i=0; i < strlen(s); i++){
        num = num*10 + s[i] - '0';
    }
    return num;
}

int importProduct(Graph graph, char *file){
    FILE *fpin = fopen(file, "r");

    if(fpin == NULL){
        printf("khong mo duoc file %s",file);
        return -1;
    }
    else {
        int ma, n = 0;
        char ten[100];

        while(!feof(fpin)){
            n++;
            fscanf(fpin,"%d %s\n", &ma, ten);
            
            addVertex(graph, new_jval_i(ma), new_jval_s(strdup(ten)));

        }

        fclose(fpin);
        return n;
    }
}

int importOrder(Graph graph, char *file){
    FILE *fpin = fopen(file, "r");

    if(fpin == NULL){
        printf("khong mo duoc file %s",file);
        return -1;
    }
    else {
        int nl = getFileLine(fpin), n, u, v;
        char line[100], *ss, s[100];
        char **list = malloc(100*sizeof(char*));
        JRB vertex;

        for(int i = 0; i < 100; i++){
            list[i] = NULL;
        }

        for(int i = 0; i < nl; i++){
            ss = fgets(s,100,fpin);

            s[strlen(s) - 1] = '\0';

            n = split(s, ' ', list);
            for(int i = 0; i < n; i++){
                u = getNum(list[i]);
                for(int j = i+1; j < n; j++){
                    v = getNum(list[j]);
                    addEdge(graph, new_jval_i(u), new_jval_i(v), 1);
                    addEdge(graph, new_jval_i(v), new_jval_i(u), 1);

                    // printf("%d %d\n", u, v);
                }
                vertex = jrb_find_gen(graph.vertices, new_jval_i(u), graph.func);
                printf("%s ", jval_s(vertex->val));
            }
            printf("\n");
        }

        for(int i = 0; i < 100; i++){
            if(list[i] != NULL) free(list[i]);
        }

        free(list);
        fclose(fpin);
        return nl;
    }
}


void menu(){
    printf("\n\nChuong trinh thong ke san pham lien quan\n");
    printf("Author: Anh Khoa\n\n");
    printf("1. In danh sach san pham\n");
    printf("2. In danh sach giao dich\n");
    printf("3. Hien thi muc do lien quan giua 2 san pham\n");
    printf("4. In danh sach san pham lien quan\n");
    printf("5. Hien thi moi lien he giua 2 san pham\n");
    printf("6. Giai phong cay va Thoat\n");
}

int choose(){
    int n;
    do {
        printf("Nhap lua chon: ");
        if(scanf("%d",&n)==0) n=-1;
    } while(getchar()!='\n');
        return n;
}

int compareInt(Jval ja, Jval jb){
    int a = jval_i(ja);
    int b = jval_i(jb);
    
    if(a > b) 
        return 1;
    else if(a < b) 
        return -1;
    else 
        return 0;
}

int main(int argc, char *argv[]){
    Graph graph = createGraph(&compareInt);
    JRB node, n;
    int id1, id2, w;
    JRB output, output_s;

    int c;
    do {
        menu();
        c = choose();
        switch(c){
        case 1:
            importProduct(graph, "products.txt");

            jrb_traverse(node, graph.vertices){
                visitNode(node);
            }
            break;
        case 2:
            printf("\n");
            importOrder(graph, "orderhistory.txt");
            break;
        case 3:
            printf("Nhap ma san pham 1: ");
            scanf("%d%*c", &id1);
            printf("Nhap ma san pham 2: ");
            scanf("%d%*c", &id2);
            if(jrb_find_gen(graph.vertices, new_jval_i(id1), graph.func) == NULL || jrb_find_gen(graph.vertices, new_jval_i(id2), graph.func) == NULL) {
                printf("Ma so khong ton tai\n");
            } else {
                w = hasEdge(graph, new_jval_i(id1), new_jval_i(id2));
                printf("Do lien quan giua 2 san pham: %d\n", w);
            }
            break;
        case 4:   
            output = make_jrb();
            output_s = make_jrb();

            printf("Nhap ma san pham: ");
            scanf("%d%*c", &id1);
            outdegree(graph, new_jval_i(id1), output);

            if(jrb_find_gen(graph.vertices, new_jval_i(id1), graph.func) == NULL) {
                printf("Ma so khong ton tai\n");
            } else {

                jrb_traverse(node, output){
                    w = hasEdge(graph, new_jval_i(id1), node->key);
                    jrb_insert_gen(output_s, new_jval_i(w), node->key, &compareInt);
                }
                jrb_rtraverse(node, output_s){
                    n = jrb_find_gen(graph.vertices, node->val, &compareInt);
                    printf("%s\n", jval_s(n->val));
                }

                jrb_free_tree(output);   
                jrb_free_tree(output_s);      
            }
            break;
        case 5:
            printf("Nhap ma san pham 1: ");
            scanf("%d%*c", &id1);
            // printf("Nhap ma san pham 2: ");
            // scanf("%d%*c", &id2);
            if(jrb_find_gen(graph.vertices, new_jval_i(id1), graph.func) == NULL) {
                printf("Ma so khong ton tai\n");
            } else {
            BFS(graph, new_jval_i(id1), &printNodeVal, 4);

            // Dijkstra(graph, new_jval_i(id1), new_jval_i(id2), 1);
            }
            break;
        case 6:
            dropGraph(graph);
            break;
        default:
	  printf("Lua chon sai!\n");
            break;
        }
    } while(c!= 6);
    return 0;
}