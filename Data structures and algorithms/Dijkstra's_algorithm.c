#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define Up 0
#define Right 1
#define Down 2
#define Left 3



typedef struct NODE {
	char val;
	int x, y, z;
	int time;
	int flag;
	int check_v;
	int count_n;
	int distanse;
	struct NODE **neighbors;
} node;

/*
|0	|G	|D	|P1	|P2	|P3	|P4	|P5	|
---------------------------------
|0	|0	|0	|0	|0	|0	|0	|0	|
*/

/*
typedef struct HEAP{
	int *val;
	int *prio;
	int *index;

}heap;*/


node ****map;
node **heap;
int count_heap = 0;
int d_x = -1, d_y = -1;
int g_x = -1, g_y = -1;
int p1_x = -1, p1_y = -1, p2_x = -1, p2_y = -1, p3_x = -1, p3_y = -1, p4_x = -1, p4_y = -1, p5_x = -1, p5_y = -1;
int count_p = 0;
int check_g = 0;

void connections(int n, int m, char **mapa, int f)
{
	
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (mapa[i][j] >= '0' && mapa[i][j] <= '9')
			{
				for (int k = 0; k < n; k++)
				{
					for (int o = 0; o < m; o++)
					{
						if (mapa[k][o] == mapa[i][j] && (k != i || o != j))
						{
							map[f][i][j]->count_n++;
							map[f][i][j]->neighbors = realloc(map[f][i][j]->neighbors, map[f][i][j]->count_n * sizeof(node*));
							map[f][i][j]->neighbors[map[f][i][j]->count_n - 1] = map[f][k][o];
						}
					}
				}
			}
				
			if (i == 0 && j == 0)
			{
				if (mapa[i][j + 1] != 'N') 
				{
					map[f][i][j]->neighbors[Right] = map[f][i][j + 1];
				}
				if (mapa[i + 1][j] != 'N') 
				{
					map[f][i][j]->neighbors[Down] = map[f][i + 1][j];
				}
			}

			if (i == 0 && j == m - 1)
			{
				if (mapa[i][j - 1] != 'N') 
				{
					map[f][i][j]->neighbors[Left] = map[f][i][j - 1];
				}
				if (mapa[i + 1][j] != 'N')
				{
					map[f][i][j]->neighbors[Down] = map[f][i + 1][j];
				}
			}

			if (i == n - 1 && j == 0)
			{
				if (mapa[i][j + 1] != 'N') 
				{
					map[f][i][j]->neighbors[Right] = map[f][i][j + 1];
				}
				if (mapa[i - 1][j] != 'N')
				{
					map[f][i][j]->neighbors[Up] = map[f][i - 1][j];
				}
			}

			if (i == n - 1 && j == m - 1)
			{
				if (mapa[i][j - 1] != 'N')
				{
					map[f][i][j]->neighbors[Left] = map[f][i][j - 1];
				}
				if (mapa[i - 1][j] != 'N')
				{
					map[f][i][j]->neighbors[Up] = map[f][i - 1][j];
				}
			}

			if (i == 0 && j != 0 && j != m - 1)
			{
				if (mapa[i][j + 1] != 'N') {
					map[f][i][j]->neighbors[Right] = map[f][i][j + 1];
				}
				if (mapa[i + 1][j] != 'N')
				{
					map[f][i][j]->neighbors[Down] = map[f][i + 1][j];
				}
				if (mapa[i][j - 1] != 'N')
				{
					map[f][i][j]->neighbors[Left] = map[f][i][j - 1];
				}
			}

			if (i == n - 1 && j != 0 && j != m - 1)
			{
				if (mapa[i][j + 1] != 'N') {
					map[f][i][j]->neighbors[Right] = map[f][i][j + 1];
				}
				if (mapa[i - 1][j] != 'N')
				{
					map[f][i][j]->neighbors[Up] = map[f][i - 1][j];
				}
				if (mapa[i][j - 1] != 'N')
				{
					map[f][i][j]->neighbors[Left] = map[f][i][j - 1];
				}
			}

			if (i != 0 && i != n - 1 && j == m - 1)
			{
				if (mapa[i + 1][j] != 'N') {
					map[f][i][j]->neighbors[Down] = map[f][i + 1][j];
				}
				if (mapa[i - 1][j] != 'N')
				{
					map[f][i][j]->neighbors[Up] = map[f][i - 1][j];
				}
				if (mapa[i][j - 1] != 'N')
				{
					map[f][i][j]->neighbors[Left] = map[f][i][j - 1];
				}
			}

			if (i != 0 && i != n - 1 && j == 0)
			{
				if (mapa[i + 1][j] != 'N') {
					map[f][i][j]->neighbors[Down] = map[f][i + 1][j];
				}
				if (mapa[i - 1][j] != 'N')
				{
					map[f][i][j]->neighbors[Up] = map[f][i - 1][j];
				}
				if (mapa[i][j + 1] != 'N')
				{
					map[f][i][j]->neighbors[Right] = map[f][i][j + 1];
				}
			}

			if (i != 0 && i != n - 1 && j != 0 && j != m - 1)
			{
				if (mapa[i - 1][j] != 'N')
				{
					map[f][i][j]->neighbors[Up] = map[f][i - 1][j];
				}
				if (mapa[i][j + 1] != 'N')
				{
					map[f][i][j]->neighbors[Right] = map[f][i][j + 1];
				}
				if (mapa[i + 1][j] != 'N') {
					map[f][i][j]->neighbors[Down] = map[f][i + 1][j];
				}
				if (mapa[i][j - 1] != 'N')
				{
					map[f][i][j]->neighbors[Left] = map[f][i][j - 1];
				}
			}
		}
	}
}


