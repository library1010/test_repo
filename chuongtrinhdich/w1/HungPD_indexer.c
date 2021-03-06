#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define max 10000
#define maxlength 100
#define maxlengthword 50

typedef struct
{
  char word[maxlengthword];
} EleType;

struct bst_node
{
  EleType data;
  int count;//Number of times appearance
  int lines[max];//The word appear at those lines
  int nlines;
  struct bst_node* left;
  struct bst_node* right;
}; 

struct bst_node* new_node(EleType data);
struct bst_node **search_node(struct bst_node **root, EleType data);
void delete_node(struct bst_node** node);
void insert_node(struct bst_node** root, EleType data);
int inorderprint(struct bst_node *t);
int instopwords(char input[]);  

main()
{
  struct bst_node *t=NULL;
  EleType item, item1;
  char buff[maxlength];
  struct bst_node** node;
  int i, n, k = 0, flag = 0, check, h = 0; //flag: bao truoc do co dau '.', '!', '?'

  FILE *f_in;
  char filename1[]="vanban.txt";
  if((f_in = fopen(filename1, "r")) == NULL)
    {
      printf("Can not open %s!\n", filename1);
      return 0;
    }
  
   FILE *f;  
   char filename3[] = "stopw.txt";
   if((f = fopen(filename3,"r")) == NULL)  
     {  
       printf("Can not open %s!\n", filename3);  
       fclose(f); 
       return 0;  
     }  

   struct bst_node *root=NULL;
   struct bst_node** node1;
   char buff1[maxlength];
   while(1)
     {
       fscanf(f, "%s", buff1);
       strcpy(item1.word, buff1);
       insert_node(&root, item1);
       if(feof(f)) break;
     }//Doc file stopw.txt
    fclose(f);
    
    int linum = 0;
    while (1)
      {
	fscanf(f_in, "%s", buff);
     
	n = strlen(buff);	 
	if (n == 0) continue; 
	if(k!=0)
	  {
	    if ((buff[0] >= 'A') && (buff[0] <= 'Z'))
	      {
		if (flag == 0)
		  {
		    if((buff[n-1] == '.') || (buff[n-1] == '!') || (buff[n-1] == '?'))
		      {
			flag = 1;
			continue;
		      }
		    else continue;		    
		  }
	      } //Xu ly danh tu rieng
	  }
	else k+=1;


	for(i=0; i<n; i++) 
	  { 
	    int j;	  
	    for(j=0; j<n; j++)  buff[i] = tolower(buff[i]);
	    if ((buff[i] == '.') || (buff[i] == '!') || (buff[i] == '?')) 
	      {
		flag = 1;
		buff[i] = '\0';
	      }
	    else flag = 0;
	    
	    if ((buff[i] == ',') || (buff[i] == ';') || (buff[i] == '\n') || (buff[i] == '\r'))
	      {
	 	buff[i] = '\0'; 
	      } 

	  }
	strcpy(item1.word, buff);
	node = search_node(&root, item1);
	if(*node != NULL) continue; //Xu li tu trong stopw.txt
	if(isdigit(buff[0]) != 0) continue; //Xu li so

	strcpy(item.word, buff);
	//	if(feof(f_in)) break;
	if (feof(f_in)) break;
	node = search_node(&t, item);
	if(*node != NULL) {
	  ((*node)->count)++;
	  //Add to Tree
	}
	else {
	  insert_node(&t, item);
	  ((*node)->lines[(*node)->nlines]) = 0;
	  ((*node)->nlines) = 0;
	}

      }

    rewind(f_in);
    while (1)
      {
	if (feof(f_in))
	  break;
	//fscanf(f_in, "%*[^\n]%*c", buff);
	fgets(buff, 100, f_in);
	buff[strlen(buff) - 1] = '\0';
	h+=1;
	if(buff==NULL) continue;

	char *pch;
	pch = strtok(buff, ",.;?!- 0123456789");
	while(pch != NULL)
	  {
	    for(i=0; i<strlen(pch); i++)  pch[i] = tolower(pch[i]);
	    strcpy(item.word, pch);
	    node = search_node(&t, item);
	    if(*node != NULL)
	      {
		if ((*node)->lines[(*node)->nlines - 1] != h) {
		  ((*node)->lines[(*node)->nlines]) = h;
		  ((*node)->nlines)++;
		}
	      }
	    pch = strtok(NULL, ",.;?!- 0123456789");
	  }
      }



    inorderprint(t);
    printf("\n");
    fclose(f_in);
    return 0;


}

struct bst_node* new_node(EleType data)
{
  struct bst_node* result = (struct bst_node *)malloc(sizeof(struct bst_node));
  result->data = data;
  result->count = 1;
  result->left = result->right = NULL;
  result->nlines = 0;

  int i;
  for (i = 0; i < max; i++) {
    result->lines[i] = -1;
  }
  return result;
}

struct bst_node **search_node(struct bst_node **root, EleType data)
{
  int compare_result;
  struct bst_node** node = root;
  while (*node != NULL)
    {
      compare_result = strcmp(data.word, (*node)->data.word);
      if(compare_result < 0) node=&(*node)->left;
      else if (compare_result > 0) node=&(*node)->right;
      else break;
    }
  return node;
}

void delete_node(struct bst_node **node)
{
  struct bst_node* old_node = *node;
  struct bst_node** pred;
  EleType temp;
  if((*node)->left == NULL)
    {
      *node = (*node)->right;
      free(old_node);
    }
  else if((*node)->right == NULL)
    {
      *node = (*node)->left;
      free(old_node);
    }
  else
    {
      pred = &(*node)->left;
      while ((*pred)->right != NULL)
	{
	  pred = &(*pred)->right;
	}
    }

/* Swap values */
  temp = (*pred)->data;
  (*pred)->data = (*node)-> data;
  (*node)->data = temp;
  delete_node(pred);
}

void insert_node(struct bst_node **root, EleType data)
{
  struct bst_node** node= search_node(root, data);
  if(*node == NULL) *node=new_node(data);
}

int inorderprint(struct bst_node *t)
{
  FILE *f_out;
  char filename2[] = "output.txt";
  int i;

  if((f_out = fopen(filename2,"w+")) == NULL)
    {
      printf("Can not open %s!\n", filename2);
      return 0;
    }
  if(t != NULL)
    {
      inorderprint(t->left);
      printf("%s : %d ", t->data.word, t->count);
      for(i=0; i< t->nlines; i++) printf(" %d ", t->lines[i]);
      printf("\n");
      inorderprint(t->right);
    }
  fclose(f_out);
  return 0;	
}

