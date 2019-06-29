#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void* memory;

void to_byte(unsigned char *position, int tmp, int check)
{
	*position = (tmp >> 24) & 0xff;
	*(position + 1) = (tmp >> 16) & 0xff;
	*(position + 2) = (tmp >> 8) & 0xff;
	if(check == 0) *(position + 3) = tmp & 0xff;
	else *(position + 3) = (tmp & 0xff) | 1;
}

int to_int(unsigned char *position)
{
	int int_pos = *(position + 0) * 16777216 + *(position + 1) * 65536 + *(position + 2) * 256 + *(position + 3);
	return int_pos;
}

void *memory_alloc(unsigned int size)
{
	unsigned char *newBlock = memory;
	//int firstFree = newBlock[0] * 16777216 + newBlock[1] * 65536 + newBlock[2] * 256 + newBlock[3];
	int firstFree = to_int(&newBlock[0]);
	//int oldSize = newBlock[firstFree] * 16777216 + newBlock[firstFree + 1] * 65536 + newBlock[firstFree + 2] * 256 + newBlock[firstFree + 3];
	int oldSize = to_int(&newBlock[firstFree]);
	int nextBlock = 1;
	int check = 0;
	int i = firstFree;
	
	if (size < 8) size = 8;
	if (size % 2 == 1) size++;
	
	if (firstFree == 0)
	{
		//printf("ERROR 0 in first block\n");
		return NULL;
	}

	//if (((newBlock[firstFree + 4] * 16777216 + newBlock[firstFree + 5] * 65536 + newBlock[firstFree + 6] * 256 + newBlock[firstFree + 7]) == 0)
	//	&& (newBlock[firstFree + 8] * 16777216 + newBlock[firstFree + 9] * 65536 + newBlock[firstFree + 10] * 256 + newBlock[firstFree + 11]) == 0)
	if((to_int(&newBlock[firstFree + 4]) == 0) && (to_int(&newBlock[firstFree + 8]) == 0))
	{
		//printf("if next end previus 0000 0000 (only one free block)\n");
		if ((size + 8) > oldSize)
		{
			//printf("ERROR 1\n");
			return NULL;
		}
		//printf("old size = %d\n", oldSize);
		//printf("size = %d\n", size);
		//printf("fF = %d\n", firstFree);
		if ((oldSize - (size + 8)) >= 16)
		{
			//printf("if we will have remainder\n");
			to_byte(&newBlock[firstFree], size + 8, 1);//header
			/*newBlock[firstFree] = (size + 8) >> 24;//header
			newBlock[firstFree + 1] = (size + 8) >> 16 & 0xff;
			newBlock[firstFree + 2] = (size + 8) >> 8 & 0xff;
			newBlock[firstFree + 3] = ((size + 8) & 0xff) | 1;*/

			to_byte(&newBlock[firstFree + size + 4], size + 8, 1);//footer
			/*newBlock[firstFree + size + 4] = (size + 8) >> 24;//footer
			newBlock[firstFree + size + 5] = (size + 8) >> 16 & 0xff;
			newBlock[firstFree + size + 6] = (size + 8) >> 8 & 0xff;
			newBlock[firstFree + size + 7] = ((size + 8) & 0xff) | 1;*/

			to_byte(&newBlock[firstFree + size + 8], oldSize - size - 8, 0);// header for remainder
			/*newBlock[firstFree + size + 8] = (oldSize - size - 8) >> 24; // header for remainder
			newBlock[firstFree + size + 9] = (oldSize - size - 8) >> 16 & 0xff;
			newBlock[firstFree + size + 10] = (oldSize - size - 8) >> 8 & 0xff;
			newBlock[firstFree + size + 11] = (oldSize - size - 8) & 0xff;*/

			newBlock[firstFree + size + 12] = 0; // address of next free
			newBlock[firstFree + size + 13] = 0;
			newBlock[firstFree + size + 14] = 0;
			newBlock[firstFree + size + 15] = 0;

			newBlock[firstFree + size + 16] = 0; // address of prev free
			newBlock[firstFree + size + 17] = 0;
			newBlock[firstFree + size + 18] = 0;
			newBlock[firstFree + size + 19] = 0;

			to_byte(&newBlock[0], firstFree + size + 8, 0);//address of first free
			/*newBlock[0] = (firstFree + size + 8) >> 24;//address of first free
			newBlock[1] = (firstFree + size + 8) >> 16 & 0xff;
			newBlock[2] = (firstFree + size + 8) >> 8 & 0xff;
			newBlock[3] = (firstFree + size + 8) & 0xff;*/

			to_byte(&newBlock[firstFree + oldSize - 4], oldSize - size - 8, 0);// footer for remainder
			/*newBlock[firstFree + oldSize - 4] = (oldSize - size - 8) >> 24; // footer for remainder
			newBlock[firstFree + oldSize - 3] = (oldSize - size - 8) >> 16 & 0xff;
			newBlock[firstFree + oldSize - 2] = (oldSize - size - 8) >> 8 & 0xff;
			newBlock[firstFree + oldSize - 1] = (oldSize - size - 8) & 0xff;*/
			
			return newBlock + (&newBlock[firstFree] - &newBlock[0]) + 4;
		}
		else
		{
			//printf("if remainder < 16 (all free block will be alloc)\n");
			newBlock[firstFree + 3] = newBlock[firstFree + 3] | 1;
			
			newBlock[0] = 0;//address of first free
			newBlock[1] = 0;
			newBlock[2] = 0;
			newBlock[3] = 0;

			newBlock[firstFree + oldSize - 1] = newBlock[firstFree + oldSize - 1] | 1;
			//printf("\n*%d\n\n", newBlock + (&newBlock[firstFree] - &newBlock[0]) + 4);
			return newBlock + (&newBlock[firstFree] - &newBlock[0]) + 4;
		}
	}
	else if (oldSize >= (size + 8) || oldSize <= (size + 8))
	{
		//printf("oldSize >= (size + 8) || oldSize <= (size + 8))\n");
		if (oldSize < (size + 8))
		{
			//printf("oldSize < (size + 8) search ok block\n");
			while (nextBlock != 0)
			{
				//printf("statr searching\n");
				check = 1;
				//nextBlock = newBlock[i + 4] * 16777216 + newBlock[i + 5] * 65536 + newBlock[i + 6] * 256 + newBlock[i + 7];
				nextBlock = to_int(&newBlock[i + 4]);
				//oldSize = newBlock[nextBlock] * 16777216 + newBlock[nextBlock + 1] * 65536 + newBlock[nextBlock + 2] * 256 + newBlock[nextBlock + 3];
				oldSize = to_int(&newBlock[nextBlock]);
				if (oldSize >= (size + 8))
				{
					//printf("if this block is ok\n");
					firstFree = nextBlock;
					break;
				}
				else
				{
					//printf("next block\n");
					i = nextBlock;
					//continue;
				}
			}
			if (nextBlock == 0)
			{
				//printf("if we have not ok block\n");
				return NULL;
			}

		}
			if (oldSize >= (size + 8))
			{
				//printf("if first block in list is ok\n");
				if (oldSize - (size + 8) >= 16)
				{
					//printf("if we will have remainder 2\n");
					
					to_byte(&newBlock[firstFree], size + 8, 1); //header
					/*newBlock[firstFree] = (size + 8) >> 24;//header
					newBlock[firstFree + 1] = (size + 8) >> 16 & 0xff;
					newBlock[firstFree + 2] = (size + 8) >> 8 & 0xff;
					newBlock[firstFree + 3] = ((size + 8) & 0xff) | 1;*/

					to_byte(&newBlock[firstFree + size + 4], size + 8, 1);//footer
					/*newBlock[firstFree + size + 4] = (size + 8) >> 24;//footer
					newBlock[firstFree + size + 5] = (size + 8) >> 16 & 0xff;
					newBlock[firstFree + size + 6] = (size + 8) >> 8 & 0xff;
					newBlock[firstFree + size + 7] = ((size + 8) & 0xff) | 1;*/

					to_byte(&newBlock[firstFree + size + 8], oldSize - size - 8, 0); // header for remainder
					/*newBlock[firstFree + size + 8] = (oldSize - size - 8) >> 24; // header for remainder
					newBlock[firstFree + size + 9] = (oldSize - size - 8) >> 16 & 0xff;
					newBlock[firstFree + size + 10] = (oldSize - size - 8) >> 8 & 0xff;
					newBlock[firstFree + size + 11] = (oldSize - size - 8) & 0xff;*/
					
					newBlock[firstFree + size + 12] = newBlock[firstFree + 4];
					newBlock[firstFree + size + 13] = newBlock[firstFree + 5];
					newBlock[firstFree + size + 14] = newBlock[firstFree + 6];
					newBlock[firstFree + size + 15] = newBlock[firstFree + 7];
					
					newBlock[firstFree + size + 16] = newBlock[firstFree + 8];
					newBlock[firstFree + size + 17] = newBlock[firstFree + 9];
					newBlock[firstFree + size + 18] = newBlock[firstFree + 10];
					newBlock[firstFree + size + 19] = newBlock[firstFree + 11];
					
					to_byte(&newBlock[(firstFree + oldSize) - 4], oldSize - size - 8, 0);//footer for remainder \\\\\!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					/*to_byte(&newBlock[oldSize - 3], oldSize - size - 8, 0); 
					newBlock[(firstFree + oldSize) - 4] = newBlock[firstFree + size + 8]; // footer for remainder  (izmenen na verhnij xz)
					newBlock[(firstFree + oldSize) - 3] = newBlock[firstFree + size + 9];
					newBlock[(firstFree + oldSize) - 2] = newBlock[firstFree + size + 10];
					newBlock[(firstFree + oldSize) - 1] = newBlock[firstFree + size + 11];*/

					int next, prev;
					//next = newBlock[firstFree + 4] * 16777216 + newBlock[firstFree + 5] * 65536 + newBlock[firstFree + 6] * 256 + newBlock[firstFree + 7];
					next = to_int(&newBlock[firstFree + 4]);
					//prev = newBlock[firstFree + 8] * 16777216 + newBlock[firstFree + 9] * 65536 + newBlock[firstFree + 10] * 256 + newBlock[firstFree + 11];
					prev = to_int(&newBlock[firstFree + 8]);
					//printf("if we will have remainder 2\n");

					//**********************************
					if (next != 0) {
						//printf("4\n");
						to_byte(&newBlock[next + 8], (&newBlock[firstFree + size + 8] - &newBlock[0]), 0);
						/*newBlock[next + 8] = (&newBlock[firstFree + size + 8] - &newBlock[0]) >> 24;
						newBlock[next + 9] = (&newBlock[firstFree + size + 8] - &newBlock[0]) >> 16 & 0xff;
						newBlock[next + 10] = (&newBlock[firstFree + size + 8] - &newBlock[0]) >> 8 & 0xff;
						newBlock[next + 11] = (&newBlock[firstFree + size + 8] - &newBlock[0]) & 0xff;*/
					}
					//****************************************
					if (prev != 0 && check != 0) {
						//printf("5\n");
						to_byte(&newBlock[prev + 4], (&newBlock[firstFree + size + 8] - &newBlock[0]), 0);
						/*newBlock[prev + 4] = (&newBlock[firstFree + size + 8] - &newBlock[0]) >> 24;
						newBlock[prev + 5] = (&newBlock[firstFree + size + 8] - &newBlock[0]) >> 16 & 0xff;
						newBlock[prev + 6] = (&newBlock[firstFree + size + 8] - &newBlock[0]) >> 8 & 0xff;
						newBlock[prev + 7] = (&newBlock[firstFree + size + 8] - &newBlock[0]) & 0xff;*/
					}
					else {
						//printf("6\n");
						to_byte(&newBlock[0], (&newBlock[firstFree + size + 8] - &newBlock[0]), 0);
						/*newBlock[0] = (&newBlock[firstFree + size + 8] - &newBlock[0]) >> 24;
						newBlock[1] = (&newBlock[firstFree + size + 8] - &newBlock[0]) >> 16 & 0xff;
						newBlock[2] = (&newBlock[firstFree + size + 8] - &newBlock[0]) >> 8 & 0xff;
						newBlock[3] = (&newBlock[firstFree + size + 8] - &newBlock[0]) & 0xff;*/
					}
					return newBlock + (&newBlock[firstFree] - &newBlock[0]) + 4;

				}
				else
				{
					//printf("if we didn't have remainder\n");
					newBlock[firstFree + 3] = newBlock[firstFree + 3] | 1;

					if (check == 0)
					{
						//printf("esli nebylo cykla\n");
						int tmp;
						//tmp = newBlock[firstFree + 4] * 16777216 + newBlock[firstFree + 5] * 65536 + newBlock[firstFree + 6] * 256 + newBlock[firstFree + 7];
						tmp = to_int(&newBlock[firstFree + 4]);

						to_byte(&newBlock[0], tmp, 0);//address of first free
						/*newBlock[0] = tmp >> 24;//address of first free
						newBlock[1] = tmp >> 16 & 0xff;
						newBlock[2] = tmp >> 8 & 0xff;
						newBlock[3] = tmp & 0xff;*/
						
						/*newBlock[0] = newBlock[tmp + 4];
						newBlock[1] = newBlock[tmp + 5];
						newBlock[2] = newBlock[tmp + 6];
						newBlock[3] = newBlock[tmp + 7];*/

						newBlock[tmp + 8] = 0;
						newBlock[tmp + 9] = 0;
						newBlock[tmp + 10] = 0;
						newBlock[tmp + 11] = 0;

						newBlock[firstFree + oldSize - 1] = newBlock[firstFree + oldSize - 1] | 1;

						return newBlock + (&newBlock[firstFree] - &newBlock[0]) + 4;
					}

					int next, prev;
					next = to_int(&newBlock[firstFree + 4]);
					//next = newBlock[firstFree + 4] * 16777216 + newBlock[firstFree + 5] * 65536 + newBlock[firstFree + 6] * 256 + newBlock[firstFree + 7];
					prev  = to_int(&newBlock[firstFree + 8]);
					//prev = newBlock[firstFree + 8] * 16777216 + newBlock[firstFree + 9] * 65536 + newBlock[firstFree + 10] * 256 + newBlock[firstFree + 11];

					to_byte(&newBlock[next + 8], prev, 0);
					/*newBlock[next + 8] = prev >> 24;
					newBlock[next + 9] = prev >> 16 & 0xff;
					newBlock[next + 10] = prev >> 8 & 0xff;
					newBlock[next + 11] = prev & 0xff;*/

					to_byte(&newBlock[prev + 4], next, 0);
					/*newBlock[prev + 4] = next >> 24;
					newBlock[prev + 5] = next >> 16 & 0xff;
					newBlock[prev + 6] = next >> 8 & 0xff;
					newBlock[prev + 7] = next & 0xff;*/

					if (next == firstFree)//?????????????????????????????
					{
						//printf("1111111111111111111\n");
						newBlock[firstFree + 4] = 0;//ptr next
						newBlock[firstFree + 5] = 0;
						newBlock[firstFree + 6] = 0;
						newBlock[firstFree + 7] = 0;
					}
					
					newBlock[firstFree + oldSize - 1] = newBlock[firstFree + oldSize - 1] | 1;
					return newBlock + (&newBlock[firstFree] - &newBlock[0]) + 4;
				}
			}
	}
	else return NULL;
}