/*
void create_heap()
{
	heap = (node**)malloc(sizeof(node*));
}*/

void push_heap(node *v)
{
	printf("**heap[0] = %d\n", heap[0]->distanse);
	heap = (node**)realloc(heap, sizeof(node*) * count_heap + 1);//??????????????????????????????????
	
	heap[count_heap - 1] = v;
	int parent_index = (count_heap - 1) / 2;
	int tmp_index = count_heap;
	printf("parent_index = %d\n", parent_index);
	printf("tmp_index = %d\n", tmp_index);
	if (count_heap > 1 && parent_index >= 0)
		while (heap[parent_index]->distanse > heap[tmp_index]->distanse)
		{
			node *tmp = heap[parent_index];
			heap[parent_index] = heap[tmp_index];
			heap[tmp_index] = tmp;

			parent_index = (parent_index - 1) / 2;
			if (parent_index < 0)
				break;
		}
	printf("heap[0] = %d\n", heap[0]->distanse);
	count_heap++;
}

node *pop_heap()
{
	node *min = heap[0];
	//printf("*%c\n", min->val);
	heap[0] = heap[count_heap - 1];
	count_heap--;
	heap = (node**)realloc(heap, sizeof(node*) * count_heap);
	
	int start_index = 0;
	int check = 0;
	if (count_heap > 1)
	while (check >= 0)
	{
		node *tmp;
		int left_index = start_index * 2 + 1;
		int right_index = start_index * 2 + 2;
		printf("dist_start = %d\n", heap[start_index]->distanse);
		printf("left_dist = %d\n", heap[left_index]->distanse);

		if (heap[start_index]->distanse > heap[left_index]->distanse)
		{
			tmp = heap[start_index];
			heap[start_index] = heap[left_index];
			heap[left_index] = heap[start_index];
			start_index = left_index;
			check++;
		}
		else if (heap[start_index]->distanse > heap[right_index]->distanse)
		{
			tmp = heap[start_index];
			heap[start_index] = heap[right_index];
			heap[right_index] = heap[start_index];
			start_index = right_index;
			check++;
		}
		check--;
	}
	if (min->val == 'G')
	{
		for (int i = 0; i < min->count_n; i++)
		{

		}
	}
	min->check_v = 1;
	return min;
}

