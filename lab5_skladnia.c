#include <stdio.h>
#include <stdlib.h>
void* routine(void*);

//definicja struktury 1
struct thread_dataSTRUCT {
	int cnt;
	int sum;
	const char* msg;
    double data_out;
};

//definicja struktury 2 (typedef)
typedef struct {
	int cnt;
	int sum;
	const char* msg;
    double data_out;
} thread_data;


int main() {
	// obie opcje generuja dokladnie to samo
	struct thread_dataSTRUCT struktura1;	// opcja 1
	thread_data struktura2;					// opcja 2

	int tab[10], i;
	int* ptr;
	for (i = 0; i < 10; i++){
		// dwa identyczne wyrazenia:
		tab[i] = i;      // <-- dostep do tablicy przez operator []
		// *(tab+i) = i; // <-- dostep do tablicy przez operator *
	}

	i = 5;			// przyjete arbitralnie
	// dwie opcje na pobranie adresu elementu tablicy:
	ptr = &tab[i];	// <-- wybierz i-ty element tablicy 'tab' i wyłuskaj jego adres
	/* nazwa tablicy jest wskaznikiem na jej pierwszy element, dlatego taki zapis jest rowniez ok */
	ptr = tab + i;  // <-- do adresu 'tab' dodaj i * sizeof(int) i zwróć adres tego miejsca.

	printf("ptr wskazuje na element o wartosci %d \n", *ptr);
	printf("dlugosc jednego int'a w tablicy: %d bajty \n", sizeof(int));
	printf("tab+%d przemieszcza sie o %d bajtow w tablicy tab \n", i, i*sizeof(int));

	struktura2.cnt = 4;
	/*   rzutujemy adres struktury na typ void*   */
	void* pointer_to_struct = (void*) &struktura2;
	routine(pointer_to_struct);
    
    /* Odczytaj wartosc obliczona w watku */
    printf("Odebralem wartosc (3*cnt+2): %f", struktura2.data_out);

	return EXIT_SUCCESS;
}


void* routine(void* args) {
	/*   rzutowanie adresu z typu void* na thread_data*   */
	thread_data* args_converted = (thread_data*) args;
	int cnt = args_converted->cnt;
	    cnt = (*args_converted).cnt; // dokladnie to samo, co powyzej
    
    args_converted->data_out = 3*cnt + 2; // Zwroc wartosc do procesu glownego
	printf("routine: cnt = %d\n", cnt);

	return NULL;
}