int memory_free(void *valid_ptr)
{
	if ((valid_ptr == NULL) || (memory_check(valid_ptr) == 1))
		return 1;
	unsigned char *buf = valid_ptr;
	unsigned char *newFreeBlock = buf - 4;
	unsigned char *block = memory;
	int sizeNext;
	int sizePrev;
	int newSize;
	int size;

	newFreeBlock[3] &= 0xfe;
	
	size = to_int(&newFreeBlock[0]);

	

	if ((block[0] + block[1] + block[2] + block[3]) == 0) //free first
	{
		newFreeBlock[size - 1] &= 0xfe;
		to_byte(&block[0], (newFreeBlock - block), 0);

		newFreeBlock[4] = 0;
		newFreeBlock[5] = 0;
		newFreeBlock[6] = 0;
		newFreeBlock[7] = 0;

		newFreeBlock[8] = 0;
		newFreeBlock[9] = 0;
		newFreeBlock[10] = 0;
		newFreeBlock[11] = 0;

		return 0;
	}
	/////////////////////////////////////////////////////////////
	/*newFreeBlock[4] = block[0];//ptr next
	newFreeBlock[5] = block[1];
	newFreeBlock[6] = block[2];
	newFreeBlock[7] = block[3];

	int next, prev;
	next = to_int(&block[0]);
	prev = &newFreeBlock[0] - &block[0];

	to_byte(&block[next + 8], prev, 0);

	newFreeBlock[8] = 0;//ptr prev
	newFreeBlock[9] = 0;
	newFreeBlock[10] = 0;
	newFreeBlock[11] = 0;

	to_byte(&block[0], prev, 0);

	newFreeBlock[size - 1] &= 0xfe;
	return 0;*/
	/////////////////////////////////////////////////////////////

	if ((&newFreeBlock[0] == &block[4]))//if free first block
	{
		//printf("free first (dont look left)\n");
		//if right is free
		sizeNext = to_int(&newFreeBlock[size]);
		if ((sizeNext % 2) == 0)
		{
			//printf("free first and right is free\n");
			newSize = size + sizeNext;

			to_byte(&newFreeBlock[0], newSize, 0);//header
			
			newFreeBlock[4] = newFreeBlock[size + 4];//ptr next
			newFreeBlock[5] = newFreeBlock[size + 5];
			newFreeBlock[6] = newFreeBlock[size + 6];
			newFreeBlock[7] = newFreeBlock[size + 7];

			newFreeBlock[8] = newFreeBlock[size + 8];//ptr prev
			newFreeBlock[9] = newFreeBlock[size + 9];
			newFreeBlock[10] = newFreeBlock[size + 10];
			newFreeBlock[11] = newFreeBlock[size + 11];

			int next, prev;
			next = to_int(&newFreeBlock[size + 4]);
			prev = to_int(&newFreeBlock[size + 8]);

			if (next != 0) {
				to_byte(&block[next + 8], (&newFreeBlock[0] - &block[0]), 0);
			}
			if (prev != 0) {
				to_byte(&block[prev + 4], (&newFreeBlock[0] - &block[0]), 0);
			}
			else {
				to_byte(&block[0], (&newFreeBlock[0] - &block[0]), 0);
			}
			to_byte(&newFreeBlock[newSize - 4], newSize, 0); //footer
			return 0;
			
		}
		else
		{
			//printf("free first and right isnt free\n");
			newFreeBlock[size - 1] &= 0xfe;

			int next, prev;
			next = to_int(&block[0]);
			
			to_byte(&block[next + 8], (&newFreeBlock[0] - &block[0]), 0);

			newFreeBlock[4] = block[0];//ptr next
			newFreeBlock[5] = block[1];
			newFreeBlock[6] = block[2];
			newFreeBlock[7] = block[3];

			newFreeBlock[8] = 0;//ptr prev
			newFreeBlock[9] = 0;
			newFreeBlock[10] = 0;
			newFreeBlock[11] = 0;
			
			block[0] = 0;
			block[1] = 0;
			block[2] = 0;
			block[3] = 4;

			return 0;
		}
	}
	else if (block[newFreeBlock - block - 1] % 2 == 1)// if left is not free
	{
		//printf(" if left is not free\n");
		if (newFreeBlock[size + 3] % 2 == 1)// if right is not free
		{
			newFreeBlock[4] = block[0];//ptr next
			newFreeBlock[5] = block[1];
			newFreeBlock[6] = block[2];
			newFreeBlock[7] = block[3];

			int next, prev;
			next = to_int(&block[0]);
			prev = &newFreeBlock[0] - &block[0];

			to_byte(&block[next + 8], prev, 0);

			newFreeBlock[8] = 0;//ptr prev
			newFreeBlock[9] = 0;
			newFreeBlock[10] = 0;
			newFreeBlock[11] = 0;

			to_byte(&block[0], prev, 0);
			/*block[0] = block[next + 8];
			block[1] = block[next + 9];
			block[2] = block[next + 10];
			block[3] = block[next + 11];*/

			newFreeBlock[size - 1] &= 0xfe;
			return 0;
		}
		else if (newFreeBlock[size + 3] % 2 == 0)// if right is free
		{

			//printf(" if left is not free and right is free\n");
			newFreeBlock[size - 1] &= 0xfe;//!!!!!!!!!!!!!!!!!!!!!
			sizeNext = to_int(&newFreeBlock[size]);
			size += sizeNext;
			//newFreeBlock[size - 1] &= 0xfe;//!!!!!!!!!!!!!!!!!!!
			to_byte(&newFreeBlock[0], size, 0);

			int next, prev;
			next = to_int(&newFreeBlock[size - sizeNext + 4]);
			prev = to_int(&newFreeBlock[size - sizeNext + 8]);

			//rewrite ptr next and prev
			to_byte(&newFreeBlock[4], next, 0);
			if (next != 0)
			{
				to_byte(&block[next + 8], (&newFreeBlock[0] - &block[0]), 0);
			}
			//else {
			//	block[next + 8] = newfreeblock[size - sizenext + 8];
			//	block[next + 9] = newfreeblock[size - sizenext + 9];
			//	block[next + 10] = newfreeblock[size - sizenext + 10];
			//	block[next + 11] = newfreeblock[size - sizenext + 11];
			//} 
			//!!!!!!!!!
			if (prev != 0)
			{
				to_byte(&newFreeBlock[8], prev, 0);
				to_byte(&block[prev + 4], (&newFreeBlock[0] - &block[0]), 0);
			}
			else
			{
				newFreeBlock[8] = 0;
				newFreeBlock[9] = 0;
				newFreeBlock[10] = 0;
				newFreeBlock[11] = 0;

				to_byte(&block[0], (&newFreeBlock[0] - &block[0]), 0);
			}
			to_byte(&block[(newFreeBlock - block) + (size - 4)], size, 0);//footer
			return 0;
		}
	}
	else if (block[newFreeBlock - block - 1] % 2 == 0)//if left is free
	{
		//printf(" if left is free\n");
		if (newFreeBlock[size + 3] % 2 == 1)// if right is not free
		{
			//////////////////////////////////////////////////////
			newFreeBlock[4] = block[0];//ptr next
			newFreeBlock[5] = block[1];
			newFreeBlock[6] = block[2];
			newFreeBlock[7] = block[3];

			int next, prev;
			next = to_int(&block[0]);
			prev = &newFreeBlock[0] - &block[0];

			to_byte(&block[next + 8], prev, 0);

			newFreeBlock[8] = 0;//ptr prev
			newFreeBlock[9] = 0;
			newFreeBlock[10] = 0;
			newFreeBlock[11] = 0;

			to_byte(&block[0], prev, 0);

			newFreeBlock[size - 1] &= 0xfe;
			return 0;
			//////////////////////////////////////////////////////
			//printf(" if left is free and right is not free\n");
			/*newFreeBlock[size - 1] &= 0xfe;
			sizePrev = to_int(&block[newFreeBlock - block - 4]);

			unsigned char* leftBlock = newFreeBlock - sizePrev;
			//unsigned char* leftBlock = newFreeBlock - block - 4;
			newSize = size + sizePrev;

			//rewrite size in left header
			to_byte(&leftBlock[0], newSize, 0);
			
			//rewrite size in footer
			to_byte(&newFreeBlock[size - 4], newSize, 0);

			return 0;*/
		}
	}
	if ((block[newFreeBlock - block - 1] % 2 == 0) && (newFreeBlock[size + 3] % 2 == 0))//if left and right  free 
	{
		//printf(" if left and right free !\n");
		newFreeBlock[size - 1] &= 0xfe;
		sizePrev = to_int(&block[newFreeBlock - block - 4]);
		sizeNext = to_int(&newFreeBlock[size]);
		newSize = size + sizePrev + sizeNext;

		unsigned char* leftBlock = newFreeBlock - sizePrev;
		unsigned char* rightBlock = newFreeBlock + size;

		//rewrite size in left header
		to_byte(&leftBlock[0], newSize, 0);

		int next, prev;
		next = to_int(&rightBlock[4]);
		prev = to_int(&rightBlock[8]);

		if (next != 0)
		{
			to_byte(&block[next + 8], prev, 0);
		}
		if (prev != 0)
		{
			to_byte(&block[prev + 4], next, 0);
		}
		else 
		{
			to_byte(&block[0], next, 0);
		}
		to_byte(&rightBlock[sizeNext - 4], newSize, 0);//footer
		return 0;
	}
	else return 1;

	return 0;
}

