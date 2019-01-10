#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAX 30000

int NUM_WORDS = 0;
int NUM_CODE = 0;

typedef struct
{
    char ch;
    double prob;
    char code;
} code;

typedef struct
{
    code table[MAX];
    int charNum;
} codeTable;

typedef struct
{
    char ch;
    int parent;
    int lchild; // 0
    int rchild; // 1
    double weight;
} Huffman_node;

typedef Huffman_node *Huffman_T;



Huffman_T makeNullHuff(void)  //生成一个空的哈夫曼树
{
    Huffman_T t = (Huffman_T)calloc(MAX, sizeof(Huffman_node));
    return t;
}

void figureProb(char *eng, codeTable *p_tb)   //计算各字符的频率
{
    int sum = 0;
    int i;
    p_tb->charNum = 0;

    while (*eng != '\0')
    {
        sum++;
        for (i = 0; i < p_tb->charNum; i++)
        {
            if (p_tb->table[i].ch == *eng)
                break;
        }
        if (i == p_tb->charNum)
        {
            p_tb->charNum++;
            p_tb->table[i].ch = *eng;
            p_tb->table[i].prob = 1;
        }
        else
        {
            p_tb->table[i].prob++;
        }
        eng++;
    }
    for (i = 0; i < p_tb->charNum; i++)
        p_tb->table[i].prob /= sum;
}

void initHuff(codeTable tb, Huffman_T tree)  //初始化哈夫曼树
{
    int i;
    for (i = 0; i < MAX; i++)   //初始化各节点
    {
        tree[i].ch = '\0';
        tree[i].weight = 0;
        tree[i].lchild = -1;
        tree[i].rchild = -1;
        tree[i].parent = -1;
    }
    for (i = 0; i < tb.charNum; i++)   //把每个叶节点先填入
    {
        tree[i].weight = tb.table[i].prob;
        tree[i].ch = tb.table[i].ch;
    }
}

int selectMin(Huffman_T tree)   //找出当前哈夫曼数组parent = -1的节点中，weight最小的下标
{
    int i = 0;
    int min_index;
    double min;
    min_index = -1;
    min = 1;
    for (i = 0; tree[i].weight != 0; i++)
    {
        if (tree[i].parent == -1 && min >= tree[i].weight)
        {
            min = tree[i].weight;
            min_index = i;
        }
    }
    return min_index;
}

Huffman_T buildHuff(char *eng)   //构建哈夫曼树
{
    int nodeNum;
    int counter; //还剩下的parent = -1的有效节点的个数
    int tmp1, tmp2;
    codeTable tb;
    figureProb(eng, &tb);
    Huffman_T tree = makeNullHuff();
    initHuff(tb, tree);
    nodeNum = tb.charNum;
    counter = tb.charNum;
    if (counter == 1)    //如果只有一个字符
    {
        tree[0].parent = 1;
        tree[1].weight = 1;
        tree[1].rchild = 0;
        tree[1].lchild = -1;
    }
    while (counter >= 2)
    {
        tmp1 = selectMin(tree);
        if (tmp1 != -1)
            tree[tmp1].parent = nodeNum;
        tree[nodeNum].lchild = tmp1;

        tmp2 = selectMin(tree);
        if (tmp2 != -1)
            tree[tmp2].parent = nodeNum;
        tree[nodeNum].rchild = tmp2;     //把两个weight最小的，作为兄弟节点

        tree[nodeNum].weight = tree[tmp1].weight + tree[tmp2].weight;
        nodeNum++;
        counter--;
    }

    return tree;
}

int bitToHex(unsigned char *byte, char *bit)   //0 1 二进制数组bit，转化为字节数组byte
{
    int i = 0, j = i * 8, k;
    char temp;
    while (bit[j])
    {
        byte[i] = 0;
        for (k = 0; k < 8; k++)
        {
            if (bit[j + k] == 0)
                temp = '0';
            else
                temp = bit[j + k];
            byte[i] = byte[i] * 2 + (temp - '0');
        }
        i++;
        j = i * 8;
    }
    return i;
}

void chToBin(Huffman_T tree, char *eng, char *path)   //读入字符串，根据哈夫曼树，编码出二进制文件
{
    FILE *fp;
    if ((fp = fopen(path, "wb")) == NULL)
    {
        printf("open failed!\n");
        exit(1);
    }
    int i;
    char temp[100]; 
    char *p_temp;
    char write_bit[MAX];   // 得到的 0 1 数组
    memset(write_bit, 0, MAX * sizeof(char)); 
    char *p_write_bit; 
    unsigned char write_byte[MAX];  //待写入文件的字节数组

    p_write_bit = write_bit;   

    while (*eng)   //遍历每个字符
    {
        p_temp = temp;
        i = 0;
        while (tree[i].ch != *eng)   //找到相应字符的叶节点
            i++;
        while (tree[i].parent != -1)   //向上追溯至根节点
        {
            if (i == tree[tree[i].parent].lchild)
                *(p_temp++) = '0';     
            if (i == tree[tree[i].parent].rchild)
                *(p_temp++) = '1';
            i = tree[i].parent;
        }
        while (p_temp != temp)   //倒序输出
        {
            *(p_write_bit++) = *(p_temp - 1);
            p_temp--;
        }
        eng++;
    }
    NUM_CODE = bitToHex(write_byte, write_bit);  //转化为字节数组，返回字节数
    fwrite(write_byte, sizeof(unsigned char), NUM_CODE, fp);//写入文件中
    fclose(fp);
}

