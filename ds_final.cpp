#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int val[13][5];

enum colour { red = 0, green=1, yellow=2, blue=3, nocolour=4 } ;
typedef enum colour color;


struct cards
{
    int value;
    color c;
};
typedef struct cards cards;

typedef struct player player;
struct player
{
    cards  p_card;
    player* rcard;
    player* lcard;
};


player* comp = NULL;
player* complast = NULL;
player* user = NULL;
player* userlast = NULL;

cards uno[92];  //牌堆的牌
cards s[92];    //出過的牌


//把出過得牌丟到堆疊
int top = -1;
int i;
void push(cards ca)
{
    if (top == 91)
        printf("error"); //牌組沒了
    else 
    {
        top++;
        s[top].value = ca.value;   //把引入的牌堆疊到s[top]中
        s[top].c = ca.c;
    }
}


//自己拿牌  //建立link list //新增link list元素
void user_insert(cards ca)
{
    val[ca.value][ca.c]++ ;
    player* first = user;
    player* last = userlast;
    player* temp = (player*)malloc(sizeof(player));
    (temp->p_card).value = ca.value;
    (temp->p_card).c = ca.c;   //建立鏈結串列並連起來
    
    if (!first) //first==0
    {
        first = temp; //第一次建立牌組
        first->lcard = NULL;
        first->rcard = NULL;
        last = first;   
    }
    else 
    {
        temp->lcard = last; //之後加牌用到的link list
        temp->rcard = NULL;
        last->rcard = temp;
        last = temp;  
    }

    user = first;
    userlast = last; //user和ulast分別指向頭尾
}


//電腦方拿牌  //建立link list //新增link list元素
void comp_insert(cards ca) //同上方 但是是建立不同條link list
{
    val[ca.value][ca.c]++;
    player* first = comp;
    player* last = complast;
    player* temp = (player*)malloc(sizeof(player));
    (temp->p_card).value = ca.value;
    (temp->p_card).c = ca.c;
    if (!first) {
        first = temp;
        first->lcard = NULL;
        first->rcard = NULL;
        last = first;
    }
    else {
        temp->lcard = last;
        temp->rcard = NULL;
        last->rcard = temp;
        last = temp;
    }

    comp = first;
    complast = last;
}



//出牌後把原牌從自己的排組中移除 //刪除link list元素
void user_delete(cards ca)
{
    player* prev, * next;
    player* first = user;
    player* last = userlast;
    player* p = first; //p = ptr

    while (p) //將p指到要找的牌的位置
    {
        if ((p->p_card).value == ca.value && (p->p_card).c == ca.c)
            break;
        p = p->rcard;
    }

    if (p == first) //刪除第一張牌
    {
        next = p->rcard;
        next->lcard = NULL;
        first = next;
        free(p);
    }
    else if (p == last)  //刪除最後一張牌
    {
        prev = p->lcard;
        prev->rcard = NULL;
        last = prev;
        free(p);
    }
    else    //刪除中間的一張牌
    {
        prev = p->lcard;
        next = p->rcard;
        prev->rcard = next;
        next->lcard = prev;
        free(p);
    }

    user = first;
    userlast = last;
}


//出牌後把原牌從電腦方的排組中移除 //刪除link list元素  //演算法同上
void comp_delete(cards ca) {
    player* prev, * next;
    player* first = comp;
    player* last = complast;
    player* p = first;
    while (p)
    {
        if ((p->p_card).value == ca.value && (p->p_card).c == ca.c)
            break;
        p = p->rcard;
    }
    if (p == first) {
        next = p->rcard;
        next->lcard = NULL;
        first = next;
        free(p);
    }
    else if (p == last) {
        prev = p->lcard;
        prev->rcard = NULL;
        last = prev;
        free(p);
    }
    else 
    {
        prev = p->lcard;
        next = p->rcard;
        prev->rcard = next;
        next->lcard = prev;
        free(p);
    }
    comp = first;
    complast = last;
}



