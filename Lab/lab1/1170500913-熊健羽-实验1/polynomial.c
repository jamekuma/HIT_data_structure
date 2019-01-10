#include <stdio.h>
#include <stdlib.h>
#define QUO 0 //商
#define REM 1 //余数

FILE *fp = NULL;

typedef struct POLYNODE
{
    double coef;
    int exp;
    struct POLYNODE *next;
} polynode;

typedef struct POLYNODE *LIST;
typedef struct POLYNODE *pos;

typedef enum
{
    EXP,
    COEF
} read_what;

void insert(double x, int y, pos p) //向p位置插入一个coef = x, exp = y的节点
{
    pos s = (pos)malloc(sizeof(polynode));
    s->coef = x;
    s->exp = y;
    s->next = p->next;
    p->next = s;
}

void deletePoly(LIST head) //删除整个个多项式并释放空间
{
    pos p = head->next;
    while (p != NULL) //删空所有有效节点
    {
        head->next = p->next; //删除第一个有效节点
        free(p);
        p = head->next;
    }
    free(head); //释放头结点
}

LIST makeNull(void) //生成新的多项式，并置空
{
    LIST head = (LIST)malloc(sizeof(polynode));
    head->next = NULL;
    return head;
}

//从文件中读入一个多项式，
//样例: 3x^6 + 2 + 9x^2
//     3*x^7 + 2 + 9*x^2
void readPoly(LIST head)
{
    pos poly_p;       //用来遍历整个多项式，作为插入操作的位置
    poly_p = head;    //poly_p赋初值
    char s[1000];     //存储文件中读取的一行字符串
    char *c_readPoly; //对上述的字符串s进行逐项读取的指针
    int t_exp = 0;
    double t_coef = 1.0; //缺省值: t_exp = 0, t_coef = 1
    double sign = 0;     // + -> 0, - -> 1
    read_what flag = COEF;
    //表示下一次 c_readPoly指向数字时，当做指数还是系数，缺省值为系数
    fgets(s, 1000, fp); //读一行字符串，放入s中

    //下面处理s中的字符，读入各项的知识与系数
    for (c_readPoly = s; *c_readPoly != '\0'; c_readPoly++) //从头读到尾，直到结束
    {
        if (*c_readPoly <= '9' && *c_readPoly >= '0') //遇到数字
        {
            switch (flag)
            {
            case COEF: //读入系数
                sscanf(c_readPoly, "%lf", &t_coef);
                t_coef = t_coef - 2 * sign * t_coef; // 带上符号
                sign = 0;                            //符号重新置为正
                break;
            case EXP: //读入指数
                sscanf(c_readPoly, "%d", &t_exp);
                t_exp = t_exp - 2 * (int)sign * t_exp; //带上符号
                sign = 0;
                flag = COEF; //下一项应读入系数，也代表这项结束了
                break;
            }
            while ((*c_readPoly <= '9' && *c_readPoly >= '0') || *c_readPoly == '.')
                c_readPoly++; //读完数字之后，跳过之后的数字和小数点
        }

        switch (*c_readPoly)
        {
        case '+':                          //遇到 + 号，代表一项结束
            insert(t_coef, t_exp, poly_p); //把一项存入链表
            poly_p = poly_p->next;         //节点指针后移

            t_coef = 1.0;
            t_exp = 0;
            flag = COEF; //下一个数字应该读入系数
            sign = 0;    //符号置为正
            break;
        case '-':             //遇到 - 号
            if (flag == COEF) //如果下面应该读入系数，也说明一项结束
            {
                if (c_readPoly >= s + 2)
                {
                    insert(t_coef, t_exp, poly_p); //把一项存入链表
                    poly_p = poly_p->next;         //节点指针后移
                }
                t_coef = -1.0;
                t_exp = 0; //coef, exp置为初始值
            }
            sign = 1; //符号置为负
            break;
        case '^': //遇到 ^ 号
            sign = 0;
            flag = EXP; //接下来应读入指数
            break;
        case 'X':
        case 'x':
            t_exp++; //遇到x，代表exp指数至少为1，由零加1
            break;
        default:
            ;
        }
    }
    insert(t_coef, t_exp, poly_p); //将最后一项放入链表
}

