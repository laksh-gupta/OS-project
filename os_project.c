#include<stdio.h>
#include<math.h>
#include<stdlib.h>
struct process
{
    int process_id,burst_time,priority,shortness_rank,remaining_time;
    float new_rank;
    int waitting_time,turnaround_time;
};
int sort(struct process *p,int n,int type)
{
    struct process temp;
    if(type==1)
    {
        for(int i=0;i<n;i++)
            for(int j=0;j<n-i-1;j++)
                if(p[j].new_rank-p[j+1].new_rank>0)
                {
                    temp=p[j];
                    p[j]=p[j+1];
                    p[j+1]=temp;
                }
    }
    return(1);
}
int returnIndex(struct process *p,int n,int a)
{
    for(int i=0;i<n;i++)
    {
        if(p[i].process_id==a)
        return(i);
    }
    return(-1);
}
int multiLevelScheduling(struct process *que1,int n)
{
    int i,j;
    int sum=0;
    float mean,standard_deviation=0.0;
    for(i=0;i<n;i++)
    {
        sum+=que1[i].burst_time;
        int r=1,s=1;
        for(j=0;j<n;j++)
            if(j!=i && que1[j].burst_time<que1[i].burst_time)
                r++;
        que1[i].shortness_rank=r;
        que1[i].new_rank=(float)(3*que1[i].priority+que1[i].shortness_rank)/4;
    }
    mean=(float)sum/n;
    for(i=0;i<n;i++)
        standard_deviation+=(que1[i].burst_time-mean)*(que1[i].burst_time-mean);
    standard_deviation=sqrt(standard_deviation/n);
    int time_quanta=(int)(mean+0.5*standard_deviation);
    int t=n,n2,n1=n;
    int total_time=0;
    struct process *que2;
    struct process *p=que1;
    sort(que1,n,1);
    while (t)
    {
        que2=(struct process*)malloc(sizeof(struct process)*n1);
        n2=0;
        for(i=0;i<n1;i++)
        {
            if(que1[i].remaining_time<time_quanta)
            {
                total_time+=que1[i].remaining_time;
                p[returnIndex(p,n,que1[i].process_id)].turnaround_time=total_time;
                que1[i].remaining_time=0;
                t--;
            }
            else
            {
                total_time+=time_quanta;
                que1[i].remaining_time-=time_quanta;
                que2[n2++]=que1[i];
            }
            printf("-->p%d",que1[i].process_id);
        }
        printf("\n");
        que1=que2;
        n1=n2;
    }
    printf("Turnaround Times:\n");
    for(i=0;i<n;i++)
    {
        printf("%d ",p[i].turnaround_time);
    }
    return(1);
}
int main()
{
    int a;
    printf("\nSelect any one option\n1. Priority based scheduling\n2. FCFS based scheduling\n");
    scanf("%d",&a);
    int n;
    printf("\nEnter the number of processes");
    scanf("%d",&n);
    struct process queue1[n];
    if(a==1)
    for(int i=0;i<n;i++)
    {
        printf("\nenter the burst time and priority of process with id number p%d:-",i+1);
        scanf("%d %d",&queue1[i].burst_time,&queue1[i].priority);
        queue1[i].remaining_time=queue1[i].burst_time;
        queue1[i].process_id=i+1;
    }
    else
    for(int i=0;i<n;i++)
    {
        printf("\nenter the burst time of process with id number p%d:-",i+1);
        scanf("%d",&queue1[i].burst_time);
        queue1[i].priority=i+1;
        queue1[i].remaining_time=queue1[i].burst_time;
        queue1[i].process_id=i+1;
    }
    multiLevelScheduling(queue1,n);
    return(0);
}