int memory_check(void *ptr)
{
	unsigned char* buf = ptr;
	unsigned char* checkBlock = buf - 4;
	int size = to_int(&checkBlock[0]);
	if((checkBlock[3] % 2 == 1) && ((to_int(&checkBlock)) == (to_int(&checkBlock[size - 5]))))
	//if (checkBlock[3] % 2 == 1)
	{
		return 0;
	}
	else return 1;
}

void memory_init(void *ptr, unsigned int size)
{
	if (size < 16)
		return;

	memory = ptr;
	unsigned char *array_char = memory;
	if (size % 2 == 1)
		size--;
	size -= 8;

	array_char[0] = 0;  //pointer na prvy slobodny block
	array_char[1] = 0;
	array_char[2] = 0;
	array_char[3] = 4;

	to_byte(&array_char[4], size , 0); //header
	/*array_char[4] = (size - 8) >> 24; //header
	array_char[5] = (size - 8) >> 16 & 0xff;
	array_char[6] = (size - 8) >> 8 & 0xff;
	array_char[7] = (size - 8) & 0xff;*/

	array_char[8] = 0;//ptr next
	array_char[9] = 0;
	array_char[10] = 0;
	array_char[11] = 0;

	array_char[12] = 0;//ptr prev
	array_char[13] = 0;
	array_char[14] = 0;
	array_char[15] = 0;

	to_byte(&array_char[size], size, 0);//footer
	/*array_char[size - 8] = (size - 8) >> 24;//footer
	array_char[size - 7] = (size - 8) >> 16 & 0xff;
	array_char[size - 6] = (size - 8) >> 8 & 0xff;
	array_char[size - 5] = (size - 8) & 0xff;*/

	
	array_char[size + 4] = 1;//finish of memory
	array_char[size + 5] = 1;
	array_char[size + 6] = 1;
	array_char[size + 7] = 1;

	/*int i;
	for (i = 0; i < 100; i++)
	{
		printf("[%d]  %d\n", i, array_char[i]);
	}*/
}

