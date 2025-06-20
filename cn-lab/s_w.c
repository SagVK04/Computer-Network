#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
int main(){
    int n,ch,i=1;
    int recv(int);
    printf("enter number of frames:");
    scanf("%d",&n);
    while(i<=n){
        printf("Sender : sending packet no %d\n",i);
        ch=recv(i);
        switch (ch){
            case 1:
            printf("Sender : acknowledgement received\n");
            i++;
            break;
            case 2:
            printf("Sender : no acknowledgement received\n");
            printf("resend.....\n");
            break;
            case 3:
            printf("Sender : currupted acknowledgement received\n");
            printf("resend.....\n");
            break;
            default:
            printf("error in code\n");
            break;
        }
    }getch();

}
int recv(int p){
    int k;
    k=rand()%3+1;
    switch(k){
        case 1:
        printf("received packet no %d\n",p);
        return 1;
        break;
        case 2:
        return 2;
        break;
        case 3:
        printf("received packet no %d\n",p);
        return 3;
        break;
        default:
        return 4;
        
    }
}