void printPoly(LIST head) //向文件里写入一个多项式
{
    pos p;
    if (head->next == NULL)
    {
        fprintf(fp, "0"); //如果链表为空，输出'0'
    }
    for (p = head->next; p != NULL; p = p->next)
    {
        if (p->coef != 0) //若这项的系数不为零，才显示该项
        {
            if (p->coef < 0)
                fprintf(fp, " - "); //如果系数小于零，输出一个 '-' 号

            //如果指数等于零、或者系数不为+1、-1，则输出系数部分
            if ((p->coef != 1 && p->coef != -1) || p->exp == 0)
            {
                if (p->coef > 0)
                    fprintf(fp, "%.2f", p->coef);
                if (p->coef < 0)
                    fprintf(fp, "%.2f", -p->coef);
                if (p->exp != 0)
                    fprintf(fp, " * ");
            }

            //如果指数不为零，才显示指数
            if (p->exp != 0)
            {
                fprintf(fp, "x");
                if (p->exp != 1)
                    fprintf(fp, " ^ %d", p->exp);
            }

            //如果不是最后一项, 则输出一个加号
            if (p->next != NULL && p->next->coef > 0)
                fprintf(fp, " + ");
        }
    }
    fprintf(fp, "\n"); //输出换行符
}

void polySort(LIST head) //按照指数降序，选择法排序
{
    pos p0, p, q0, q, r0, r, temp;
    int max_exp;
    for (p0 = head, p = head->next; p->next != NULL; p0 = p0->next, p = p->next)
    {
        max_exp = p->exp;
        r0 = p0;
        r = p;
        for (q0 = p, q = p->next; q != NULL; q0 = q0->next, q = q->next)
            if (max_exp < q->exp)
            {
                r0 = q0;
                r = q;
                max_exp = q->exp;
            }

        p0->next = r;
        r0->next = p;

        temp = p->next;
        p->next = r->next;
        r->next = temp;

        temp = p;
        p = r;
        r = temp;
    }
}

pos polyClear(LIST head) //合并同类项, 删除0项, 并排序, 返回指向末尾节点的指针
{
    polySort(head);
    pos p, q;
    p = head->next;
    q = p->next;
    while (p != NULL && q != NULL)
    {
        if (p->exp == q->exp)
        {
            p->coef = p->coef + q->coef;
            p->next = q->next;
            free(q);
            q = p->next;
        }
        else
        {
            p = p->next;
            q = q->next;
        }
    }
    p = head;
    q = head->next;
    while (q != NULL)
    {
        if (q->coef == 0)
        {
            p->next = q->next;
            free(q);
            q = p->next;
        }
        else
        {
            p = p->next;
            q = q->next;
        }
    }
    return p;
}

LIST polyAdd(LIST head1, LIST head2) //两多项式相加，合并同类项并排序
{
    polyClear(head1);
    polyClear(head2); //首先合并同类项并排序，保证两个多项式都是指数降序
    LIST head = makeNull();
    pos p = head1->next, q = head2->next, r = head;
    while (p != NULL && q != NULL)
    {
        if (p->exp == q->exp) //若系数相同，直接相加
        {
            insert(p->coef + q->coef, p->exp, r);
            r = r->next;
            p = p->next;
            q = q->next; //指针均向后移
        }

        //不相同，把系数较大的插入链表中，其指针向后移
        else if (p->exp > q->exp)
        {
            insert(p->coef, p->exp, r);
            r = r->next;
            p = p->next;
        }
        else
        {
            insert(q->coef, q->exp, r);
            r = r->next;
            q = q->next;
        }
    }

    //把剩余项放在多项式末尾
    while (p != NULL)
    {
        insert(p->coef, p->exp, r);
        r = r->next;
        p = p->next;
    }
    while (q != NULL)
    {
        insert(q->coef, q->exp, r);
        r = r->next;
        q = q->next;
    }
    return head;
}

LIST polySub(LIST head1, LIST head2) //两多项式相减，合并同类项并排序
{
    // 基本思想同多项式加法
    polyClear(head1);
    polyClear(head2);
    LIST head = makeNull();
    pos p = head1->next, q = head2->next, r = head;
    while (p != NULL && q != NULL)
    {
        if (p->exp == q->exp)
        {
            insert(p->coef - q->coef, p->exp, r); //改为减运算
            r = r->next;
            p = p->next;
            q = q->next;
        }
        else if (p->exp > q->exp)
        {
            insert(p->coef, p->exp, r);
            r = r->next;
            p = p->next;
        }
        else
        {
            insert(-q->coef, q->exp, r); //插入减数的相反数
            r = r->next;
            q = q->next;
        }
    }
    while (p != NULL)
    {
        insert(p->coef, p->exp, r);
        r = r->next;
        p = p->next;
    }
    while (q != NULL)
    {
        insert(-q->coef, q->exp, r); //插入减数的相反数
        r = r->next;
        q = q->next;
    }
    return head;
}

