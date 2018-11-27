#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * BIGNUM
 */
typedef struct bignum{
    long num_of_digits;
    char* digit;
    int sign; //plus by default
}bignum;
//big num functions
void destroy_big_num(struct bignum* num);
/*
 * STACK
 */
//stack variables
int stack_MAXSIZE = 1024;       
struct bignum* stack[1024];     
int top = -1;
//stack functions
int isEmpty();
int isFull();
struct bignum* pop();
void push(struct bignum* num);
void clear();
//extern assembly functions
extern void add_op(char* a, char* b, long a_length, long b_length, char* res);
extern void mul_op(char* a, char* b, long a_length, long b_length, char* res);
extern int sub_op(char* a, char* b, long a_length, long b_length, char* res);
//general functions
void add_in_assembly(struct bignum* num1, struct bignum* num2);
void sub_in_assembly(struct bignum* num1, struct bignum* num2);
void mul_in_assembly(struct bignum* num1, struct bignum* num2);
void div_in_assembly(struct bignum* num1, struct bignum* num2);
void complete_num_by_filling_zeroes(struct bignum* num, long new_length);
int num_of_leading_zeroes(struct bignum* num);
void print_result(struct bignum* res);
int is_smaller(struct bignum*num1, struct bignum*num2);
int is_bigger_equal(struct bignum*num1, struct bignum*num2);
void remove_leading_zeroes_but_one(struct bignum* num);
void div_in_assembly_helper(bignum*first, bignum*sec, bignum*res, bignum*fact, int ans_sign);
void div_in_assembly_helper_2(bignum*first, bignum*sec, bignum*res, bignum*fact);
void divide_by_two(bignum*num);
void mult_by_two(bignum*num);
void longDivision(bignum*num, int divisor);
int is_zero(bignum*num);


int main(int argc, char **argv) {

    
    char* buffer=malloc(sizeof(char)*10);
    int index=0;
     int hold=1;
    int c=0;
    int maxlength=10;
    int sign=0;
    buffer[index++]='0'; //we add 0 at msb for the case of having carry 
    
    while((c=fgetc(stdin))!=EOF){
        
       if(c=='+'){
            if(hold){
                struct bignum* num= malloc(sizeof(bignum));
                num->num_of_digits=index;
                buffer=realloc(buffer,index+1);
                buffer[index]='\0';
                num->digit=buffer;
                num->sign= sign;
                sign=0;
                maxlength=10;
                push(num);
                hold=0;
                buffer=malloc(sizeof(char)*10);
                index=0;
                buffer[index++]='0';
            }
            struct bignum* num1=pop();
            struct bignum* num2=pop();
            add_in_assembly(num1, num2);
            hold=0;
       }else if(c=='-'){
                if(hold){
                struct bignum* num= malloc(sizeof(bignum));
                num->num_of_digits=index;
                buffer=realloc(buffer,index+1);
                buffer[index]='\0';
                num->digit=buffer;
                num->sign= sign;
                sign=0;
                push(num);
                maxlength=10;
                hold=0;
                buffer=malloc(sizeof(char)*10);
                index=0;
                buffer[index++]='0';
                }
                struct bignum* num2=pop();
                struct bignum* num1=pop();
                sub_in_assembly(num1, num2);
                hold=0;
       }else if(c=='*'){
                if(hold){
                struct bignum* num= malloc(sizeof(bignum));
                num->num_of_digits=index;
                buffer=realloc(buffer,index+1);
                buffer[index]='\0';
                num->digit=buffer;
                num->sign= sign;
                sign=0;
                maxlength=10;
                push(num);
                hold=0;
                buffer=malloc(sizeof(char)*10);
                index=0;
                buffer[index++]='0';
                }
                struct bignum* num1=pop();
                struct bignum* num2=pop();
                mul_in_assembly(num1, num2);
                hold=0;
       }else if(c=='/'){
                if(hold){
                struct bignum* num= malloc(sizeof(bignum));
                num->num_of_digits=index;
                buffer=realloc(buffer,index+1);
                buffer[index]='\0';
                num->digit=buffer;
                num->sign= sign;
                sign=0;
                push(num);
                hold=0;
                buffer=malloc(sizeof(char)*10);
                maxlength=10;
                index=0;
                buffer[index++]='0';
                }
                struct bignum* num2=pop();
                struct bignum* num1=pop();
                div_in_assembly(num1, num2);
                hold=0;
       }else if(c=='_'){
                sign=1;
       }else if(c=='p'){
                if(hold){
                struct bignum* num= malloc(sizeof(bignum));
                num->num_of_digits=index;
                buffer=realloc(buffer,index+1);
                buffer[index]='\0';
                num->digit=buffer;
                num->sign= sign;
                sign=0;
                push(num);
                hold=0;
                buffer=malloc(sizeof(char)*10);
                maxlength=10;
                index=0;
                buffer[index++]='0';
                }
                struct bignum* res= stack[top];
                print_result(res);
       }else if(c=='q'){
            free(buffer);
            break;
       }else if(c=='c'){
                if(hold){
                    free(buffer);
                    buffer=malloc(sizeof(char)*10);
                    index=0;
                    hold=1;
                    c=0;
                    maxlength=10;
                    sign=0;
                    buffer[index++]='0'; //we add 0 at msb for the case of having carry 
                }
                clear();
       }else if(  c>='0' && c<='9' ){
                if(index==maxlength){
                maxlength+=10;
                buffer= realloc(buffer,sizeof(char)*maxlength);
                }
                *(buffer+(index++))=c;
                hold=1;
       }else{
           if(hold){
                struct bignum* num= malloc(sizeof(bignum));
                num->num_of_digits=index;
                buffer=realloc(buffer,index+1);
                buffer[index]='\0';
                num->digit=buffer;
                num->sign= sign;
                sign=0;
                push(num);
                buffer= malloc(sizeof(char)*10);
                maxlength=10;
                index=0;
                buffer[index++]='0';
                hold=0;
           }
       }
   }
    
    return 0;   
}

