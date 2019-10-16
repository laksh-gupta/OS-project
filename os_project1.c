#include<stdio.h>
#include<stdlib.h>
#include<math.h>
struct process{
    int process_id,burst_time,priority,shortness_rank,remaining_time;
    float new_rank;
    int waitting_time,turnaround_time;
};
int sort(struct process *p,int n,int type){
    struct process temp;
    if(type==1){
        for(int i=0;i<n-1;i++)
            for(int j=0;j<n-i-1;j++)
                if(p[j].new_rank-p[j+1].new_rank>0){
                    temp=p[j];
                    p[j]=p[j+1];
                    p[j+1]=temp;
                }
    }
    else if(type==2){
        for(int i=0;i<n-1;i++)
            for(int j=0;j<n-1-i;j++)
                if(p[j].burst_time>p[j+1].burst_time){
                    temp=p[j];
                    p[j]=p[j+1];
                    p[j+1]=temp;
                }
    }
    else if(type==3){
        for(int i=0;i<n-1;i++)
            for(int j=0;j<n-i-1;j++)
                if(p[j].priority>p[j+1].priority){
                    temp=p[j];
                    p[j]=p[j+1];
                    p[j+1]=temp;
                }
    }
    else if(type==4){
        for(int i=0;i<n-1;i++)
            for(int j=0;j<n-i-1;j++)
                if(p[j].process_id>p[j+1].process_id){
                    temp=p[j];
                    p[j]=p[j+1];
                    p[j+1]=temp;
                }
    }
    return(1);
}
int print_pretty(struct process *p,int n,int t){
    float avg_wt=0,avg_tt=0;
    sort(p,n,4);
    printf("\nProcess_id\tWaitting Time\tTurnaround Time\n");
    for(int i;i<n;i++){
        avg_wt+=p[i].waitting_time;
        avg_tt+=p[i].turnaround_time;
        printf("\tP%d\t\t%d\t\t%d\n",p[i].process_id,p[i].waitting_time,p[i].turnaround_time);
    }
    avg_wt=(float)avg_wt/n;
    avg_tt=(float)avg_tt/n;
    printf("\tAverage Waitting Time:- %.2f\n",avg_wt);
    printf("\tAverage Turnaround Time:- %.2f\n",avg_tt);
    printf("\tThroughput:- %f\n",(float)t/n);
}
int returnIndex(struct process *p,int n,int a){
    for(int i=0;i<n;i++){
        if(p[i].process_id==a)
        return(i);
    }
    return(-1);
}
int fcfs(struct process *que1,int n){
    int total_time=0;
    for(int i=0;i<n;i++){
        que1[i].waitting_time=total_time;
        total_time+=que1[i].burst_time;
        que1[i].turnaround_time=total_time;
        printf("-P%d-",que1[i].process_id);
    }
    print_pretty(que1,n,total_time);
    return(1);
}
int sjf(struct process *que1,int n){
    sort(que1,n,2);
    int total_time=0;
    for(int i=0;i<n;i++){
        que1[i].waitting_time=total_time;
        total_time+=que1[i].burst_time;
        que1[i].turnaround_time=total_time;
        printf("-P%d-",que1[i].process_id);
    }
    print_pretty(que1,n,total_time);
    return(1);
}
int rr(struct process *que1,int n){
    int time_quanta;
    printf("Enter the time quanta:\t");
    scanf("%d",&time_quanta);
    int total_time=0;
    int n1=n,count=0;
    while(n1){
        if(que1[count].remaining_time<=time_quanta && que1[count].remaining_time>0){
            total_time+=que1[count].remaining_time;
            que1[count].remaining_time=0;
            que1[count].turnaround_time=total_time;
            que1[count].waitting_time=total_time-que1[count].burst_time;
            printf("-P%d-",que1[count++].process_id);
            n1--;
        }
        else if(que1[count].remaining_time>0){
            que1[count].remaining_time-=time_quanta;
            total_time+=time_quanta;
            printf("-P%d-",que1[count++].process_id);
        }
        else
            count++;
        if(count==n)
            count=count%n;
    }
    print_pretty(que1,n,total_time);
    return(1);
}
int priority(struct process *que1,int n){
    sort(que1,n,3);
    int total_time=0;
    for(int i=0;i<n;i++){
        que1[i].waitting_time=total_time;
        total_time+=que1[i].burst_time;
        que1[i].turnaround_time=total_time;
        printf("-P%d-",que1[i].process_id);
    }
    print_pretty(que1,n,total_time);
    return(1);
}
int multiLevelScheduling(struct process *que1,int n){
    int i,j;
    int sum=0;
    float mean,standard_deviation=0.0;
    int r=1;
    for(i=0;i<n;i++){
        sum+=que1[i].burst_time;
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
    while (t){
        que2=(struct process*)malloc(sizeof(struct process)*n1);
        n2=0;
        for(i=0;i<n1;i++){
            if(que1[i].remaining_time<time_quanta){
                total_time+=que1[i].remaining_time;
                int x=returnIndex(p,n,que1[i].process_id);
                p[x].turnaround_time=total_time;
                p[x].waitting_time=total_time-p[x].burst_time;
                que1[i].remaining_time=0;
                t--;
            }
            else{
                total_time+=time_quanta;
                que1[i].remaining_time-=time_quanta;
                que2[n2++]=que1[i];
            }
            printf("-P%d-",que1[i].process_id);
        }
        que1=que2;
        n1=n2;
    }
    print_pretty(p,n,total_time);
    return(1);
}

int main(){
    int c,n;
    while(1){
        printf("\n\t\tAvailable Scheduling Algorithms\n\t1. FCFS(first come first served)\n\t2. SJF(shortest job first)\n\t3. RR(round robin)\n");
        printf("\t4. Priority based\n\t5. Modified multilevel(our propossed alorithm)\n\t6. Exit\nSelect any one option:-\t");
        scanf("%d",&c);
        if(c<6){
            printf("Enter the number of processes:-\t");
            scanf("%d",&n);
            struct process queue1[n];
            if(c<4)
                for(int i=0;i<n;i++){
                    printf("enter the burst time of process with id number p%d:-\t",i+1);
                    scanf("%d",&queue1[i].burst_time);
                    queue1[i].remaining_time=queue1[i].burst_time;
                    queue1[i].process_id=i+1;
                }
            else
                for(int i=0;i<n;i++){
                    printf("enter the burst time and priority of process with id number p%d:-\t",i+1);
                    scanf("%d %d",&queue1[i].burst_time,&queue1[i].priority);
                    queue1[i].remaining_time=queue1[i].burst_time;
                    queue1[i].process_id=i+1;
                }
            switch (c){
                case 1: fcfs(queue1,n);
                        break;
                case 2: sjf(queue1,n);
                        break;
                case 3: rr(queue1,n);
                        break;
                case 4: priority(queue1,n);
                        break;
                case 5: multiLevelScheduling(queue1,n);
                        break;
                default: printf("Wrong option selected");
            }
        }
        else{
            exit(0);
        }
    }
    return(0);
}
