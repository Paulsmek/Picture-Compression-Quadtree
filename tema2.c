#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "arbore.h"
//Lavric Paul 312CD

//functie de create nod din arbore
Nod_arbore * create_nod (int x, int y, unsigned int width, unsigned char red, unsigned char green, unsigned char blue, unsigned char type) {
    Nod_arbore * arbore = (Nod_arbore*) malloc(sizeof(Nod_arbore));
    arbore -> width = width;
    arbore -> height = width;
    arbore -> x = x;
    arbore -> y = y; 
    arbore -> type = type;
    arbore -> culoare.red = red;
    arbore -> culoare.green = green;
    arbore -> culoare.blue = blue;
    arbore -> top_left = NULL;
    arbore -> top_right = NULL;
    arbore -> bottom_right = NULL;
    arbore -> bottom_left = NULL;
    return arbore; 
} 

//functia de creat arborele 
Nod_arbore * create_arbore (int x, int y, unsigned int width, unsigned int width_total, int factor, int iteratii, Pixel *image) {
    //calculez mean scorul pentru partea din imagine pe care o memorez sau o mai sparg in 4
    unsigned long long red = 0, green = 0, blue = 0, mean = 0;
    for (int i = x; i < x + width; i++) {
        for (int j = y; j < y + width; j++) {
            red += image[(i * width_total + j)].red;
            green += image[(i * width_total + j)].green;
            blue += image[(i * width_total + j)].blue;
    
        }
            
    }
   
    red = floor(red / (width * width));
    green = floor(green / (width * width));
    blue = floor(blue / (width * width));
    for (int i = x; i < x + width; i++) {
        for (int j = y; j < y + width; j++) {
            mean += (red - image[(i * width_total + j)].red) * (red - image[(i * width_total + j)].red);
            mean += (green - image[(i * width_total + j)].green) * (green - image[(i * width_total + j)].green);
            mean += (blue - image[(i * width_total + j)].blue) * (blue - image[(i * width_total + j)].blue);

        }
             
    }  
    mean = mean / (3 * width * width);
    if (floor(mean) <= factor) { 
        return create_nod(x, y, width, red, green, blue, 0);//arborele nu se mai separa din acest punct
    }
    else {
        //separare arbore
        Nod_arbore *top_left = create_arbore(x, y, width/2, width_total, factor, iteratii + 1, image);
        Nod_arbore *top_right = create_arbore(x, y + width/2, width/2, width_total, factor, iteratii + 1, image);
        Nod_arbore *bottom_right = create_arbore(x + width/2, y + width/2, width/2, width_total, factor, iteratii + 1, image);
        Nod_arbore *bottom_left = create_arbore(x + width/2, y, width/2, width_total, factor, iteratii + 1, image);
        Nod_arbore *node = create_nod(x,y,width,red,green,blue,0);
        node -> top_left = top_left;
        node -> top_right = top_right;
        node -> bottom_right = bottom_right;
        node -> bottom_left = bottom_left;
        return node;

    }
}

//functie de maxim
int max(int a, int b) {
    if(a > b) return a;
    return b;
}

//functie de adancime maxima a arborelui
int maxDepth (Nod_arbore * arbore) {
    if (arbore == NULL)
        return 0;
    else {
       
        int t_lDepth = maxDepth(arbore->top_left);
        int t_rDepth = maxDepth(arbore->top_right);
        int b_lDepth = maxDepth(arbore->bottom_left);
        int b_rDepth = maxDepth(arbore->bottom_right);
        return max(max(t_lDepth, t_rDepth), max(b_lDepth, b_rDepth)) + 1;
    }
}

//functie de calcul a nodurilor terminale (task 1)
int noduri_finale(Nod_arbore * arbore) {
    if(arbore == NULL) return 0;
    if(arbore -> top_left == NULL && arbore -> top_right == NULL && arbore -> bottom_left == NULL && arbore -> bottom_right == NULL) return 1;
    else return noduri_finale(arbore -> top_left) + noduri_finale(arbore -> top_right) + noduri_finale(arbore -> bottom_left) + noduri_finale(arbore -> bottom_right);
}

//functie de calcul al latimii maxime (width)
int maxim_arbore(Nod_arbore * arbore){
    if(arbore -> top_left == NULL && arbore -> top_right == NULL && arbore -> bottom_left == NULL && arbore -> bottom_right == NULL) return arbore -> width;
    return max(max(maxim_arbore(arbore -> top_left), maxim_arbore(arbore -> top_right)), max(maxim_arbore(arbore -> bottom_left), maxim_arbore(arbore -> bottom_right)));
}

