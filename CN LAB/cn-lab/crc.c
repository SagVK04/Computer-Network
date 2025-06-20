#include<stdio.h>
#include<string.h>
#define N strlen(gen_poly)
char data[28];
char check_value[28];
char gen_poly[10];
int data_length, i, j;
void XOR() {
    for(j = 1; j < N; j++) check_value[j] = ((check_value[j] == gen_poly[j]) ? '0' : '1');
}
void receiver() {
    printf("Enter the received data: ");
    scanf("%s", data);
    printf("\n------------------------------\n");
    printf("Data received: %s\n", data);
    crc();
    for(i = 0; (i < N - 1) && (check_value[i] != '1'); i++);
    if(i < N - 1) printf("\n❌ Error detected in received data!\n");
    else printf("\n✅ No error detected in received data.\n");
}
void crc() {
    for(i = 0; i < N; i++) check_value[i] = data[i];
    do {
        if(check_value[0] == '1') XOR();
        for(j = 0; j < N - 1; j++) check_value[j] = check_value[j + 1];
        check_value[j] = data[i++];
    }
    while(i <= data_length + N - 1);
}
int main() {
    printf("\nEnter data to be transmitted: ");
    scanf("%s", data);
    printf("\nEnter the Generating Polynomial: ");
    scanf("%s", gen_poly);
    data_length = strlen(data);
    for(i = data_length; i < data_length + N - 1; i++) data[i] = '0';
    //data[i] = '\0';
    printf("\n------------------------------\n");
    printf("Data padded with n-1 zeros: %s\n", data);
    printf("\n------------------------------\n");
    crc();
    printf("CRC Check Value: %s\n", check_value);
    for(i = data_length; i < data_length + N - 1; i++) data[i] = check_value[i - data_length];
    //data[i] = '\0';
    printf("\nFinal data to be sent: %s\n", data);
    printf("------------------------------\n");
    receiver();
    return 0;
}