int set_bit(char val, int z, int x, int y)
{
	if (val == 'G')
		z += 64;
	if (val == 'D')
		z += 32;
	if (val == 'P' && p1_x == x && p1_y == y && (z >= 32 || z > 64))
		z += 16;
	if (val == 'P' && p2_x == x && p2_y == y && (z >= 32 || z > 64))
		z += 8;
	if (val == 'P' && p3_x == x && p3_y == y && (z >= 32 || z > 64))
		z += 4;
	if (val == 'P' && p4_x == x && p4_y == y && (z >= 32 || z > 64))
		z += 2;
	if (val == 'P' && p5_x == x && p5_y == y && (z >= 32 || z > 64))
		z += 1;
	return z;
}

void dijkstra()
{
	
	heap = (node**)malloc(sizeof(node*));
	map[0][0][0]->distanse = 0;
	heap[0] = map[0][0][0];
	count_heap++;
	int tmp_z = 0;
	int tmp_x = 0;
	int tmp_y = 0;
	//printf("%d\n", map[0][0][0]->neighbors[1]->distanse);
	//push_heap(map[0][0][0]);

	node *min;
	while (1)
	{
		if (count_heap == 0)
			break;
		printf("count_h = %d\n", count_heap);
		min = pop_heap();
		tmp_x = min->x;
		tmp_y = min->y;
		if (min->val == 'G' && min->val == 'D' && min->val == 'P')
		{
			printf("set_bit \n");
			tmp_z = set_bit(min->val, min->z, min->x, min->y);
		}
		else tmp_z = min->z;
		if (tmp_z >= 64)
			for (int i = 0; i < map[tmp_z][tmp_y][tmp_x]->count_n; i++)
			{
				if (map[tmp_z][tmp_y][tmp_x]->neighbors[i] != NULL && map[tmp_z][tmp_y][tmp_x]->neighbors[i]->check_v == 0)
				{
					map[tmp_z][tmp_y][tmp_x]->neighbors[i]->distanse = min->distanse + map[tmp_z][tmp_y][tmp_x]->neighbors[i]->time;
					push_heap(map[tmp_z][tmp_y][tmp_x]->neighbors[i]);
				}
			}
		else 
			for (int i = 0; i < 4; i++)
				if (map[tmp_z][tmp_y][tmp_x]->neighbors[i] != NULL && map[tmp_z][tmp_y][tmp_x]->neighbors[i]->check_v == 0)
				{
					printf("PUSH 4\n");
					map[tmp_z][tmp_y][tmp_x]->neighbors[i]->distanse = min->distanse + map[tmp_z][tmp_y][tmp_x]->neighbors[i]->time;
					push_heap(map[tmp_z][tmp_y][tmp_x]->neighbors[i]);
				}
		
		
	}

}

/*
void dijkstra3(graph_t *g, int a, int b) {
	int i, j;
	a = a - 'a';
	b = b - 'a';
	for (i = 0; i < g->vertices_len; i++) {
		vertex_t *v = g->vertices[i];
		v->dist = INT_MAX;
		v->prev = 0;
		v->visited = 0;
	}
	vertex_t *v = g->vertices[a];
	v->dist = 0;
	heap_t *h = create_heap(g->vertices_len);
	push_heap(h, a, v->dist);
	while (h->len) {
		i = pop_heap(h);
		if (i == b)
			break;
		v = g->vertices[i];
		v->visited = 1;
		for (j = 0; j < v->edges_len; j++) {
			edge_t *e = v->edges[j];
			vertex_t *u = g->vertices[e->vertex];
			if (!u->visited && v->dist + e->weight <= u->dist) {
				u->prev = i;
				u->dist = v->dist + e->weight;
				push_heap(h, e->vertex, u->dist);
			}
		}
	}
}
*/