void HexTobit(char *bit, unsigned char *byte, int num_hex) //字节数组byte，转化为0 1 二进制数组bit
{
    int num = 0;
    unsigned char temp;
    for (int i = 0; i < num_hex; i++)
    {
        temp = *byte;
        for (int i = 0; i < 8; i++)
        {
            *(bit + 7 - i) = temp % 2 + '0';
            temp /= 2;
        }
        bit += 8;
        byte++;
        num++;
    }
    // return num * 8;
}

void binToCh(Huffman_T tree, char *r_path, char *w_path)   //根据编码文件及哈夫曼树，译码出文本文件
{
    FILE *fp_r, *fp_w;
    int num;
    int root_index, index;
    if ((fp_r = fopen(r_path, "rb")) == NULL)
    {
        printf("open failed!\n");
        exit(1);
    }
    unsigned char read_byte[MAX];
    char read_bit[MAX];
    char write[MAX];
    memset(read_byte, 0, MAX * sizeof(unsigned char));
    num = fread(read_byte, sizeof(unsigned char), MAX, fp_r);  //读字节

    HexTobit(read_bit, read_byte, num);  // 转化为0 1 位
    read_bit[num * 8] = '\0';
    char *p_ch = read_bit;

    root_index = 0;
    while (tree[root_index].parent != -1)  //找到树根节点
        root_index++;

    index = root_index; //树根节点开始
    int i = 0;

    while (*p_ch)
    {
        if (*p_ch == '0') //为 0 向左
        {
            index = tree[index].lchild;
            p_ch++;
        }
        else if (*p_ch == '1')  //为 1 向右
        {
            index = tree[index].rchild;
            p_ch++;
        }
        if (index == -1)   //找到头了，回到根节点
            index = root_index;
        if (tree[index].ch != '\0')   //找到了叶节点，存起来
        {
            write[i++] = tree[index].ch;
            index = root_index;
        }
    }
    write[i] = '\0';

    if ((fp_w = fopen(w_path, "w+")) == NULL)
    {
        printf("open failed!\n");
        exit(1);
    }
    char *p_write = write;
    int counter = 0;
    // fputs(write, fp_w);
    while (counter++ < NUM_WORDS)
        fputc(*(p_write++), fp_w);  //输出到文件中

    fclose(fp_w);
    fclose(fp_r);
}

void printCode(Huffman_T tree, FILE **fp)  //打印出每个字符的编码
{
    int i = 0, j, k;
    char temp[MAX];
    while (tree[i].weight != 0)   //遍历所有表项
    {
        if (tree[i].ch != '\0')   //如果是叶节点，则向上追溯
        {
            if (tree[i].ch != '\n')
                fprintf(*fp, "%c \t", tree[i].ch);
            else
                fprintf(*fp, "\\n \t");
            j = 0;
            k = i;
            while (tree[k].parent != -1)
            {
                if (k == tree[tree[k].parent].lchild)
                    temp[j++] = '0';
                else
                    temp[j++] = '1';
                k = tree[k].parent;
            }
            j--;
            for (; j >= 0; j--)    //反向输出
                fprintf(*fp, "%c", temp[j]);
            fprintf(*fp, "\n");
        }
        i++;
    }
}

void test(Huffman_T tree, char *path)  //打印出编码表、计算压缩率
{
    FILE *fp;
    if ((fp = fopen(path, "w+")) == NULL)
    {
        printf("open failed!\n");
        exit(1);
    }

    fprintf(fp, "compression ratio: %lf %%\n", (double)NUM_CODE * 100 / NUM_WORDS);
    
    fprintf(fp, "\n");
    fprintf(fp, "coding table is below:\n");
    //  printf("time:%lf s\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
    printCode(tree, &fp);
}

int main()
{
    FILE *fp;
    if ((fp = fopen(".\\words.txt", "r+")) == NULL)
    {
        printf("open failed!\n");
        exit(1);
    }
    char s[MAX];
    char *p_ch = s;
    while ((*(p_ch++) = fgetc(fp)) != EOF)
        ;
    *(p_ch - 1) = '\0';
    NUM_WORDS = p_ch - s - 1;
    fclose(fp);

    Huffman_T t = buildHuff(s);
    
    chToBin(t, s, ".\\coding.bin");
    binToCh(t, ".\\coding.bin", ".\\out.txt");

    test(t, ".\\result.txt");

    return 0;
}