//print pentru taskul 2 in functie de adancimea nodurilor (iteratia_buna)
void print2(Nod_arbore* arbore, int iteratia_buna, int iteratie_actuala, FILE *output) {
    if(arbore == NULL) return ;
    if(iteratia_buna == iteratie_actuala) {
    if(arbore -> top_left != NULL && arbore -> top_right != NULL && arbore -> bottom_left != NULL && arbore -> bottom_right != NULL){
        unsigned char tip = 0;
        fwrite(&tip, sizeof(unsigned char), 1, output);
    }
    else {
        unsigned char tip = 1;
        fwrite(&tip, sizeof(unsigned char), 1, output);
        fwrite(&arbore -> culoare.red, sizeof(unsigned char), 1, output);
        fwrite(&arbore -> culoare.green, sizeof(unsigned char), 1, output);
        fwrite(&arbore -> culoare.blue, sizeof(unsigned char), 1, output);
    }
    }
    else {
        print2(arbore -> top_left, iteratia_buna, iteratie_actuala + 1, output);
        print2(arbore -> top_right, iteratia_buna, iteratie_actuala + 1, output);
        print2(arbore -> bottom_right, iteratia_buna, iteratie_actuala + 1, output);
        print2(arbore -> bottom_left, iteratia_buna, iteratie_actuala + 1, output);
    }
}

//functie care rezolva task 2 
void solve2(Nod_arbore * arbore, int maxim, FILE * output){
    int iter = 0;
    while (iter < maxim) {
        print2(arbore, iter, 0, output);
        iter ++;
    }
    
}

//functie de free arbore
void eliberare(Nod_arbore *arbore){
    if(arbore != NULL) {
        eliberare(arbore -> top_left);
        eliberare(arbore -> top_right);
        eliberare(arbore -> bottom_left);
        eliberare(arbore -> bottom_right);
        free(arbore);
    }
}

//functie ce creeaza arborele pentru task 3
Nod_arbore * create_arbore3(unsigned int width, FILE *file_handle){
   unsigned char type;
   int nr_noduri = 1; 
   Nod_arbore* arbore = create_nod(0, 0, width, 1, 0, 0, 0);
   TQueue *coada = create_queue(arbore); //coada pentru noduri
   fread(&type, sizeof(unsigned char), 1, file_handle);
   if (type == 0) ;
   else {
        unsigned char red,green,blue;
        fread(&red, sizeof(unsigned char), 1, file_handle);
        fread(&green, sizeof(unsigned char), 1, file_handle);
        fread(&blue, sizeof(unsigned char), 1, file_handle);
        while(coada -> head != NULL){
        coada = dequeue(coada);
        }
        free(coada);
        free(arbore);
        return create_nod(0, 0, width, red, green, blue, 1);
   }
   Nod_arbore * iter; 
   //pentru fiecare nod neterminal, adica unul care se afla in coada, citesc si memorez datele "copiilor"
   while (nr_noduri > 0){
        iter = coada -> head -> arbore; 
        unsigned char type_tl, type_tr, type_br, type_bl;
        //top left
        fread(&type_tl, sizeof(unsigned char), 1, file_handle);
        if (type_tl == 0) {
            iter -> top_left = create_nod(iter -> x, iter -> y, iter->width / 2, 0, 0, 0, 0);
            coada = enqueue(coada, iter -> top_left);
            nr_noduri++;
        }
        else {
            unsigned char red,green,blue;
            fread(&red, sizeof(unsigned char), 1, file_handle);
            fread(&green, sizeof(unsigned char), 1, file_handle);
            fread(&blue, sizeof(unsigned char), 1, file_handle);
            iter -> top_left = create_nod(iter -> x, iter -> y, iter->width / 2, red, green, blue, 1);
        }
        //top right
        fread(&type_tr, sizeof(unsigned char), 1, file_handle);
        if (type_tr == 0) {
            iter -> top_right = create_nod(iter -> x, iter -> y + iter -> width / 2, iter->width / 2, 0, 0, 0, 0);
            coada = enqueue(coada, iter -> top_right);
            nr_noduri++;
        }
        else {
            unsigned char red,green,blue;
            fread(&red, sizeof(unsigned char), 1, file_handle);
            fread(&green, sizeof(unsigned char), 1, file_handle);
            fread(&blue, sizeof(unsigned char), 1, file_handle);
            iter -> top_right = create_nod(iter -> x, iter -> y + iter -> width / 2, iter->width / 2, red, green, blue, 1);
        }
        //bottom right
        fread(&type_br, sizeof(unsigned char), 1, file_handle);
        if (type_br == 0) {
            iter -> bottom_right = create_nod(iter -> x + iter -> width / 2, iter -> y + iter -> width / 2, iter->width / 2, 0, 0, 0, 0);
            coada = enqueue(coada, iter -> bottom_right);
            nr_noduri++;
        }
        else {
            unsigned char red,green,blue;
            fread(&red, sizeof(unsigned char), 1, file_handle);
            fread(&green, sizeof(unsigned char), 1, file_handle);
            fread(&blue, sizeof(unsigned char), 1, file_handle);
            iter -> bottom_right = create_nod(iter -> x + iter -> width / 2, iter -> y + iter -> width / 2, iter->width / 2, red, green, blue, 1);
        }
        //bottom left
        fread(&type_bl, sizeof(unsigned char), 1, file_handle);  
        if (type_bl == 0) {
            iter -> bottom_left = create_nod(iter -> x + iter -> width / 2, iter -> y, iter->width / 2, 0, 0, 0, 0);
            coada = enqueue(coada, iter -> bottom_left);
            nr_noduri++;
        }
        else {
            unsigned char red,green,blue;
            fread(&red, sizeof(unsigned char), 1, file_handle);
            fread(&green, sizeof(unsigned char), 1, file_handle);
            fread(&blue, sizeof(unsigned char), 1, file_handle);
            iter -> bottom_left = create_nod(iter -> x + iter -> width / 2, iter -> y, iter->width / 2, red, green, blue, 1);
        }

        nr_noduri--;
        coada = dequeue(coada); //urmatorul nod din coada
   }
   //free coada(daca mai are noduri)
   while (coada -> head != NULL){
    coada = dequeue(coada);
   }
   free(coada);

   
   return arbore;

}