void remove_leading_zeroes_but_one(struct bignum* num){
    int i= num_of_leading_zeroes(num);
    num->num_of_digits= num->num_of_digits+(-i+1);
    char* new_arr=malloc(sizeof(char)*num->num_of_digits+1);
    for(long j=0; j<num->num_of_digits; j++){
        new_arr[j]=num->digit[j+(i-1)];
    }
    new_arr[num->num_of_digits]='\0';
    free(num->digit);
    num->digit=new_arr;
}

bignum* getcopy(bignum* src){
    bignum* copy=malloc(sizeof(bignum));
    copy->num_of_digits=src->num_of_digits;
    copy->digit=malloc(sizeof(char)*src->num_of_digits+1);
    copy->sign=src->sign;
    for(long i=0; i<copy->num_of_digits; i++){
        copy->digit[i]= src->digit[i];
    }
    copy->digit[copy->num_of_digits]='\0';
    return copy;
}



void add_in_assembly(struct bignum* num1, struct bignum* num2){

    long res_long= num1->num_of_digits;
    int res_sign=0;
    if(num2->num_of_digits > res_long){
        res_long= num2->num_of_digits;
        complete_num_by_filling_zeroes(num1, num2->num_of_digits);
    }else{
        complete_num_by_filling_zeroes(num2, num1->num_of_digits);
    }

    res_long++;
    char* res=malloc(sizeof(char)*res_long);
        for(long i=0; i<res_long; i++){
        res[i]='0';
    }
    
    if(num1->sign==0 && num2->sign==0 ){// a+b
       add_op(num1->digit+num1->num_of_digits-1,num2->digit+num2->num_of_digits-1,num1->num_of_digits , num2->num_of_digits ,res+res_long-1);
        res_sign=0;
    num1->num_of_digits=res_long;
    free(num1->digit);
    destroy_big_num(num2);
    num1->digit=res;
    num1->sign=res_sign;
    push(num1);
    }else if(num1->sign==0 && num2->sign==1 ){// a+(-b) => a-b
            num2->sign=0;
            free(res);
            sub_in_assembly(num1, num2);
    }else if(num1->sign==1 && num2->sign==0 ){// (-a)+b => b-a
            free(res);
            num1->sign=0;
            sub_in_assembly(num2, num1);
    }else if(num1->sign==1 && num2->sign==1 ){// (-a)+(-b) => -(a+b)
            free(res);
            num1->sign=0;
            num2->sign=0;
            add_in_assembly(num1, num2);//this gonna calc a+b
            bignum* curr= pop();        //this gonna pop thre product a+b
            curr->sign=1;                //this gonna change the sign to minus
            push(curr);
    }

}

