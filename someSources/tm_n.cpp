
# include <stdio.h>
# include <stdlib.h>


void autonoma( void) {
	/*
		senza dichiarazione dati in C
		alloca un vettore di scalari-32bit
		lo valorizza elemento a elemento
		lo dealloca
	*/

	__asm {
		mov  ebx, 4  // size_of_each_scalar  // size
		push ebx
		mov  ebx, 5  // vector_cardinality   // num
		push ebx
		// allocation
		call calloc  // num==vector_cardinality, size==size_of_each_scalar
		// in eax is now contained the void* allocated
		// ci sara' ancora dopo le pop
		pop ebx
		pop ebx

		// scorrimento
		mov ebx, eax // take the address where the vector starts
		mov ecx, 5 // LENGTH lv   // equivale a: mov ecx, card(i.e. vector cardinality)
		xor esi, esi // clear ESI to let it the pointer to the current scalar within the vector
		// kordell
		TheLoop:
			mov eax, [ebx+esi] // ram->cpu
				inc eax // alterazione contenuto dello scalare
			mov [ebx+esi], eax // cpu->ram
			add esi, TYPE long // current_ptr += sizeof(long)
		loop TheLoop

		// free
		push ebx
		call free
		pop ebx
	}// END __asm

}// END "autonoma"


void * callocCaller( size_t vector_cardinality, size_t size_of_each_scalar) {
	void * s = NULL;

	__asm {
		mov  ebx, size_of_each_scalar  // size
		push ebx
		mov  ebx, vector_cardinality   // num
		push ebx

		call calloc  // num==vector_cardinality, size==size_of_each_scalar
		// in eax is now contained the void* allocated
		mov s, eax

		pop ebx
		pop ebx
	}

	return s;
}// END: void callocCaller( void) {

void freeCaller( void * gptr) {

	__asm {
		mov  ebx, gptr
		push ebx

		call free

		pop ebx
	}
}



void array( void) {// versione obsoleta: usa un pointer al posto di LEA==load effective address
	const long card = 3;
	long lv[card] = { 1, 2, 3};
	long * lvp = &(lv[0]);// pointer utilizzato al posto di LEA

	__asm {
		mov ebx, lvp // take the address where the vector starts
		mov ecx, LENGTH lv   // equivale a: mov ecx, card(i.e. vector cardinality)
		xor esi, esi // clear ESI to let it the pointer to the current scalar within the vector

		TheLoop:
			mov eax, [ebx+esi] // ram->cpu
				inc eax // alterazione contenuto dello scalare
			mov [ebx+esi], eax // cpu->ram
			add esi, TYPE long // current_ptr += sizeof(long)
		loop TheLoop
	}
}// end function array


void printfCaller( void) {
	char format[] = "%s %s \n";
	char * fp = &(format[0]);
	char h[]	  = "Hallo";
	char * hp = &(h[0]);
	char w[]	  = "World";
	char * wp = &(w[0]);

	long lv[] = {0, 1};

   __asm  {
	    mov eax, wp
		push eax
	    mov eax, hp
		push eax
	    mov eax, fp
		push eax

		call printf

		pop eax
		pop eax
		pop eax
   }
}


const long card = 3;
long lv[card] = { 1, 2, 3};
void array1( void) {// versione giusta: usa LEA==load effective address

	__asm {
		lea ebx, lv // take the address where the vector starts
		mov ecx, LENGTH lv   // equivale a: mov ecx, card(i.e. vector cardinality)
		xor esi, esi // clear ESI to let it the pointer to the current scalar within the vector

		TheLoop:
			mov eax, [ebx+esi] // ram->cpu
				inc eax // alterazione contenuto dello scalare
			mov [ebx+esi], eax // cpu->ram
			add esi, TYPE long // current_ptr += sizeof(long)
		loop TheLoop
	}
}// end function array


const long sa_card = 3;
struct TheStruct {
	long			b4;
	short			b2;
	unsigned long   b4_2;
	unsigned char	b1;
	unsigned long   b4_1;
};
TheStruct structArray[sa_card] = {{1,2,3,4,5}, {6,7,8,9,10}, {11,12,13,14,15}};
void struct_array( void) {// versione giusta: usa LEA==load effective address

	__asm {
		lea ebx, structArray	  // take the address where the vector starts
		mov ecx, LENGTH structArray   // equivale a: mov ecx, card(i.e. vector cardinality)
		xor esi, esi // clear ESI to let it the pointer to the current scalar within the vector

		TheLoop:
			// first field
			mov eax, [ebx+esi] // ram->cpu
				inc eax // alterazione contenuto dello scalare
			mov [ebx+esi], eax // cpu->ram

			// second field
			xor eax,eax
			mov ax, [ebx+esi+ TYPE long] // ram->cpu
				inc ax // alterazione contenuto dello scalare
			mov [ebx+esi+ TYPE long], ax // cpu->ram

			// third field
			xor eax,eax
			mov eax, [ebx+esi+ TYPE long + TYPE long] // ram->cpu
				inc eax // alterazione contenuto dello scalare
			mov [ebx+esi+ TYPE long + TYPE long], eax // cpu->ram

			// fourth field
			xor eax,eax
			mov al, [ebx+esi+ TYPE long + TYPE long + TYPE long] // ram->cpu
				inc al // alterazione contenuto dello scalare
			mov [ebx+esi+ TYPE long + TYPE long + TYPE long], al // cpu->ram

			// fifth field
			xor eax,eax
			mov eax, [ebx+esi+ TYPE long + TYPE long+ TYPE long + TYPE long] // ram->cpu
				inc eax // alterazione contenuto dello scalare
			mov [ebx+esi+ TYPE long + TYPE long+ TYPE long + TYPE long], eax // cpu->ram

			add esi, TYPE TheStruct // current_ptr += sizeof(TheStruct)
		loop TheLoop
	}

}// end function struct_array



void struct_array1( void) {// versione giusta: usa LEA==load effective address

	for( int c=0; c<sa_card; c++ ) {
		structArray[c].b4++;
		structArray[c].b2++;
		structArray[c].b4_2++;
		structArray[c].b1++;
		structArray[c].b4_1++;
	}
}// end function struct_array1


int main (void)
{
	struct_array();
	struct_array1();

	return 0;
}