//functie care creeaza imaginea in image
void create_image(Nod_arbore * arbore, Pixel *image, unsigned int width_total){
    if(arbore -> type == 1) {
        for(int i = arbore -> x; i< arbore -> x + arbore -> width; i++){
            for(int j = arbore -> y; j< arbore -> y + arbore -> width; j++){
               image[i * width_total + j] = arbore -> culoare;
            }
        } 
    }
    else {   
        create_image(arbore -> top_left, image, width_total);
        create_image(arbore -> top_right, image, width_total);
        create_image(arbore -> bottom_left, image, width_total);
        create_image(arbore -> bottom_right, image, width_total);
    }
}

int main(int argc, char const *argv[]) 
{   
    //declararea fisierelor si a factorului de compresie
    int factor;
    FILE *file_handle;
    FILE *output;
    if(strcmp(argv[1],"-c2") == 0 || strcmp(argv[1],"-c1") == 0){
        factor = atoi(argv[2]);
        file_handle = fopen(argv[3], "rb");
        output = fopen(argv[4], "w");
    }
    else {
        file_handle = fopen(argv[2], "rb");
        output = fopen(argv[3], "w");
    }
    if(!file_handle) {
        printf("eroare");
        return 1;
    }
    if(!output) {
        printf("eroare");
        return 1;
    }
    if(strcmp(argv[1],"-c1") == 0) {
    //citirea din ppm file
    unsigned char file_type[3];
    unsigned int width, height, max_val_colour;
    fscanf(file_handle,"%s\n", file_type);
    fscanf(file_handle,"%d ", &width);
    fscanf(file_handle,"%d\n", &height);
    fscanf(file_handle,"%d", &max_val_colour);
    fseek(file_handle, 1, SEEK_CUR);
    //citirea pixelilor 
    Pixel * image = (Pixel *) malloc(sizeof(Pixel) * width * width); 
    fread(image, sizeof(Pixel), width * width, file_handle);
    //creearea arborelui
    Nod_arbore *arbore = create_arbore(0, 0, width, width, factor, 1, image);
    fprintf(output, "%d\n", maxDepth(arbore));
    fprintf(output, "%d\n", noduri_finale(arbore));
    fprintf(output, "%d\n", maxim_arbore(arbore));
    eliberare(arbore);
    free(image);
    }
    if(strcmp(argv[1],"-c2") == 0){
    //aceeasi citire
    char file_type[3];
    unsigned int width, height, max_val_colour;
    fscanf(file_handle,"%s\n", file_type);
    fscanf(file_handle,"%d", &width);
    fscanf(file_handle,"%d\n", &height);
    fscanf(file_handle,"%d", &max_val_colour);
    fseek(file_handle, 1, SEEK_CUR);
    Pixel * image = (Pixel *) malloc(sizeof(Pixel) * width * width); 
    fread(image, sizeof(Pixel), width * width, file_handle);
    Nod_arbore *arbore = create_arbore(0, 0, width, width, factor, 1, image);
    //outputul
    fwrite(&width, sizeof(unsigned int), 1, output);
    solve2(arbore, maxDepth(arbore), output);
    eliberare(arbore);
    free(image);
    }
    if(strcmp(argv[1],"-d") == 0){
        unsigned int width;
        fread(&width, sizeof(unsigned int), 1, file_handle);
        Nod_arbore * arbore = create_arbore3(width, file_handle);
        Pixel * image = (Pixel *) malloc(sizeof(Pixel) * width * width);
        //creez imaginea 
        fprintf(output, "P6\n");
        fprintf(output, "%d %d\n", width, width);
        unsigned int max_val_colour = 255;
        fprintf(output,"%d\n", max_val_colour);
        create_image(arbore, image, width);
        for (int i = 0; i< width; i++) {
        for (int j = 0; j< width; j++){
            fwrite(&image[i * width + j].red, sizeof(unsigned char), 1, output);
            fwrite(&image[i * width + j].green, sizeof(unsigned char), 1, output);
            fwrite(&image[i * width + j].blue, sizeof(unsigned char), 1, output);
        }  
        }
        free(image);
        eliberare(arbore);
    }
    fclose(file_handle); 
    fclose(output);


    return 0;
}