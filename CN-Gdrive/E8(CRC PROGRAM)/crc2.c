#include<stdio.h>
#include<string.h>

#define N strlen(gen_poly)

char data[28];
char check_value[28];
char gen_poly[10];
int data_length,i,j;

void XOR(){
	for(j=1; j<N; j++){
		check_value[j] = ((check_value[j] == gen_poly[j])?'0':'1');
	}
}
void receiver(){
	printf("\nenter the received data:");
	scanf("%s",data);
	printf("\n-----------------------------\n");
	printf("data received: %s",data);
	crc();
	int error = 0;
	for(int i=0; i<N-1; i++){
		if(check_value[i] != '0'){
			error=1;
			break;
		}
	}
		if(error)	printf("\n Error detected \n\n");
		else	printf("\nNo error detected\n\n");
}

void crc(){
	for(i=0; i<N; i++)	check_value[i]=data[i];
	do{
		if(check_value[0]=='1')	XOR();
		for(j =0; j<N-1; j++)	check_value[j] = check_value[j+1];
		check_value[j]=data[i++];
	}while(i<=data_length+N-1);
}

int main(){
	printf("\nEnter data to be transmitted: ");
	scanf("%s",data);
	printf("\n Enter the generating polynomial: ");
	scanf("%s",gen_poly);
	data_length=strlen(data);
	for(i=data_length; i<data_length+N-1; i++)	data[i]='0';
	printf("\n---------------------------------");
	printf("\n Data padded with n-1 zeros: %s",data);
	printf("\n---------------------------------");
	crc();
	printf("\nCRC or check value is : %s",check_value);
	for(i=data_length; i<data_length+N-1; i++)	data[i]=check_value[i-data_length];
	printf("\n-------------------------------");
	printf("\n Final data to be sent: %s",data);
	printf("\n-------------------------------");
	receiver();
	return 0;
}