int dijkstra2(int **newMapa, int count, int start, int finish, int *way)
{
	int *min_long; // минимальное расстояние
	int *vertex; // посещенные вершины
	int temp;
	int min_index, min;
	int i;
	int checkH = 0;

	min_long = (int*)malloc((count + 1) * sizeof(int));
	vertex = (int*)malloc((count + 1) * sizeof(int));


	for (i = 0; i<count; i++)
	{
		min_long[i] = 100000;
		vertex[i] = 1;
	}
	min_long[start] = 0;

	do {
		min_index = 100000;
		min = 100000;
		for (i = 0; i < count; i++)
		{
			if ((vertex[i] == 1) && (min_long[i]<min))
			{
				min = min_long[i];
				min_index = i;
			}
		}
		if (min_index != 100000)
		{
			for (i = 0; i<count; i++)
			{
				if (newMapa[min_index][i] > 0)
				{
					temp = min + newMapa[min_index][i];
					if (temp < min_long[i])
						min_long[i] = temp;
				}
			}
			vertex[min_index] = 0;
		}
	} while (min_index < 100000);

	int *row;
	row = (int*)malloc(sizeof(int) * (count + 1));
	for (i = 0; i < count; i++) //i <= count
		row[i] = -100;
	int tmp = finish;
	int j = 0;
	for (i = 0; i < count; i++)
	{
		j = 0;
		while (j < count)
		{
			if (newMapa[tmp][j] == 0)
			{
				j++;
				continue;
			}
			if (min_long[j] + newMapa[tmp][j] == min_long[tmp])
			{
				if (newMapa[tmp][j] == 2)
					checkH = 1;
				row[i] = tmp;
				tmp = j;
				break;
			}
			j++;
		}
		if (tmp == start)
			break;
	}
	row[i + 1] = start;
	for (i = 0; i < count; i++)
		way[i] = row[i];
	return min_long[finish];
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty)
{
	map = (node****)malloc(sizeof(node***) * 128);
	////
	for (int f = 0; f < 128; f++)
	{
	////

		map[f] = (node***)malloc(sizeof(node**) * n);
		for (int i = 0; i < n; i++)
		{
			map[f][i] = (node**)malloc(sizeof(node*) * m);
			for (int j = 0; j < m; j++)
			{
				map[f][i][j] = (node*)malloc(sizeof(node));
				map[f][i][j]->val = mapa[i][j];
				map[f][i][j]->x = j;
				map[f][i][j]->y = i;
				map[f][i][j]->z = f;
				map[f][i][j]->flag = 0;
				map[f][i][j]->distanse = INT_MAX;
				map[f][i][j]->check_v = 0;
				map[f][i][j]->count_n = 4;
				map[f][i][j]->neighbors = (node**)malloc(sizeof(node*) * 4);
				for (int k = 0; k < 4; k++)
				{
					map[f][i][j]->neighbors[k] = (node*)malloc(sizeof(node));
					map[f][i][j]->neighbors[k] = NULL;
				}
				if (mapa[i][j] == 'H')
					map[f][i][j]->time = 2;
				else 
					map[f][i][j]->time = 1;

				if (map[f][i][j] == 'P')
				{
					if (p1_x == -1)
					{
						p1_x = j;
						p1_y = i;
						count_p++;
					}
					else if (p2_x == -1)
					{
						p2_x = j;
						p2_y = i;
						count_p++;
					}
					else if (p3_x == -1)
					{
						p3_x = j;
						p3_y = i;
						count_p++;
					}
					else if (p4_x == -1)
					{
						p4_x = j;
						p4_y = i;
						count_p++;
					}
					else if (p5_x == -1)
					{
						p5_x = j;
						p5_y = i;
						count_p++;
					}
				}

				if (map[f][i][j] == 'D' && d_x == -1)
				{
					d_x = j;
					d_y = i;
				}
				if (map[f][i][j] == 'G' && g_x == -1)
				{
					g_x = j;
					g_y = i;
				}

			}

		}
		//printf("*%c\n", map[f][0][0 + 1]->val);
	/////
		connections(n, m, mapa, f);
	}
	////
	
	//dijkstra();
	heap = (node**)malloc(sizeof(node*));
	//for (int i = 0; i < 5; i++)
	//{
		//heap[0] = (node*)malloc(sizeof(node));
	//}
	//heap[0] = map[0][0][0];
	for (int i = 0; i < 5; i++)
	{
		push_heap(map[0][0][i]);
	}
	int *a = (int*)malloc(sizeof(int)*2);
	a[0] = 0;
	return a;
}