//自己沒牌出要加牌 
void draw_card(player* first, player* last, int top1) //top1:牌堆中的最上面一張牌 
{
    if (first == user) //first引入玩家or電腦
        user_insert(uno[top1]);
    else
        comp_insert(uno[top1]);

    if (uno[i].value == s[top].value || (uno[i].c == s[top].c && uno[i].c !=4) ) //抽到的牌可以出就出 特殊牌不行
    {
        push(uno[i]);

        printf("\n\n--->\n\n");
        printf(" draw card but can place\n");
        printf("\n The card is:");
        printf("\n[%d] ", s[top].value);
        if (s[top].c == 0)
            printf("[red]\n");
        else if (s[top].c == 1)
            printf("[green]\n");
        else if (s[top].c == 2)
            printf("[yellow]\n");
        else if (s[top].c == 3)
            printf("[blue]\n");

        if (first == user)
            user_delete(uno[i]);
        else
            comp_delete(uno[i]);
    }
}


//出牌是否正確並把原牌從自己的排組中移除
void match_user(int num, int top1) //num輸入的數字(要刪除的位置) top1:牌堆中的最上面一張牌
{
    int cnt = 0;
    player* p = user;
    while (p && cnt < num - 1) //指到欲刪除牌的頭
    {
        p = p->rcard;
        cnt++;
    }

    if ((p->p_card).value > 10) //出特殊牌
    {
        push(p->p_card);
        user_delete(p->p_card);
    }
    else if ((p->p_card).value != s[top].value && (p->p_card).c != s[top].c)
    {
        printf("\n NO MATCHING CARD. YOU LOSE YOUR TURN");
        draw_card(user, userlast, top1);
    }
    else  //出一般牌
    {
        push(p->p_card);
        user_delete(p->p_card);
    }
}


//電腦方判斷要出甚麼牌
void match_comp(int top1)
{
    int c = 0;
    player* p = comp;
    player* q = complast;

    while (p && q)
    {
        //如果有牌出就出並把那張牌刪掉
        if (s[top].value == (p->p_card).value || s[top].c == (p->p_card).c)
        {
            push(p->p_card);
            comp_delete(p->p_card);
            c++;
            break;
        }
        else if (s[top].value == (q->p_card).value || s[top].c == (q->p_card).c)
        {
            push(q->p_card);
            comp_delete(q->p_card);
            c++;
            break;
        }
        p = p->rcard;
        q = q->lcard;
    }

    p = comp;

    if (c == 0) //沒出到牌
    {
        while (p) //出特殊牌
        {
            if ((p->p_card).value > 10)
            {
                push(p->p_card);
                comp_delete(p->p_card);
                c++;
                break;
            }
            p = p->rcard;
        }
        if (c == 0) //再沒有就抽牌
        {
            printf(" computer draw a card \n");
            draw_card(comp, complast, top1);
        }
    }
}


void shuffle(int time) //洗牌次數
{
    for (int idx = 0; idx < time; idx++)
    {
        int i = rand()%91;
        int j = rand() %91;
        cards temp = uno[i];
        uno[i] = uno[j];
        uno[j] = temp;
    }
}


//受特殊牌作用
void wild_card(int v)
{
    int col;

    switch (s[top].value)
    {
    case 10:
        if (v)  //玩家出
        {
            comp_insert(uno[i]);
            i++;
            comp_insert(uno[i]);
            i++;
        }
        else  //電腦出
        {
            printf("\n Computer has placed draw two cards\n");
            user_insert(uno[i]);
              i++;
            user_insert(uno[i]);
              i++;           
        }
        break;

    case 11:
        if (v)
        {
            printf("\n 0 red\n 1 green\n 2 yellow\n 3 blue");
            printf("\n Enter color no:");
            scanf_s("%d", &col);
            s[top].c = col;
            printf("\n The color of top card is");
            if (s[top].c == 0)
                printf(" red\n");
            else if (s[top].c == 1)
                printf(" green\n");
            else if (s[top].c == 2)
                printf(" yellow\n");
            else if (s[top].c == 3)
                printf(" blue\n");           
        }
        else
        {
            printf("\n Computer has placed color change:\n");
            col = (rand() % 3) ;
            s[top].c = col;
            printf("\n The color of top card is");
            if (s[top].c == 0)
                printf(" red\n");
            else if (s[top].c == 1)
                printf(" green\n");
            else if (s[top].c == 2)
                printf(" yellow\n");
            else if (s[top].c == 3)
                printf(" blue\n");           
        }
        break;

    case 12:
        if (v)
        {
            printf("\n 0 red\n 1 green\n 2 yellow\n 3 blue");
            printf("\n Enter color:");
            scanf_s("%d", &col);
            s[top].c = col;
            printf("\n The color of top card is");
            if (s[top].c == 0)
                printf(" red\n");
            else if (s[top].c == 1)
                printf(" green\n");
            else if (s[top].c == 2)
                printf(" yellow\n");
            else if (s[top].c == 3)
                printf(" blue\n");
            for (int k = 0; k < 4; k++)
            {
                comp_insert(uno[i]);
                i++;
            }
        }
        else
        {
            printf("\n Computer has placed draw four and color change\n");
            col = (rand() % 3);
            s[top].c = col;
            printf("\n The color of top card is");
            if (s[top].c == 0)
                printf(" red\n");
            else if (s[top].c == 1)
                printf(" green\n");
            else if (s[top].c == 2)
                printf(" yellow\n");
            else if (s[top].c == 3)
                printf(" blue\n");
            for (int k = 0; k < 4; k++)
            {
                user_insert(uno[i]);
                i++;
            }
            
        }
        break;
    }
}