void mul_in_assembly(struct bignum* num1, struct bignum* num2){
    long res_long= num1->num_of_digits + num2->num_of_digits -1 ;
    char* res=malloc(sizeof(char)*res_long);
    for(long i=0; i<res_long; i++){
        res[i]='0';
    }
    if(num1->num_of_digits >= num2->num_of_digits){
        mul_op(num1->digit+(num1->num_of_digits-1),
        num2->digit+(num2->num_of_digits-1),
        num1->num_of_digits-1,
        num2->num_of_digits,
        res+(res_long-1));    

    }else{
        mul_op(num2->digit+(num2->num_of_digits-1),
        num1->digit+(num1->num_of_digits-1),
        num2->num_of_digits-1,
        num1->num_of_digits,
        res+(res_long-1));
    }

    struct bignum* num=malloc(sizeof(bignum));
    num->num_of_digits= res_long;
    num->digit= res;
    int sign= num1->sign + num2->sign;
    if(sign==1){
        num->sign=1;
    }else{
        num->sign=0;
    }
    push(num);
    destroy_big_num(num1);
    destroy_big_num(num2);
    

}
void sub_in_assembly(struct bignum* num1, struct bignum* num2){
  //  printf("num1=%s, num2=%s\n",num1->digit, num2->digit);
    long res_long= num1->num_of_digits;
    int res_sign=0;
    if(num2->num_of_digits > res_long){
        res_long= num2->num_of_digits;
        complete_num_by_filling_zeroes(num1, num2->num_of_digits);
    }else{
        complete_num_by_filling_zeroes(num2, num1->num_of_digits);
    }
    char* res=malloc(sizeof(char)*res_long);
    

    if(num1->sign==0 && num2->sign==0 ){// a-b
       if(sub_op(num1->digit,num2->digit,num1->num_of_digits , num2->num_of_digits ,res)<0){
           
        res_sign=1;
        sub_op(num2->digit,num1->digit,num2->num_of_digits , num1->num_of_digits ,res);
    }
    num1->num_of_digits=res_long;
    free(num1->digit);
    destroy_big_num(num2);
    num1->digit=res;
    num1->sign=res_sign;
    push(num1);
    }else if(num1->sign==0 && num2->sign==1 ){// a-(-b) => a+b
            num2->sign=0;
            free(res);
            add_in_assembly(num1, num2);
    }else if(num1->sign==1 && num2->sign==0 ){// (-a)-b => -(a+b)
            num1->sign=0;
            free(res);
            add_in_assembly(num1, num2);//this gonna calc a+b
            bignum* curr= pop();        //this gonna pop thre product a+b
            curr->sign=1;                //this gonna change the sign to minus
            push(curr);
    }else if(num1->sign==1 && num2->sign==1 ){// (-a)-(-b) => b-a
            if(sub_op(num2->digit,num1->digit,num2->num_of_digits , num1->num_of_digits ,res)<0){
            res_sign=1;
            sub_op(num1->digit,num2->digit,num1->num_of_digits , num2->num_of_digits ,res);
            }
            num1->num_of_digits=res_long;
            free(num1->digit);
            destroy_big_num(num2);
            num1->digit=res;
            num1->sign=res_sign;
            push(num1);
            }
            
}



void div_in_assembly(struct bignum* num1, struct bignum* num2){

    
    bignum* res=malloc(sizeof(bignum));
    res->num_of_digits=2;
    res->sign=0;
    res->digit=malloc(sizeof(char)*3);
    res->digit[0]='0';
    res->digit[1]='0';
    res->digit[2]='\0';
    
    if(is_zero(num2)>0){
        printf("dividing by zero, unexpected behavior !\n");
        destroy_big_num(num1);
        destroy_big_num(num2);
        clear();
        push(res);
        return;
    }
    
    bignum* factor=malloc(sizeof(bignum));
    factor->num_of_digits=2;
    factor->sign=0;
    factor->digit=malloc(sizeof(char)*3);
    factor->digit[0]='0';
    factor->digit[1]='1';
    factor->digit[2]='\0';
    
    int ans_sign=0;
    if( (num1->sign+num2->sign)==1 ){
        ans_sign=1;
    }
    
    num1->sign=0;
    num2->sign=0;
    if(is_bigger_equal(num1,num2)>0){
        div_in_assembly_helper(num1, num2, res, factor, ans_sign);
    }else{
        destroy_big_num(factor);
        res->sign=0;
        destroy_big_num(num1);
        destroy_big_num(num2);
        push(res);
    }
    
}

void div_in_assembly_helper(bignum*first, bignum*sec, bignum*res, bignum*fact, int ans_sign){
    
    div_in_assembly_helper_2(first, sec, res, fact);
    if( is_bigger_equal(first,sec)>0 ){
        sub_in_assembly(first, sec);
        first= pop();
        add_in_assembly(res, fact);
        res= pop();
    }else{
        destroy_big_num(sec);
        destroy_big_num(fact);
    }
    res->sign=ans_sign;
    push(res);
    destroy_big_num(first);
    return;
    
}



void div_in_assembly_helper_2(bignum*first, bignum*sec, bignum*res, bignum*fact){
    if( is_smaller(first, sec)>0 ){
        divide_by_two(sec);
        divide_by_two(fact);
        return;
    }
    mult_by_two(fact);
    mult_by_two(sec);
    div_in_assembly_helper_2(first, sec, res, fact);
    if( is_bigger_equal(first, sec)>0){        
        bignum* sec_copy= getcopy(sec);
        sub_in_assembly(first, sec_copy);
        first= pop();
        bignum* fact_copy= getcopy(fact);
        add_in_assembly(res, fact_copy);
        res= pop();
    }
    divide_by_two(sec);
    divide_by_two(fact);
    return;
}

