#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#ifndef BUFFER
#define BUFFER 1024
#endif

void escribirMensaje(int tub[],char cadena[]);
void leerMensaje(int tub[],char cadena[]);
int cantLetras(char cadena[]);
int cantNumeros(char cadena[]);
int cantPuntosYComas(char cadena[]);


int main(){
	pid_t hijos[3], padre;
    int tub1[2],tub2[2],tub3[2];
    char cadena[BUFFER];
    padre=getpid();
    pipe(tub1);pipe(tub2);pipe(tub3);


    for(int i=0;i<3;i++){
        if(!(hijos[i]=fork()))
            break;
    }

    if(padre==getpid()){
    	int i=0;
    	FILE *txt=fopen("archivo.txt","r");
	    if(txt==NULL){
	        printf("Error al abrir el archivo\n");
	    }else{
	        while(!feof(txt)){
	           	cadena[i] = fgetc(txt);
	            i++;
	       	}
	    }
        cadena[i]='\0';
        fclose(txt);
        escribirMensaje(tub1,cadena);
        escribirMensaje(tub2,cadena);
        escribirMensaje(tub3,cadena);
        for (int i = 0; i < 3; i++){
        	wait(NULL);
        }
    }else{
    	if(hijos[0]==0){
    		leerMensaje(tub1,&cadena);
    		int n=cantLetras(cadena);
    		printf("El arcihvo tiene: %d letras \n", n);
    	}else{
    		if (hijos[1]==0){
    			leerMensaje(tub2,&cadena);
    			int n=cantNumeros(cadena);
    			printf("El arcihvo tiene: %d numeros \n", n);
    		}else{
    			if(hijos[2]==0){
					leerMensaje(tub3,&cadena);
					int n=cantPuntosYComas(cadena);
    				printf("El arcihvo tiene: %d puntos o comas \n", n);
    			}
    		}	
    	}
    }
    return 0;
}


void escribirMensaje(int tub[],char cadena[]){
	close(tub[0]);
	write(tub[1], cadena, BUFFER);
    close(tub[1]);
}

void leerMensaje(int tub[],char cadena[]){
	close(tub[1]);
	int n=read(tub[0], cadena, BUFFER);
	cadena[n]='\0';
    close(tub[0]);
}

int cantLetras(char cadena[]){
	int i=0, cont=0;
	while(cadena[i]!='\0'){
		if(cadena[i]>=65 && cadena[i]<=90 || cadena[i]>=97 && cadena[i]<=122){
			cont++;	
		}
		i++;
	}
	return cont;

}

int cantNumeros(char cadena[]){
	int i=0, cont=0;
	while(cadena[i]!='\0'){
		if(cadena[i]>=48 && cadena[i]<=57){
			cont++;	
		}
		i++;
	}
	return cont;

}

int cantPuntosYComas(char cadena[]){
	int i=0, cont=0;
	while(cadena[i]!='\0'){
		if(cadena[i]==46 || cadena[i]==44){
			cont++;	
		}
		i++;
	}
	return cont;
}