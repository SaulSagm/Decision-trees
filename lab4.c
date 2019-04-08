#include <stdio.h> //Libraries
#include <string.h>
#include <stdlib.h>
#include <math.h>

struct conditions   //Structure for the attributes registered with their
{                   //different paths and stage they are.
    char name[30];
    char path1[50];
    char path2[50];
    char path3[50];
    int num_paths;
    int stage;
}cond[10];

struct dataSet      //Structure for saving the data sets 
{
    char Set[50];
}dataSet[30];

struct allSets      //Structure for saving a single data of the data set
{
    char singleSet[20];
}singleData[150];

struct gainInf      //Structure for saving the values for calculating the
{                   //gain of any attribute
    double v,inf1,inf2,inf3;
}gain[10];

struct node         //Structure for saving the name of the node with its gain,
{                   //this for deciding which is greater or better
    char name[15];
    double val;
}node[10];

double entropy(double tot,double pos,double neg,double med)
{                           //Function for calculating the entropy
    double val;
    if(neg==0&&med==0)
    {
        val=0;
    }
    else
        if(med==0)
            val=-(pos/tot)*log2(pos/tot)-(neg/tot)*log2(neg/tot);
        else
            val=-(pos/tot)*log2(pos/tot)-(neg/tot)*log2(neg/tot)-(med/tot)*log2(med/tot);
    return val;
}