int main()
{

	int i;
	int *p;
	int *lol;
	p = (int*)malloc(100 * sizeof(int));
	char **mapa = (char**)malloc(5 * sizeof(char*));
	for (i = 0; i < 5; i++)
		mapa[i] = (char*)malloc(5 * sizeof(char));

	mapa[0][0] = 'C';
	mapa[0][1] = 'H';
	mapa[0][2] = 'H';
	mapa[0][3] = 'D';
	//mapa[0][4] = 'G';
	mapa[0][4] = 'C';

	mapa[1][0] = 'H';
	//mapa[1][0] = '0';
	mapa[1][1] = 'C';
	mapa[1][2] = 'C';
	mapa[1][3] = 'H';
	//mapa[1][4] = '0';
	mapa[1][4] = 'H';

	mapa[2][0] = 'C';
	mapa[2][1] = 'H';
	mapa[2][2] = 'P';
	mapa[2][3] = 'P';
	mapa[2][4] = 'C';

	mapa[3][0] = 'P';
	mapa[3][1] = 'H';
	mapa[3][2] = 'H';
	mapa[3][3] = 'H';
	mapa[3][4] = 'C';
	
	mapa[4][0] = 'C';
	mapa[4][1] = 'H';
	mapa[4][2] = 'H';
	mapa[4][3] = 'H';
	mapa[4][4] = 'C';
	p = zachran_princezne(mapa, 5, 5, 0, &lol);

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			printf("%d ", map[0][i][j]->count_n);
		}
		printf("\n");
	}
	system("pause");
	/*mapa[0][0] = 'C';
	mapa[0][1] = 'C';
	mapa[0][2] = 'C';
	mapa[0][3] = 'H';
	mapa[0][4] = 'H';
	mapa[0][5] = 'C';
	mapa[0][6] = 'H';

	mapa[1][0] = 'C';
	mapa[1][1] = 'C';
	mapa[1][2] = 'H';
	mapa[1][3] = 'H';
	mapa[1][4] = 'H';
	mapa[1][5] = 'C';
	mapa[1][6] = 'H';

	mapa[2][0] = 'H';
	mapa[2][1] = 'C';
	mapa[2][2] = 'D';
	mapa[2][3] = 'C';
	mapa[2][4] = 'H';
	mapa[2][5] = 'H';
	mapa[2][6] = 'C';

	mapa[3][0] = 'H';
	mapa[3][1] = 'H';
	mapa[3][2] = 'H';
	mapa[3][3] = 'C';
	mapa[3][4] = 'H';
	mapa[3][5] = 'H';
	mapa[3][6] = 'C';

	mapa[4][0] = 'H';
	mapa[4][1] = 'C';
	mapa[4][2] = 'H';
	mapa[4][3] = 'H';
	mapa[4][4] = 'C';
	mapa[4][5] = 'C';
	mapa[4][6] = 'C';

	mapa[5][0] = 'H';
	mapa[5][1] = 'C';
	mapa[5][2] = 'H';
	mapa[5][3] = 'H';
	mapa[5][4] = 'P';
	mapa[5][5] = 'H';
	mapa[5][6] = 'H';

	mapa[6][0] = 'P';
	mapa[6][1] = 'C';
	mapa[6][2] = 'H';
	mapa[6][3] = 'C';
	mapa[6][4] = 'H';
	mapa[6][5] = 'H';
	mapa[6][6] = 'P';*/
	//translate_matrix(mapa, 5, 5);
	//p = zachran_princezne(mapa, 7, 7, 0, &lol);
	//getchar();
	return 0;
}