void divide_by_two(bignum*num){
    int divisor = 2;    
    longDivision(num,  divisor);
}

void mult_by_two(bignum*num){
    
    bignum* two=malloc(sizeof(bignum));
    two->num_of_digits=2;
    two->sign=0;
    two->digit=malloc(sizeof(char)*3);
    two->digit[0]='0';
    two->digit[1]='2';
    two->digit[2]='\0';
    
    bignum* num_copy=malloc(sizeof(bignum));
    num_copy->num_of_digits= num->num_of_digits;
    num_copy->sign= num->sign;
    num_copy->digit=malloc(sizeof(char)*num->num_of_digits+1);
    num_copy->digit[num->num_of_digits]='\0';
    for(long i=0; i<num_copy->num_of_digits; i++){
        num_copy->digit[i]= num->digit[i];
    }
    
    mul_in_assembly(two, num_copy);
    
    bignum*res= pop();
    
    num->num_of_digits=res->num_of_digits;
    free(num->digit);
    num->digit=res->digit;
    free(res);
}

int is_zero(bignum*num){
    if(num->num_of_digits==1){
        return 1;
    }
    if(num->digit[1]=='0'){
        return 1;
    }
    return -1;
}
int is_bigger_equal(struct bignum*num1, struct bignum*num2){
    if(num1->num_of_digits > num2->num_of_digits){
        return 1; 
    }
    if(num1->num_of_digits < num2->num_of_digits){
        return -1; 
    }
    for(long i=0; i<num1->num_of_digits; i++){
        if(num2->digit[i] > num1->digit[i]){
            return -1;
        }
        if(num2->digit[i] < num1->digit[i]){
            return 1;
        }
    }
    return 1;
}

int is_smaller(struct bignum*num1, struct bignum*num2){
    if(num1->num_of_digits < num2->num_of_digits){
        return 1; 
    }
    if(num1->num_of_digits > num2->num_of_digits){
        return -1; 
    }
    if(is_bigger_equal(num1,num2)>0){
        return -1;
    }
    return 1;

}


void complete_num_by_filling_zeroes(struct bignum* num, long new_length){
    char* newdigit=malloc(sizeof(char)*new_length);
    long j= new_length - num->num_of_digits;
    long i=0;
    for(; i< j; i++){
        newdigit[i]='0';
    }
    for(; i< new_length; i++){
        newdigit[i]=num->digit[i-j];
    }
    free(num->digit);
    num->digit=newdigit;
    num->num_of_digits=new_length;
    
    num->digit=realloc(num->digit, num->num_of_digits+1);
    num->digit[num->num_of_digits]='\0';
}

int num_of_leading_zeroes(struct bignum* num){
    int i=0;
    while(i<num->num_of_digits-1 && num->digit[i]=='0'){
        i++;
    }
    return i;
}

void destroy_big_num(struct bignum* num){
    if(num){
        if(num->digit){
            free(num->digit);
        }
    free(num);
    }
}

void print_result(struct bignum* res){
    
    if(res->sign==1){
        printf("-");
    }
    if(res->digit[0]=='0' && res->num_of_digits>1){
        printf("%s\n",res->digit+1);
    }else{
        printf("%s\n",res->digit);
    }
}


void longDivision(bignum*num, int divisor){
    char* ans=malloc(sizeof(char)*num->num_of_digits+1);
    long ans_idx=0;
    ans[ans_idx++]='0';
    ans[num->num_of_digits]='\0';

    long idx = 0;
    int temp = num->digit[idx] - '0';
    while (temp < divisor && (idx+1)<num->num_of_digits)
       temp = temp * 10 + (num->digit[++idx] - '0');

    while (num->num_of_digits > idx && (ans_idx< num->num_of_digits)){
        ans[ans_idx++]= (temp / divisor) + '0';
        temp = (temp % divisor) * 10 + num->digit[++idx] - '0';
    }
    ans[ans_idx]='\0';
    num->num_of_digits=ans_idx;
    free(num->digit);
    num->digit=ans;
}

//stack impl

int isEmpty(){
   if(top == -1)
      return 1;
   else
      return 0;
}
   
int isFull() {
   if(top == stack_MAXSIZE)
      return 1;
   else
      return 0;
}


struct bignum* pop(){
    
   struct bignum* num;
	
   if(!isEmpty()){
      num = stack[top--];  
   }
      return num;
}

void push(struct bignum* num) {
    remove_leading_zeroes_but_one(num);
   if(!isFull()){
      stack[++top]= num;
   }
}

void clear(){
    while(top!=-1){
        bignum* a=pop();
        destroy_big_num(a);
    }
}