int main() 
{
    int a=0,b=0,c=0,i=0,k=0,l=0;        //Variables that work as counters
    int n_cond=0,n_dataSet=0,n_Set=0,states,max=0;  //Counters for some information
    double pos=0,neg=0,med=0,tot,subtot=0;   //Variables for calculating the entropy
    double sum=0;   
    int flag=0;
    double ent_raiz;
    char str1[50],str2[50],raiz[20];    
    char rel[]="@relation",relation[20],att[]="@attribute",attribute[35];
    char *pch1,*pch2,*pch3,*pch4,*pch5;     //Pointers
    
    while(scanf("%s",&str1)!=EOF)
    {
        if(strstr(str1,rel))    //We consider if the string is about the relations
        {
            pch1=strtok(str1,". "); //In this part the string is divided by spaces           
            while(pch1!=NULL)
            {
                if(a==1)
                    strcpy(relation,pch1);
                pch1=strtok(NULL,". ");
                a++;
            }
        }
        a=0;

        if(strstr(str1,att))    //We consider if the string is about the attributes
        {
            pch2=strtok(str1,". ");
            while(pch2!=NULL)
            {
                if(b==1)
                    strcpy(cond[n_cond].name,pch2); //We save the name of the column
                else
                    if(b==2)
                        strcpy(attribute,pch2); //We save the string with the variables
                b++;
                pch2=strtok(NULL,". ");
            }
            b=0;
            
            pch3=strtok(attribute,"{}, ");
            while(pch3!=NULL)
            {
                if(c==0)
                {
                    strcpy(cond[n_cond].path1,pch3);    //We save the first path
                    cond[n_cond].num_paths=1;
                }
                else
                    if(c==1)
                    {
                        strcpy(cond[n_cond].path2,pch3);    //We save the second path
                        cond[n_cond].num_paths=2;
                    }
                    else
                        if(c==2)
                        {
                            strcpy(cond[n_cond].path3,pch3);    //We save the third path
                            cond[n_cond].num_paths=3;
                        }
                c++;
                pch3=strtok(NULL,"{}, ");
            }
            c=0;
            n_cond++;
            cond[n_cond-1].stage=n_cond;
        }
        
        if(strcmp(str1,"@data")==0) //When the string has this instruction it
            flag=1;                 //activates a flag
        
        if(flag==1)     //If the flag is active, the following strings are saved
        {               //in the structure of data sets
            pch4=strtok(str1," ");
            while(pch4!=NULL)
            {
                strcpy(dataSet[n_dataSet].Set,pch4);
                pch4=strtok(NULL," ");
            }
            n_dataSet++;
        }
    
        fflush(stdin);  //Clean inputs
    }

    states=n_cond;
    
    for(i=0;i<n_dataSet;i++)    //In this cycle, every data is saved in the structure
    {                           //single data
        pch5=strtok(dataSet[i].Set,", ");
        while(pch5!=NULL)
        {
            strcpy(singleData[n_Set].singleSet,pch5);
            n_Set++;
            pch5=strtok(NULL,", ");
        }
    }
    
    for(i=1;i<=n_dataSet;i++)   //Here the entropy is calculated
    {
        if(strcmp(singleData[i*n_cond-1].singleSet,cond[n_cond-1].path1)==0)
            pos++;
        if(strcmp(singleData[i*n_cond-1].singleSet,cond[n_cond-1].path2)==0)
            neg++;
        if(strcmp(singleData[i*n_cond-1].singleSet,cond[n_cond-1].path3)==0)
            med++;
    }
    tot=pos+neg+med;    //Sum the total
    
    ent_raiz=entropy(tot,pos,neg,med); //Use the previous function to calculate
    
    for(i=0;i<3;i++)    //Initialize in 0 the values of gain
    {
        gain[i].v=0;
        gain[i].inf1=0;
        gain[i].inf2=0;
        gain[i].inf3=0;
    }
    
    //printf("%f %f %f %f %f\n",pos,neg,med,tot,ent_raiz);
    
    while(states!=0)    //For obtaining the initial node
    {
        for(i=1;i<=n_dataSet;i++)   //Calculate the gain(D,A)
        {
            if(strcmp(singleData[i*n_cond-states].singleSet,cond[n_cond-states].path1))
            {
                gain[0].v++;
                if(strcmp(singleData[i*n_cond-1].singleSet,cond[n_cond-1].path1))
                    gain[0].inf1++;
                if(strcmp(singleData[i*n_cond-1].singleSet,cond[n_cond-1].path2))
                    gain[0].inf2++;
                if(strcmp(singleData[i*n_cond-1].singleSet,cond[n_cond-1].path3))
                    gain[0].inf3++;
            }
            
            if(strcmp(singleData[i*n_cond-states].singleSet,cond[n_cond-states].path2))
            {
                gain[1].v++;
                if(strcmp(singleData[i*n_cond-1].singleSet,cond[n_cond-1].path1))
                    gain[1].inf1++;
                if(strcmp(singleData[i*n_cond-1].singleSet,cond[n_cond-1].path2))
                    gain[1].inf2++;
                if(strcmp(singleData[i*n_cond-1].singleSet,cond[n_cond-1].path3))
                    gain[1].inf3++;
            }
            
            if(strcmp(singleData[i*n_cond-states].singleSet,cond[n_cond-states].path3))
            {
                gain[2].v++;
                if(strcmp(singleData[i*n_cond-1].singleSet,cond[n_cond-1].path1))
                    gain[2].inf1++;
                if(strcmp(singleData[i*n_cond-1].singleSet,cond[n_cond-1].path2))
                    gain[2].inf2++;
                if(strcmp(singleData[i*n_cond-1].singleSet,cond[n_cond-1].path3))
                    gain[2].inf3++;
            }
        }
        
        for(i=0;i<3;i++)
            subtot=subtot+gain[i].v;    //Calculate subtot or v
        
        for(i=0;i<3;i++)
        {                   //Sum of the different entropies
            tot=gain[i].inf1+gain[i].inf2+gain[i].inf3;
            sum=sum-(gain[i].v/subtot)*entropy(tot,gain[i].inf1,gain[i].inf2,gain[i].inf3);
        }

        if(l!=(n_cond-1))
        {
            node[l].val=ent_raiz+sum;
            strcpy(node[l].name,cond[l].name);  
        }                                       
        sum=0;
        for(i=0;i<3;i++)    //Reset of values
        {
            gain[i].v=0;
            gain[i].inf1=0;
            gain[i].inf2=0;
            gain[i].inf3=0;
        }
        l++;
        states--;
        subtot=0;
        sum=0;
    }
    
    for(i=0;i<l;i++)
    {
        if(node[i].val>=node[i+1].val&&(i+1)!=l)    //Choosing the highes node 
            strcpy(raiz,node[i].name);              //with better entropy
    }
    
    return 0;
}