//判斷是否最後一張牌
int nodes(player* p)
{
    int cnt = 0;
    while (p)
    {
        cnt++;
        p = p->rcard; //直到p指向null
    }
    return cnt;
}

//顯示牌組
void display()
{
    player* p = user;
    int num = 1;
    while (p)
    {
        printf(" %d.  [%d] ", num,(p->p_card).value);
        if ((p->p_card).c == 0)
            printf("[red]\n");
        else if ((p->p_card).c == 1)
            printf("[green]\n");
        else if ((p->p_card).c == 2)
            printf("[yellow]\n");
        else if ((p->p_card).c == 3)
            printf("[blue]\n");
        else if ((p->p_card).c == 4)
            printf("[No colour]\n");
        num++;
        p = p->rcard;
    }
}


int main()
{
    int k, j = 0, choice=0, variable = 0;
    int cardnum2 = 0, cardnum3 = 100; //2.3先不同值
    int ccards=1;
    color l = 0 ;
    printf("\n********** UNO CARD GAME **********\n\n This is a card game played between YOU and computer\n This card game has of 92 cards\n\n 76 cards are number([0]*1 and [1-9]*2 for four colors\n 8 cards are +2 cards for four colors (number is [10])\n 4 cards are wild clolor cards        (number is [11])\n 4 cards are wild clolor and +4 cards (number is [12])\n\n");

    for (int h = 0; h < 13; h++)
    {
        for (int j = 0; j < 5; j++) 
            val[h][j] = 0;
    }

    for (k = 0; k < 40; k++)
    {
        uno[k].value = j;
        uno[k].c = l;
        j++;
        if (j == 10)
        {
            j = 0;
            l++;
        }
        if (l == 4)
            l = 0;
    }

    int x=1, y = 1;
    for (k = 40; k < 76; k++)
    {
        uno[k].value = x;
        uno[k].c = y;
        x++;
        if (x == 10)
        {
            x = 0;
            y++;
        }
        if (y == 4)
            y = 0;
    }

    l = 0;
    for (k = 76; k < 84; k++)
    {
        uno[k].value = 10;
        uno[k].c = l;
        l++;
        if (l == 4)
            l = 0;
    }

    for (k = 84; k < 88; k++)
    {
        uno[k].value = 11;
        uno[k].c = 4;
    }
    for (k = 88; k < 92; k++)
    {
        uno[k].value = 12;
        uno[k].c = 4;
    }

    int time;
    printf(" input shuffle time : ");
    scanf_s("%d", &time);
    shuffle(time);

    for (i = 0; i < 7; i++)
        comp_insert(uno[i]);
    for (i = 7; i < 14; i++)
        user_insert(uno[i]);
    
    push(uno[i]);

    i++;


    do
    {
        printf("\n........................................\n");
        printf("\n Top card is:");
        printf("\n [%d] ", s[top].value);
        if (s[top].c == 0)
            printf("[red]\n");
        else if (s[top].c == 1)
            printf("[green]\n");
        else if (s[top].c == 2)
            printf("[yellow]\n");
        else if (s[top].c == 3)
            printf("[blue]\n");
        else if (s[top].c == 4)
            printf("[No colour]\n");


        variable = 0; //0 is user drws card

        if (s[top].value > 9 && (cardnum3==((cardnum2)+1) /*如果電腦沒出牌*/))   
        {                         //玩家出變色或+2 跳回玩家回合時 會視為電腦出玩家剛剛出的
            printf("\n----------------\n");
            printf("\n Your cards are:\n\n");

            display();

            player* p = user; //算自己有幾張牌
            int cardnum = 1;
            while (p)
            {
                cardnum++;
                p = p->rcard;
            }


            printf("\n........................................\n");
            printf("\n Enter your choice \n Press 1 no matching card \n Press 2 have matching card \n Press 3 for UNO\n 1. drawcard  2. match  3.uno:");
            scanf_s("%d", &choice);

            if (choice == 1)
            {
                draw_card(user, userlast, i);
                i++;
            }
            else if (choice == 2)
            {
                int num;
                printf("\n Enter number:");
                scanf_s("%d", &num);

                if (num >= cardnum)  //出錯牌
                {
                    printf("\n\n no this card\n input again\n");
                    continue;
                }
                else
                {
                    match_user(num, i);
                    i++;
                }
            }
            else if (choice == 3)
            {
                int cnt = nodes(user);
                if (cnt != 1 || ((uno[i].value != s[top].value) && (uno[i].c != s[top].c)))
                    printf("\n Cheater ! you lose \n\n");
                else
                    printf("\n you win\n\n");
                exit(1);
            }
        }

        else
        {
            if (s[top].value > 9)
                wild_card(variable);

            printf("\n----------------\n");
            printf("\n Your cards are:\n\n");

            display();

            player* p = user; //算自己有幾張牌
            int cardnum = 1;
            while (p)
            {
                cardnum++;
                p = p->rcard;
            }


            printf("\n........................................\n");
            printf("\n Enter your choice \n Press 1 no matching card \n Press 2 have matching card \n Press 3 for UNO\n 1. drawcard  2. match  3.uno:");
            scanf_s("%d", &choice);

            if (choice == 1)
            {
                draw_card(user, userlast, i);
                i++;
            }
            else if (choice == 2)
            {
                int num;
                printf("\n Enter number:");
                scanf_s("%d", &num);

                if (num >= cardnum)  //出錯牌
                {
                    printf("\n\n no this card\n input again\n");
                    continue;
                }
                else
                {
                    match_user(num, i);
                    i++;
                }
            }
            else if (choice == 3)
            {
                int cnt = nodes(user);
                if (cnt != 1 || uno[i].value != s[top].value || uno[i].c != s[top].c)
                    printf("\n Cheater ! you lose \n\n");
                else
                    printf("\n you win\n\n");
                exit(1);
            }
        }
        
       
        //電腦
        variable = 1;

        if (choice == 1)  //如果玩家抽牌 原本電腦出的會被當成玩家出的
        {
            printf("\n........................................\n........................................\n\n");
            printf("\n Computer is playing it's turn\n");

            player* ptr = comp; //算電腦有幾張牌
            int cardnum2 = 1;

            while (ptr)
            {
                cardnum2++;
                ptr = ptr->rcard;
            }

            match_comp(i);

            player* ptr1 = comp; //算match後有幾張牌
            cardnum3 = 1;
            while (ptr1)
            {
                cardnum3++;
                ptr1 = ptr1->rcard;
            }

            i++;
        }

        else
        {
            if (s[top].value > 9)
                wild_card(variable);

            printf("\n........................................\n........................................\n\n");
            printf("\n Computer is playing it's turn\n");

            player* ptr2 = comp; //算電腦有幾張牌
            int cardnum2 = 1;
            while (ptr2)
            {
                cardnum2++;
                ptr2 = ptr2->rcard;
            }

            match_comp(i);

            player* ptr3 = comp; //算match後有幾張牌
            cardnum3 = 1;
            while (ptr3)
            {
                cardnum3++;
                ptr3 = ptr3->rcard;
            }


            i++;
        }



        ccards = nodes(comp);
        if (ccards == 1)
        {
            printf("\n Computer wins \n\n");
            exit(1);
        }
    } while (ccards != 1 || choice != 3);

    system("pause");
    return 0;
}
