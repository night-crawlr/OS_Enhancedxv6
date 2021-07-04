1) proc structure in Proc.h

newly added variables

  int ctime;             //creation time
  int rtime;             //runtime
  int w_time;            //waiting time for pickup that is used in part2
  int iotime;            //IO time
  int etime;             //exit time
  int priority;          //priority
  int n_run;             //No of times the process was picked up by scheduler
  int cur_q;             //current queue in case mlfq scheduler
  int r_time_q[5];       //current running tick time in cur_q  this resets to 0 when the running is complete
  int total_r_time_q[5]; //totall running tick time in cur_q  this doenot resets to 0 ie this is combined of running time of process in that queue
  int wait_start;        //starting to wait when it is not picked by processor
  int patience_time;     //setting some value in proc.c

2)waitx syscall

coppying the wait sys call and adding

        //For Assignment start
        cprintf("(%d)   (%d)   (%d)   (%d)\n", p->etime, p->ctime, p->rtime, p->iotime);
        *wtime = p->etime - p->ctime - p->rtime - p->iotime;
        *rtime = p->rtime;
        //For Assignment end

these lines make waitx

3)time command in time.c
this prints waitime and rtime of process
if (pid == 0)
    {
        //child
        exec(argv[1], &argv[1]);
        printf(1, "EXECUTION OF %s FAILED \n", argv[1]);
        exit();
    }
    waitx(&wtime, &rtime);

    printf(1, "THE WAITING_TIME (EXCLUDING I/O TIME) OF %s IS %d\n", argv[1], wtime);
    printf(1, "THE RUNNING_TIME OF %s IS %d\n", argv[1], rtime);
    exit();

4)ps userprogram uses myps() syscall ps in ps.c myps() in proc.c
myps() loops over ptable and retricve process

TASK 2

1)FCFS-> priority using creation time

struct proc *fcfs(struct proc *low_c)
{
  struct proc *p;

  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if (p->state != RUNNABLE)
      continue;
    if (p->pid <= 2) // not taking first 2 process
      continue;
    if (low_c->ctime > p->ctime)
      low_c = p;
  }
  return low_c;
}

2) PBS

set_priority sys calll is setp in this folder

int setp(int new_priority, int pid)
{
  struct proc *p;

  acquire(&ptable.lock);

  int is_there = 0;
  int oldpriority = 60;
  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if (p->pid == pid)
    {
      oldpriority = p->priority;
      p->priority = new_priority;
      is_there++;
      break;
    }
  }

  release(&ptable.lock);

  if (is_there == 0)
  {
    cprintf("PROCESS WITH PID %d DOESNOT EXIT\n", pid);
    return -1;
  }
  return oldpriority;
}

For scheduler

struct proc *priority_scheduler(struct proc *max_prio)
{
  struct proc *p;

  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if (p->state != RUNNABLE)
      continue;
    if (max_prio->priority > p->priority)
      max_prio = p;
  }
  return max_prio;
}

c) mlfq

age is modifie in

void check_age(int qno)
{
  if (qno == 0) // no aging is required as this is the h-que
    return;
  for (int i = 0; i < cur_size[qno]; i++)
  {
    if (queue[qno][i]->state != RUNNABLE)
      continue;

    if (ticks - queue[qno][i]->wait_start > queue[qno][i]->patience_time)
    {
      queue[qno - 1][cur_size[qno - 1]] = queue[qno][i];
      cur_size[qno - 1]++;
      queue[qno][i]->wait_start = ticks;
      queue[qno][i]->r_time_q[qno] = 0;
      queue[qno][i]->cur_q--;
      for (int j = i; j < (cur_size[qno] - 1); j++)
        queue[qno][j] = queue[qno][j + 1];
      cur_size[qno]--;
    }
  }
}


scheduler in 


void mlfq_scheduler(struct cpu *c, int qno)
{
  //cprintf("CC\n");
  for (int i = 0; i < cur_size[qno]; i++)
  {
    if (queue[qno][i]->state != RUNNABLE)
      continue;
    queue[qno][i]->wait_start = ticks; // As process is taken to processor last waited time is updated to ticks this will also be updated when its running
    c->proc = queue[qno][i];
    switchuvm(queue[qno][i]);
    queue[qno][i]->state = RUNNING;
    queue[qno][i]->n_run++;
    swtch(&c->scheduler, queue[qno][i]->context);
    switchkvm();
    c->proc = 0;

    if (((queue[qno][i]->killed == 1) || (queue[qno][i]->r_time_q[qno] >= pow(2, qno))) && queue[qno][i]->pid > 2)
    {
      struct proc *dummy;
      dummy = queue[qno][i];
      queue[qno][i]->r_time_q[qno] = 0;
      for (int j = i; j < (cur_size[qno] - 1); j++)
      {
        queue[qno][j] = queue[qno][j + 1];
      }
      cur_size[qno]--;

      if (!(dummy->killed))
      {
        if (qno != 4)
        {
          dummy->cur_q++;
          qno++;
        }
        queue[qno][cur_size[qno]] = dummy;
        cur_size[qno]++;
      }
    }
  }
}


REPORT :

On running command time benchmark &

default (RRB) :
wtime = 13
rtime = 5

FCFS :
wtime =0;
rtime = 6;

PBS : 
wtime =1;
rtime = 5;

MLFQ : 
wtime =22;
rtime =4;