typedef struct block {
	unsigned char *ptr_mem;
	int size;
}block;

int test()
{
	

	printf("Choose scenar(1, 2, 3 or 4)\n");
	printf("1 - Control intersecting blocks\n");
	printf("2 - allocation of small blocks (8 bytes) of 1 000 bytes\n");
	printf("3 - allocation of big blocks (500 bytes) of 50 000 bytes\n");
	printf("4 - marger of 3 blocks\n");
	
	int n;
	scanf("%d", &n);
	if (n == 1)
	{
		char region[200];
		memory_init(region, 200);

		block *ptr_arr[200];
		unsigned char *mem = memory;
		time_t t;
		srand((unsigned)time(&t));

		for (int i = 0; i < 200; i++)
		{
			ptr_arr[i] = NULL;
		}

		for (int i = 0; i < 20; i++)
		{

			for (int j = 0; j < 20; j++)
			{
				ptr_arr[j] = (block*)malloc(sizeof(block));

				int r = (rand() % 200) + 1;
				ptr_arr[j]->ptr_mem = (char*)memory_alloc(r);
				ptr_arr[j]->size = r;
				//printf("r = %d\n", r);
				if (ptr_arr[j]->ptr_mem != NULL)
					memset((ptr_arr[j]->ptr_mem), 222, r);

			}
			for (int j = 0; j < 20; j++)
			{
				if (ptr_arr[j]->ptr_mem != NULL)
				{
					unsigned char *block_mem = (ptr_arr[j]->ptr_mem) - 4;
					for (int k = 4; k < ptr_arr[j]->size + 4; k++)
					{
						if (block_mem[k] != 222)
						{
							printf("ERROR #1 \n");
							return 1;
						}
					}
				}
			}
			for (int j = 0; j < 20; j++)
			{

				int r = (rand() % 20);
				if (ptr_arr[r]->ptr_mem != NULL)
					memory_free(ptr_arr[r]->ptr_mem);
			}

			/*for (int i = 0; i < 200; i++)
			{
				printf("[%d]  %d\n", i, mem[i]);
			}
			system("pause");*/

		}
	}
	else if (n == 2)
	{
		char region[1000];
		memory_init(region, 1000);

		unsigned char *ptr_arr[1000];
		unsigned char *mem = memory;

		for (int i = 0; i < 1000; i++)
		{
			ptr_arr[i] = NULL;
		}
		for (int i = 0; i < 1000; i++)
		{
			ptr_arr[i] = (char*)memory_alloc(8);
			
			if (to_int(&mem[0]) == 0)
			{
				printf("%d   8-bajt blocks were allocated from 1000 bajt\n", i); 
				system("pause");
				return 0;
			}

		}
	}
	else if (n == 3)
	{
		char region[50000];
		memory_init(region, 50000);
		unsigned char *ptr_arr[50000];
		unsigned char *mem = memory;

		for (int i = 0; i < 50000; i++)
		{
			ptr_arr[i] = NULL;
		}
		for (int i = 0; i < 50000; i++)
		{
			//printf("%d\n", i);
			ptr_arr[i] = (char*)memory_alloc(500);
			/*for (int j = 0; j < 20; j++)
			{
				printf("[**%d]  %d\n", j, mem[j]);
			}
			system("pause");*/
			if (ptr_arr[i] == NULL)
			{
				printf("%d   500-bajt blocks were allocated from 50 000 bajt\n", i);
				system("pause");
				return 0;
			}

		}
	}
	else if (n == 4) 
	{
		char region[100];
		memory_init(region, 100);
		unsigned char *mem = memory;
		char* pointer = (char*)memory_alloc(10);
		if (pointer)
			memset(pointer, 77, 10);
		char* pointer1 = (char*)memory_alloc(10);
		if (pointer1)
			memset(pointer1, 77, 10);
		char* pointer2 = (char*)memory_alloc(42);
		if (pointer2)
			memset(pointer2, 77, 42);
		int i;
		for (i = 0; i < 100; i++)
		{
			printf("[*%d]  %d\n", i, mem[i]);
		}
		if (pointer1)
			memory_free(pointer1);
		for (i = 0; i < 100; i++)
		{
			printf("[**%d]  %d\n", i, mem[i]);
		}
		system("pause");
		if (pointer2)
			memory_free(pointer2);
		for (i = 0; i < 100; i++)
		{
			printf("[**%d]  %d\n", i, mem[i]);
		}
		system("pause");
		if (pointer)
			memory_free(pointer);
		for (i = 0; i < 100; i++)
		{
			printf("[**%d]  %d\n", i, mem[i]);
		}
		system("pause");
		
		pointer = (char*)memory_alloc(70);
		if (pointer)
			memset(pointer, 77, 42);
		for (i = 0; i < 100; i++)
		{
			printf("[**%d]  %d\n", i, mem[i]);
		}
		system("pause");
	}
	
}