void polyCopy(LIST head1, LIST head2) //把head2多项式拷贝到head1
{
    pos p1 = head1;
    pos p2 = head2->next;
    for (; p2 != NULL; p2 = p2->next, p1 = p1->next)
        insert(p2->coef, p2->exp, p1);
}

double polyValue(LIST head, double x) //代入x求值(秦九韶)
{
    polyClear(head);
    double res = 0;
    double temp_coef;
    int i;
    pos p = head->next;
    for (i = head->next->exp; i >= 0; i--) //一共迭代 最高次+1 次
    {
        if (i == p->exp)
        {
            temp_coef = p->coef;
            if (p->next != NULL)
                p = p->next;
        }
        else
            temp_coef = 0.0;
        res = res * x + temp_coef;
    }
    return res;
}

LIST polyMult(LIST head1, LIST head2) //多项式相乘
//使用循环链表
{
    LIST head = makeNull(), circle_head;
    int max_length = head1->next->exp + head2->next->exp;
    //循坏链表长度为两个最高次项的指数相加
    int i;
    int new_exp;
    double new_coef;
    pos p, p1, p2, circle_rear;
    polyClear(head1);
    polyClear(head2); //整理两个多项式

    //构造循环链表，把每个次项的系数先置为0.0
    for (i = 0, p = head; i <= max_length; i++, p = p->next)
    {
        insert(0.0, max_length - i, p);
    }
    p->next = head->next;  //把链表首尾相接，形成循环链表
    circle_head = p->next; //标记表头节点
    circle_rear = p;       //标记表尾节点

    for (p1 = head1->next; p1 != NULL; p1 = p1->next)
    {
        for (p2 = head2->next; p2 != NULL; p2 = p2->next)
        {
            new_coef = p1->coef * p2->coef;
            new_exp = p1->exp + p2->exp;
            //遍历每一项，计算出待相加项的系数与指数

            while (p->exp != new_exp)
                p = p->next; //找到该次项在循环链表中的位置

            p->coef += new_coef; //相应次项的系数增加
        }
    }
    head->next = circle_head; //重新将循环链表还原
    circle_rear->next = NULL; //表尾节点置为空
    polyClear(head);          //整理结果
    return head;
}

LIST *polyDiv(LIST head1, LIST head2) /*多项式相除，返回一个数组首地址，
                                        两个元素分别为商多项式和余数多项式的头指针*/
{
    LIST *array = (LIST *)calloc(2, sizeof(LIST));
    array[QUO] = makeNull();
    array[REM] = makeNull(); //分别为商多项式和余数多项式申请空间
    pos circleHead_rem;      //循环链表的头指针
    pos circleRear_rem;      //循环链表的尾指针
    pos highestExp;          //系数不为零的最高次项的位置
    pos p1, p2;              //辅助位置指针
    int new_exp, i;
    double new_coef;
    polyClear(head1);
    polyClear(head2);                  //整理待运算的多项式
    int max_length; 
    pos p_quo = array[QUO];
    pos p_rem = array[REM];
    if (head2->next == NULL) //如果除数多项式为0，返回空指针，报错
    {
        return NULL;
    }
    if (head1->next == NULL) //如果被除多项式为0,则商和余数均为0，直接返回
    {
        return array;
    }

    if (head1->next->exp < head2->next->exp) /*如果被除多项式的次数 大于除数多项式的
                                                  次数，则商为0，余数为被除多项式 */
        polyCopy(array[REM], head1);

    else
    {
        max_length = head1->next->exp; //余数多项式的最大长度减1 为被除多项式的次数
        /*把余数多项式初始化为被除多项式。并将其变为循环链表*/
        for (i = 0, p_rem = array[REM]; i <= max_length; i++, p_rem = p_rem->next)
        {   // 插入max_length + 1个元素，指数降序，系数为0.0
            insert(0.0, max_length - i, p_rem);
        }
        circleHead_rem = array[REM]->next;     //保存循环链表的头节点
        circleRear_rem = p_rem;                //保存循环链表的尾节点
        circleRear_rem->next = circleHead_rem; //首尾相接
        highestExp = circleHead_rem;           //初始化 系数不为零的最高次项的位置
        for (p1 = head1->next, p_rem = circleHead_rem; p1 != NULL; p1 = p1->next)
        {   //把被除多项式填入循环链表
            while (p_rem->exp != p1->exp) //找到对应项的位置
                p_rem = p_rem->next;
            p_rem->coef += p1->coef;
        }

        /*开始除法过程*/
        while (highestExp->exp >= head2->next->exp) //余数多项式次数小于除数多项式次数时停止循环
        {
            insert(highestExp->coef / head2->next->coef, highestExp->exp - head2->next->exp, p_quo);
            //计算出商多项式的一项并插入链表中
            p_quo = p_quo->next;

            /*更新余数多项式*/
            for (p2 = head2->next; p2 != NULL; p2 = p2->next)
            {
                new_coef = p_quo->coef * p2->coef;
                new_exp = p_quo->exp + p2->exp;
                //商多项式与除数多项式相乘
                while (p_rem->exp != new_exp)
                    p_rem = p_rem->next;
                p_rem->coef -= new_coef; //找到对应次项，更新 (相减)
            }

            /*更新系数不为零的最高次项的位置*/
            while (highestExp->coef == 0)
            {
                highestExp = highestExp->next;
                if (highestExp == circleRear_rem) //如果转了一圈,转到了尾部
                    if (highestExp->coef == 0)
                        goto jumpout; /*尾部仍然系数为零，证明整个循环链表为0，
                                            直接结束除法，使用goto跳出多重循环*/
            }
            // polyClear(array[REM]);
            // if (array[REM]->next == NULL)
            //     break;
        }
    }
jumpout:
    circleRear_rem->next = NULL; //解除循环链表
    polyClear(array[QUO]);
    polyClear(array[REM]); //整理商多项式与余数多项式
    return array;
}