// Vlastna funkcia main() je pre vase osobne testovanie. Dolezite: pri testovacich scenaroch sa nebude spustat!
int main()
{
	char region[50000];
	memory_init(region, 50000);
	if (test() == 1)
	{
		printf("ERROR: Prekryvajuce sa bloky\n");
		system("pause");
		return 0;
	}
	else 
	{
		printf("TEST OK\n");
		system("pause");
		return 0;
	}
	/*unsigned char *reg = region;
	char* pointer = (char*)memory_alloc(10);
	if (pointer)
		memset(pointer, 77, 10);
	char* pointer1 = (char*)memory_alloc(10);
	if (pointer1)
		memset(pointer1, 77, 10);
	char* pointer2 = (char*)memory_alloc(42);
	if (pointer2)
		memset(pointer2, 77, 42);
	int i;
	for (i = 0; i < 100; i++)
	{
		printf("[*%d]  %d\n", i, reg[i]);
	}
	if (pointer1)
		memory_free(pointer1);
	for (i = 0; i < 100; i++)
	{
		printf("[**%d]  %d\n", i, reg[i]);
	}
	system("pause");
	if (pointer2)
		memory_free(pointer2);
	for (i = 0; i < 100; i++)
	{
		printf("[**%d]  %d\n", i, reg[i]);
	}
	system("pause");
	if (pointer)
		memory_free(pointer);
	for (i = 0; i < 100; i++)
	{
		printf("[**%d]  %d\n", i, reg[i]);
	}
	system("pause");*/
	return 0;
}