void figure(char op, LIST poly1, LIST poly2) //根据读取的操作符，判断运算类别并运算，输出结果
{
    LIST poly3;
    LIST poly4; //两个结果多项式
    LIST *resDiv;
    double val, resVal;
    char temps[100], *p_temp;

    /*根据操作符，进行不同的运算*/
    switch (op)
    {
    case '+':
        poly3 = polyAdd(poly1, poly2);
        break;
    case '-':
        poly3 = polySub(poly1, poly2);
        break;
    case '*':
        poly3 = polyMult(poly1, poly2);
        break;
    case '/':
        resDiv = polyDiv(poly1, poly2);
        break;
    case 'x':    
    case 'X':      //遇到x，即为求某点多项式的值
        fgets(temps, 100, fp);
        for (p_temp = temps; *p_temp != '\0'; p_temp++)
        {
            if (*p_temp >= '0' && *p_temp <= '9')
            {
                sscanf(p_temp, "%lf", &val);  //若读取到数字，则写入变量val中
                break;
            }
        }
        break;
    default:
        ;
    }
    fclose(fp);  //读数据完毕，关闭文件，开始写数据

    if ((fp = fopen("F:\\data_structure\\Lab\\lab1\\test.txt", "a")) == NULL)
    {      // 写入方式：在文件末尾添加文本
        printf("open failed\n"); //若打开文件失败，则退出程序
        exit(1);
    }

    if (op == '/')  //若运算为除法，则需输出两个多项式
    {
        if (resDiv == NULL)  //除数为零，不合法
        {
            fprintf(fp, "除数为零，错误\n");
            exit(1);
        }           
        poly3 = resDiv[QUO]; 
        poly4 = resDiv[REM];
        fprintf(fp, "商：");
        printPoly(poly3);
        deletePoly(poly3);  //多项式使用完毕，释放其所有空间
        fprintf(fp, "余数：");
        printPoly(poly4);
        deletePoly(poly4); //多项式使用完毕，释放其所有空间
    }
    else
    {
        switch (op)
        {
        case '+':
            fprintf(fp, "加法结果：");
            printPoly(poly3);
            deletePoly(poly3);
            break;
        case '-':
            fprintf(fp, "减法结果：");
            printPoly(poly3);
            deletePoly(poly3);
            break;
        case '*':
            fprintf(fp, "乘法结果：");
            printPoly(poly3);
            deletePoly(poly3);
            break;
        case 'x':
        case 'X':
            fprintf(fp, "x = %.2lf时多项式1的值为：%.2lf\n", val, polyValue(poly1, val));
            fprintf(fp, "x = %.2lf时多项式2的值为：%.2lf\n", val, polyValue(poly2, val));
            break;
        default:
            fprintf(fp, "符号错误！\n");
        }
    }
}

int main()
{
    LIST poly1 = makeNull();
    LIST poly2 = makeNull();
    char op;
    if ((fp = fopen("F:\\data_structure\\Lab\\lab1\\test.txt", "r")) == NULL)
    {
        printf("open failed\n"); //若打开文件失败，则退出程序
        exit(1);
    }
    readPoly(poly1);
    readPoly(poly2);

    op = fgetc(fp);

    figure(op, poly1, poly2);

    deletePoly(poly1);
    deletePoly(poly2);
    
    fclose(fp);
    return 0;
}